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


TH1F * AddHist(THStack *stack, TLegend * leg,  TFile *f, const char * name, const char * sample, const char * legsamp, int style, int color){
   char full_name[200];
   sprintf(full_name, "%s_%s", name, sample);
   TH1F * h = f->Get(full_name);
   if (h){
      h->SetFillStyle(style);
      h->SetLineColor(color);
      h->SetFillColor(color);
      if (leg) { leg->AddEntry(h, legsamp, "f"); }
      stack->Add(h);
   }
   return h;
}

THStack * GetBkgStack(TFile *f, const char * name, const char * xtitle, TLegend * leg, int mode==0){
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
      AddHist(stack, leg, f, name, "ZAA", "ZAA", FILL0, MYMAGENTA2);
      AddHist(stack, leg, f, name, "ZH",  "HZ",  FILL0, MYGREEN2);
      AddHist(stack, leg, f, name, "WH",  "HW",  FILL0, MYCYAN1);   
      AddHist(stack, leg, f, name, "H",   "H",   FILL0, MYBLUE2);
      AddHist(stack, leg, f, name, "AA",  "AA",  FILL0, MYMAGENTA1);
   }
   return stack;
}

TH1F * GetSigHist(TFile *f, const char * name, const char * tag, int color, int lstyle, TLegend * leg, const char * legtag){
   char full_name[200];

   sprintf(full_name, "%s_%s", name, tag);
   TH1F * h = f->Get(full_name);

   if (leg) {
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
   }
   
   h->SetFillStyle(0);
   h->SetFillColor(color);
   h->SetLineColor(color);   
   h->SetLineStyle(lstyle);   
   if (leg) { leg->AddEntry(h, legtag, "l"); }  

   return h;
}

mkplots(){
   gStyle->SetOptStat(0);

   double basemin = 0.001;

   TFile * f = new TFile("latest.root");
   //f->ls();
   
   
   TCanvas * c1 = new TCanvas("c1", "");
   TLegend * leg1 = new TLegend(0.65, 0.65, 0.80, 0.85);
   c1->SetLogy();
   c1->cd();
   THStack * hmet_bkg = GetBkgStack(f, "h0met_nopu", "Missing Transverse Energy [GeV]", leg1);
   TH1F    * hmet_sig1 = GetSigHist(f, "h0met_nopu", "hxx_100GeV", MYRED1, LSOLID, leg1, "HXX (100 GeV)");
   hmet_sig1->Scale(1E4);
   hmet_sig1->SetLineWidth(2);
   hmet_sig1->Draw("HSAME");
   hmet_bkg->Draw("HSAME");
   hmet_sig1->Draw("HSAME");
   leg1->Draw();
   hmet_sig1->SetAxisRange(1E1, 5E7, "Y");
   hmet_sig1->GetXaxis()->SetTitle("Missing Transverse Energy [GeV]");
   c1->SaveAs("met_precuts.png");
  
   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.65, 0.65, 0.80, 0.85);
   c2->SetLogy();
   c2->cd();
   THStack * hmet_bkg = GetBkgStack(f, "h1met_nopu", "Missing Transverse Energy [GeV]", leg2);
   TH1F    * hmet_sig1 = GetSigHist(f, "h1met_nopu", "hxx_1GeV", MYRED1, LSOLID, leg2, "HXX (100 GeV)");
   hmet_sig1->Scale(1E4);
   hmet_sig1->SetLineWidth(2);
   hmet_sig1->Draw();
   hmet_bkg->Draw("HSAME");
   hmet_sig1->Draw("HSAME");
   leg2->Draw();
   hmet_sig1->SetAxisRange(1E1, 1E6,"Y");
   hmet_sig1->SetAxisRange(200., 499.,"X");
   hmet_sig1->GetXaxis()->SetTitle("Missing Transverse Energy [GeV]");
   //hmet_sig1->GetXaxis()->SetTitleSize(0.5);
   //hmet_bkg->GetXaxis()->SetRange(75., 250.);
   //hmet_bkg->GetYaxis()->SetRange(1E-4, 150.);
   c2->SaveAs("met_postcuts.png");

   
   TCanvas * c3 = new TCanvas("c3", "");
   TLegend * leg3 = new TLegend(0.65, 0.65, 0.80, 0.85);
   //c3->SetLogy();
   c3->cd();
   TH1F    * hmgg_sig1 = GetSigHist(f, "h0mgg", "hxx_1GeV", MYRED1, LSOLID, leg3, "HXX (100 GeV)");
   TH1F    * hmgg_bkg1 = GetSigHist(f, "h0mgg", "ZH",  MYCYAN1,  LSOLID, leg3, "HZ");
   TH1F    * hmgg_bkg2 = GetSigHist(f, "h0mgg", "WH",  MYORANGE1, LSOLID, leg3, "HW");
   TH1F    * hmgg_bkg3 = GetSigHist(f, "h0mgg", "H",   MYBLUE1,  LSOLID, leg3, "H");
   TH1F    * hmgg_bkg4 = GetSigHist(f, "h0mgg", "AA",  MYMAGENTA2,  LSOLID, leg3, "AA");
   TH1F    * hmgg_bkg5 = GetSigHist(f, "h0mgg", "ZAA", MYGREEN2, LSOLID, leg3, "ZAA");
   hmgg_sig1->Scale(1/hmgg_sig1->Integral());
   hmgg_bkg1->Scale(1/hmgg_bkg1->Integral());
   hmgg_bkg2->Scale(1/hmgg_bkg2->Integral());
   hmgg_bkg3->Scale(1/hmgg_bkg3->Integral());
   hmgg_bkg4->Scale(1/hmgg_bkg4->Integral());
   hmgg_bkg5->Scale(1/hmgg_bkg5->Integral());
   hmgg_sig1->SetLineWidth(2);
   hmgg_bkg1->SetLineWidth(2);
   hmgg_bkg2->SetLineWidth(2);
   hmgg_bkg3->SetLineWidth(2);
   hmgg_bkg4->SetLineWidth(2);
   hmgg_bkg5->SetLineWidth(2);
   hmgg_sig1 ->SetFillColor(kRed);
   hmgg_sig1 ->SetFillStyle(3005);
   hmgg_sig1->Draw();
   hmgg_bkg1->Draw("HSAME");
   hmgg_bkg2->Draw("HSAME");
   hmgg_bkg3->Draw("HSAME");
   hmgg_bkg4->Draw("HSAME");
   hmgg_bkg5->Draw("HSAME");
   hmgg_sig1->Draw("HSAME");
   leg3->Draw();
   hmgg_sig1->SetAxisRange(0, 0.65,"Y");
   //hmgg_sig1->SetAxisRange(100., 150.,"X");
   hmgg_sig1->GetXaxis()->SetTitle("Diphoton Invariant Mass [GeV]");
   hmgg_sig1->GetYaxis()->SetTitle("Fraction of Events");
   TLine *l1 = new TLine(116, 0, 116, 0.2);
   l1->SetLineWidth(3);
   l1->Draw();
   TLine *l2 = new TLine(136, 0, 136, 0.2);
   l2->SetLineWidth(3);
   l2->Draw();
   c3->SaveAs("mgg.png");


   TCanvas * c4 = new TCanvas("c4", "");
   TLegend * leg4 = new TLegend(0.65, 0.65, 0.80, 0.85);
   //c3->SetLogy();
   c4->cd();
   TH1F    * h0met_sig1 = GetSigHist(f, "h0met_nopu", "hxx_1GeV", MYRED1, LSOLID, leg4, "HXX (100 GeV)");
   TH1F    * h0met1 = GetSigHist(f, "h0met_nopu", "ZH",  MYCYAN1,  LSOLID, leg4, "HZ");
   TH1F    * h0met2 = GetSigHist(f, "h0met_nopu", "WH",  MYGREEN1, LSOLID, leg4, "HW");
   TH1F    * h0met3 = GetSigHist(f, "h0met_nopu", "H",   MYBLUE1,  LSOLID, leg4, "H");
   TH1F    * h0met4 = GetSigHist(f, "h0met_nopu", "AA",  MYCYAN2,  LSOLID, leg4, "AA");
   TH1F    * h0met5 = GetSigHist(f, "h0met_nopu", "ZAA", MYGREEN2, LSOLID, leg4, "ZAA");
   h0met_sig1 ->Scale(1/h0met_sig1->Integral());
   h0met1     ->Scale(1/h0met1->Integral());
   h0met2     ->Scale(1/h0met2->Integral());
   h0met3     ->Scale(1/h0met3->Integral());
   h0met4     ->Scale(1/h0met4->Integral());
   h0met5     ->Scale(1/h0met5->Integral());
   h0met_sig1 ->SetLineWidth(2);
   h0met1     ->SetLineWidth(2);
   h0met2     ->SetLineWidth(2);
   h0met3     ->SetLineWidth(2);
   h0met4     ->SetLineWidth(2);
   h0met5     ->SetLineWidth(2);
   h0met_sig1 ->SetFillColor(kRed);
   h0met_sig1 ->SetFillStyle(3005);
   h0met_sig1 ->Draw();
   h0met1     ->Draw("HSAME");
   h0met2     ->Draw("HSAME");
   h0met3     ->Draw("HSAME");
   h0met4     ->Draw("HSAME");
   h0met5     ->Draw("HSAME");
   h0met_sig1 ->Draw("HSAME");
   leg4       ->Draw();
   h0met_sig1 ->SetAxisRange(0, 0.07,"Y");
   //h0met_sig1->SetAxisRange(100., 150.,"X");
   h0met_sig1->GetXaxis()->SetTitle("Missing Transverse Energy [GeV]");
   h0met_sig1->GetYaxis()->SetTitle("Fraction of Events");
   TLine *l3 = new TLine(200, 0, 200, 0.04);
   l3->SetLineWidth(3);
   l3->Draw();
   c4->SaveAs("met_unitnorm.png");
   
   return;
   TCanvas * c6 = new TCanvas("c6", "");
   TLegend * leg6 = new TLegend(0.6, 0.65, 0.80, 0.90);
   leg6->SetFillStyle(0);
   leg6->SetBorderSize(0);
   leg6->SetTextSize(0.025);
   c6->cd();
   TH1F    * h0met_nopu_8TEV     = GetSigHist(f, "h0met_nopu", "8TEV",     MYRED1,     LSOLID, leg6, "HXX (1 GeV), 8 TeV, No Pileup");
   TH1F    * h0met_nopu_8TEVPU   = GetSigHist(f, "h0met_nopu", "8TEVPU",   MYGREEN2,   LSOLID, leg6, "HXX (1 GeV), 8 TeV, Pileup");
   TH1F    * h0met_nopu_100TEV   = GetSigHist(f, "h0met_nopu", "100TEV",   MYCYAN1,    LSOLID, leg6, "HXX (1 GeV), 100 TeV, No Pileup");
   TH1F    * h0met_nopu_100TEVPU = GetSigHist(f, "h0met_nopu", "100TEVPU", MYBLUE2,    LSOLID, leg6, "HXX (1 GeV), 100 TeV, Pileup");
   h0met_nopu_8TEV     ->Scale(1/h0met_nopu_8TEV->Integral());
   h0met_nopu_8TEVPU   ->Scale(1/h0met_nopu_8TEVPU->Integral());
   h0met_nopu_100TEV   ->Scale(1/h0met_nopu_100TEV->Integral());
   h0met_nopu_100TEVPU ->Scale(1/h0met_nopu_100TEVPU->Integral());
   h0met_nopu_8TEV     ->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_8TEVPU   ->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_100TEV   ->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_100TEVPU ->GetXaxis()->SetTitle("Missing Transverse Mass [GeV]");
   h0met_nopu_8TEV     ->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_8TEVPU   ->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_100TEV   ->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_100TEVPU ->GetYaxis()->SetTitle("Fraction of Events");
   h0met_nopu_8TEV     ->SetLineWidth(2);
   h0met_nopu_8TEVPU   ->SetLineWidth(2);
   h0met_nopu_100TEV   ->SetLineWidth(2);
   h0met_nopu_100TEVPU ->SetLineWidth(2);
   h0met_nopu_8TEV     ->GetYaxis()->SetTitleOffset(1.4);
   h0met_nopu_8TEVPU   ->GetYaxis()->SetTitleOffset(1.4);
   h0met_nopu_100TEV   ->GetYaxis()->SetTitleOffset(1.4);
   h0met_nopu_100TEVPU ->GetYaxis()->SetTitleOffset(1.4);
   //h0met_nopu_->SetFillColor(kRed);
   //h0met_nopu_->SetFillStyle(3005);
   h0met_nopu_8TEV     ->Draw("HSAME");
   h0met_nopu_8TEVPU   ->Draw("HSAME");
   h0met_nopu_100TEV   ->Draw("HSAME");
   h0met_nopu_100TEVPU ->Draw("HSAME");
   leg6->Draw();
   //TLine *l3 = new TLine(100, 0, 100, 0.04);
   //l3->SetLineWidth(3);
   //l3->Draw();
   c6->SaveAs("met_signals.png");


}
