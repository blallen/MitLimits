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
    LimitTask(TaskProcesses *taskProcesses);
    virtual ~LimitTask();

    // Set Up Task
    void                 SetCutVariable   (const char* var)  { fCutVariable = TString(var); }
    void                 SetCutValue      (double z)         { fCutValue = z; }
    void                 SetRootFileName  (const char* file) { fRootFileName = TString(file); }
        
    // Write Data Card from nTuples
    void                 WriteDataCard       (CardType cType);
					      
  private:        
    // Open ROOT file and read histograms
    void                 ReadRootFile        ();

    // Write Header for any Data Card
    void                 WriteHeader         ();
    void                 WriteSystematics    ();

    // Functions for counting experiment cards
    void                 WriteDataIntegral   (const char* draw);
    void                 WriteMCIntegral     (const char* draw);
    void                 WriteSystIntegral   ();
    int                  GetCutBin           (const TH1D* hTmp);
    double               GetPercError        (double ObsInt, const char* process, 
					      const char* syst, const char* direction);

    // Functions for Binned Shape analysis cards
    void                 WriteShapeBinned    ();
    void                 WriteSystBinned     ();
    //void                 WriteDataBinned     (const char* draw);

    // Helper to set scale for a histogram
    void                 OverlayEmptyHist    () const;

    TaskProcesses       *fTask;         // analysis task to be plotted
        
    FILE                *fCard;         // textfile for data card
    TString              fRootFileName; // name of rootfile 
    TFile               *fRootFile;     // root file for reading;

    TH1D*                fDataHist;     // data histogram if it exists
    std::vector<TH1D*>   fSigHists;     // list of signal histograms
    std::vector<TH1D*>   fBgHists;      // list of background histograms
    std::vector<TH1D*>   fHistsToPlot;  // list of all MC histograms

    double               fExpBg;        // total number of background events expected

    TString              fCutVariable;  // name of variable being investigated
    double               fCutValue;     // value at which to start integral

    TFile               *fBinnedOut;   // root file of histograms for binned shape analysis

    ClassDef(LimitTask, 0) // LimitTask plots analysis task with various options
  };
}
#endif
