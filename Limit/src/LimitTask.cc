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
#include "MitLimits/Limit/interface/LimitTask.h"

ClassImp(mithep::LimitTask)

using namespace std;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
LimitTask::LimitTask(TaskProcesses *taskProcesses) :
  fTask        (taskProcesses),
  fCard        (0),
  fRootFileName("mitLimitTask"),
  fRootFile    (0),
  fDataHist    (0),
  fExpBg       (0),
  fCutVariable ("-"),
  fCutValue    (0),
  fBinnedOut   (0)
{
  // Constructor

  // If the task samples samples not yet already defined, do it now
  if (!fTask) {
    // read all environment variables
    TString home     = Utils::GetEnv("HOME");
    TString lmtDir = Utils::GetEnv("MIT_LMT_DIR");
    TString lmtCfg   = Utils::GetEnv("MIT_LMT_CFG");
    
    // define sample
    fTask = new TaskProcesses("","");
    fTask->SetNameTxt(lmtCfg.Data());
    fTask->ReadFile((lmtDir + TString("/config")).Data());
    fTask->Show();
  }
}

//--------------------------------------------------------------------------------------------------
LimitTask::~LimitTask()
{
  // Destructor
  /*
  if (fTask)
    delete fTask;
  if (fDataHist)
    delete fDataHist;

  // I do not understand why when deleting the histograms I get a crash but I do, so I just clear
  // which will leave me all the histograms dangeling around

  for (unsigned int i=0; i<fSigHists.size(); ++i) {
    delete fSigHists[i];
  }
  // fSigHists.clear();
  for (unsigned int i=0; i<fBgHists.size(); ++i) {
    delete fBgHists[i];
  }
  // fBgHists.clear();
  for (unsigned int i=0; i<fHistsToPlot.size(); ++i) {
    delete fHistsToPlot[i];
  }
  // fHistsToPlot.clear();
  */
}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteDataCard(int cType)
{

  // Interface to producing all type of data cards

  // give overview of what we plot
  printf("\n LimitTask::WriteDataCard -- Writing\n\n");
  printf("   source    : %s\n   variable  : %s\n   cut value : %.2f\n\n",fRootFileName.Data(),fCutVariable.Data(),fCutValue);
  printf("   signal samples: %d\n",fTask->NSigProcesses());
  printf("   background samples: %d\n",fTask->NBgProcesses());
  printf("   data      : %d\n\n",fTask->NDataProcesses());

  // make sure there is something to do
  if ((fTask->NSigProcesses() + fTask->NBgProcesses() + fTask->NDataProcesses()) < 1) {
    printf("    ERROR - no samples to plot. EXIT!\n\n");
    return;
  }

  // Read ROOT file and add histograms to vectors
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
  fCard = fopen (fRootFileName+"_"+(Long_t)fCutValue+"_"+type+".txt","w");
  //fCard = fopen (fRootFileName+"_"+fCutVariable.Data()+"_"+(Long_t)fCutValue+"_"+type+".txt","w");
  fprintf(fCard, "# Data card for %s %s \n", fRootFileName.Data(), fCutVariable.Data());

  //Write to Data Card
  WriteHeader();
  if (cType == Binned) WriteShapeBinned();
  WriteDataIntegral(fCutVariable.Data());
  WriteMCIntegral(fCutVariable.Data(),cType);
  WriteSystematics();

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
  if (fDataHist && fTask->NDataProcesses()) 
    {
      NObs = fDataHist->Integral(GetCutBin(fDataHist),fDataHist->GetNbinsX());
    }
  else NObs = -1;
  fprintf(fCard, "#-----------------------------------------------------#\n");
  fprintf(fCard, "# Block to specify bins and number of observed events #\n");
  fprintf(fCard, "#-----------------------------------------------------#\n");
  fprintf(fCard, "bin %s \n", draw);
  fprintf(fCard, "observation %f \n", NObs);
  //printf("\n Wrote data values");
}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteMCIntegral(const char* draw, int type)
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
    fprintf(fCard, "%20s", draw);

  fprintf(fCard, "\n%s %5s", "process", " ");
  for (unsigned int i = 0; i < fHistsToPlot.size(); i++)
    {    
      TString name = fHistsToPlot[i]->GetName();
      fprintf(fCard, "%20s", name.Data());
    }

  fprintf(fCard, "\n%s %5s", "process", " ");
  for (unsigned int i = 0; i < fSigHists.size(); i++)
    {
      float j = i + 1;
      j -= fSigHists.size(); //need to call number of signal samples
      fprintf(fCard, "%20.0f", j); 
    }
  for (unsigned int i = 0; i < fBgHists.size(); i++)
    {
      fprintf(fCard, "%20i", i+1); 
    }

  double rate = 0;
  fprintf(fCard, "\n%s %8s", "rate", " ");
  for (unsigned int i = 0; i < fHistsToPlot.size(); i++)
    {
      rate = fHistsToPlot[i]->Integral(GetCutBin(fHistsToPlot[i]),fHistsToPlot[i]->GetNbinsX()+1);
      if (type == Binned && rate)
	rate = -1;
      fprintf(fCard, "%20.2f", rate);
    }
}

//--------------------------------------------------------------------------------------------------
int LimitTask::GetCutBin(const TH1D* hTmp)
{
  /*
  double width = hTmp->GetXaxis()->GetBinWidth(1);
  double tmpBin = (fCutValue - hTmp->GetBinLowEdge(1))/width;
  UInt_t CutBin = 0;
  if (fmod(tmpBin,1))
    {	
      tmpBin+=1;
      printf("Cut Value %.2f not divisible by bin width %.2f for histogram %s!\n", fCutValue,width,hTmp->GetName()); 
      CutBin = tmpBin + 1;
      printf("Using Cut Value of %.2f instead! Consider rebinning histogram and running again!\n", hTmp->GetXaxis()->GetBinLowEdge(CutBin));
    }
  else CutBin = tmpBin + 1;
  */

  Int_t CutBin = -1;
  for (Int_t i0 = 1; i0 < hTmp->GetNbinsX(); i0++) {
    if (TMath::Abs( (hTmp->GetBinLowEdge(i0) - fCutValue)/fCutValue ) < 0.01)
      // no more floating point comparisons
      CutBin = i0;
  }

  if (CutBin == -1) {
    printf("Cut Value %.2f does not match any bin boundary for histogram %s!\n Taking entire histogram! \n", fCutValue, hTmp->GetName());
    CutBin = 0;
  }

  return CutBin;
}


//--------------------------------------------------------------------------------------------------
TString LimitTask::GetSystValue(const char* type, const char* syst) {
  // some comparison scheme that determines what to do for each type of systematic and returns the appropriate value to put in the data card
  return TString(syst);
}

//--------------------------------------------------------------------------------------------------
void LimitTask::WriteSystematics()
{
  fprintf(fCard, "\n#------------------------------------#");
  fprintf(fCard, "\n# Block to specify systematic errors #");
  fprintf(fCard, "\n#------------------------------------#");
  //Add systematic errors to data card
   
  
  TString SystName = "default";
  TString SystType = "none";
  
  TString SystValue = "!";
  
  //  const TString *SystName = 0;
  // const TString *SystType = 0;
  // TString *SystValue = 0;

  for (unsigned int i0 = 0; i0 < fTask->NSystematics(); i0++) {
    SystName = *fTask->GetSystematic(i0);
    SystType = *fTask->GetSystType(i0);
    // printf("\n Working on syst: %20s %10s ", SystName.Data(), SystType.Data());
    fprintf(fCard, "\n%s %6s ", SystName.Data(), SystType.Data());
    for (unsigned int i1 = 0; i1 < fTask->NSigProcesses(); i1++) {
      SystValue = GetSystValue(SystType.Data(), fTask->GetSigProcess(i1)->GetSystematic(i0)->Data());
      // printf("%12s", SystValue.Data());
      fprintf(fCard, "%12s", SystValue.Data());
    }
    for (unsigned int i1 = 0; i1 < fTask->NBgProcesses(); i1++) {
      SystValue = GetSystValue(SystType.Data(), fTask->GetBgProcess(i1)->GetSystematic(i0)->Data());
      // printf("%12s", SystValue.Data());
      fprintf(fCard, "%12s", SystValue.Data());
    }
  }
  
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
	  fprintf(fCard, "%12.0f", fTask->GetSigProcess(i)->GetSystematic(j)->Atof());
	}
      for (unsigned int i = 0; i < fTask->NBgProcesses(); i++)
	{
	  fprintf(fCard, "%12.0f", fTask->GetBgProcess(i)->GetSystematic(j)->Atof());
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
	  if (p->GetSystematic(j)->Atof())
	    {
	      ObsInt = fSigHists[i]->Integral(GetCutBin(fSigHists[i]),fSigHists[i]->GetNbinsX()+1);
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
	  if (p->GetSystematic(j)->Atof())
	    {
	      ObsInt = fBgHists[i]->Integral(GetCutBin(fBgHists[i]),fBgHists[i]->GetNbinsX()+1);
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
  double DirInt = hTmp->Integral(GetCutBin(hTmp),hTmp->GetNbinsX()+1);
  double Perc = 1 + ((DirInt - ObsInt) / ObsInt);
  
  return Perc;
}


//--------------------------------------------------------------------------------------------------
void LimitTask::WriteShapeBinned()
{
  Int_t CutVal = fCutValue;
  TString Outname = fRootFileName+TString("_")+(Long_t)CutVal+TString("_Binned.root_")+TString(Utils::GetEnv("MIT_LMT_CFG"));
  fBinnedOut = new TFile(Outname.Data(), "RECREATE");
  fBinnedOut->cd();

  printf("\nDoes data hist exist? %d", (fDataHist != 0));
  if (fDataHist)
    {
      for (int j = 0; j < fDataHist->GetNbinsX(); j++)
	{
	  if (j < GetCutBin(fDataHist))
	    fDataHist->SetBinContent(j, 0);
	}
      fDataHist->SetName("data_obs");
      fDataHist->Write();
    }

  for (UInt_t i = 0; i < fHistsToPlot.size(); i++)
    {
      for (int j = 0; j < fHistsToPlot[i]->GetNbinsX(); j++)
	{
	  if (j < GetCutBin(fHistsToPlot[i]))
	    fHistsToPlot[i]->SetBinContent(j, 0);
	}
       fHistsToPlot[i]->Write();
    }

  for (UInt_t i0 = 0; i0 < fSigSystHists.size(); i0++) {
    for (Int_t i1 = 0; i1 < fSigSystHists[i0]->GetNbinsX(); i1++) {
      if (i1 < GetCutBin(fSigSystHists[i0]))
	fSigSystHists[i0]->SetBinContent(i1, 0);
    }
    fSigSystHists[i0]->Write();
  }

  
  for (UInt_t i0 = 0; i0 < fBgSystHists.size(); i0++) {
    for (Int_t i1 = 0; i1 < fBgSystHists[i0]->GetNbinsX(); i1++) {
      if (i1 < GetCutBin(fBgSystHists[i0]))
	fBgSystHists[i0]->SetBinContent(i1, 0);
    }
    fBgSystHists[i0]->Write();
  }

  delete fBinnedOut;


  fprintf(fCard, "#-----------------------------------------------------------#\n");
  fprintf(fCard, "# Block to specify path to histos for binned shape analysis #\n");
  fprintf(fCard, "#-----------------------------------------------------------#\n");
  fprintf(fCard, "shapes * * %s $PROCESS $PROCESS_$SYSTEMATIC\n", Outname.Data());

  
}
//--------------------------------------------------------------------------------------------------
void LimitTask::ReadRootFile()
{
  // Check if histograms have already been read
  if ((fHistsToPlot.size() > 0) || fDataHist)
    {
      printf(" Histograms already exist. Deleting and re-reading! \n");
      if (fDataHist)
	{	
	  delete fDataHist;
	  fDataHist = 0;
	}
      fSigHists.clear();
      fBgHists.clear();
      fExpBg = 0;
      fHistsToPlot.clear();
    }

  // Point fDataHist at data histogram
  for (UInt_t i=0; i<fTask->NDataProcesses(); i++)
    {
      const Process *p = fTask->GetDataProcess(i);
      // TH1D *hTmp = (TH1D*)fRootFile->Get((p->Name()->Data()+TString("_obs")).Data()); // normally
      TH1D *hTmp = (TH1D*)fRootFile->Get(p->Name()->Data()); // for using Nick's plots
      if (!hTmp) 
	{
	  printf(" WARNING -- No histogram found for Data. Using sum of background processes instead!\n");
	  continue;
	}
      fDataHist = new TH1D(*hTmp);
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

      for (UInt_t i1 = 0; i1 < fTask->NSystematics(); i1++) {
	TString shape = "shape";
	if (*fTask->GetSystType(i1) != shape) continue;
	if (!p->GetSystematic(i1)->Atoi()) continue;
	TString UpName = *p->Name()+TString("_")+*fTask->GetSystematic(i1)+TString("Up");
	// printf("UpName: %s\n", UpName.Data());
	hTmp = (TH1D*)fRootFile->Get(UpName.Data());
	fSigSystHists.push_back(hTmp);
	
	TString DownName = *p->Name()+TString("_")+*fTask->GetSystematic(i1)+TString("Down");
	// printf("DownName: %s\n", DownName.Data());
	hTmp = (TH1D*)fRootFile->Get(DownName.Data());
	fSigSystHists.push_back(hTmp);
      }
    }

  // Add background processes to appropriate vectors
  bool SimData = !fDataHist;
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
      if (SimData)
	{
	  if (i == 0)
	    {
	      fDataHist = new TH1D(*hTmp);
	      fDataHist->SetName("data_obs");
	      fDataHist->SetTitle("data_obs");
	    }
	  else
	    fDataHist->Add(hTmp);
	  fExpBg += hTmp->Integral(GetCutBin(hTmp),hTmp->GetNbinsX()+1);
	}
      
      for (UInt_t i1 = 0; i1 < fTask->NSystematics(); i1++) {
	TString shape = "shape";
	if (*fTask->GetSystType(i1) != shape) continue;
	if (!p->GetSystematic(i1)->Atoi()) continue;
	TString UpName = *p->Name()+TString("_")+*fTask->GetSystematic(i1)+TString("Up");
	// printf("UpName: %s\n", UpName.Data());
	hTmp = (TH1D*)fRootFile->Get(UpName.Data());
	fBgSystHists.push_back(hTmp);

	TString DownName = *p->Name()+TString("_")+*fTask->GetSystematic(i1)+TString("Down");
	// printf("DownName: %s\n", DownName.Data());
	hTmp = (TH1D*)fRootFile->Get(DownName.Data());
	fBgSystHists.push_back(hTmp);
      }
      
    }
}
