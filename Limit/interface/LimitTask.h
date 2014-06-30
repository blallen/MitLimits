//--------------------------------------------------------------------------------------------------
// LimitTask
//
// This class implements LimitTask which converts histograms into limits
//
// Authors: B. Allen [with extensive copying from PlotTask by C.Paus, J.Bendavid]
//--------------------------------------------------------------------------------------------------
#ifndef MITLIMITS_LIMIT_LIMITTASK_H
#define MITLIMITS_LIMIT_LIMITTASK_H

#include <TFile.h>
#include <TH1D.h>
#include "MitLimits/Input/interface/TaskProcesses.h"
#include "MitPlots/Style/interface/HistStyles.h"

namespace mithep 
{
  enum CardType {
    Integral,          // Pull integral contributions for counting experiment data card
    Binned,            // Save Histograms for a Binned Shape analysis data card
    Unbinned           // Does things for an Unbinned Shape analysis data card (in development)
  };
  
  class LimitTask
  {
  public:
    LimitTask(TaskProcesses *taskProcesses, const double lumi);
    ~LimitTask();

    // Define histogram ranges
    void                 SetHistRanges    (double xmin,   double xmax,
					   double ymin=0, double ymax=0) {
                                           fHistXMinimum = xmin; fHistXMaximum = xmax;
                                           fHistMinimum  = ymin; fHistMaximum  = ymax; }
    void                 SetHistMinimum   (double min)     { fHistMinimum = min; }
    void                 SetHistMaximum   (double max)     { fHistMaximum = max; }
    void                 SetHistXMinimum  (double min)     { fHistXMinimum = min; }
    void                 SetHistXMaximum  (double max)     { fHistXMaximum = max; }
    void                 SetPngFileName   (const char *n)  { fPngFileName = n; }
    // Other plot parameters
    void                 SetHistStyles    (HistStyles *hS) { fHistStyles = hS; }
    void                 SetAxisTitles    (const char* xtit, const char* ytit = "Number of Events");
    void                 SetLogy          (bool b)         { fLogy = b; }
    void                 SetXLegend       (double x)       { fXLegend = x; }
    void                 SetYLegend       (double y)       { fYLegend = y; }
    void                 SetNRebin        (UInt_t n)       { fNRebin = n; }
    void                 SetNBins         (UInt_t n)       { fNBins = n;  }
    void                 SetDrawExp       (const char* draw, const char* sel);
    
    void                 SetPuTarget      (const TH1D *h)  { fPuTarget = h; }
    
    static float         PuWeight         (Int_t npu);
        
    // Write Data Card from nTuples
    void                 WriteDataCard       (CardType cType, const char* file, const char* channel);
					      
  private:
    void                 DrawFrame           ();
    void                 DrawHistograms      ();
        
    // Open ROOT file and read histograms
    void                 ReadRootFile        ();

    // Write Header for any Data Card
    void                 WriteHeader         ();
    void                 WriteSystematics    ();

    // Functions for counting experiment cards
    void                 WriteDataIntegral   (const char* draw);
    void                 WriteMCIntegral     (const char* draw);
    void                 WriteSystIntegral   ();
    double               GetPercError        (double ObsInt, const char* process, 
					      const char* syst, const char* direction);

    // Functions for Binned Shape analysis cards
    void                 WriteShapeBinned    ();
    void                 WriteSystBinned     ();
    //void                 WriteDataBinned     (const char* draw);

    // Helper to set scale for a histogram
    void                 OverlayEmptyHist    () const;

    TaskProcesses       *fTask;         // analysis task to be plotted
    HistStyles          *fHistStyles;   // styles for plotting
    const double         fTargetLumi;   // target luminosity for task
    
    FILE                *fCard;         // textfile for data card
    TString              fRootFileName; // name of rootfile 
    TFile               *fRootFile;     // root file for reading;

    UInt_t               fIdxHistMax;   // index of histogram with maximum entry
    UInt_t               fNRebin;       // rebin parameter (default is 1)
    TH1D*                fEmptyHist;    // empty identical histogram
    TH1D*                fDataHist;     // data histogram if it exists
    double               fHistMinimum;  // histogram maximum to be displayed
    double               fHistMaximum;  // histogram minimum to be displayed
    double               fHistXMinimum; // histogram x-axis maximum to be displayed
    double               fHistXMaximum; // histogram x-axis minimum to be displayed
    TString              fAxisTitleX;   // x axis title
    TString              fAxisTitleY;   // y axis title
    bool                 fLogy;         // use log scale on y axis?
    double               fXLegend;      // x position of upper left legend box
    double               fYLegend;      // y position of upper left legend box
    UInt_t               fNBins;        // number of bins for TTree-derived histograms
    TString              fDrawExp;      // draw expression for TTree::Draw
    TString              fSelExp;       // selection expression for TTree::Draw
    TString              fPngFileName;  // name of the png file for the plos
    std::vector<TH1D*>   fSigHists;     // list of signal histograms
    std::vector<TH1D*>   fBgHists;      // list of background histograms

    std::vector<TH1D*>   fHistsToPlot;  // list of all MC histograms

    const TH1D          *fPuTarget;
    static const TH1D   *sPuWeights;

    ClassDef(LimitTask, 0) // LimitTask plots analysis task with various options
  };
}
#endif
