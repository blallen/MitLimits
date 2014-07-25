#include "MitPlots/Style/interface/MitStyle.h" 
 #include "MitLimits/Limit/interface/LimitTask.h" 
 #include "TLegend.h" 
 #include "TH1.h" 
 #include "TH1D.h" 
 #include "TStyle.h" 
 using namespace std; 
 using namespace mithep; 
 //================================================================================================== 

void PlotExpectedLimits_WorkingPoint_metRaw_Integral() 
{
// setup graphics stuff before starting 
 MitStyle::Init(); 
 TStyle *MitStyle = gStyle;// new TStyle("MIT-Style","The Perfect Style for Plots ;-)");

 MitStyle->SetCanvasDefW      (1000);
 MitStyle->SetPadBottomMargin(0.18);
 MitStyle->SetPadLeftMargin  (0.18);
 MitStyle->SetPadRightMargin (0.18);

 TCanvas *canvas = new TCanvas;
 TH1D* hExpectedLimits = new TH1D("ExpLimit", "Expected Model Independent Limits",2,250,350);
 MitStyle::InitHistWide(hExpectedLimits,"metRaw","Expected Limits",kBlack);
 hExpectedLimits->SetMarkerSize(2.5);

hExpectedLimits->Fill(250,0.6660);

hExpectedLimits->Fill(300,0.8164);

hExpectedLimits->Draw("hist");
;
 canvas->SaveAs("Plots/WorkingPoint_metRaw_ExpLimits.png");
}
