// $Id: TaskProcesses.cc,v 1.2 2014/06/27 14:24:52 fabstoec Exp $

#include <TSystem.h>
#include "MitLimits/Input/interface/TaskProcesses.h"

ClassImp(mithep::TaskProcesses)

using namespace std;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
TaskProcesses::TaskProcesses(const char *name, const char* dir) :
  fName          (name),
  fNameTxt       (name),
  fDir           (dir),
  fNSigProcesses (0),
  fNBgProcesses  (0),
  fNDataProcesses(0),
  fNSystematics  (0)
{
  // Constructor
}

//--------------------------------------------------------------------------------------------------
void TaskProcesses::Show() const
{
  // Show present list of defined samples

  printf("\n ==== Analysis task overview -- %s ====\n\n",fName.Data());
  printf(" Histogram directory: %s\n\n",fDir.Data());
  printf(" Dataset name                   Skim   Legend               Histogram file");
  printf("                                                  Cross Section [pb]   Scale\n");
  printf(" ------------------------------------------------------------------------");
  printf("----------------------------------------------------------------------------\n");
  for (UInt_t i=0; i<fDataProcesses.size(); i++)
    fDataProcesses[i].Show();
  printf(" ------------------------------------------------------------------------");
  printf("----------------------------------------------------------------------------\n");
  for (UInt_t i=0; i<fSigProcesses.size(); i++)
    fSigProcesses[i].Show();
  printf(" ------------------------------------------------------------------------");
  printf("----------------------------------------------------------------------------\n");
  for (UInt_t i=0; i<fBgProcesses.size(); i++)
    fBgProcesses[i].Show();
  
  return;
}

//--------------------------------------------------------------------------------------------------
const Process *TaskProcesses::GetSigProcess(UInt_t iProcess) const
{
  // Get sample corresponding to given sample number. Return NULL pointer if index out of range.

  if (iProcess >= fNSigProcesses)
    return 0;

  return &fSigProcesses[iProcess];
}

//--------------------------------------------------------------------------------------------------
const Process *TaskProcesses::GetBgProcess(UInt_t iProcess) const
{
  // Get sample corresponding to given sample number. Return NULL pointer if index out of range.

  if (iProcess >= fNBgProcesses)
    return 0;

  return &fBgProcesses[iProcess];
}

//--------------------------------------------------------------------------------------------------
const Process *TaskProcesses::GetDataProcess(UInt_t iProcess) const
{
  // Get data sample. Return NULL pointer if not available.

  if (iProcess >= fNDataProcesses)
    return 0;

  return &fDataProcesses[iProcess];
}

//--------------------------------------------------------------------------------------------------
Process *TaskProcesses::AddSigProcess(const char* name, const char* type)
{
  // Adding another sample (vector takes care of memory management)

  Process* tmpProcess = new Process(name,type);
  fSigProcesses.push_back(*tmpProcess);
  fNSigProcesses++;
  // cleanup after yourself
  delete tmpProcess;

  return &fSigProcesses[fSigProcesses.size()-1];
}

//--------------------------------------------------------------------------------------------------
Process *TaskProcesses::AddBgProcess(const char* name, const char* type)
{
  // Adding another sample (vector takes care of memory management)

  Process* tmpProcess = new Process(name,type);
  fBgProcesses.push_back(*tmpProcess);
  fNBgProcesses++;
  // cleanup after yourself
  delete tmpProcess;

  return &fBgProcesses[fBgProcesses.size()-1];
}

//--------------------------------------------------------------------------------------------------
Process *TaskProcesses::AddDataProcess(const char* name, const char* type)
{
  // Adding another the data sample (existing definition is overwritten)

  Process* tmpProcess = new Process(name,type);
  fDataProcesses.push_back(*tmpProcess);
  fNDataProcesses++;
  // cleanup after yourself
  delete tmpProcess;

  return &fDataProcesses[fDataProcesses.size()-1];
}

//--------------------------------------------------------------------------------------------------
// TString *TaskProcesses::AddSystematic(const char* name)
void TaskProcesses::AddSystematic(const char* name)
{
  // Adding another systematic (vector takes care of memory management)

  TString *Name = new TString(name);
  fSystematics.push_back(*Name);
  fNSystematics++;
  delete Name;

  // return &fSystematics[fSystematics.size()-1];
  return;
}


//--------------------------------------------------------------------------------------------------
const TString *TaskProcesses::GetSystematic(UInt_t iSystematic) const
{
  // Get sample corresponding to given sample number. Return NULL pointer if index out of range.

  if (iSystematic >= fNSystematics)
    return 0;

  return &fSystematics[iSystematic];
}

//--------------------------------------------------------------------------------------------------
void TaskProcesses::ReadFile(const char* dir)
{
  // Reading the full task setup from a single file

  char    name[1024], type[1024], SystNames[5][1024]; //legend[1024], json[1024];
  //float   xsec,scale,overlap;
  UInt_t syst[5];

  Long64_t size;
  Long_t   id, flags, modtime;

  // construct name of the config file
  TString txtFile = TString(dir)+TString("/")+fNameTxt+TString(".txt");
  printf("\n Initializing analysis task from  %s\n\n",txtFile.Data());

  // test whether file exists and say something if it does not 
  if (gSystem->GetPathInfo(txtFile.Data(),&id,&size,&flags,&modtime) != 0) {
    printf("\n TaskProcesses::ReadFile() - ERROR - file does not exist. EXIT! %s\n\n",txtFile.Data());
    return;
  }

  // open file in a pipe (leaves options for filtering)
  FILE *f = gSystem->OpenPipe((TString("cat ")+txtFile+TString("| grep -v ^#")).Data(),"r");
  printf("           Cross Section [pb]  Dataset name                              ");
  printf("Legend               Skim?  \n");
  printf(" ------------------------------------------------------------------------");
  printf("----------------------------\n");
  
  
  //Add systematics to TaskProcess
  fscanf(f,"%s %s",SystNames[0],SystNames[1]);
  // TString *tmpName = 0;
  for (int i = 0; i < 2; i++)
    {
      // tmpName = AddSystematic(SystNames[i]);
      AddSystematic(SystNames[i]);
    }
    
  while (fscanf(f,"%s %s %u %u",name,type,&syst[0],&syst[1])
	 != EOF) {
    // show what was read
    printf(" adding: %-10s %-6s\n",
	   name,type);
    
    Process *tmpProcess = 0;
    TString data = "data";
    TString sig = "sig";
    if (type == data)                                          // found 'the data process
      {
	tmpProcess = AddDataProcess(name,type);
      }
    else if (type == sig)                                      // define new signal process
      {
	tmpProcess = AddSigProcess(name,type);   
      }                            
    else                                                       // define new background process
      tmpProcess = AddBgProcess(name,type);

    //Add systematics to process
    // UInt_t *tmpSyst = 0;
    for (int i = 0; i < 2; i++)
      {
	// tmpSyst = tmpProcess->AddSystematic(syst[i]);
	tmpProcess->AddSystematic(syst[i]);
      }
    
    // Convert '~' -> ' '
    // TString tmpLegend = TString(legend);
    //tmpLegend.ReplaceAll(TString("~"),TString(" "));
    //tmpProcess->SetLegend(tmpLegend.Data());
  }
  gSystem->ClosePipe(f);

  return;
}
