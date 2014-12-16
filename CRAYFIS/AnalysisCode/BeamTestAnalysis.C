BeamTestAnalysis(){
  
  TFile * data = new TFile("SPS_HTC_11_10.root");
  //TFile * data = new TFile("data/2014.11.tmp/01/fb13e9e800108f89_b1cde24dbf527186_1414910718.root");
  //data->ls();
  TNtuple * data_tuple = (TNtuple*)data->Get("events");
  //data_tuple->Print();
  Long64_t n = data_tuple->GetEntries();
  
  vector<int>*    pix_val = new vector<int>();
  vector<int>*    pix_x   = new vector<int>();
  vector<int>*    pix_y   = new vector<int>();
  ULong64_t*      t;

  vector<int>*       test      = new vector<int>();
  vector<int>*       v0pix_val = new vector<int>();
  vector<int>*       v0pix_x   = new vector<int>();
  vector<int>*       v0pix_y   = new vector<int>();
  vector<int>*       v0t       = new vector<int>();
  vector<int>*       v1pix_val = new vector<int>();
  vector<int>*       v1pix_x   = new vector<int>();
  vector<int>*       v1pix_y   = new vector<int>();
  //vector<ULong64_t>* v1t       = new vector<ULong64_t>();

  data_tuple->SetBranchAddress("pix_val", &pix_val);
  data_tuple->SetBranchAddress("pix_x", &pix_x);
  data_tuple->SetBranchAddress("pix_y", &pix_y);
  data_tuple->SetBranchAddress("t", t);
  
  Int_t xpix = 480;
  Int_t ypix = 320;

  // control plots, pre-cuts
  TH1F * h0pix_val = new TH1F("h0pix_val", "", 100, 0, 400); 
  TH1F * h0pix_x   = new TH1F("h0pix_x",   "", xpix, 0, xpix); 
  TH1F * h0pix_y   = new TH1F("h0pix_y",   "", ypix, 0, ypix); 
  TH1F * h0t       = new TH1F("h0t",       "", 100, 1, 1000);
  TH2F * h0xy      = new TH2F("h0xy",      "", xpix, 0, xpix, ypix, 0, ypix);

  // selection plots, post-cuts
  TH1F * h1pix_val = new TH1F("h1pix_val", "", 100, 0, 400); 
  TH1F * h1pix_x   = new TH1F("h1pix_x",   "", xpix, 0, xpix); 
  TH1F * h1pix_y   = new TH1F("h1pix_y",   "", ypix, 0, ypix); 
  TH1F * h1t       = new TH1F("h1t",       "", 100, 1, 1000);
  TH2F * h1xy      = new TH2F("h1xy",      "", xpix, 0, xpix, ypix, 0, ypix);

  int hits = 0;
  ULong64_t starttime;

  for ( Int_t entry = 0; entry < n; ++entry)
  {
    data_tuple->GetEntry(entry);
    cout << "Raw time:      " << t << endl;
    
    if(entry == 0) starttime = t;
    //cout << (int)starttime << endl;

    for(Int_t i = 0; i < pix_val->size(); i++)
    {
      t = t - starttime;
      cout << "Adjusted time: " << t << endl;
      v0pix_val->push_back(pix_val->at(i));
      v0pix_x  ->push_back(pix_x->at(i));
      v0pix_y  ->push_back(pix_y->at(i));
      v0t      ->push_back((int)t);
      
      h0pix_val->Fill(pix_val->at(i));
      h0pix_x  ->Fill(pix_x->at(i));
      h0pix_y  ->Fill(pix_y->at(i));
      h0xy     ->Fill(pix_x->at(i), pix_y->at(i));
      h0t      ->Fill((int)t);
      //cout << t << endl;
    
      if (pix_val->at(i) > 10) hits++;
    }
  }
 
  cout << "INFO: Preselection Hits:    " << hits << endl;

  // hot pixel removal
  Int_t xbins = h0pix_x->GetNbinsX();
  Int_t ybins = h0pix_y->GetNbinsX();
  Int_t xavg = 0;
  Int_t yavg = 0;
  vector<int> xinds;
  vector<int> yinds;
  for(Int_t i = 0; i < xbins; i++) xavg += h0pix_x->GetBinContent(i);
  xavg = xavg / xbins;
  for(Int_t i = 0; i < ybins; i++) yavg += h0pix_y->GetBinContent(i);
  yavg = yavg / ybins;
  cout << "INFO: Average X Occupancy:  " << xavg << endl;
  cout << "INFO: Average Y Occupancy:  " << yavg << endl;


  // hot pixel removal
  for(Int_t i = 0; i < xbins; i++){
    if(h0pix_x->GetBinContent(i) < 5*xavg){
      h1pix_x->Fill(h0pix_x->GetBinContent(i));
    }
    for(Int_t j = 0; j < ybins; j++){
      if(h0pix_x->GetBinContent(i) < 5*xavg & h0pix_y->GetBinContent(j) < 5*yavg){
        xinds.push_back(i);
        yinds.push_back(j);
        //h1xy->Fill(h0pix_x->GetBinContent(i), h0pix_y->GetBinContent(j));
      }
    }
  }
  for(Int_t i = 0; i < ybins; i++){
    if(h0pix_y->GetBinContent(i) < 5*yavg){
      h1pix_y->Fill(h0pix_y->GetBinContent(i));
    }
  }
  //cout << xinds.size() << " " << v0pix_x->size() << endl;
  for(Int_t i = 0; i < xinds.size(); i++){
    //h1xy->Fill(v0pix_x->at(xinds[i]), v0pix_y->at(yinds[i]));
    //cout << "INDS  " << xinds[i] << " " << yinds[i] << endl;
    //cout << "VALS  " << v0pix_x->at(i) << " " << v0pix_y->at(i) << endl;
  }



  // hot pixel removal
  //static const Int_t xpix = 500;
  //static const Int_t ypix = 350;
  //Double_t occ[xpix][ypix];
  Double_t occ[500][350];
  Int_t xpix = 500;
  Int_t ypix = 350;
  for (Int_t i = 0; i < xpix; i++)
  {
    for (Int_t j = 0; j < ypix; j++)
    {
      occ[i][j] = 0;
    }
  }
  Int_t * xind;
  Int_t * yind;
  for (Int_t i = 0; i < v0pix_val->size(); i++)
  {
    //cout << v0pix_x->at(i) << " " << v0pix_y->at(i) << endl;
    xind = v0pix_x->at(i);
    yind = v0pix_y->at(i);
    occ[xind][yind] += 1;
    int xind1 = 499;
    int yind1 = 349;
    occ[xind1][yind1] = 123;
  }
  int occ_tot = 0;
  for (Int_t i = 0; i < xpix; i++)
  {
    for (Int_t j = 0; j < ypix; j++)
    {
      //cout << occ[i][j] << endl;
      occ_tot += occ[i][j];
    }
  } 
  double avg_occ = occ_tot / (xpix*ypix);
  //cout << "INFO: Avg occupancy:    " << avg_occ << endl;
  vector<int> indrem;
  for (Int_t i = 0; i < xpix; i++)
  {
    for (Int_t j = 0; j < ypix; j++)
    {
      if( occ[i][j] > 10*avg_occ)
      {
        indrem.push_back(i);
	continue;
      }
      v1pix_x->push_back(v0pix_x->at(i));
      v1pix_y->push_back(v0pix_y->at(j));
    }
  }
  v1pix_val = v0pix_val;
  v1t = v0t;
  for (Int_t i = 0; i < indrem.size(); i++)
  {
    v1pix_val->erase(v1pix_val->begin() + indrem[i]);
    //v1t      ->erase(v1t->begin() + indrem[i]);
  }
  


  // Create function for FFT
  double dt = 0.1;
  vector<int> func;
 // starttime = v1t[0];
 // cout << starttime << endl;




  TFile *f = new TFile("results/histos.root", "UPDATE");
  h0t      ->Write();
  h0pix_val->Write();
  h0pix_x  ->Write();
  h0pix_y  ->Write();
  h0xy     ->Write();
  h1t      ->Write();
  h1pix_val->Write();
  h1pix_x  ->Write();
  h1pix_y  ->Write();
  h1xy     ->Write();
  f->Close();






return;
  gStyle->SetOptStat(0);
  TCanvas * c1 = new TCanvas("c1", "");
  TLegend * leg1 = new TLegend(0.6, 0.65, 0.80, 0.90);
  c1->SetLogy();
  c1->cd();
  //pix_val_data->SetLineColor(kRed);
  h0pix_val->SetXTitle("Pixel Value");
  h0pix_val->Draw("");
  leg1->SetFillStyle(0);
  leg1->SetBorderSize(0);
  leg1->SetTextSize(0.04);
  leg1->AddEntry(h0pix_val, "Beam on", "l");
  leg1->Draw();
 
}
