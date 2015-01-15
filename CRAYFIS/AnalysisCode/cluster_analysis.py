#!/usr/bin/env python

from ROOT import gROOT, TCanvas, TF1
#import ROOT as r
#r.PyConfig.IgnoreCommandLineOptions = True
from glob import glob
import os
import datetime
import time
import numpy as np
import itertools as it
import uuid

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

    print "Hellooo"
