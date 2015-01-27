enum {MYCYAN1     = kCyan-4};
enum {MYCYAN2     = kCyan-3};
enum {MYGREEN1    = kGreen-4};
enum {MYGREEN2    = kGreen-3};
enum {MYORANGE1   = kOrange-4};
enum {MYORANGE2   = kOrange-3};
enum {MYMAGENTA1  = kMagenta};
enum {MYMAGENTA2  = kMagenta+2};
enum {MYBLUE1     = kBlue};
enum {MYBLUE2     = kBlue-2};
enum {MYRED1      = kRed};
enum {MYRED2      = kRed-2};
enum {MYBLACK     = kBlack};

enum {FILL0 = 1001, FILL1 = 3004, FILL2 = 3013, FILL3 = 3005, FILL4=3014, FILL5 = 3007}; 
enum {FILLSIG1 = 3002, FILLSIG2 = 3003};

enum {LSOLID=1, LDASHED=2};


TH1F * GetHist(TFile *f, const char * name, const char * tag, const char * xtitle, int color, int lstyle, TLegend * leg, const char * legtag){
   char full_name[200];

   sprintf(full_name, "%s;%s", name, tag);
   TH1F * h = (TH1F *) f->Get(full_name);
   if(h){
     h->SetFillStyle(0);
     h->SetFillColor(color);
     h->SetLineColor(color);   
     h->SetLineStyle(lstyle);   
     h->SetXTitle(xtitle);
     if(leg){
       leg->AddEntry(h, legtag, "l");
       leg->SetFillStyle(0);
       leg->SetBorderSize(0);
       leg->SetTextSize(0.04);
     }
   }
   return h;
}

void mkplots(){
   gStyle->SetOptStat(0);

   TFile * f = new TFile("../results/histos.root");
   //1: 08f89 SPS_S32
   //2: 23587 SPS_HTC
   //3: 6b24c SPS_S34
   //4: 6d7bc SPS_HTC3D
   //5: 95ea0 SPS_S33
   //6: 960d6 SPS_S31
   
   TCanvas * c1 = new TCanvas("c1", "");
   TLegend * leg1 = new TLegend(0.65, 0.65, 0.85, 0.85);
   c1->SetLogy();
   c1->cd();
   TH1F    * h1t1 = GetHist(f, "h1t", "1", "Time [s]", MYBLACK, LSOLID, leg1, "SPS_S32");
   TH1F    * h1trackt1 = GetHist(f, "h1trackt", "1", "Time [s]", MYRED1, LSOLID, leg1, "SPS_S32 Tracks");
   h1t1->Draw();
   h1trackt1->Draw("HSAME");
   leg1->Draw();
   c1->SaveAs("plots/h1t1.png");

   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.65, 0.65, 0.85, 0.85);
   c2->SetLogy();
   c2->cd();
   TH1F    * h1t2 = GetHist(f, "h1t", "2", "Time [s]", MYBLACK, LSOLID, leg2, "SPS_HTC");
   TH1F    * h1trackt2 = GetHist(f, "h1trackt", "2", "Time [s]", MYRED1, LSOLID, leg2, "SPS_HTC Tracks");
   h1t2->Draw();
   h1trackt2->Draw("HSAME");
   leg2->Draw();
   c2->SaveAs("plots/h1t2.png");
 
   TCanvas * c3 = new TCanvas("c3", "");
   TLegend * leg3 = new TLegend(0.65, 0.65, 0.85, 0.85);
   c3->SetLogy();
   c3->cd();
   TH1F    * h1t3 = GetHist(f, "h1t", "3", "Time [s]", MYBLACK, LSOLID, leg3, "SPS_S34");
   TH1F    * h1trackt3 = GetHist(f, "h1trackt", "3", "Time [s]", MYRED1, LSOLID, leg3, "SPS_S34 Tracks");
   h1t3->Draw();
   h1trackt3->Draw("HSAME");
   leg3->Draw();
   c3->SaveAs("plots/h1t3.png");
   
   TCanvas * c4 = new TCanvas("c4", "");
   TLegend * leg4 = new TLegend(0.65, 0.65, 0.85, 0.85);
   c4->SetLogy();
   c4->cd();
   TH1F    * h1t4 = GetHist(f, "h1t", "4", "Time [s]", MYBLACK, LSOLID, leg4, "SPS_HTC3D");
   TH1F    * h1trackt4 = GetHist(f, "h1trackt", "4", "Time [s]", MYRED1, LSOLID, leg4, "SPS_HTC3D Tracks");
   h1t4->Draw();
   h1trackt4->Draw("HSAME");
   leg4->Draw();
   c4->SaveAs("plots/h1t4.png");
   
   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.65, 0.65, 0.85, 0.85);
   c5->SetLogy();
   c5->cd();
   TH1F    * h1t5 = GetHist(f, "h1t", "5", "Time [s]", MYBLACK, LSOLID, leg5, "SPS_S33");
   TH1F    * h1trackt5 = GetHist(f, "h1trackt", "5", "Time [s]", MYRED1, LSOLID, leg5, "SPS_S33 Tracks");
   h1t5->Draw();
   h1trackt5->Draw("HSAME");
   leg5->Draw();
   c5->SaveAs("plots/h1t5.png");
   
   TCanvas * c6 = new TCanvas("c6", "");
   TLegend * leg6 = new TLegend(0.65, 0.65, 0.85, 0.85);
   c6->SetLogy();
   c6->cd();
   TH1F    * h1t6 = GetHist(f, "h1t", "6", "Time [s]", MYBLACK, LSOLID, leg6, "SPS_S31");
   TH1F    * h1trackt6 = GetHist(f, "h1trackt", "6", "Time [s]", MYRED1, LSOLID, leg6, "SPS_S31 Tracks");
   h1t6->Draw();
   h1trackt6->Draw("HSAME");
   leg6->Draw();
   c6->SaveAs("plots/h1t6.png");
 


   /*
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

*/

}
