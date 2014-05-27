// Really simple starting macro to plot L1Efficiencies

void plotRatesBasic(){

  gROOT->LoadMacro("tdrStyle.C");
  setTDRStyle();
  TH1::SetDefaultSumw2();

  TFile *fileInput= new TFile("../RateExample.root");
  TTree* ntuple=fileInput->Get("RateExample/Ntuple"); 

  Int_t entries=ntuple->GetEntries(); 

  double scaleRate = 11246*2590/entries;

  ntuple->Draw("pt[0]>>PtHisto(65,0,65)","");
  ntuple->Draw("ptIso[0]>>PtHistoIso(65,0,65)","");

  
  TH1F *Rate=PtHisto->Clone(); Rate->SetName("Rate");
  TH1F *RateIso=PtHistoIso->Clone(); RateIso->SetName("RateIso");

  for (int i=0; i<Rate->GetNbinsX(); i++) {
            double rate=Rate->Integral(i,-1);
            Rate->SetBinContent(i,rate);
            rate=RateIso->Integral(i,-1);
            RateIso->SetBinContent(i,rate);
  }

  Rate->Scale(scaleRate);
  RateIso->Scale(scaleRate);

  // Plot
  TCanvas *myC= new TCanvas("myC");
  myC->cd();
  myC->SetLogy();

  Rate->Draw();
  RateIso->Draw("sames");

  // Some styling:
  Rate->SetLineColor(kBlue);
  Rate->SetMarkerColor(kBlue);
  Rate->SetMarkerStyle(20);

  RateIso->SetLineColor(kRed);
  RateIso->SetMarkerColor(kRed);
  RateIso->SetMarkerStyle(20);

  Rate->SetXTitle("EG p_{T} [GeV]");
  Rate->SetYTitle("Rate [Hz]");

  TLegend *leg = new TLegend(0.5,0.2,0.9,0.5,NULL,"brNDC");
  leg->SetFillStyle(0);
  TLegendEntry *entry=leg->AddEntry("NULL","EXAMPLE","");
  entry=leg->AddEntry(Rate,"EG Rate","lp");
  entry=leg->AddEntry(RateIso,"isoEG Rate","lp");

  leg->Draw();

  // Save a plot
 
  myC->SaveAs("rateExample.png");

}
