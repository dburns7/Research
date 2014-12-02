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


TH1F * AddHist(THStack *stack, TLegend * leg,  TFile *f, const char * name, const char * sample, const char * legsamp, int style, int color, double & mw, int & n){
   char full_name[200];
   sprintf(full_name, "%s_%s", name, sample);
   TH1F * h = f->Get(full_name);
   if (h){
      
      mw += h->GetSumOfWeights();
      h->SetFillStyle(style);
      h->SetLineColor(color);
      h->SetFillColor(color);
      //h->Scale(1E-3);
      cout << n << endl;
      n += 1;
      if (leg && n==1) { leg->AddEntry(h, legsamp, "f"); }
      stack->Add(h);
   }
   return h;
}

THStack * GetBkgStack(TFile *f, const char * name, const char * xtitle, TLegend * leg, double & mw, int & n, int mode==0){
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
      AddHist(stack, leg, f, name, "2e2mu", "ZZ", FILL0, MYCYAN1, mw, n);
      AddHist(stack, leg, f, name, "2e2tau", "ZZ", FILL0, MYCYAN1, mw, n);
      AddHist(stack, leg, f, name, "2mu2tau", "ZZ", FILL0, MYCYAN1, mw, n);
      AddHist(stack, leg, f, name, "4e", "ZZ", FILL0, MYCYAN1, mw, n);
      AddHist(stack, leg, f, name, "4mu", "ZZ", FILL0, MYCYAN1, mw, n);
      AddHist(stack, leg, f, name, "4tau", "ZZ", FILL0, MYCYAN1, mw, n);

   }
   return stack;
}

TH1F * GetSigHist(TFile *f, const char * name, const char * tag, const char * xtitle, int color, int lstyle, TLegend * leg, const char * legtag){
   char full_name[200];

   sprintf(full_name, "%s_%s", name, tag);
   TH1F * h = f->Get(full_name);

   h->SetFillStyle(0);
   h->SetFillColor(color);
   h->SetLineColor(color);   
   h->SetLineStyle(lstyle);   
   h->SetXTitle(xtitle);
   if (leg) { leg->AddEntry(h, legtag, "l"); }  

   return h;
}

mkplots(){
   gStyle->SetOptStat(0);

   double basemin = 0.001;

   TFile * f = new TFile("latest.root");
   //TFile * f2 = new TFile("ZZIndep/latest_Full_Combo.root");
   //f->ls();
   

   TCanvas * c1 = new TCanvas("c1", "");
   TLegend * leg1 = new TLegend(0.2, 0.65, 0.40, 0.90);
   c1->SetLogy();
   c1->cd();
   double mw = 0;
   int n = 0;
   THStack * hmet_mc   = GetBkgStack(f, "histPFMET", "Missing Transverse Energy [GeV]", leg1, mw, n);
   TH1F    * hmet_data = GetSigHist(f, "histPFMET", "data", "Missing Transverse Energy [GeV]", MYRED1, LSOLID, leg1, "Data");
   n = 0;
   //TH1F    * hmet_sig  = GetSigHist(f2, "h0met_nopu", "hxx_100GeV", "Missing Transverse Energy [GeV]", kBlack, LSOLID, leg1, "M_X = 100 GeV");
   hmet_data->Scale(1 / 0.000617832); 
   double dw = hmet_data->GetSumOfWeights();
   //double sw = hmet_sig->GetSumOfWeights();
   //hmet_sig->Scale(1E4);
   //hmet_sig->Scale(10 * dw / sw);
   //cout << dw / sw << endl;
   cout << dw << " " << mw << endl;
   cout << dw / mw << endl;
   hmet_data->Draw("EP");
   hmet_mc->Draw("HSAME");
   hmet_data->Draw("EPSAME");
   //hmet_sig->Draw("HSAME");
   leg1->Draw();
   c1->SaveAs("met.png");

   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.65, 0.65, 0.85, 0.90);
   c2->SetLogy();
   c2->cd();
   double mw = 0;
   n = 0;
   THStack * hmet_ctrl_mc   = GetBkgStack(f, "histPFMETctrl", "Missing Transverse Energy [GeV]", leg2, mw, n);
   TH1F    * hmet_ctrl_data = GetSigHist(f, "histPFMETctrl", "data", "Missing Transverse Energy [GeV]", MYRED1, LSOLID, leg2, "Data");
   n = 0;
   //TH1F    * hmet_sig  = GetSigHist(f2, "h0met_nopu", "hxx_100GeV", "Missing Transverse Energy [GeV]", kBlack, LSOLID, leg1, "M_X = 100 GeV");
   hmet_ctrl_data->Scale(1 / 0.000617832); 
   
   //double dw = hmet_data->GetSumOfWeights();
   //double sw = hmet_sig->GetSumOfWeights();
   //hmet_sig->Scale(1E4);
   //hmet_sig->Scale(10 * dw / sw);
   //cout << dw / sw << endl;
   //cout << dw << " " << mw << endl;
   //cout << dw / mw << endl;
   hmet_ctrl_data->Draw("EP");
   hmet_ctrl_mc->Draw("HSAME");
   hmet_ctrl_data->Draw("EPSAME");
   //hmet_sig->Draw("HSAME");
   leg2->Draw();
   c2->SaveAs("met_ctrl.png");
return;

   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.6, 0.65, 0.8, 0.90);
   //c2->SetLogy();
   c2->cd();
   n = 0;
   THStack * hmllll_mc   = GetBkgStack(f, "histZZMass", "Four Lepton Invariant Mass [GeV]", leg2, mw, n);
   TH1F    * hmllll_data = GetSigHist(f, "histZZMass", "data", "Four Lepton Invariant Mass [GeV]", MYRED1, LSOLID, leg2, "Data");
   hmllll_data->Scale(1 / 0.000617832);
   hmllll_data->Draw("EP");
   hmllll_mc->Draw("HSAME");//HSAME
   hmllll_data->Draw("EPSAME");
   leg2->Draw();
   c2->SaveAs("mllll.png");
  














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
