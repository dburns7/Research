void limitPlots(){

/*
Expected  2.5%: r < 5.0551
Expected 16.0%: r < 6.7639
Expected 50.0%: r < 9.3438
Expected 84.0%: r < 13.0310
Expected 97.5%: r < 17.2971

Expected  2.5%: r < 5.7314
Expected 16.0%: r < 7.6687
Expected 50.0%: r < 10.5938
Expected 84.0%: r < 14.7742
Expected 97.5%: r < 19.6111

Expected  2.5%: r < 1.5301
Expected 16.0%: r < 2.0473
Expected 50.0%: r < 2.8281
Expected 84.0%: r < 3.9216
Expected 97.5%: r < 5.2191

Expected  2.5%: r < 1.7329
Expected 16.0%: r < 2.3187
Expected 50.0%: r < 3.2031
Expected 84.0%: r < 4.4671
Expected 97.5%: r < 5.9296

Expected  2.5%: r < 2.0880
Expected 16.0%: r < 2.7938
Expected 50.0%: r < 3.8594
Expected 84.0%: r < 5.3516
Expected 97.5%: r < 7.1222

59 52 195 172 143
*/

static const Int_t n = 5;
Double_t _mchi[n]  = {1, 10, 100, 500, 1000};

Double_t _2siglow[n]  = {5.055, 5.731, 1.530, 1.733, 2.088};
Double_t _1siglow[n]  = {6.764, 7.669, 2.047, 2.319, 2.794};
Double_t _middle[n]   = {9.344, 10.59, 2.828, 3.203, 3.859};
Double_t _1sighigh[n] = {13.03, 14.77, 3.922, 4.467, 5.352};
Double_t _2sighigh[n] = {17.30, 19.61, 5.219, 5.930, 7.122};
Double_t _seleff[n]   = {0.01966, 0.01733, 0.065, 0.05733, 0.04766};

//smeared MC
/*Double_t _2siglow[n]  = {3.534, 3.512, 0.6403, 0.5347, 0.5368};
Double_t _1siglow[n]  = {4.693, 4.690, 0.8526, 0.7154, 0.7182};
Double_t _middle[n]   = {6.531, 6.469, 1.184, 0.9883, 0.9922};
Double_t _1sighigh[n] = {9.057, 9.021, 1.641, 1.370, 1.376};
Double_t _2sighigh[n] = {12.05, 11.97, 2.184, 1.824, 1.831};
Double_t _seleff[n]   = {212.22E-5, 213.6E-5, 1170.6E-5, 1400.64E-5, 1393.83E-5}
*/

TGraph *g2siglow  = new TGraph(n, _mchi, _2siglow);
TGraph *g1siglow  = new TGraph(n, _mchi, _1siglow);
TGraph *gmiddle   = new TGraph(n, _mchi, _middle);
TGraph *g1sighigh = new TGraph(n, _mchi, _1sighigh);
TGraph *g2sighigh = new TGraph(n, _mchi, _2sighigh);
TGraph *grshade1   = new TGraph(2*n);
TGraph *grshade2   = new TGraph(2*n);
for(i=0;i<n;i++){
  grshade1->SetPoint(i, _mchi[i], _1sighigh[i]);
  grshade1->SetPoint(n+i, _mchi[n-i-1], _1siglow[n-i-1]);
}
for(i=0;i<n;i++){
  grshade2->SetPoint(i, _mchi[i], _2sighigh[i]);
  grshade2->SetPoint(n+i, _mchi[n-i-1], _2siglow[n-i-1]);
}

gStyle->SetOptStat(0);
TCanvas *ce = new TCanvas("ce");
ce->cd();
//ce->DrawFrame(1,0,1000,3);
ce->SetLogx();
ce->SetLogy();
//grshade1->SetFillStyle(3013);

TLegend *leg1 = new TLegend(0.65,0.65,0.85,0.85);
leg1->SetFillStyle(0);
leg1->SetBorderSize(0);

TH2F * hframe = new TH2F("hframe", "", 10, 0.9, 1.1E3, 10, 0.5, 50.0);
//geres_1000_7->SetLineWidth(WIDTH);
hframe->GetXaxis()->SetTitle("m_{#chi} [GeV]");
hframe->GetXaxis()->SetTitleOffset(1.3);
//hframe->GetXaxis()->SetLabelSize(0.05);
hframe->GetYaxis()->SetTitle("95% CL upper limit #sigma (pp>hxx>llllxx) [fb]");
hframe->GetYaxis()->SetTitleOffset(1.2);
//hframe->GetYaxis()->SetLabelSize(0.05);
hframe->Draw();
//gsigma->Draw("LP");
grshade2->SetFillColor(5);
grshade2->Draw("f");
grshade1->SetFillColor(3);
grshade1->Draw("f");
//g1siglow->Draw("l");
//g1sighigh->Draw("l");
gmiddle->SetLineWidth(2);
gmiddle->SetLineStyle(2);
gmiddle->Draw("l");

TH1F* dummy1 = new TH1F("dummy","",100, 0, 10);
dummy1->SetLineStyle(2);
dummy1->SetLineWidth(2);

leg1->AddEntry(dummy1, "Expected");
leg1->AddEntry(grshade1, "#pm 1 #sigma", "F");
leg1->AddEntry(grshade2, "#pm 2 #sigma", "F");
leg1->SetTextSize(0.04);
leg1->Draw();
/*
TPaveText *pt = new TPaveText(1, 20, 5, 29, "NB");
pt->SetBorderSize(1);
pt->SetLineColor(0);
pt->SetTextFont(40);
pt->SetFillColor(0);
pt->AddText("#surd #sum #pm s = 8 TeV");
pt->Draw();
*/

ce->SaveAs("sigma_limits.png");

TCanvas * c2 = new TCanvas("c2", "");
TH2F * hframe2 = new TH2F("hframe2", "", 10, 0.9, 1.1E3, 10, 0.0, 1E-1);
hframe2->GetXaxis()->SetTitle("m_{#chi} [GeV]");
hframe2->GetXaxis()->SetTitleOffset(1.3);
hframe2->GetYaxis()->SetTitle("Selection Efficiency");
hframe2->GetYaxis()->SetTitleOffset(1.2);
hframe2->Draw();
TLegend * leg2 = new TLegend(0.6, 0.65, 0.80, 0.90);
leg2->SetFillStyle(0);
leg2->SetBorderSize(0);
leg2->SetTextSize(0.04);
c2->cd();
c2->SetLogx();
TGraph *gseleff   = new TGraph(n, _mchi, _seleff);
gseleff->SetLineWidth(2);
gseleff->Draw("l");
dummy1->SetLineStyle(1);
leg2->AddEntry(dummy1, "HXX Combined");
leg2->Draw();
c2->SaveAs("seleff.png");

}
