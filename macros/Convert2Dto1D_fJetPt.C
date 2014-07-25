#include <TROOT.h> 
#include <TMath.h> 
#include <TPad.h> 
#include <TFile.h> 
#include <TH1.h> 
#include <TH1D.h> 
#include <TH2.h> 
#include <TH2D.h> 
#include "MitPlots/Style/interface/MitStyle.h" 

using namespace std; 
using namespace mithep; 

void Convert2Dto1D_fJetPt() 
{

MitStyle::Init(); 
TFile *f2D = new TFile("DataCard_WorkingPoint.root"); 
TFile *f1D; 
TH2D* h2Dtmp = 0; 
TH1D* h1Dtmp = 0; 
int Ybin = 0;

f1D = new TFile(TString("DataCard_WorkingPoint_fJetPt_300_metRaw.root"), "RECREATE");
h2Dtmp = (TH2D*)f2D->Get("WHinv");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("WHinv", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("ZH->qqinv");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("ZH->qqinv", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("ggHinv");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("ggHinv", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("Z+jets");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("Z+jets", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("W+jets");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("W+jets", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("VV");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("VV", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("top");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("top", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("Z->ll");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("Z->ll", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("QCD");
Ybin = (300 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("QCD", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

f1D = new TFile(TString("DataCard_WorkingPoint_fJetPt_250_metRaw.root"), "RECREATE");
h2Dtmp = (TH2D*)f2D->Get("WHinv");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("WHinv", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("ZH->qqinv");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("ZH->qqinv", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("ggHinv");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("ggHinv", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("Z+jets");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("Z+jets", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("W+jets");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("W+jets", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("VV");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("VV", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("top");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("top", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("Z->ll");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("Z->ll", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

h2Dtmp = (TH2D*)f2D->Get("QCD");
Ybin = (250 / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;
h1Dtmp = h2Dtmp->ProjectionX("QCD", Ybin, h2Dtmp->GetNbinsY()+1, "o");
MitStyle::InitHist(h1Dtmp,"metRaw","Number of Events",kBlack);
f1D->cd(); 
h1Dtmp->Write();

return;
}