enum {MYCYAN1     = kCyan-4};
enum {MYCYAN2     = kCyan-3};
enum {MYGREEN1    = kGreen-4};
enum {MYGREEN2    = kGreen-3};
enum {MYORANGE1   = kOrange-4};
enum {MYORANGE2   = kOrange-3};
enum {MYMAGENTA1  = kMagenta};
enum {MYMAGENTA2  = kMagenta+2};
enum {MYBLUE1    = kBlue};
enum {MYBLUE2    = kBlue-2};
enum {MYRED1     = kRed};
enum {MYRED2     = kRed-2};

enum {FILL0 = 1001, FILL1 = 3004, FILL2 = 3013, FILL3 = 3005, FILL4=3014, FILL4 = 3007}; 
enum {FILLSIG1 = 3002, FILLSIG2 = 3003};

enum {LSOLID=1, LDASHED=2};


TH1F * AddHist(THStack *stack, TLegend * leg,  TFile *f, const char * name, const char * sample, const char * legsamp, int style, int color, int & n){
   char full_name[200];
   sprintf(full_name, "%s_%s", name, sample);
   TH1F * h = f->Get(full_name);
   if (h){
      h->SetFillStyle(style);
      h->SetLineColor(color);
      h->SetFillColor(color);
      n += 1;
      if (leg) { leg->AddEntry(h, legsamp, "f"); }
      //if (leg && (n==1 || n==9)) { leg->AddEntry(h, legsamp, "f"); }
      stack->Add(h);
   }
   return h;
}

THStack * GetBkgStack(TFile *f, const char * name, const char * xtitle, TLegend * leg, int & n, int mode==0){
   char full_name[200];
   
   if (leg) {
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
   }

   sprintf(full_name, "stack_%s", name);
   THStack * stack = new THStack(full_name, "");

   TH1F * h_base = f->Get(name);
   h_base->SetXTitle(xtitle);
   stack->SetHistogram(h_base);
   if (mode == 0)
   {
      AddHist(stack, leg, f, name, "ggZZ2l2l", "ggZZ2l2l", FILL0, MYORANGE2,  n);
      AddHist(stack, leg, f, name, "4e",       "4e", FILL0, MYGREEN2,   n);
      AddHist(stack, leg, f, name, "4mu",      "4mu", FILL0, MYORANGE1,  n);
      AddHist(stack, leg, f, name, "4tau",     "4tau", FILL0, MYMAGENTA1, n);
      AddHist(stack, leg, f, name, "ggZZ4l",   "ggZZ4l", FILL0, MYMAGENTA2, n);
      AddHist(stack, leg, f, name, "2e2mu",    "2e2mu", FILL0, MYCYAN1,    n);
      AddHist(stack, leg, f, name, "2e2tau",   "2e2tau", FILL0, MYGREEN1,   n);
      AddHist(stack, leg, f, name, "2mu2tau",  "2mu2tau", FILL0, MYCYAN2,    n);
   }
   return stack;
}

TH1F * GetSigHist(TFile *f, const char * name, const char * tag, const char * xtitle, int color, int lstyle, TLegend * leg, const char * legtag){
   char full_name[200];

   sprintf(full_name, "%s_%s", name, tag);
   TH1F * h = f->Get(full_name);

   if(h){
     h->SetFillStyle(0);
     h->SetFillColor(color);
     h->SetLineColor(color);   
     h->SetLineStyle(lstyle);   
     h->SetXTitle(xtitle);
     if (leg) { leg->AddEntry(h, legtag, "l"); }  
   }

   return h;
}

mkplots(){
   gStyle->SetOptStat(0);

   TFile * f = new TFile("latest.root");
   
   TCanvas * c1 = new TCanvas("c1", "");
   TLegend * leg1 = new TLegend(0.15, 0.56, 0.35, 0.85);
   c1->SetLogy();
   c1->cd();
   int n = 0;
   THStack * hmet_mc   = GetBkgStack(f, "histPFMET", "Missing Transverse Energy [GeV]", leg1, n);
   TH1F    * hmet_data = GetSigHist(f, "histPFMET", "data", "Missing Transverse Energy [GeV]", MYRED1, LSOLID, leg1, "Data");
   hmet_data->Draw("EP");
   hmet_mc->Draw("HSAME");
   hmet_data->Draw("EPSAME");
   leg1->Draw();
   c1->SaveAs("met.png");

   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.7, 0.56, 0.9, 0.85);
   c2->SetLogy();
   c2->cd();
   n = 0;
   THStack * hmet_ctrl_mc   = GetBkgStack(f, "histPFMETctrl", "Missing Transverse Energy [GeV]", leg2, n);
   TH1F    * hmet_ctrl_data = GetSigHist(f, "histPFMETctrl", "data", "Missing Transverse Energy [GeV]", MYRED1, LSOLID, leg2, "Data");
   n = 0;
   hmet_ctrl_data->Draw("EP");
   hmet_ctrl_mc->Draw("HSAME");
   hmet_ctrl_data->Draw("EPSAME");
   leg2->Draw();
   c2->SaveAs("met_ctrl.png");

   TCanvas * c3 = new TCanvas("c3", "");
   TLegend * leg3 = new TLegend(0.7, 0.56, 0.9, 0.85);
   c3->SetLogy();
   c3->cd();
   n = 0;
   THStack * hmllll_mc   = GetBkgStack(f, "histZZMass", "Four Lepton Invariant Mass [GeV]", leg3, n);
   TH1F    * hmllll_data = GetSigHist(f, "histZZMass", "data", "Four Lepton Invariant Mass [GeV]", MYRED1, LSOLID, leg3, "Data");
   hmllll_data->Draw("EP");
   hmllll_mc->Draw("HSAME");
   hmllll_data->Draw("EPSAME");
   leg3->Draw();
   c3->SaveAs("mllll.png");
  
   TCanvas * c4 = new TCanvas("c4", "");
   TLegend * leg4 = new TLegend(0.7, 0.56, 0.9, 0.85);
   c4->SetLogy();
   c4->cd();
   n = 0;
   THStack * hmz1_mc   = GetBkgStack(f, "histZ1Mass", "Leading Dilepton Invariant Mass [GeV]", leg4, n);
   TH1F    * hmz1_data = GetSigHist(f, "histZ1Mass", "data", "Leading Dilepton Invariant Mass [GeV]", MYRED1, LSOLID, leg4, "Data");
   hmz1_data->Draw("EP");
   hmz1_mc->Draw("HSAME");
   hmz1_data->Draw("EPSAME");
   leg4->Draw();
   c4->SaveAs("mz1.png");

   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.7, 0.56, 0.9, 0.85);
   c5->SetLogy();
   c5->cd();
   n = 0;
   THStack * hmz2_mc   = GetBkgStack(f, "histZ2Mass", "Subleading Dilepton Invariant Mass [GeV]", leg5, n);
   TH1F    * hmz2_data = GetSigHist(f, "histZ2Mass", "data", "Subleading Dilepton Invariant Mass [GeV]", MYRED1, LSOLID, leg5, "Data");
   hmz2_data->Draw("EP");
   hmz2_mc->Draw("HSAME");
   hmz2_data->Draw("EPSAME");
   leg5->Draw();
   c5->SaveAs("mz2.png");













   return;
   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.6, 0.65, 0.80, 0.90);
   c2->SetLogy();
   c2->cd();
   THStack * hmet_bkg = GetBkgStack(f, "h1met_nopu", "Missing Transverse Energy [GeV]", leg2);
   TH1F    * hmet_sig1 = GetSigHist(f, "h1met_nopu", "hxx_100GeV", MYRED1, LSOLID, leg2, "HXX (100 GeV)");
   hmet_sig1->SetAxisRange(75., 250.,"X");
   hmet_bkg->Draw();
   hmet_bkg->GetXaxis()->SetLimits(75., 250.);
   hmet_sig1->Draw("HSAME");
   leg2->Draw();
   c2->SaveAs("met_postcuts.png");


   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.6, 0.65, 0.80, 0.90);
   leg5->SetFillStyle(0);
   leg5->SetBorderSize(0);
   leg5->SetTextSize(0.04);
   c5->cd();
   TH1F    * h0mllll_hxx_1GeV       = GetSigHist(f, "h0mllll", "hxx_100GeV",       MYRED1,     LSOLID, leg5, "HXX (100 GeV)");
   TH1F    * h0mllll_h_zz_4l        = GetSigHist(f, "h0mllll", "h_zz_4l",        MYGREEN2,   LSOLID, leg5, "H > ZZ > 4L");
   TH1F    * h0mllll_wh_zh_tth_hzz  = GetSigHist(f, "h0mllll", "wh_zh_tth_hzz",  MYCYAN1,    LSOLID, leg5, "HW HW Htt, H > ZZ");
   TH1F    * h0mllll_zz_4l          = GetSigHist(f, "h0mllll", "zz_4l",          MYBLUE2,    LSOLID, leg5, "ZZ > 4L");
   TH1F    * h0mllll_wh_zh_tth_hww  = GetSigHist(f, "h0mllll", "wh_zh_tth_hww",  MYMAGENTA1, LSOLID, leg5, "HW HW Htt, H > WW");
   h0mllll_hxx_1GeV->Scale(1/h0mllll_hxx_1GeV->Integral());
   h0mllll_h_zz_4l->Scale(1/h0mllll_h_zz_4l->Integral());
   h0mllll_wh_zh_tth_hzz->Scale(1/h0mllll_wh_zh_tth_hzz->Integral());
   h0mllll_zz_4l->Scale(1/h0mllll_zz_4l->Integral());
   h0mllll_wh_zh_tth_hww->Scale(1/h0mllll_wh_zh_tth_hww->Integral());
   h0mllll_hxx_1GeV->GetXaxis()->SetTitle("Four Lepton Mass [GeV]");
   h0mllll_h_zz_4l->GetXaxis()->SetTitle("Four Lepton Mass [GeV]");
   h0mllll_wh_zh_tth_hzz->GetXaxis()->SetTitle("Four Lepton Mass [GeV]");
   h0mllll_zz_4l->GetXaxis()->SetTitle("Four Lepton Mass [GeV]");
   h0mllll_wh_zh_tth_hww->GetXaxis()->SetTitle("Four Lepton Mass [GeV]");
   h0mllll_hxx_1GeV->GetYaxis()->SetTitle("Fraction of Events");
   h0mllll_h_zz_4l->GetYaxis()->SetTitle("Fraction of Events");
   h0mllll_wh_zh_tth_hzz->GetYaxis()->SetTitle("Fraction of Events");
   h0mllll_zz_4l->GetYaxis()->SetTitle("Fraction of Events");
   h0mllll_wh_zh_tth_hww->GetYaxis()->SetTitle("Fraction of Events");
   h0mllll_hxx_1GeV->SetAxisRange(80., 200.,"X");
   h0mllll_h_zz_4l->SetAxisRange(80., 200.,"X");
   h0mllll_wh_zh_tth_hzz->SetAxisRange(80., 200.,"X");
   h0mllll_zz_4l->SetAxisRange(80., 200.,"X");
   h0mllll_wh_zh_tth_hww->SetAxisRange(80., 200.,"X");
   h0mllll_hxx_1GeV->Draw();
   h0mllll_h_zz_4l->Draw("HSAME");
   h0mllll_wh_zh_tth_hzz->Draw("HSAME");
   h0mllll_zz_4l->Draw("HSAME");
   h0mllll_wh_zh_tth_hww->Draw("HSAME");
   leg5->Draw();
   c5->SaveAs("plots/mllll.png");

   TCanvas * c6 = new TCanvas("c6", "");
   TLegend * leg6 = new TLegend(0.6, 0.65, 0.80, 0.90);
   leg6->SetFillStyle(0);
   leg6->SetBorderSize(0);
   leg6->SetTextSize(0.04);
   c6->cd();
   TH1F    * h0met_nopu_hxx_1GeV       = GetSigHist(f, "h0met_nopu", "hxx_100GeV",       MYRED1,     LSOLID, leg6, "HXX (100 GeV)");
   TH1F    * h0met_nopu_h_zz_4l        = GetSigHist(f, "h0met_nopu", "h_zz_4l",        MYGREEN2,   LSOLID, leg6, "H > ZZ > 4L");
   TH1F    * h0met_nopu_wh_zh_tth_hzz  = GetSigHist(f, "h0met_nopu", "wh_zh_tth_hzz",  MYCYAN1,    LSOLID, leg6, "HW HW Htt, H > ZZ");
   TH1F    * h0met_nopu_zz_4l          = GetSigHist(f, "h0met_nopu", "zz_4l",          MYBLUE2,    LSOLID, leg6, "ZZ > 4L");
   TH1F    * h0met_nopu_wh_zh_tth_hww  = GetSigHist(f, "h0met_nopu", "wh_zh_tth_hww",  MYMAGENTA1, LSOLID, leg6, "HW HW Htt, H > WW");
   h0met_nopu_hxx_1GeV->Scale(1/h0met_nopu_hxx_1GeV->Integral());
   h0met_nopu_h_zz_4l->Scale(1/h0met_nopu_h_zz_4l->Integral());
   h0met_nopu_wh_zh_tth_hzz->Scale(1/h0met_nopu_wh_zh_tth_hzz->Integral());
   h0met_nopu_zz_4l->Scale(1/h0met_nopu_zz_4l->Integral());
   h0met_nopu_wh_zh_tth_hww->Scale(1/h0met_nopu_wh_zh_tth_hww->Integral());
   h0met_nopu_hxx_1GeV->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_h_zz_4l->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_wh_zh_tth_hzz->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_zz_4l->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_wh_zh_tth_hww->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_hxx_1GeV->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_h_zz_4l->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_wh_zh_tth_hzz->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_zz_4l->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_wh_zh_tth_hww->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_zz_4l->Draw();
   h0met_nopu_hxx_1GeV->Draw("HSAME");
   h0met_nopu_h_zz_4l->Draw("HSAME");
   h0met_nopu_wh_zh_tth_hzz->Draw("HSAME");
   h0met_nopu_wh_zh_tth_hww->Draw("HSAME");
   leg6->Draw();
   c6->SaveAs("plots/met_unitnorm.png");



}
