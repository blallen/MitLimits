// $Id: TaskProcesses.cc,v 1.2 2014/06/27 14:24:52 fabstoec Exp $

#include <TSystem.h>
#include "MitLimits/Input/interface/TaskProcesses.h"
#include "TObjArray.h"
#include "TObjString.h"
#include <fstream>

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
void TaskProcesses::AddSystematic(const char* name, const char* type)
{
  // Adding another systematic (vector takes care of memory management)

  TString *Name = new TString(name);
  TString *Type = new TString(type);
  fSystematics.push_back(*Name);
  fSystTypes.push_back(*Type);
  fNSystematics++;
  delete Name;
  delete Type;

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
const TString *TaskProcesses::GetSystType(UInt_t iSystematic) const
{
  // Get sample corresponding to given sample number. Return NULL pointer if index out of range.

  if (iSystematic >= fNSystematics)
    return 0;

  return &fSystTypes[iSystematic];
}

//--------------------------------------------------------------------------------------------------
void TaskProcesses::ReadFile(const char* dir)
{
  // Reading the full task setup from a single file

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
  // FILE *f = gSystem->OpenPipe((TString("cat ")+txtFile+TString("| grep -v ^#")).Data(),"r");
  ifstream f(txtFile.Data());
  /*
  printf("           Cross Section [pb]  Dataset name                              ");
  printf("Legend               Skim?  \n");
  printf(" ------------------------------------------------------------------------");
  printf("----------------------------\n");
  */
  
  string line;
  TString *lines[66];
  UInt_t nSyst = 0;
  while (getline(f, line))
    {
      lines[nSyst] = new TString(line);
      nSyst++;
    }
  
  TString    name, type, region, SystName, SystType, SystValue;

  Process *tmpProcess = 0;
  TString data = "data";
  TString sig = "sig";

  TObjArray *process = lines[0]->Tokenize("\t ");
  TObjArray *ptype = lines[1]->Tokenize("\t ");
  //  TObjArray *pregion = lines[2]->Tokenize("\t ");
  TObjArray *tmpsyst = 0;
  // printf("\n Number of entries in (first, second) line: (%i, %i) \n", process->GetEntries(),ptype->GetEntries());
  for (Int_t i0 = 1; (i0 < process->GetEntries()) && (i0 < ptype->GetEntries()); i0++) {
    name = ((TObjString*)process->At(i0))->String();
    type = ((TObjString*)ptype->At(i0))->String();
    // region = ((TObjString*)pregion->At(i0))->String();

    /*
    // show what was read
    printf(" adding: %-10s %-6s\n",
	   name.Data(),type.Data()); // ,region.Data());
    */

    if (type == data)                                          // found the data process
      tmpProcess = AddDataProcess(name.Data(),type.Data());
    else if (type == sig)                                      // define new signal process
      tmpProcess = AddSigProcess(name.Data(),type.Data());   
    else                                                       // define new background process
      tmpProcess = AddBgProcess(name.Data(),type.Data());
    
    for (UInt_t i1 = 2; i1 < nSyst; i1++) {
      tmpsyst = lines[i1]->Tokenize("\t ");
      // printf("\n Number of entries in %i line: %i \n", i1, tmpsyst->GetEntries());
            
      if (i0 == 1) {
	SystName = ((TObjString*)tmpsyst->At(0))->String();
	SystType = ((TObjString*)tmpsyst->At(1))->String();
	AddSystematic(SystName,SystType);
      }

      SystValue = ((TObjString*)tmpsyst->At(i0+1))->String();
      tmpProcess->AddSystematic(SystValue);
      
      delete tmpsyst;
    }
  }
  
  delete process;
  delete ptype;
  
  f.close();
  
  return;
}
