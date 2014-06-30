#include <vector>
#include <TROOT.h>
#include <TSystem.h>
#include <TMath.h>
#include <TPad.h>
//#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TBox.h>
#include <TMarker.h>
#include <TLatex.h>
#include <TTree.h>
#include "MitCommon/Utils/interface/Utils.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitPlots/Style/interface/MitStyle.h"
#include "MitPlots/Plot/interface/PlotTask.h"
#include "MitLimits/Limit/interface/LimitTask.h"

ClassImp(mithep::LimitTask)

using namespace std;
using namespace mithep;

const TH1D *LimitTask::sPuWeights = 0;

//--------------------------------------------------------------------------------------------------
LimitTask::LimitTask(TaskProcesses *taskProcesses, const double lumi) :
  fTask        (taskProcesses),
  fHistStyles  (0),
  fTargetLumi  (lumi),
  fRootFileName("mitLimitTask"),
  fIdxHistMax  (-1),
  fNRebin      (1),
  fEmptyHist   (0),
  fDataHist    (0),
  fHistMinimum (0),
  fHistMaximum (0),
  fHistXMinimum(0),
  fHistXMaximum(0),
  fAxisTitleX  (""),
  fAxisTitleY  ("Number of Events"),
  fLogy        (false),
  fXLegend     (65.),
  fYLegend     (94.),
  fNBins       (100),
  fPngFileName ("mitLimitTask.png"),
  fPuTarget    (0)
{
  // Constructor

  // If the task samples samples not yet already defined, do it now
  if (!fTask) {
    // read all environment variables
    TString home     = Utils::GetEnv("HOME");
    TString mitMyAna = Utils::GetEnv("MIT_USER_DIR");
    TString hstDir   = Utils::GetEnv("MIT_ANA_HIST");
    TString anaCfg   = Utils::GetEnv("MIT_ANA_CFG");
    TString prdCfg   = Utils::GetEnv("MIT_PROD_CFG");
    
    // define sample
    fTask = new TaskProcesses(prdCfg.Data(),hstDir.Data());
    fTask->SetNameTxt(anaCfg.Data());
    fTask->ReadFile((mitMyAna + TString("/config")).Data());
  }
}

//--------------------------------------------------------------------------------------------------
LimitTask::~LimitTask()
{
  // Destructor

  if (fTask)
    delete fTask;
  if (fHistStyles)
    delete fHistStyles;
  if (fEmptyHist)
    delete fEmptyHist;
  if (fDataHist)
    delete fDataHist;

  // I do not understand why when deleting the histograms I get a crash but I do, so I just clear
  // which will leave me all the histograms dangeling around

  //for (unsigned int i=0; i<fHists.size(); ++i) {
  //  delete fHists[i];
  //}
  fSigHists.clear();
  //for (unsigned int i=0; i<fStackedHists.size(); ++i) {
  //  delete fStackedHists[i];
  //}
  fBgHists.clear();
  //for (unsigned int i=0; i<fHistsToPlot.size(); ++i) {
  //  delete fHistsToPlot[i];
  //}
  fHistsToPlot.clear();
}

//--------------------------------------------------------------------------------------------------
void LimitTask::SetAxisTitles(const char* xtit, const char* ytit)
{
  fAxisTitleX = TString(xtit);
  fAxisTitleY = TString(ytit);

  return;
}

//--------------------------------------------------------------------------------------------------
void LimitTask::SetDrawExp(const char* draw, const char* sel)
{
  fDrawExp = TString(draw);
  fSelExp  = TString(sel);

  return;
}

//--------------------------------------------------------------------------------------------------
void LimitTask::DrawFrame()
{
  // find the maximum
  double maximum = 0;
  if (fHistMaximum > 0)
    maximum = fHistMaximum;
  else {
    for (unsigned int i=0; i<fHistsToPlot.size(); i++) { 
      TH1D *h = fHistsToPlot[i];
      if (h->GetMaximum() > maximum)
	maximum = h->GetMaximum();
    }
  }

  // draw the frame
  TH1D *hTmp = new TH1D("FRAME","FRAME",fNBins,fHistXMinimum,fHistXMaximum);
  MitStyle::InitHist(hTmp,fAxisTitleX.Data(),fAxisTitleY.Data(),kBlack);
  if (fHistMinimum != 0)
    hTmp->SetMinimum(fHistMinimum);
  hTmp->SetMaximum(maximum*1.1);
  // draw the frame large enough for the largest contribution (+10%)
  hTmp->DrawCopy("hist");

  // delete the temporary histogram
  if (hTmp)
    delete hTmp;
}

//--------------------------------------------------------------------------------------------------
void LimitTask::DrawHistograms()
{
  // here we draw
  fHistStyles->ResetStyle();
  const HistStyle *hStyle = 0;
  for (unsigned int i=0; i<fHistsToPlot.size(); i++) { 
    TH1D *h = fHistsToPlot[i];
    hStyle = fHistStyles->CurrentStyle();
    h->SetLineColor(hStyle->Color());
    h->Draw("same,hist");        // do not use DrawCopy ... it does not display the first histogram
    fHistStyles->NextStyle();
  }
}

//--------------------------------------------------------------------------------------------------
void LimitTask::OverlayEmptyHist() const
{
  // Overlay an empty histogram onto the picture
  if (fEmptyHist)
    fEmptyHist->Draw("same");

  return;
}

//--------------------------------------------------------------------------------------------------
float LimitTask::PuWeight(Int_t npu)
{
  if (npu<0)
    return 1.0;
  if (!sPuWeights)
    return 1.0;
  
  return sPuWeights->GetBinContent(sPuWeights->FindFixBin(npu));
}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteDataCard(CardType cType, const char* file, const char* channel)
{

  // Interface to producing all type of data cards

  // give overview of what we plot
  printf(" LimitTask::WriteDataCard -- Writing\n\n");
  printf("   source    : %s\n   variable  : %s\n\n",file,channel);
  printf("   signal samples: %d\n",fTask->NSigProcesses());
  printf("   background samples: %d\n",fTask->NBgProcesses());
  printf("   data      : %d\n\n",fTask->NDataProcesses());

  // make sure there is something to do
  if ((fTask->NSigProcesses() + fTask->NBgProcesses() + fTask->NDataProcesses()) < 1) {
    printf("    ERROR - no samples to plot. EXIT!\n\n");
    return;
  }

  // Read ROOT file and add histograms to vectors
  fRootFileName = file;
  fRootFile = new TFile((fRootFileName.Data()+TString(".root")).Data());
  if (fRootFile->IsOpen() == kFALSE) 
    {
      printf(" WARNING -- File  %s  does not exist! EXIT.\n",
             fRootFileName.Data());
      return;
    }
  ReadRootFile();

  // Create Data Card
  TString type;
  if (cType == Integral) type = "Integral";
  else if (cType == Binned) type = "Binned";
  else if (cType == Unbinned) type = "Unbinned";
  fCard = fopen (fRootFileName+"_"+type+".txt","w");
  fprintf(fCard, "# Data card for %s %s \n", file, channel);

  //Write to Data Card
  WriteHeader();
  if (cType == Binned) WriteShapeBinned();
  WriteDataIntegral(channel);
  WriteMCIntegral(channel);
  if (cType == Integral) WriteSystIntegral();
  else if (cType == Binned) WriteSystBinned();
     

  //if(canvas)
  //delete canvas;
  fclose (fCard);
  //delete fRootFile;
  return;
}
//--------------------------------------------------------------------------------------------------
void LimitTask::WriteHeader()
{
  fprintf(fCard, "#--------------------------------------------------------------------#\n");
  fprintf(fCard, "# Block to specify number of channels, backgrounds,and uncertainties #\n");
  fprintf(fCard, "#--------------------------------------------------------------------#\n");
  fprintf(fCard, "imax 1  number of channels \n");
  fprintf(fCard, "jmax *  number of backgrounds \n");
  //fprintf(fCard, "jmax %u  number of backgrounds \n",fTask->NBgProcesses());
  //pitfprintf(fCard, "kmax *  number of sources of systematic uncertainties \n");
  fprintf(fCard, "kmax %u  number of sources of systematic uncertainties \n", fTask->NSystematics());
  //printf("\n Wrote channel info");

}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteDataIntegral(const char* draw)
{
  double NObs;
  if (fDataHist) 
    NObs = fDataHist->Integral();
  else NObs = -1;
  fprintf(fCard, "#-----------------------------------------------------#\n");
  fprintf(fCard, "# Block to specify bins and number of observed events #\n");
  fprintf(fCard, "#-----------------------------------------------------#\n");
  fprintf(fCard, "bin %s \n", draw);
  fprintf(fCard, "observation %.0f \n", NObs);
  //printf("\n Wrote data values");
}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteMCIntegral(const char* draw)
{
  //maybe have dynamic column widths?
  /*
  printf("\nStart");
  //Record MC rates to data card
  TString *name[50];
  int length = 0;
  for (unsigned int i = 0; i < fHistsToPlot.size(); i++)
    {    
      *name[i] = TString(fHistsToPlot[i]->GetName());
      printf("\nGot name %i", i);
      
      printf("\nString name: %s", *name[i]->Data());
      //printf("\nString length: %i", name[i]->Sizeof());
      //if (name[i]->Sizeof() > length) length = name[i]->Sizeof();
      printf("\nMax length: %i", length);
    }
  */
  fprintf(fCard, "#-------------------------------------------------------#\n");
  fprintf(fCard, "# Block to specify expected signal and background rates #\n");
  fprintf(fCard, "#-------------------------------------------------------#\n");

  fprintf(fCard, "%s %9s", "bin", " ");
  for (unsigned int i = 0; i < fHistsToPlot.size(); i++)
    fprintf(fCard, "%12s", draw);

  fprintf(fCard, "\n%s %5s", "process", " ");
  for (unsigned int i = 0; i < fHistsToPlot.size(); i++)
    {    
      TString name = fHistsToPlot[i]->GetName();
      fprintf(fCard, "%12s", name.Data());
    }

  fprintf(fCard, "\n%s %5s", "process", " ");
  for (unsigned int i = 0; i < fSigHists.size(); i++)
    {
      float j = i + 1;
      j -= fSigHists.size(); //need to call number of signal samples
      fprintf(fCard, "%12.0f", j); 
    }
  for (unsigned int i = 0; i < fBgHists.size(); i++)
    {
      fprintf(fCard, "%12i", i+1); 
    }

  fprintf(fCard, "\n%s %8s", "rate", " ");
  for (unsigned int i = 0; i < fHistsToPlot.size(); i++)
    {
      double rate = fHistsToPlot[i]->Integral();
      fprintf(fCard, "%12.2f", rate);
    }
}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteSystematics()
{
  return;
}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteSystBinned()
{
   fprintf(fCard, "\n#------------------------------------#");
  fprintf(fCard, "\n# Block to specify systematic errors #");
  fprintf(fCard, "\n#------------------------------------#");
  //Add systematic errors to data card

  for (unsigned int j = 0; j < fTask->NSystematics(); j++)
    {      
      fprintf(fCard, "\n%s %6s ", fTask->GetSystematic(j)->Data(), "shape");
      for (unsigned int i = 0; i < fTask->NSigProcesses(); i++)
	{
	  fprintf(fCard, "%12u", *fTask->GetSigProcess(i)->GetSystematic(j));
	}
      for (unsigned int i = 0; i < fTask->NBgProcesses(); i++)
	{
	  fprintf(fCard, "%12u", *fTask->GetBgProcess(i)->GetSystematic(j));
	}
    }
}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteSystIntegral()
{
  fprintf(fCard, "\n#------------------------------------#");
  fprintf(fCard, "\n# Block to specify systematic errors #");
  fprintf(fCard, "\n#------------------------------------#");
  //Add systematic errors to data card

  double ObsInt, UpPerc, DownPerc;
  for (unsigned int j = 0; j < fTask->NSystematics(); j++)
    {     
      fprintf(fCard, "\n%s %6s ", fTask->GetSystematic(j)->Data(), "lnN");
      for (unsigned int i = 0; i < fTask->NSigProcesses(); i++)
	{
	  const Process *p = fTask->GetSigProcess(i);
	  if (*p->GetSystematic(j))
	    {
	      ObsInt = fSigHists[i]->Integral();
	      UpPerc = GetPercError(ObsInt, p->Name()->Data(), fTask->GetSystematic(j)->Data(), "Up");
	      DownPerc = GetPercError(ObsInt, p->Name()->Data(), fTask->GetSystematic(j)->Data(), "Down");
	      fprintf(fCard, "%7.2f/%3.2f", DownPerc, UpPerc);
	    }
	  else 
	    fprintf(fCard, "%12s", "-");
	}
      for (unsigned int i = 0; i < fTask->NBgProcesses(); i++)
	{
	  const Process *p = fTask->GetBgProcess(i);
	  if (*p->GetSystematic(j))
	    {
	      ObsInt = fBgHists[i]->Integral();
	      UpPerc = GetPercError(ObsInt, p->Name()->Data(), fTask->GetSystematic(j)->Data(), "Up");
	      DownPerc = GetPercError(ObsInt, p->Name()->Data(), fTask->GetSystematic(j)->Data(), "Down");
	      fprintf(fCard, "%7.2f/%3.2f", DownPerc, UpPerc);
	    }
	  else 
	    fprintf(fCard, "%12s", "-");
	}
    }
}

//--------------------------------------------------------------------------------------------------
double LimitTask::GetPercError(double ObsInt, const char* process, const char* syst, const char* direction)
{
  TString Name = TString(process)+TString("_")+TString(syst)+TString(direction);
  TH1D* hTmp = (TH1D*)fRootFile->Get(Name.Data());
  if (!hTmp) 
    {
      printf(" WARNING -- %s histogram not found. Next process!\n",Name.Data());
      return 0;
    }
  double DirInt = hTmp->Integral();
  double Perc = 1 + ((DirInt - ObsInt) / ObsInt);
  
  return Perc;
}


//--------------------------------------------------------------------------------------------------
void LimitTask::WriteShapeBinned()
{
  fprintf(fCard, "#-----------------------------------------------------------#\n");
  fprintf(fCard, "# Block to specify path to histos for binned shape analysis #\n");
  fprintf(fCard, "#-----------------------------------------------------------#\n");
  fprintf(fCard, "shapes * * %s $PROCESS $PROCESS_$SYSTEMATIC\n", (fRootFileName.Data()+TString(".root")).Data());
}
//--------------------------------------------------------------------------------------------------
void LimitTask::ReadRootFile()
{
  // Check if histograms have already been read
  if ((fHistsToPlot.size() > 0) || fDataHist)
    {
      printf(" WARNING - histograms already exist. EXIT.");
      return;
    }

  // Add signal processes to appropriate vectors
  for (UInt_t i=0; i<fTask->NSigProcesses(); i++) 
    {
      const Process *p = fTask->GetSigProcess(i);
      TH1D *hTmp = (TH1D*)fRootFile->Get(p->Name()->Data());
      if (!hTmp) 
	{
	  printf(" WARNING -- No histogram found for sample %s. Next sample!\n", p->Name()->Data());
	  continue;
	}
      fSigHists.push_back(hTmp);
      fHistsToPlot.push_back(hTmp);
    }

  // Add background processes to appropriate vectors
  for (UInt_t i=0; i<fTask->NBgProcesses(); i++) 
    {
      const Process *p = fTask->GetBgProcess(i);
      TH1D *hTmp = (TH1D*)fRootFile->Get(p->Name()->Data());
      if (!hTmp) 
	{
	  printf(" WARNING -- No histogram found for sample %s. Next sample!\n", p->Name()->Data());
	  continue;
	}
      fBgHists.push_back(hTmp);
      fHistsToPlot.push_back(hTmp);
    }

  // Point fDataHist at data histogram
  for (UInt_t i=0; i<fTask->NDataProcesses(); i++)
    {
      const Process *p = fTask->GetDataProcess(i);
      TH1D *hTmp = (TH1D*)fRootFile->Get((p->Name()->Data()+TString("_obs")).Data());
      if (!hTmp) 
	{
	  printf(" WARNING -- No histogram found for Data. Observed number of events = -1\n");
	  continue;
	}
      fDataHist = new TH1D(*hTmp);
    }
}
