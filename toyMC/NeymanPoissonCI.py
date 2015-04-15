#!/usr/bin/env python

from ROOT import *
import numpy as np
import math
import sys
import argparse

def Poisson(k, lam):
  return math.pow(lam, k) * math.exp(-lam) / math.factorial(k)

def plots():
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
 

if __name__ == "__main__":
 
  S = 2
  B = 10
  D = 1
  n = 10

  lam = 0.1
  pois = []
  for i in range(0, 10):
    lam = i*lam
    pois.append(np.random.poisson(lam, n))
  
