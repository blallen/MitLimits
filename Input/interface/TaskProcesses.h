//--------------------------------------------------------------------------------------------------
// $Id: h.TaskProcesses,v 1.2 2014/06/27 14:24:52 fabstoec Exp $
//
// TaskProcesses
//
// This class implements TaskProcesses which fundamentally is a collection of processes used in a given
// analysis task. The data process is not part of the list but is stored separately.
//
// This class manages the data and list of Monte Carlo processes for limit setting
//
// Authors: B.Allen [borrowing heavily from C.Paus]
//--------------------------------------------------------------------------------------------------
#ifndef MITLIMITS_INPUT_TASKSAMPLES_H
#define MITLIMITS_INPUT_TASKSAMPLES_H

#include <vector>
#include <TString.h>
#include "MitLimits/Input/interface/Process.h"

namespace mithep
{
  class TaskProcesses
  {
  public:
    TaskProcesses(const char* name, const char* dir);
    virtual ~TaskProcesses() { }

    const TString          *Name      ()            const { return &fName; }
    const TString          *NameTxt   ()            const { return &fNameTxt; }
    void                    SetNameTxt(const char* n)     { fNameTxt = TString(n); }
    const TString          *Dir       ()            const { return &fDir; }
    void                    Show      ()            const; // show list of task samples
    UInt_t                  NSigProcesses ()           const { return fNSigProcesses; }
    UInt_t                  NBgProcesses  ()           const { return fNBgProcesses; }
    UInt_t                  NDataProcesses()           const { return fNDataProcesses; }
    const Process          *GetSigProcess(UInt_t i)    const; // return 'i'th signal process
    const Process          *GetBgProcess(UInt_t i)     const; // return 'i'th background process
    const Process          *GetDataProcess(UInt_t i)   const; // return 'i'th data process

    UInt_t                   NSystematics()                   const { return fNSystematics; }
    const TString           *GetSystematic(UInt_t i)         const; //return 'i'th systematic

    

    void                    ReadFile     (const char* dir);

  private:
    Process                *AddDataProcess(const char* name, const char* type);
    Process                *AddSigProcess    (const char* name, const char* type);
    Process                *AddBgProcess    (const char* name, const char* type);

    // TString                *AddSystematic(const char* name);
    void                    AddSystematic(const char* name);

    TString                 fName;         // location of production
    TString                 fNameTxt;      // location of configuration (usually same as prod.)
    TString                 fDir;          // directory with sample files (sample can overwrite)

    UInt_t                  fNSigProcesses;  // number of signal samples
    UInt_t                  fNBgProcesses;   // number of background processes
    UInt_t                  fNDataProcesses; // number of data processes
    std::vector<Process>    fSigProcesses;   // list of signal processes in the ROOT file
    std::vector<Process>    fBgProcesses;    // list of background processes in the ROOT file
    std::vector<Process>    fDataProcesses;  // data process (not part of the list

    UInt_t                   fNSystematics; // number of systematics to apply to process
    std::vector<TString>    fSystematics;  // vector of systematics to apply to process

    ClassDef(TaskProcesses, 0) // TaskProcesses manages a list of samples used in an analysis task
  };
}
#endif
