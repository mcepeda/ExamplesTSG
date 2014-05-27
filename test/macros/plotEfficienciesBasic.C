// Really simple starting macro to plot L1Efficiencies

void plotEfficienciesBasic(){

  gROOT->LoadMacro("tdrStyle.C");
  setTDRStyle();
  TH1::SetDefaultSumw2();

  TFile *fileInput= new TFile("../efficiencyExample.root");
  TTree* ntuple=fileInput->Get("rlxEGEfficiency/Ntuple"); 
  TTree* ntupleIso=fileInput->Get("isoEGEfficiency/Ntuple");

  // For this example we will only look at the lead electron 
  ntuple->Draw("ptReco[0]>>EGEffi(20,0,100)","ptL1[0]>25&&abs(etaReco[0])<2.5");
  ntuple->Draw("ptReco[0]>>Den(20,0,100)","abs(etaReco[0])<2.5");
  ntupleIso->Draw("ptReco[0]>>isoEGEffi(20,0,100)","ptL1[0]>25&&abs(etaReco[0])<2.5");
  ntupleIso->Draw("ptReco[0]>>Den2(20,0,100)","abs(etaReco[0])<2.5");

  EGEffi->Divide(Den);
  isoEGEffi->Divide(Den2);

  // Plot
  TCanvas *myC= new TCanvas("myC");
  myC->cd();
  EGEffi->Draw("");
  isoEGEffi->Draw("sames");

  // Some styling:
  EGEffi->SetLineColor(kBlue);
  isoEGEffi->SetLineColor(kRed);
  EGEffi->SetMarkerColor(kBlue);
  isoEGEffi->SetMarkerColor(kRed);
  EGEffi->SetMarkerStyle(20);
  isoEGEffi->SetMarkerStyle(21);

  EGEffi->SetXTitle("EG p_{T} [GeV]");
  EGEffi->SetYTitle("Efficiency");

  EGEffi->GetYaxis()->SetRangeUser(0,1.1);

  TLegend *leg = new TLegend(0.5,0.2,0.9,0.5,NULL,"brNDC");
  leg->SetFillStyle(0);
  TLegendEntry *entry=leg->AddEntry("NULL","EXAMPLE","");
  entry=leg->AddEntry(EGEffi,"EG Efficiency","lp");
  entry=leg->AddEntry(isoEGEffi,"isoEG Efficiency","lp");
  leg->Draw();

  // Save a plot
 
  myC->SaveAs("effiExample.png");


}
