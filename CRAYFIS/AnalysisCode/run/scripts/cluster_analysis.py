#!/usr/bin/env python

from ROOT import *
import numpy as np
import math
import sys
import argparse

def dist(x1, x2, y1, y2):
  return math.sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))

def times(time):
  times = []
  curtime = time[0]
  cnt = 0 
  for i in range(0, len(time)):
    if curtime == time[i]:
      continue
    times.append(curtime)
    curtime = time[i]
  return times

def pix_per_frame(time):
  pix_cnt = []
  curtime = time[0]
  cnt = 0 
  for i in range(0, len(time)):
    if curtime == time[i]:
      cnt = cnt + 1 
      continue
    pix_cnt.append(cnt)
    cnt = 0 
    curtime = time[i]
    cnt = 1 
  return pix_cnt

def sort_arrs_by_time(x, y, val, time):
  curtime = time[0]
  xarr = []
  yarr = []
  valarr = []
  tarr = []
  xtemp = []
  ytemp = []
  valtemp = []
  ttemp = []
  for i in range(0, len(time)):
    if curtime == time[i]:
      xtemp.append(x[i])
      ytemp.append(y[i])
      valtemp.append(val[i])
      ttemp.append(time[i])
      continue
    curtime = time[i]
    xarr.append(xtemp)
    yarr.append(ytemp)
    valarr.append(valtemp)
    tarr.append(ttemp)
    xtemp = []
    ytemp = []
    valtemp = []
    ttemp = []
    xtemp.append(x[i])
    ytemp.append(y[i])
    valtemp.append(val[i])
    ttemp.append(time[i])
  return (xarr, yarr, valarr, tarr)

def sort_arrs_by_thresh(xarr, yarr, valarr, thresh):
  lowx = []
  lowy = []
  lowval = []
  highx = []
  highy = []
  highval = []
  high = False
  for i in range(0, len(valarr)):
    for j in range(0, len(valarr[i])):
      if valarr[i][j] >= thresh:
        high = True
      else:
        high = False
    if high:
      highx.append(xarr[i])
      highy.append(yarr[i])
      highval.append(valarr[i])
    else:
      lowx.append(xarr[i])
      lowy.append(yarr[i])
      lowval.append(valarr[i])
  return (lowx, lowy, lowval, highx, highy, highval)

def count_hit_pix(xarr, yarr, valarr, thresh):
  cnt = []
  tempcnt = 0
  for i in range(0, len(xarr)):
    for j in range(0, len(xarr[i])):
      if valarr[i][j] >= 5:
        if valarr[i][j] >= thresh:
          tempcnt += 1
    cnt.append(tempcnt)
    tempcnt = 0
  return cnt

def count_hits(time, tstart, tend):
  time = sorted(time)
  times = []
  curtime = time[0]
  cnt = 0
  for i in range(0, len(time)):
    if time[i] < tstart or time[i] > tend: continue
    if curtime == time[i]:
      continue
    times.append(curtime)
    curtime = time[i]
  return len(times)

def rate(time, tstart, tend):
  time = sorted(time)
  return count_hits(time, tstart, tend) / (tend - tstart)
  #return count_hits(time, tstart, tend) / (time[-1] - time[0])

# This function uses the x and y arrays of a single frame to create a cluster array that has 
# the indexes separated into the different clusters according to thier x and y positions.
# Ex: [[0,3],[1,2]] as the cluarr output would be x[0] and x[3] are in a cluster together
# and x[1] and x[2] are in another cluster
def cluster_frame(x, y):
  #initialize clarr with the first index already in
  clarr = [[]]
  #make sure any pixels were detected
  if len(x) >= 1:
    clarr[0].append(0)
  else: return clarr
  #variables to help cluster
  alrInArray = False
  putInCluster = False
  #loop through every combination of x and y
  for i in range(0, len(x)):
    for j in range(0, len(x)):
      #skip if comparing with itself
      if i == j:
        continue
      #check to see if in same cluster or not
      if dist(x[i], x[j], y[i], y[j]) > 30:
        alrInArray = False
        #IF NOT IN SAME CLUSTER
        #check to see if j index is already in cluster array
        for k in range(0, len(clarr)):
          for l in range(0, len(clarr[k])):
            #if j index is in clarr mark it as already in
            if j == clarr[k][l]:
              alrInArray = True
        #if j was not found in clarr then put it in right cluster
        if alrInArray == False:
          #variable to make sure don't double put in index
          putInCluster == False
          for m in range(0, len(clarr)):
            for n in range(0, len(clarr[m])):
              #if within distance with an index in a cluster put it into 
              # that cluster
              if (putInCluster == False) and (dist(x[j], x[clarr[m][n]], y[j], y[clarr[m][n]]) <= 30):
                clarr[m].append(j)
                #mark as put in cluster
                putInCluster = True
          #if j was not put into any cluster since it was too far away
          # from all current clusters put into new cluster
          if putInCluster == False:
            clarr.append([j])
      #this means i and j are in the same cluster
      else:
        alrInArray = False
        #check to see which cluster i is in
        for k in range(0, len(clarr)):
          for l in range(0, len(clarr[k])):
            #if i is in that cluster append j to it
            if i == clarr[k][l]:
              #check to make sure j is not already in cluster
              for m in range(0, len(clarr[k])):
                if j == clarr[k][m]:
                  #if it is in already mark it as such
                  alrInArray = True
                  continue
              #if not in array already, then append it
              if(alrInArray == False):
                clarr[k].append(j)
  return clarr

# This function takes in the indexed cluarr and uses it to sort the x, y, and val arrays into 
# clusters
def cluster_merge_frame(x, y, val, cluarray):
  xclu = []
  yclu = []
  valclu = []
  for i in range(0, len(cluarray)):
    xclu.append([])
    yclu.append([])
    valclu.append([])
    for j in range(0, len(cluarray[i])):
      xclu[i].append(x[cluarray[i][j]])
      yclu[i].append(y[cluarray[i][j]])
      valclu[i].append(val[cluarray[i][j]])
  return xclu, yclu, valclu

# This function takes in the clustered x and y arrays and loops over them to find the length 
# of each cluster Output is an array of int lengths
def cluster_length_frame(xc, yc):
  clulength = []
  #select cluster
  for i in range(0, len(xc)):
    #initialize with dummy length 0
    clulength.append(0)
    #need to loop through every combination within cluster
    for j in range(0, len(xc[i])):
      for k in range(0, len(xc[i])):
        distance = dist(xc[i][j], xc[i][k], yc[i][j], yc[i][k])
        if distance > clulength[i]:
          clulength[i] = distance
  return clulength

# Calculate the wedgeness, or measure of angle particle passes through sensor
def wedgeness_frame(xc, yc, ):
  wedgeness = []
  for i in range(0, len(xc)):
    x1 = 0
    x2 = 0
    y1 = 0
    y2 = 0
    pixval_1 = 0
    pixval_2 = 0
    avg3_1 = 0
    avg3_2 = 0
    avg5_1 = 0
    avg5_2 = 0
    clulength.append(0)
    for j in range(0, len(xc[i])):
      for k in range(0, len(yc[i])):
        distance = dist(xc[i][j], xc[i][k], yc[i][j], yc[i][k])
        if distance > clulength[i]:
          clulength[i] = distance
          x1 = xc[i][j]
          x2 = xc[i][k]
          y1 = yc[i][j]
          y2 = yc[i][k]

# Sort arrays of frames into clusters
def process_clusters(xarr, yarr, valarr, tarr):
  totclarr = []
  for i in range(0, len(xarr)):
    totclarr.append(cluster_frame(xarr[i], yarr[i]))
  totxclu = []
  totyclu = []
  totvalclu = []
  tottclu = []
  totclucnt = []
  for i in range(0, len(totclarr)):
    tmpx, tmpy, tmpval = cluster_merge_frame(xarr[i], yarr[i], valarr[i], totclarr[i])
    totxclu.append(tmpx)
    totyclu.append(tmpy)
    totvalclu.append(tmpval)
    totclucnt.append(len(tmpx))
    tottclu.append(tarr[i])
  totclulen = []
  for i in range(0, len(totxclu)):
    totclulen.append(cluster_length_frame(totxclu[i], totyclu[i]))
  clulengths = []
  trackt = []
  hastrack = False
  for i in range(0, len(totclulen)):
    for j in range(0, len(totclulen[i])):
      if totclulen[i][j] > 1:
        clulengths.append(totclulen[i][j])
      if totclulen[i][j] > 10:
        hastrack = True
    if hastrack == True:
      trackt.append(tottclu[i][0])
    hastrack = False
  return (totxclu, totyclu, totvalclu, totclucnt, clulengths, trackt)

def remove_hot_pix(x, y, val, time, mult_fact):
  tmpx = []
  tmpy = []
  tmpval = []
  tmptime = []
  xocc = [0] * 500
  yocc = [0] * 500
  for i in range(0, len(x)):
    xocc[int(x[i])] += 1
    yocc[int(y[i])] += 1
  activex_pix = 0
  activey_pix = 0
  for i in range(0, len(xocc)):
    if xocc[i] > 0:
      activex_pix += 1
    if yocc[i] > 0:
      activey_pix += 1
  xavg_occ = sum(xocc) / activex_pix
  yavg_occ = sum(yocc) / activey_pix
  print 'Average x occupancy: ', xavg_occ
  print 'Average y occupancy: ', yavg_occ
  for i in range(0, len(x)):
    if float(xocc[int(x[i])]) < mult_fact*xavg_occ and float(yocc[int(y[i])]) < mult_fact*yavg_occ:
      tmpx.append(x[i])
      tmpy.append(y[i])
      tmpval.append(val[i])
      tmptime.append(time[i])
  x = tmpx
  y = tmpy
  val = tmpval
  time = tmptime
  return (x, y, val, time)

def get_data(tree):
  time = []
  val  = []
  x    = []
  y    = []
  avg3 = []
  avg5 = []
  orientx = []
  orienty = []
  orientz = []
  for evt in t:
    vval = np.array(evt.pix_val)
    vx   = np.array(evt.pix_x)
    vy   = np.array(evt.pix_y)
    vavg3 = np.array(evt.pix_avg3)
    vavg5 = np.array(evt.pix_avg5)

    for i in range(0, vval.size):
      time.append(evt.t/1E3)
      val.append(vval[i])
      x.append(vx[i])
      y.append(vy[i])
      avg3.append(vavg3[i])
      avg5.append(vavg5[i])
      #orientx.append(evt.orient_x)
      #orienty.append(evt.orient_y)
      #orientz.append(evt.orient_z)
  return (time, val, x, y, avg3, avg5, orientx, orienty, orientz)

if __name__ == "__main__":
 
  parser = argparse.ArgumentParser(description="Analyze single phone run")
  parser.add_argument('--dev', required=True, help='the device ID')
  parser.add_argument('--runfile', required=True, help='the root file for the run')
  parser.add_argument('--hotpix', default=0.5, type=float, help='remove pixels with occ > hotpix * avgocc')
  
  args = parser.parse_args()

  f = TFile(args.runfile)
  t = f.Get("events")
  time, val, x, y, avg3, avg5, orientx, orienty, orientz = get_data(t)

  #gStyle.SetOptStat(0)
  
  # Book histograms
  h0t       = TH1F("h0t",       "", 1000, 0,  5E5)
  h0val     = TH1F("h0val",     "", 100,  0,  300)
  h0x       = TH1F("h0x",       "", 500,  0,  500)
  h0y       = TH1F("h0y",       "", 500,  0,  500)
  h0xy      = TH2F("h0xy",      "", 500,  0,  500, 500, 0, 500);
  h1t       = TH1F("h1t",       "", 1000, 0,  5E5)
  h1trackt  = TH1F("h1trackt",  "", 1000, 0,  5E5)
  h1val     = TH1F("h1val",     "", 100,  0,  300)
  h1valclu  = TH1F("h1valclu",  "", 100,  0,  300)
  h1x       = TH1F("h1x",       "", 500,  0,  500)
  h1y       = TH1F("h1y",       "", 500,  0,  500)
  h1xy      = TH2F("h1xy",      "", 500,  0,  500, 500, 0, 500);
  h1xyclu   = TH2F("h1xyclu",   "", 500,  0,  500, 500, 0, 500);
  h1xybkg   = TH2F("h1xybkg",   "", 500,  0,  500, 500, 0, 500);
  h1orientx = TH1F("h1orientx", "", 60,   -5, 5)
  h1orienty = TH1F("h1orienty", "", 60,   -5, 5)
  h1orientz = TH1F("h1orientz", "", 60,   -5, 5)
  h1clulen  = TH1F("h1clulen",  "", 100,   0, 100)

  # Set t0 for all runs
  starttime = 1415577738.16
  time[:] = [t - starttime for t in time]

  for i in range(0, len(time)):
    h0t  .Fill(time[i])
    h0val.Fill(val[i])
    h0x  .Fill(x[i])
    h0y  .Fill(y[i])
    h0xy .Fill(x[i], y[i])

  x, y, val, time = remove_hot_pix(x, y, val, time, args.hotpix)
  
  for i in range(0, len(time)):
    h1t  .Fill(time[i])
    h1val.Fill(val[i])
    h1x  .Fill(x[i])
    h1y  .Fill(y[i])
    h1xy .Fill(x[i], y[i])
    #h1orientx.Fill(orientx[i])
    #h1orienty.Fill(orienty[i])
    #h1orientz.Fill(orientz[i])

  #print 'Start time:               ', min(time), 's'
  print 'Hit rate over entire run: ', rate(time, 0, 500E3), 'frames/s'
  print 'Hit frames during beam:   ', count_hits(time, 150E3, 200E3), 'frames'
  print 'Hit rate during beam:     ', rate(time, 150E3, 200E3), 'frames/s'

  # reorganize x,y,val,time into array of arrays by time stamp = frames
  xarr, yarr, valarr, tarr = sort_arrs_by_time(x, y, val, time)

  # loop through arrs and classify as above or below hit thresh
  #lowx, lowy, lowval, highx, highy, highval = sort_arrs_by_thresh(xarr, yarr, valarr, 15)

  # process clusters
  totxclu, totyclu, totvalclu, totclucnt, clulengths, trackt = process_clusters(xarr, yarr, valarr, tarr)
  
  #print min(time), ' to ', max(time)
  for i in range(0, len(clulengths)):
    if clulengths[i] > 0:
      h1clulen.Fill(clulengths[i])
  for i in range(0, len(trackt)): 
    #print trackt[i]
    h1trackt.Fill(trackt[i])
   
  #print len(totxclu)
  for i in range(0, len(totxclu)):
    for j in range(0, len(totxclu[i])):
      h1xybkg.Fill(totxclu[i][j][0], totyclu[i][j][0])
      h1valclu.Fill(totvalclu[i][j][0])

  h1xyclu = h1xy.Clone("h1xyclu")
  h1xyclu.Add(h1xybkg, -1)

  g = TFile("../results/histos.root", "update")
  h0t  .Write()
  h0val.Write()
  h0x  .Write()
  h0y  .Write()
  h0xy .Write()
  h1t  .Write()
  h1val.Write()
  h1valclu.Write()
  h1x  .Write()
  h1y  .Write()
  h1xy .Write()
  h1xyclu.Write()
  h1xybkg.Write()
  h1orientx.Write()
  h1orienty.Write()
  h1orientz.Write()
  h1clulen.Write()
  h1trackt.Write()
  g.Close()
