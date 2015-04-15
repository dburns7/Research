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

enum {FILL0 = 1001, FILL1 = 3004, FILL2 = 3013, FILL3 = 3005, FILL4=3014, FILL5 = 3007}; 
enum {FILLSIG1 = 3002, FILLSIG2 = 3003};

enum {LSOLID=1, LDASHED=2};


TH1F * AddHist(THStack *stack, TLegend * leg,  TFile *f, const char * name, const char * sample, const char * legsamp, int style, int color){
   char full_name[200];
   sprintf(full_name, "%s_%s", name, sample);
   TH1F * h = (TH1F *)f->Get(full_name);
   if (h){
      h->SetFillStyle(style);
      h->SetLineColor(color);
      h->SetFillColor(color);
      if (leg) { leg->AddEntry(h, legsamp, "f"); }
      stack->Add(h);
   }
   return h;
}

THStack * GetBkgStack(TFile *f, const char * name, const char * xtitle, TLegend * leg, int mode=0){
   char full_name[200];

   if (leg) {
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
   }

   sprintf(full_name, "stack_%s", name);
   THStack * stack = new THStack(full_name, "");

   TH1F * h_base = (TH1F *)f->Get(name);
   h_base->SetXTitle(xtitle);
   stack->SetHistogram(h_base);

   if (mode == 0)
   {
/*
      AddHist(stack, leg, f, name, "vbh_h_zz_4l",   "VV > H > ZZ > 4L",  FILL0, MYGREEN2);
      AddHist(stack, leg, f, name, "gg_zz_4l",      "gg > ZZ > 4L",      FILL0, MYORANGE1);
      AddHist(stack, leg, f, name, "gg_zz_2l2l",    "gg > ZZ > 2L2L",    FILL0, MYBLUE1);
      AddHist(stack, leg, f, name, "qq_zz_2e2mu",   "qq > ZZ > 2e2mu",   FILL0, MYCYAN2);
      AddHist(stack, leg, f, name, "gg_h_zz_4l",    "gg > H > ZZ > 4L",  FILL0, MYMAGENTA2);
      AddHist(stack, leg, f, name, "qq_zz_4e",      "qq > ZZ > 4e",      FILL0, MYGREEN1);   
      AddHist(stack, leg, f, name, "qq_zz_4mu",     "qq > ZZ > 4mu",     FILL0, MYORANGE2);  
      AddHist(stack, leg, f, name, "wh_zh_tth_hzz", "HW HW Htt, H > ZZ", FILL0, MYCYAN1);   
      AddHist(stack, leg, f, name, "wh_zh_tth_hww", "HW HZ Htt, H > WW", FILL0, MYMAGENTA1);
*/
      AddHist(stack, leg, f, name, "zh_hzz",  "HZ, H > ZZ",  FILL0, MYCYAN1);   
      AddHist(stack, leg, f, name, "zh_hww",  "HZ, H > WW",  FILL0, MYMAGENTA1);
      AddHist(stack, leg, f, name, "h_zz_4l", "H > ZZ > 4L", FILL0, MYGREEN2);
      AddHist(stack, leg, f, name, "zz_4l",   "ZZ > 4L",     FILL0, MYBLUE2);
   }
   return stack;
}

TH1F * GetSigHist(TFile *f, const char * name, const char * tag, int color, int lstyle, TLegend * leg, const char * legtag){
   char full_name[200];

   sprintf(full_name, "%s_%s", name, tag);
   TH1F * h = (TH1F *)f->Get(full_name);

   h->SetFillStyle(0);
   h->SetFillColor(color);
   h->SetLineColor(color);   
   h->SetLineStyle(lstyle);   
   if (leg) { leg->AddEntry(h, legtag, "l"); }  

   return h;
}

int mkplots_13tev(){
   
   gStyle->SetOptStat(0);

   double basemin = 0.001;

   TFile * f = new TFile("latest_13tev.root");
   //f->ls();
   
   TCanvas * c1 = new TCanvas("c1", "");
   TLegend * leg1 = new TLegend(0.6, 0.65, 0.80, 0.90);
   c1->SetLogy();
   c1->cd();
   THStack * hmet_bkg = GetBkgStack(f, "h0met_nopu", "Missing Transverse Energy [GeV]", leg1);
   TH1F    * hmet_sig1 = GetSigHist(f, "h0met_nopu", "hxx_1GeV", MYRED1, LSOLID, leg1, "HXX (1 GeV, 1 fb)");
   //hmet_sig1->Scale(100);
   hmet_sig1->SetLineWidth(2);
   hmet_sig1->Draw("HSAME");
   hmet_bkg->Draw("HSAME");
   hmet_sig1->Draw("HSAME");
   leg1->Draw();
   hmet_sig1->SetAxisRange(1E-4, 5E2, "Y");
   hmet_sig1->GetXaxis()->SetTitle("Missing Transverse Energy [GeV]");
   c1->SaveAs("met_precuts.png");

   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.6, 0.65, 0.80, 0.90);
   c2->SetLogy();
   c2->cd();
   /*TH2F * hframe = new TH2F("hframe", "", 10, 2E18, 1E21, 10, 0.0, 0.75);
   hframe->GetXaxis()->SetTitle("Primary Energy [eV]");
   hframe->GetXaxis()->SetTitleOffset(1.3);
   hframe->GetYaxis()->SetTitle("Fractional Energy Resolution");
   hframe->GetYaxis()->SetTitleOffset(1.2);
   hframe->Draw();*/
   THStack * hmet_bkg2 = GetBkgStack(f, "h1met_nopu", "Missing Transverse Energy [GeV]", leg2);
   TH1F    * hmet_sig12 = GetSigHist(f, "h1met_nopu", "hxx_1GeV", MYRED1, LSOLID, leg2, "HXX (1 GeV, 1 fb)");
   //hmet_sig12->Scale(100);
   hmet_sig12->SetLineWidth(2);
   hmet_sig12->Draw();
   hmet_bkg2->Draw("HSAME");
   hmet_sig12->Draw("HSAME");
   leg2->Draw();
   hmet_sig12->SetAxisRange(1E-4, 150.,"Y");
   hmet_sig12->SetAxisRange(125., 250.,"X");
   hmet_sig12->GetXaxis()->SetTitle("Missing Transverse Energy [GeV]");
   //hmet_sig1->GetXaxis()->SetTitleSize(0.5);
   //hmet_bkg->GetXaxis()->SetRange(75., 250.);
   //hmet_bkg->GetYaxis()->SetRange(1E-4, 150.);
   c2->SaveAs("met_postcuts.png");

   TCanvas * c3 = new TCanvas("c3", "");
   TLegend * leg3 = new TLegend(0.15, 0.65, 0.35, 0.90);
   leg3->SetFillStyle(0);
   leg3->SetBorderSize(0);
   leg3->SetTextSize(0.04);
   c3->cd();
   TH1F    * h0mll_l_hxx_1GeV       = GetSigHist(f, "h0mll_l", "hxx_1GeV",       MYRED1,     LSOLID, leg3, "HXX (1 GeV, 1 fb)");
   TH1F    * h0mll_l_h_zz_4l        = GetSigHist(f, "h0mll_l", "h_zz_4l",        MYGREEN2,   LSOLID, leg3, "H > ZZ > 4L");
   TH1F    * h0mll_l_wh_zh_tth_hzz  = GetSigHist(f, "h0mll_l", "zh_hzz",  MYCYAN1,    LSOLID, leg3, "HZ, H > ZZ");
   TH1F    * h0mll_l_zz_4l          = GetSigHist(f, "h0mll_l", "zz_4l",          MYBLUE2,    LSOLID, leg3, "ZZ > 4L");
   TH1F    * h0mll_l_wh_zh_tth_hww  = GetSigHist(f, "h0mll_l", "zh_hww",  MYMAGENTA1, LSOLID, leg3, "HZ, H > WW");
   h0mll_l_hxx_1GeV->Scale(1/h0mll_l_hxx_1GeV->Integral());
   h0mll_l_h_zz_4l->Scale(1/h0mll_l_h_zz_4l->Integral());
   h0mll_l_wh_zh_tth_hzz->Scale(1/h0mll_l_wh_zh_tth_hzz->Integral());
   h0mll_l_zz_4l->Scale(1/h0mll_l_zz_4l->Integral());
   h0mll_l_wh_zh_tth_hww->Scale(1/h0mll_l_wh_zh_tth_hww->Integral());
   h0mll_l_hxx_1GeV->GetXaxis()->SetTitle("Leading Dilepton Mass [GeV]");
   h0mll_l_h_zz_4l->GetXaxis()->SetTitle("Leading Dilepton Mass [GeV]");
   h0mll_l_wh_zh_tth_hzz->GetXaxis()->SetTitle("Leading Dilepton Mass [GeV]");
   h0mll_l_zz_4l->GetXaxis()->SetTitle("Leading Dilepton Mass [GeV]");
   h0mll_l_wh_zh_tth_hww->GetXaxis()->SetTitle("Leading Dilepton Mass [GeV]");
   h0mll_l_hxx_1GeV->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_l_h_zz_4l->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_l_wh_zh_tth_hzz->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_l_zz_4l->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_l_wh_zh_tth_hww->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_l_hxx_1GeV->SetAxisRange(0., 110.,"X");
   h0mll_l_h_zz_4l->SetAxisRange(0., 110.,"X");
   h0mll_l_wh_zh_tth_hzz->SetAxisRange(0., 110.,"X");
   h0mll_l_zz_4l->SetAxisRange(0., 110.,"X");
   h0mll_l_wh_zh_tth_hww->SetAxisRange(0., 110.,"X");
   h0mll_l_hxx_1GeV->SetAxisRange(0., 0.25,"Y");
   h0mll_l_h_zz_4l->SetAxisRange(0., 0.25,"Y");
   h0mll_l_wh_zh_tth_hzz->SetAxisRange(0., 0.25,"Y");
   h0mll_l_zz_4l->SetAxisRange(0., 0.25,"Y");
   h0mll_l_wh_zh_tth_hww->SetAxisRange(0., 0.25,"Y");
   /*h0mll_l_hxx_1GeV->
   h0mll_l_h_zz_4l->
   h0mll_l_wh_zh_tth_hzz->
   h0mll_l_zz_4l->
   h0mll_l_wh_zh_tth_hww->*/
   h0mll_l_hxx_1GeV->SetLineWidth(2);
   h0mll_l_h_zz_4l->SetLineWidth(2);
   h0mll_l_wh_zh_tth_hzz->SetLineWidth(2);
   h0mll_l_zz_4l->SetLineWidth(2);
   h0mll_l_wh_zh_tth_hww->SetLineWidth(2);
   h0mll_l_hxx_1GeV->SetFillColor(kRed);
   h0mll_l_hxx_1GeV->SetFillStyle(3005);
   h0mll_l_hxx_1GeV->Draw();
   h0mll_l_h_zz_4l->Draw("HSAME");
   h0mll_l_wh_zh_tth_hzz->Draw("HSAME");
   h0mll_l_zz_4l->Draw("HSAME");
   h0mll_l_wh_zh_tth_hww->Draw("HSAME");
   h0mll_l_hxx_1GeV->Draw("HSAME");
   leg3->Draw();
   TLine *l1 = new TLine(50, 0, 50, 0.1);
   //l1->SetLineColor(kRed);
   l1->SetLineWidth(3);
   l1->Draw();
   TLine *l2 = new TLine(106, 0, 106, 0.1);
   l2->SetLineWidth(3);
   l2->Draw();
   c3->SaveAs("mll_l.png");

   TCanvas * c4 = new TCanvas("c4", "");
   TLegend * leg4 = new TLegend(0.6, 0.65, 0.80, 0.90);
   leg4->SetFillStyle(0);
   leg4->SetBorderSize(0);
   leg4->SetTextSize(0.04);
   c4->cd();
   TH1F    * h0mll_sl_hxx_1GeV       = GetSigHist(f, "h0mll_sl", "hxx_1GeV",       MYRED1,     LSOLID, leg4, "HXX (1 GeV, 1 fb)");
   TH1F    * h0mll_sl_h_zz_4l        = GetSigHist(f, "h0mll_sl", "h_zz_4l",        MYGREEN2,   LSOLID, leg4, "H > ZZ > 4L");
   TH1F    * h0mll_sl_wh_zh_tth_hzz  = GetSigHist(f, "h0mll_sl", "zh_hzz",  MYCYAN1,    LSOLID, leg4, "HZ, H > ZZ");
   TH1F    * h0mll_sl_zz_4l          = GetSigHist(f, "h0mll_sl", "zz_4l",          MYBLUE2,    LSOLID, leg4, "ZZ > 4L");
   TH1F    * h0mll_sl_wh_zh_tth_hww  = GetSigHist(f, "h0mll_sl", "zh_hww",  MYMAGENTA1, LSOLID, leg4, "HZ, H > WW");
   h0mll_sl_hxx_1GeV->Scale(1/h0mll_sl_hxx_1GeV->Integral());
   h0mll_sl_h_zz_4l->Scale(1/h0mll_sl_h_zz_4l->Integral());
   h0mll_sl_wh_zh_tth_hzz->Scale(1/h0mll_sl_wh_zh_tth_hzz->Integral());
   h0mll_sl_zz_4l->Scale(1/h0mll_sl_zz_4l->Integral());
   h0mll_sl_wh_zh_tth_hww->Scale(1/h0mll_sl_wh_zh_tth_hww->Integral());
   h0mll_sl_hxx_1GeV->GetXaxis()->SetTitle("Subleading Dilepton Mass [GeV]");
   h0mll_sl_h_zz_4l->GetXaxis()->SetTitle("Subleading Dilepton Mass [GeV]");
   h0mll_sl_wh_zh_tth_hzz->GetXaxis()->SetTitle("Subleading Dilepton Mass [GeV]");
   h0mll_sl_zz_4l->GetXaxis()->SetTitle("Subleading Dilepton Mass [GeV]");
   h0mll_sl_wh_zh_tth_hww->GetXaxis()->SetTitle("Subleading Dilepton Mass [GeV]");
   h0mll_sl_hxx_1GeV->SetAxisRange(0., 110.,"X");
   h0mll_sl_h_zz_4l->SetAxisRange(0., 110.,"X");
   h0mll_sl_wh_zh_tth_hzz->SetAxisRange(0., 110.,"X");
   h0mll_sl_zz_4l->SetAxisRange(0., 110.,"X");
   h0mll_sl_wh_zh_tth_hww->SetAxisRange(0., 110.,"X");
   h0mll_sl_hxx_1GeV->SetAxisRange(0., 0.35,"Y");
   h0mll_sl_h_zz_4l->SetAxisRange(0., 0.35,"Y");
   h0mll_sl_wh_zh_tth_hzz->SetAxisRange(0., 0.35,"Y");
   h0mll_sl_zz_4l->SetAxisRange(0., 0.35,"Y");
   h0mll_sl_wh_zh_tth_hww->SetAxisRange(0., 0.35,"Y");
   h0mll_sl_hxx_1GeV->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_sl_h_zz_4l->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_sl_wh_zh_tth_hzz->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_sl_zz_4l->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_sl_wh_zh_tth_hww->GetYaxis()->SetTitle("Fraction of Events");
   h0mll_sl_hxx_1GeV->SetLineWidth(2);
   h0mll_sl_h_zz_4l->SetLineWidth(2);
   h0mll_sl_wh_zh_tth_hzz->SetLineWidth(2);
   h0mll_sl_zz_4l->SetLineWidth(2);
   h0mll_sl_wh_zh_tth_hww->SetLineWidth(2);
   h0mll_sl_hxx_1GeV->SetFillColor(kRed);
   h0mll_sl_hxx_1GeV->SetFillStyle(3005);
   h0mll_sl_hxx_1GeV->Draw();
   h0mll_sl_h_zz_4l->Draw("HSAME");
   h0mll_sl_wh_zh_tth_hzz->Draw("HSAME");
   h0mll_sl_zz_4l->Draw("HSAME");
   h0mll_sl_wh_zh_tth_hww->Draw("HSAME");
   h0mll_sl_wh_zh_tth_hzz->Draw();
   h0mll_sl_hxx_1GeV->Draw("HSAME");
   h0mll_sl_h_zz_4l->Draw("HSAME");
   h0mll_sl_zz_4l->Draw("HSAME");
   h0mll_sl_wh_zh_tth_hww->Draw("HSAME");
   leg4->Draw();
   TLine *l3 = new TLine(12, 0, 12, 0.1);
   l3->SetLineWidth(3);
   l3->Draw();
   TLine *l4 = new TLine(110, 0, 110, 0.1);
   l4->SetLineWidth(3);
   l4->Draw();
   c4->SaveAs("mll_sl.png");

   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.6, 0.65, 0.80, 0.90);
   leg5->SetFillStyle(0);
   leg5->SetBorderSize(0);
   leg5->SetTextSize(0.04);
   c5->cd();
   TH1F    * h0mllll_hxx_1GeV       = GetSigHist(f, "h0mllll", "hxx_1GeV",       MYRED1,     LSOLID, leg5, "HXX (1 GeV, 1 fb)");
   TH1F    * h0mllll_h_zz_4l        = GetSigHist(f, "h0mllll", "h_zz_4l",        MYGREEN2,   LSOLID, leg5, "H > ZZ > 4L");
   TH1F    * h0mllll_wh_zh_tth_hzz  = GetSigHist(f, "h0mllll", "zh_hzz",  MYCYAN1,    LSOLID, leg5, "HZ, H > ZZ");
   TH1F    * h0mllll_zz_4l          = GetSigHist(f, "h0mllll", "zz_4l",          MYBLUE2,    LSOLID, leg5, "ZZ > 4L");
   TH1F    * h0mllll_wh_zh_tth_hww  = GetSigHist(f, "h0mllll", "zh_hww",  MYMAGENTA1, LSOLID, leg5, "HZ, H > WW");
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
   h0mllll_hxx_1GeV->SetAxisRange(0., .25,"Y");
   h0mllll_h_zz_4l->SetAxisRange(0., .25,"Y");
   h0mllll_wh_zh_tth_hzz->SetAxisRange(0., .25,"Y");
   h0mllll_zz_4l->SetAxisRange(0., .25,"Y");
   h0mllll_wh_zh_tth_hww->SetAxisRange(80., .25,"Y");
   h0mllll_hxx_1GeV->SetLineWidth(2);
   h0mllll_h_zz_4l->SetLineWidth(2);
   h0mllll_wh_zh_tth_hzz->SetLineWidth(2);
   h0mllll_zz_4l->SetLineWidth(2);
   h0mllll_wh_zh_tth_hww->SetLineWidth(2);
   h0mllll_hxx_1GeV->SetFillColor(kRed);
   h0mllll_hxx_1GeV->SetFillStyle(3005);
   h0mllll_hxx_1GeV->Draw();
   h0mllll_h_zz_4l->Draw("HSAME");
   h0mllll_wh_zh_tth_hzz->Draw("HSAME");
   h0mllll_zz_4l->Draw("HSAME");
   h0mllll_wh_zh_tth_hww->Draw("HSAME");
   h0mllll_hxx_1GeV->Draw("HSAME");
   leg5->Draw();
   TLine *l5 = new TLine(105, 0, 105, 0.15);
   l5->SetLineWidth(3);
   l5->Draw();
   TLine *l6 = new TLine(145, 0, 145, 0.15);
   l6->SetLineWidth(3);
   l6->Draw();
   c5->SaveAs("mllll.png");
   
   
   TCanvas * c6 = new TCanvas("c6", "");
   TLegend * leg6 = new TLegend(0.6, 0.65, 0.80, 0.90);
   leg6->SetFillStyle(0);
   leg6->SetBorderSize(0);
   leg6->SetTextSize(0.04);
   c6->cd();
   TH1F    * h0met_nopu_hxx_1GeV       = GetSigHist(f, "h0met_nopu", "hxx_1GeV",       MYRED1,     LSOLID, leg6, "HXX (1 GeV, 1 fb)");
   TH1F    * h0met_nopu_h_zz_4l        = GetSigHist(f, "h0met_nopu", "h_zz_4l",        MYGREEN2,   LSOLID, leg6, "H > ZZ > 4L");
   TH1F    * h0met_nopu_wh_zh_tth_hzz  = GetSigHist(f, "h0met_nopu", "zh_hzz",  MYCYAN1,    LSOLID, leg6, "HZ, H > ZZ");
   TH1F    * h0met_nopu_zz_4l          = GetSigHist(f, "h0met_nopu", "zz_4l",          MYBLUE2,    LSOLID, leg6, "ZZ > 4L");
   TH1F    * h0met_nopu_wh_zh_tth_hww  = GetSigHist(f, "h0met_nopu", "zh_hww",  MYMAGENTA1, LSOLID, leg6, "HZ, H > WW");
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
   h0met_nopu_hxx_1GeV->SetLineWidth(2);
   h0met_nopu_h_zz_4l->SetLineWidth(2);
   h0met_nopu_wh_zh_tth_hzz->SetLineWidth(2);
   h0met_nopu_zz_4l->SetLineWidth(2);
   h0met_nopu_wh_zh_tth_hww->SetLineWidth(2);
   h0met_nopu_hxx_1GeV->SetFillColor(kRed);
   h0met_nopu_hxx_1GeV->SetFillStyle(3005);
   h0met_nopu_zz_4l->Draw();
   h0met_nopu_hxx_1GeV->Draw("HSAME");
   h0met_nopu_h_zz_4l->Draw("HSAME");
   h0met_nopu_wh_zh_tth_hzz->Draw("HSAME");
   h0met_nopu_wh_zh_tth_hww->Draw("HSAME");
   leg6->Draw();
   TLine *l7 = new TLine(125, 0, 125, 0.04);
   l7->SetLineWidth(3);
   l7->Draw();
   c6->SaveAs("met_unitnorm.png");


  return 0;













/*
   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c5->SetLogy();
   c5->cd();
   THStack * hmjl_bkg = GetBkgStack(f, "h0mjl", "M(jl) [GeV]", leg5);
   TH1F    * hmjl_sig1 = GetSigHist (f, "h0mjl", "hxx1", MYRED1, LSOLID, leg5, "HXX (1 GeV)");
   TH1F    * hmjl_sig2 = GetSigHist (f, "h0mjl", "hxx1000", MYRED1, LDASHED, leg5, "HXX (1000 GeV)");
   hmjl_bkg->SetMinimum(basemin);
   hmjl_bkg->Draw();
   hmjl_sig1->Draw("HSAME");   
   hmjl_sig2->Draw("HSAME");   
   leg5->Draw();

   TCanvas * ctest = new TCanvas("ctest", "");
   TLegend * legtest = new TLegend(0.7, 0.65, 0.90, 0.90);
   ctest->SetLogy();
   ctest->cd();
   THStack * htest_bkg  = GetBkgStack(f, "h0jdphi", "test variable [GeV]", legtest);
   TH1F    * htest_sig1 = GetSigHist (f, "h0jdphi", "hxx1", MYRED1, LSOLID, legtest, "HXX (1 GeV)");
   TH1F    * htest_sig2 = GetSigHist (f, "h0jdphi", "hxx1000", MYRED1, LDASHED, legtest, "HXX (1000 GeV)");
   htest_bkg->SetMinimum(basemin);
   htest_bkg->Draw();
   htest_sig1->Draw("HSAME");
   htest_sig2->Draw("HSAME");
   legtest->Draw();

   return;

   TCanvas * ctest = new TCanvas("ctest", "");
   TLegend * legtest = new TLegend(0.7, 0.65, 0.90, 0.90);
   ctest->SetLogy();
   ctest->cd();
   THStack * htest_bkg  = GetBkgStack(f, "h0j2pt", "test variable [GeV]", legtest);
   TH1F    * htest_sig1 = GetSigHist (f, "h0j2pt", "hxx1", MYRED1, LSOLID, legtest, "HXX (1 GeV)");
   TH1F    * htest_sig2 = GetSigHist (f, "h0j2pt", "hxx1000", MYRED1, LDASHED, legtest, "HXX (1000 GeV)");
   htest_bkg->SetMinimum(basemin);
   htest_bkg->Draw();
   htest_sig1->Draw("HSAME");
   htest_sig2->Draw("HSAME");
   legtest->Draw();



   
   

   TCanvas * ctest = new TCanvas("ctest", "");
   TLegend * legtest = new TLegend(0.7, 0.65, 0.90, 0.90);
   ctest->SetLogy();
   ctest->cd();
   THStack * htest_bkg  = GetBkgStack(f, "htestx", "test variable [GeV]", legtest);
   TH1F    * htest_sig1 = GetSigHist (f, "htestx", "hxx1", MYRED1, LSOLID, legtest, "HXX (1 GeV)");
   TH1F    * htest_sig2 = GetSigHist (f, "htestx", "hxx1000", MYRED1, LDASHED, legtest, "HXX (1000 GeV)");
   htest_bkg->SetMinimum(basemin);
   htest_bkg->Draw();
   htest_sig1->Draw("HSAME");
   htest_sig2->Draw("HSAME");
   legtest->Draw();


   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c5->SetLogy();
   c5->cd();
   THStack * hnjet_bkg = GetBkgStack(f, "h1njet", "Num Jet", leg5);
   TH1F    * hnjet_sig = GetSigHist(f, "h1njet", "hxx1", MYRED1, LSOLID, leg5, "HXX (1 GeV)");
   hnjet_bkg->SetMinimum(basemin);
   hnjet_bkg->Draw();
   hnjet_sig->Draw("HSAME");
   leg5->Draw();

   TCanvas * c6 = new TCanvas("c6", "");
   TLegend * leg6 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c6->SetLogy();
   c6->cd();
   THStack * hnbjet_bkg = GetBkgStack(f, "h0nbjet", "Num B-Tags", leg6);
   TH1F    * hnbjet_sig = GetSigHist(f, "h0nbjet", "hxx1", MYRED1, LSOLID, leg6, "HXX (1 GeV)");
   hnbjet_bkg->SetMinimum(basemin);
   hnbjet_bkg->Draw();
   hnbjet_sig->Draw("HSAME");
   leg6->Draw();




   return;

   TCanvas * c3 = new TCanvas("c3", "");
   TLegend * leg3 = new TLegend(0.65, 0.65, 0.90, 0.90);
   c3->Divide(2,2);

   c3->cd(1);
   c3->GetPad(1)->SetLogy();
   THStack * hj1pt_bkg = GetBkgStack(f, "hj1pt", "Jet 1 pT [GeV]", NULL);
   hj1pt_bkg->SetMinimum(basemin);
   hj1pt_bkg->Draw();

   c3->cd(2);
   c3->GetPad(2)->SetLogy();
   THStack * hj2pt_bkg = GetBkgStack(f, "hj2pt", "Jet 2 pT [GeV]", leg3);
   hj2pt_bkg->SetMinimum(basemin);
   hj2pt_bkg->Draw();
   leg3->Draw();

   c3->cd(3);
   c3->GetPad(3)->SetLogy();
   THStack * hj1eta_bkg = GetBkgStack(f, "hj1eta", "Jet 1 eta [GeV]", NULL);
   hj1eta_bkg->SetMinimum(basemin);
   hj1eta_bkg->Draw();

   c3->cd(4);
   c3->GetPad(4)->SetLogy();
   THStack * hj2eta_bkg = GetBkgStack(f, "hj2eta", "Jet 2 eta [GeV]", NULL);
   hj2eta_bkg->SetMinimum(basemin);
   hj2eta_bkg->Draw();



   TCanvas * c4 = new TCanvas("c4", "");
   TLegend * leg3 = new TLegend(0.65, 0.65, 0.90, 0.90);
   c4->Divide(2,2);

   c4->cd(1);
   c4->GetPad(1)->SetLogy();
   THStack * hl1pt_bkg = GetBkgStack(f, "hl1pt", "Lept 1 pT [GeV]", NULL);
   hl1pt_bkg->SetMinimum(basemin);
   hl1pt_bkg->Draw();

   c4->cd(2);
   c4->GetPad(2)->SetLogy();
   THStack * hl2pt_bkg = GetBkgStack(f, "hl2pt", "Lept 2 pT [GeV]", leg3);
   hl2pt_bkg->SetMinimum(basemin);
   hl2pt_bkg->Draw();
   leg3->Draw();

   c4->cd(3);
   c4->GetPad(3)->SetLogy();
   THStack * hl1eta_bkg = GetBkgStack(f, "hl1eta", "Lept 1 eta [GeV]", NULL);
   hl1eta_bkg->SetMinimum(basemin);
   hl1eta_bkg->Draw();

   c4->cd(4);
   c4->GetPad(4)->SetLogy();
   THStack * hl2eta_bkg = GetBkgStack(f, "hl2eta", "Lept 2 eta [GeV]", NULL);
   hl2eta_bkg->SetMinimum(basemin);
   hl2eta_bkg->Draw();

   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c5->SetLogy();
   c5->cd();
   THStack * hnjet_stage1_bkg = GetBkgStack(f, "hnjet_stage1", "Num Jets", leg5);
   hnjet_stage1_bkg->SetMinimum(1.0);
   hnjet_stage1_bkg->Draw("nostack");
   leg5->Draw();
   c5->SaveAs("plots/njet_stage1.png");

*/


   //c3->SaveAs("plots/jets.png");

   /*
   TCanvas * c = new TCanvas("c", "");
   c->SetLogy();
   c->cd();
   THStack * bkg_mll = new THStack("bkg_mll","");
   THStack * sig_mll = new THStack("bkg_mll","");

   TH1F * hmll            = (TH1F *) f->Get("hmll");   
   TH1F * hmll_h          = (TH1F *) f->Get("hmll_h");   
   TH1F * hmll_diboson    = (TH1F *) f->Get("hmll_diboson");   
   TH1F * hmll_z          = (TH1F *) f->Get("hmll_z");
   TH1F * hmll_hxx_llvvxx = (TH1F *) f->Get("hmll_hxx_llvvxx");

   hmll_h       ->SetFillStyle(3004);
   hmll_diboson ->SetFillStyle(3005);
   hmll_z       ->SetFillStyle(3006);
   hmll_hxx_llvvxx->SetFillStyle(3003);


   hmll->SetXTitle("dilepton invariant mass [GeV]");


   hmll_h       ->SetFillColor(1);
   hmll_diboson ->SetFillColor(3);
   hmll_z       ->SetFillColor(4);
   hmll_hxx_llvvxx->SetFillColor(2);

   bkg_mll->SetHistogram(hmll);
   bkg_mll->Add(hmll_h      );
   bkg_mll->Add(hmll_diboson);
   bkg_mll->Add(hmll_z    );  
   bkg_mll->SetMinimum(basemin);
   

   bkg_mll->Draw();
   hmll_hxx_llvvxx->Draw("HSAME");
   */
}
