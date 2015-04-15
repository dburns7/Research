#!/usr/bin/env python

from ROOT import *
import numpy as np
import math
import sys
import argparse

def Poisson(k, lam):
  return math.pow(lam, k) * math.exp(-lam) / math.factorial(k)

def LLR(D, S, B):
  return -math.log( Poisson(D, S+B) / Poisson(D, B) )

def LLR_B(S, B, n):
  vLLR_B = []
  for i in range(0, n):
    D_B = np.random.poisson(B)
    vLLR_B.append(LLR(D_B, S, B))
  return vLLR_B

def LLR_SB(S, B, n):
  vLLR_SB = []
  for i in range(0, n):
    D_SB = np.random.poisson(S+B)
    vLLR_SB.append(LLR(D_SB, S, B))
  return vLLR_SB

def CL(h, D):
  axis = h.GetXaxis()
  xmin = D
  xmax = 5000
  bmin = axis.FindBin(xmin)
  bmax = axis.FindBin(xmax)
  CL_B = h.Integral(bmin, bmax)
  CL_B -= h.GetBinContent(bmin) * (xmin - axis.GetBinLowEdge(bmin)) / axis.GetBinWidth(bmin)
  CL_B -= h.GetBinContent(bmax) * (axis.GetBinUpEdge(bmax) - xmax)  / axis.GetBinWidth(bmax)
  return CL_B

def CL_S(S, B, D, n, plots):
  vLLR_B  = LLR_B (S, B, n)
  vLLR_SB = LLR_SB(S, B, n)

  hLLR_B  = TH1F("hLLR_B",  "", 55, -5, 5)
  hLLR_SB = TH1F("hLLR_SB", "", 55, -5, 5)

  for i in range(0, n): 
    hLLR_B. Fill(vLLR_B[i])
    hLLR_SB.Fill(vLLR_SB[i])

  hLLR_B .Scale(1/hLLR_B .Integral())
  hLLR_SB.Scale(1/hLLR_SB.Integral())

  CL_B  = CL(hLLR_B,  D)
  CL_SB = CL(hLLR_SB, D)
  CL_S = CL_SB / CL_B
  print "CL_B:  ", CL_B
  print "CL_SB: ", CL_SB
  print "CL_S:  ", CL_S
  
  if plots:
    gStyle.SetOptStat(0)
    c1 = TCanvas()
    leg1 = TLegend(0.60, 0.65, 0.75, 0.85);
    c1.cd()
    hLLR_B.SetLineColor(kBlue);
    hLLR_B.Draw()
    hLLR_SB.SetLineColor(kRed);
    hLLR_SB.Draw("HSAME")
    l1 = TLine(D, 0, D, 0.1);
    l1.SetLineWidth(3);
    l1.Draw();
    leg1.AddEntry(hLLR_B, "Background", "l")
    leg1.AddEntry(hLLR_SB, "Signal+Background", "l")
    leg1.AddEntry(l1, "Data", "l")
    leg1.SetFillStyle(0)
    leg1.SetBorderSize(0)
    leg1.SetTextSize(0.04)
    leg1.Draw()
    c1.SaveAs("LLRs.png")

    g = TFile("limitplots.root", "update")
    hLLR_B  .Write()
    hLLR_SB .Write()
    g.Close()
 
  return CL_S

def k_sigma(S_0, B, D, n, alpha, eps):
  k = CL_S(S, B, D, n, False)
  return k

if __name__ == "__main__":
 
  #parser = argparse.ArgumentParser(description="Analyze single phone run")
  #parser.add_argument('--dev', required=True, help='the device ID')
  #parser.add_argument('--hotpix', default=0.5, type=float, help='remove pixels with occ > hotpix * avgocc')
  
  #args = parser.parse_args()

  S = 2
  B = 10
  D = 1
  n = 100000

  k = 1
  CL_S = CL_S(k*S, B, D, n, True)

