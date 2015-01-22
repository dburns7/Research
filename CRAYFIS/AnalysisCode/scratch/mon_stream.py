#!/usr/bin/env python

import ROOT as r
r.PyConfig.IgnoreCommandLineOptions = True
from glob import glob
import os
import datetime
import time
import numpy as np
import itertools as it
import uuid

r.gStyle.SetOptStat(0)

PLOT_HEIGHT=400
PLOT_WIDTH=300

canvasses = {}
def get_canvas(canvas_name):
    if not canvas_name in canvasses:
        canvasses[canvas_name] = r.TCanvas(canvas_name,canvas_name,PLOT_HEIGHT,PLOT_WIDTH)
    return canvasses[canvas_name]

already_seen=set()
def get_device_files(source_path, dev_id):
    global already_seen
    all_files = set(glob(os.path.join(source_path, '*/*/*%s*.root'%(dev_id))))
    new_files = all_files.difference(already_seen)
    already_seen.update(new_files)
    return new_files

def get_device_files2(source_path, dev_id, starttime):
    files = []
    pathtime = datetime.datetime.utcfromtimestamp(starttime) - datetime.timedelta(days=1)
    maxtime = 0
    while pathtime <= (datetime.datetime.utcnow() + datetime.timedelta(days=1)):
        path = os.path.join(source_path, pathtime.strftime('%Y.%m/%d'))
        print "look in",path
        pathfiles = glob(os.path.join(path, '*%s*.root*'%(dev_id)))
        # get the timestamp from the file
        for f in pathfiles:
            ftime = int(f.split('.root')[0].split('_')[-1])
            if ftime <= starttime: continue
            files.append(f)
            maxtime = max(maxtime, ftime)
        pathtime += datetime.timedelta(days=1)
    print "new maxtime:",maxtime
    return files, maxtime

def get_data(t, earliest_time=0):
    times = []
    npix = []
    pixmax = []
    pixavg = []
    bg_avg = []
    bg_var = []
    for evt in t:
        times.append(evt.t/1e3)
        npix.append(evt.pix_n)
        pixvals = np.array(evt.pix_val)
        pixmax.append(pixvals.max())
        pixavg.append(pixvals.mean())
        bg_avg.append(evt.bg_lvl)
        bg_var.append(evt.bg_var)
    data = {}
    data['times'] = times
    data['npix'] = npix
    data['pixmax'] = pixmax
    data['pixavg'] = pixavg
    data['bg_avg'] = bg_avg
    data['bg_var'] = bg_var

    data['xy'] = []
    if len(data['times']) == 0:
        return data

    pixel_reso = 30 # seconds

    mintime = min(times)
    maxtime = max(times)
    t0 = max(mintime, earliest_time)
    while t0 < maxtime:
        t.Draw("pix_x:pix_y>>h_xy(80,0,320,80,0,320)","t/1e3>=%d && t/1e3<%d"%(t0,t0+pixel_reso),"goff")
        t0 += pixel_reso
        h = r.TH2F()
        r.gROOT.FindObject("h_xy").Copy(h)
        # set the histogram name to something random so root doesn't complain
        # about reallocationg "h_xy"
        h.SetName(str(uuid.uuid1()))
        data['xy'].append((h, min(times)))
    return data

def add_xb_info(txb, data):
    data['xb_times'] = []
    data['xb_L1'] = []
    for entry in txb:
        data['xb_times'].append(entry.start_time/1e3)
        data['xb_L1'].append(entry.L1_thresh)

def merge_data(d1, d2):
    list_keys = ('times', 'npix', 'pixmax','pixavg','xb_times','xb_L1',
            'bg_avg', 'bg_var', )
    for k in list_keys:
        if not k in d1:
            d1[k] = np.array([], dtype=float)
        d1[k] = np.hstack([d1[k], d2[k]])

    histo_keys = ('xy',)
    for k in histo_keys:
        if not k in d1:
            d1[k] = []
        d1[k].extend(d2[k])

def time_hist(times, pad, maxtime=600):
    binwidth = 5       # seconds
    tstop = times.max()
    tstart = max(times.min(), tstop-maxtime)
    nbins = int((tstop-tstart)/float(binwidth))
    tlast = tstart + binwidth*nbins
    to_plot = times[times>tstart]-time.time()
    if len(to_plot)==0:
        return
    tstop = max(to_plot.max(), 0)
    h = r.TH1F('h_time','h_time',nbins,to_plot.min(),tstop)
    #canvas = get_canvas('c_time')
    #canvas.cd()
    pad.cd()
    print "drawing to pad", pad
    print "drawd"
    map(h.Fill, to_plot)
    h.Draw()
    pad.Update()
    pad._h=h
    #canvas.Update()

def time_plot(times, vals, pad, title, maxtime=600, logz=True):
    binwidth = 10       # seconds
    tstop = times.max()
    tstart = max(times.min(), tstop-maxtime)
    nbinsx = int((tstop-tstart)/float(binwidth)+1)
    vtimes = times>tstart
    ptimes = times[vtimes]-time.time()
    if len(ptimes)==0:
        print "nothing to plot, returning."
        return
    pvals = vals[vtimes]
    nbinsy = int(pvals.max())+1
    tstop = max(ptimes.max(), 0)
    h = r.TH2F(title,title,nbinsx,ptimes.min(),tstop,nbinsy,0,int(pvals.max())+1)
    for x,y in zip(ptimes,pvals):
        h.Fill(x,y)
    #h.Draw()
    #canvas_name = 'c_%s'%fig
    #canvas = get_canvas(canvas_name)
    #canvas.cd()
    pad.cd()
    print "drawing to pad2", pad
    h.Draw('colz')
    print "draw2"
    pad.SetLogz(logz)
    pad.Update()
    pad._h=h
    #canvas.Update()

def xy_plot(histos, pad, fig='pixel', maxtime=600, logz=True, drop_old=True):
    to_drop = []
    cutoff = time.time() - maxtime
    h0 = None
    ndraw = 0
    for h, starttime in histos:
        if starttime < cutoff:
            to_drop.append((h,starttime))
            continue
        ndraw += 1
        if not h0:
            h0 = r.TH2F()
            h.Copy(h0)
        else:
            h0.Add(h)

    print "plotting composite of %d histos" % ndraw
    print "dropped %d old histos" % len(to_drop)
    map(histos.remove, to_drop)

    if not h0:
        return

    h0.SetTitle(fig)
    #canvas = get_canvas('c_%s'%fig)
    #canvas.cd()
    pad.cd()
    h0.Draw("colz")
    pad.SetLogz(logz)
    pad.Update()
    pad._h=h0

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="draw plots from a certain device")
    parser.add_argument('--source', default='ntuple', help='the source directory with ntuples (expects yyyy.mm/dd structure)')
    parser.add_argument('--dev', required=True, help='the device ID')
    parser.add_argument('--since', default=0, type=float, help='number of minutes back to start')
    parser.add_argument('--interval', default=2, type=float, help='update interval, in seconds')

    args = parser.parse_args()

    import pylab as pl
    pl.ion()


    # i have no idea why but the timestamps on the files are different
    # from both UTC and unix time... for now, you can use this
    # fudgefactor to make sure you're only looking at recent data.
    time_correction = 7 # hours

    starttime = datetime.datetime.utcnow() - datetime.timedelta(minutes=args.since)  + datetime.timedelta(hours=time_correction)

    earliest_pix_time = time.time() # don't try to draw pixels for past data

    c1 = r.TCanvas("monitor1", "monitor %s"%args.dev, 1100, 600)
    c1.cd()
    pad1_1 = r.TPad("pad1_1", "pad1_1", 0, 0, 0.25, 0.5)
    pad1_1.Draw()
    c1.cd()
    pad1_2 = r.TPad("pad1_2", "pad1_2", 0.25, 0, 0.5, 0.5)
    pad1_2.Draw()
    c1.cd()
    pad1_3 = r.TPad("pad1_3", "pad1_3", 0, 0.5, 0.25, 1)
    pad1_3.Draw()
    c1.cd()
    pad1_4 = r.TPad("pad1_4", "pad1_4", 0.25, 0.5, 0.5, 1)
    pad1_4.Draw()
    c1.cd()
    pad1_5 = r.TPad("pad1_5", "pad1_5", 0.5, 0, 0.75, 0.5)
    pad1_5.Draw()
    c1.cd()
    pad1_6 = r.TPad("pad1_6", "pad1_6", 0.75, 0, 1, 0.5)
    pad1_6.Draw()
    c1.cd()
    pad1_7 = r.TPad("pad1_7", "pad1_7", 0.5, 0.5, 0.75, 1)
    pad1_7.Draw()
    c1.cd()
    pad1_8 = r.TPad("pad1_8", "pad1_8", 0.75, 0.5, 1, 1)
    pad1_8.Draw()
    c1.cd()

    #last_update = int(starttime.strftime('%s'))
    data_acc = {}
    # call get_device_files once to make the initial list of files
    get_device_files(args.source, args.dev)

    all_pads = (pad1_1,pad1_2,pad1_3,pad1_4,pad1_5,pad1_6,pad1_7,pad1_8)
    for pad in all_pads:
        pad.cd()
        loadtxt = r.TPaveText(0.,0.,1.,1.)
        loadtxt.SetFillStyle(0)
        loadtxt.AddText("Waiting for data...")
        loadtxt.Draw()
        pad.Update()
        pad._loadtxt = loadtxt
    c1.Update()
    while True:
        new_files = get_device_files(args.source, args.dev)
        if len(new_files) == 0:
            print "no new files."
        else:
            #last_update = new_time
            print 'found %d new files' % (len(new_files))

        t = r.TChain("events")
        map(t.Add, new_files)
        txb = r.TChain("exposureblock")
        map(txb.Add, new_files)

        new_data = get_data(t, earliest_pix_time)
        print "len(new_data['times']) =", len(new_data['times'])
        add_xb_info(txb, new_data)

        merge_data(data_acc, new_data)
        print 'len set(times):', len(set(data_acc['times']))
        if len(data_acc['times'])==0:
            # oops, nothing to plot yet.
            time.sleep(args.interval)
            continue

        time_hist(data_acc['times'], pad1_1)
        time_plot(data_acc['times'], data_acc['npix'], pad1_2, title='npix')
        time_plot(data_acc['times'], data_acc['pixmax'], pad1_3, title='pixmax')
        time_plot(data_acc['times'], data_acc['pixavg'], pad1_4, title='pixavg')
        time_plot(data_acc['times'], data_acc['bg_avg'], pad1_5, title='bg_var')
        time_plot(data_acc['times'], data_acc['bg_var'], pad1_6, title='bg_avg')
        time_plot(data_acc['xb_times'], data_acc['xb_L1'], pad1_7, title='L1')
        c1.Update()

        xy_plot(data_acc['xy'], pad1_8)


        time.sleep(args.interval)
