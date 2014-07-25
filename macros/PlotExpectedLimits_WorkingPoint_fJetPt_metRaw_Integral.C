#include "MitPlots/Style/interface/MitStyle.h" 
 #include "MitLimits/Limit/interface/LimitTask.h" 
 #include "TLegend.h" 
 #include "TH2.h" 
 #include "TH2D.h" 
 #include "TStyle.h" 
 using namespace std; 
 using namespace mithep; 
 //================================================================================================== 

void PlotExpectedLimits_WorkingPoint_fJetPt_metRaw_Integral() 
{
// setup graphics stuff before starting 
 MitStyle::Init(); 
 TStyle *MitStyle = gStyle;// new TStyle("MIT-Style","The Perfect Style for Plots ;-)");

 MitStyle->SetCanvasDefW      (1000);
 MitStyle->SetPadBottomMargin(0.18);
 MitStyle->SetPadLeftMargin  (0.18);
 MitStyle->SetPadRightMargin (0.18);

 MitStyle->SetTitleFont  (42   ,"Z");
 MitStyle->SetTitleSize  (0.055,"Z");
 MitStyle->SetTitleOffset(1.700,"Z");
 MitStyle->SetLabelOffset(0.010,"Z");
 MitStyle->SetLabelSize  (0.050,"Z");
 MitStyle->SetLabelFont  (42   ,"Z");
 MitStyle->SetTickLength (-0.01,"Z");
 MitStyle->SetTitleFont  (42,"Z");

TCanvas *canvas = new TCanvas;
 TH2D* hExpectedLimits = new TH2D("ExpLimit", "Expected Model Independent Limits",2,250,350,2,250,350);
 MitStyle::InitHistWide(hExpectedLimits,"metRaw","fJetPt",kBlack);
 hExpectedLimits->SetMarkerSize(2.5);
 hExpectedLimits->GetZaxis()->SetTitle("Expected Limit");

hExpectedLimits->Fill(250,300,0.7754);

hExpectedLimits->Fill(250,250,0.6660);

hExpectedLimits->Fill(300,300,0.8164);

hExpectedLimits->Fill(300,250,0.8164);

hExpectedLimits->Draw("colz");
 hExpectedLimits->Draw("sameTEXT");
 canvas->SaveAs("Plots/WorkingPoint_ExpLimits_fJetPt_metRaw.png");
}
