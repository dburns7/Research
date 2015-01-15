import numpy as np
import matplotlib.pyplot as plt
import math
import sys

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
  xtemp = []
  ytemp = []
  valtemp = []
  for i in range(0, len(time)):
    if curtime == time[i]:
      xtemp.append(x[i])
      ytemp.append(y[i])
      valtemp.append(val[i])
      continue
    curtime = time[i]
    xarr.append(xtemp)
    yarr.append(ytemp)
    valarr.append(valtemp)
    xtemp = []
    ytemp = []
    valtemp = []
    xtemp.append(x[i])
    ytemp.append(y[i])
    valtemp.append(val[i])
  #print xarr
  return (xarr, yarr, valarr)

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

def count_hits(xarr, yarr, valarr, thresh):
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

###This function uses the x and y arrays of a single frame
###to create a cluster array that has the indexes
###separated into the different clusters
###according to thier x and y positions.
###Ex: [[0,3],[1,2]] as the cluarr output
###would be x[0] and x[3] are in a cluster together
###and x[1] and x[2] are in another cluster
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

###This function takes in the indexed cluarr and uses
###it to sort the x, y, and val arrays into clusters
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

###This function takes in the clustered x and y arrays
###and loops over them to find the length of each cluster
###Output is an array of int lengths
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

def process_clusters(xarr, yarr, valarr):
  totclarr = []
  for i in range(0, len(xarr)):
    totclarr.append(cluster_frame(xarr[i], yarr[i]))
  totxclu = []
  totyclu = []
  totvalclu = []
  totclucnt = []
  for i in range(0, len(totclarr)):
    tmpx, tmpy, tmpval = cluster_merge_frame(xarr[i], yarr[i], valarr[i], totclarr[i])
    totxclu.append(tmpx)
    totyclu.append(tmpy)
    totvalclu.append(tmpval)
    totclucnt.append(len(tmpx))
  totclulen = []
  for i in range(0, len(totxclu)):
    totclulen.append(cluster_length_frame(totxclu[i], totyclu[i]))
  clulengths = []
  for i in range(0, len(totclulen)):
    for j in range(0, len(totclulen[i])):
      clulengths.append(totclulen[i][j])
  return (totclucnt, clulengths)

#ID, x, y, val, pix_avg3, pix_avg5, pix_m, time, longitude, latitude = np.loadtxt(sys.argv[1], unpack = True)
ID, x, y, val, avg3, avg5, pix_near_max, evt_ID, time, evt_gpsLon, evt_gpsLat, evt_pixAvg, evt_pixStd, evt_orientx, evt_orienty, evt_orientz, xb_, ID, xb_time, xb_L1thresh, xb_L2thresh = np.loadtxt(sys.argv[1], unpack = True)

'''
# plot the number of pixel hits
plt.figure()
h, xedges, yedges = np.histogram2d(x, y, bins = 60, range=[[0,350], [0,350]])
extent = [yedges[0], yedges[-1], xedges[-1], xedges[0]]
plt.imshow(h, extent = extent, interpolation = 'nearest')
plt.colorbar()
plt.gca().invert_yaxis()
plt.xlabel("pixel x")
plt.ylabel("pixel y")
plt.title("Number of Pixel Hits")

# plot the pixel vals
plt.figure()
nval, bins, patches = plt.hist(val, bins = 60, normed=1)
plt.semilogy()
plt.xlabel("Pixel Value")
plt.ylabel("Fraction of Events")
#plt.gca().set_yscale("log")

# plot the pixel # vs time
t = times(time)
pix = pix_per_frame(time)
plt.figure()
plt.semilogy()
plt.plot(t, pix)
plt.xlabel("Time")
plt.ylabel("Number of Hit Pixels")

# plot # pixels/frame
plt.figure()
plt.semilogy()
n, bins, patches = plt.hist(pix, 30, normed=1)
plt.xlabel("Pixels/frame")
plt.ylabel("Fraction of Events")
'''
# reorganize x,y,val,time into array of arrays by time stamp
xarr, yarr, valarr = sort_arrs_by_time(x, y, val, time)

# loop through arrs and classify as above or below hit thresh
lowx, lowy, lowval, highx, highy, highval = sort_arrs_by_thresh(xarr, yarr, valarr, 15)

# process clusters
totclucnt, clulengths = process_clusters(highx, highy, highval)
#print clulengths

# plot # clusters/frame
plt.figure()
n, bins, patches = plt.hist(totclucnt, 30, normed=1)
plt.semilogy()
plt.xlabel("Clusters/frame")
plt.ylabel("Fraction of Events")

# plt lengths of clusters
plt.figure()
n, bins, patches = plt.hist(clulengths, 60, normed=1)
plt.xlabel("Cluster Length")
plt.ylabel("Fraction of Events")


'''
# loop through high arrs and count hits above thresh
cnt = count_hits(highx, highy, highval, 15)
#print sum(cnt)
#print time[-1]-time[0]

#calculate ratio of hits to clusters per frame
ratio = []
for i in range(0, len(cnt)):
  ratio.append(cnt[i]/totclucnt[i])
plt.figure()
n, bins, patches = plt.hist(ratio, 15, normed=1)
plt.xlabel("(Hits/Clusters)/Frame")
plt.ylabel("Fraction of Events")

print len(ratio)
'''

'''
# plot pixel vals for fixed time with cluster
timestamp = 5
#print clx[timestamp]
#print clval[timestamp]
xval = []
yval = []
for i in range(0, len(clx[timestamp])):
  for j in range(0, int(clval[timestamp][i])):
    xval.append(clx[timestamp][i])
    yval.append(cly[timestamp][i])
plt.figure()
hval, xedges, yedges = np.histogram2d(xval, yval, bins = 350, range=[[0,350], [0,350]])
extent = [yedges[0], yedges[-1], xedges[-1], xedges[0]]
plt.imshow(hval, extent = extent, interpolation = 'nearest')
plt.gca().invert_yaxis()
plt.colorbar()
plt.xlabel("pixel x")
plt.ylabel("pixel y")
plt.title("Pixel Value")

# loop through frames, find frames with clusters, plot them
cnt = 0
for k in range(0, len(clx)):
  if (len(clx[k]) < 10): 
    continue 
  cnt +=1
  if cnt > 0: 
    break
  #print cnt
  xval = []
  yval = []
  for i in range(0, len(clx[k])):
    for j in range(0, int(clval[k][i])):
      xval.append(clx[k][i])
      yval.append(cly[k][i])
  plt.figure()
  hval, xedges, yedges = np.histogram2d(xval, yval, bins = 350, range=[[0,350], [0,350]])
  extent = [yedges[0], yedges[-1], xedges[-1], xedges[0]]
  plt.imshow(hval, extent = extent, interpolation = 'nearest')
  plt.gca().invert_yaxis()
  plt.colorbar()
  #plt.winter()
  plt.xlabel("pixel x")
  plt.ylabel("pixel y")
  plt.title("Pixel Value at timestamp "+str(k))
  
# plot the high and low pixel counts
_lowx = []
_lowy = []
_lowval = []
_highx = []
_highy = []
_highval = []
for i in range(0, len(lowx)):
  for j in range(0, len(lowx[i])):
    _lowx.append(lowx[i][j])
    _lowy.append(lowy[i][j])
    _lowval.append(lowval[i][j])
for i in range(0, len(highx)):
  for j in range(0, len(highx[i])):
    _highx.append(highx[i][j])
    _highy.append(highy[i][j])
    _highval.append(highval[i][j])
plt.figure()
hlow, xedges, yedges = np.histogram2d(_lowx, _lowy, bins = 60)
extent = [yedges[0], yedges[-1], xedges[-1], xedges[0]]
plt.imshow(hlow, extent = extent, interpolation = 'nearest')
plt.colorbar()
plt.xlabel("pixel x")
plt.ylabel("pixel y")
plt.title("Number of Pixel Hits")
plt.figure()
hhigh, xedges, yedges = np.histogram2d(_highx, _highy, bins = 60)
extent = [yedges[0], yedges[-1], xedges[-1], xedges[0]]
plt.imshow(hhigh, extent = extent, interpolation = 'nearest')
plt.colorbar()
plt.xlabel("pixel x")
plt.ylabel("pixel y")
plt.title("Number of Pixel Hits")
'''


plt.show()



