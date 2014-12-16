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
   TLegend * leg1 = new TLegend(0.7, 0.65, 0.85, 0.85);
   c1->SetLogy();
   c1->cd();
   THStack * hmet_bkg = GetBkgStack(f, "h0met_nopu", "Missing Transverse Energy [GeV]", leg1);
   TH1F    * hmet_sig1 = GetSigHist(f, "h0met_nopu", "100TEVPU", MYRED1, LSOLID, leg1, "HXX (1 GeV)");
   hmet_sig1->Scale(1E3);
   hmet_sig1->SetLineWidth(2);
   hmet_sig1->Draw("HSAME");
   hmet_bkg->Draw("HSAME");
   hmet_sig1->Draw("HSAME");
   leg1->Draw();
   hmet_sig1->SetAxisRange(1E1, 1E7, "Y");
   hmet_sig1->GetXaxis()->SetTitle("Missing Transverse Energy [GeV]");
   c1->SaveAs("met_precuts.png");
  

   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.7, 0.65, 0.85, 0.85);
   c2->SetLogy();
   c2->cd();
   THStack * hmet_bkg = GetBkgStack(f, "h1met_nopu", "Missing Transverse Energy [GeV]", leg2);
   TH1F    * hmet_sig1 = GetSigHist(f, "h1met_nopu", "100TEVPU", MYRED1, LSOLID, leg2, "HXX (1 GeV)");
   hmet_sig1->Scale(1E3);
   hmet_sig1->SetLineWidth(2);
   hmet_sig1->Draw();
   hmet_bkg->Draw("HSAME");
   hmet_sig1->Draw("HSAME");
   leg2->Draw();
   hmet_sig1->SetAxisRange(1E1, 1E7,"Y");
   hmet_sig1->SetAxisRange(100., 499.,"X");
   hmet_sig1->GetXaxis()->SetTitle("Missing Transverse Energy [GeV]");
   //hmet_sig1->GetXaxis()->SetTitleSize(0.5);
   //hmet_bkg->GetXaxis()->SetRange(75., 250.);
   //hmet_bkg->GetYaxis()->SetRange(1E-4, 150.);
   c2->SaveAs("met_postcuts.png");

/*
102  115    h0mll.add_sample(20, "_8TEV");
103   63 116    h0mll.add_sample(21, "_8TEVPU");
104    64 117    h0mll.add_sample(22, "_100TEV");
105     65 118    h0mll.add_sample(23, "_100TEVPU");
106      66 119    
107       67 120    cutflow.add_sample_name(20, "8TEV");
108        68 121    cutflow.add_sample_name(21, "8TEVPU");
109         69 122    cutflow.add_sample_name(22, "100TEV");
110          70 123    cutflow.add_sample_name(23, "100TEVPU");
111 */

   
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
   c6->SaveAs("met_unitnorm.png");


}
