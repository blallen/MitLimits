//--------------------------------------------------------------------------------------------------
// $Id: Process.h,v 1.2 2014/06/27 11:30:30 paus Exp $
//
// Process
//
// This class implements Process which defines all relevant properties of a data sample needed to
// make plots.
//
// Authors: B.Allen [borrowing heavily from C.Paus]
//--------------------------------------------------------------------------------------------------

#ifndef MITLIMITS_INPUT_SAMPLE_H
#define MITLIMITS_INPUT_SAMPLE_H

#include <TString.h>

namespace mithep 
{
  class Process
  {
  public:
    Process() {}
    Process(const char* name, const char* type);
    ~Process() {}
    
    const TString           *Name()                           const { return &fName; }
    const TString           *Type()                           const { return &fType; }
    UInt_t                   NSystematics()                   const { return fNSystematics; }
    const UInt_t            *GetSystematic(UInt_t i)         const; //return 'i'th systematic

    UInt_t                  *AddSystematic(UInt_t i);

    void                     Show()                           const; // show process properties

  private:
    TString                  fName;         // name of the process
    TString                  fType;         // whether process is sig or bg (or data)
    UInt_t                   fNSystematics; // number of systematics to apply to process
    std::vector<UInt_t>      fSystematics;  // vector of systematics to apply to process
    
    ClassDef(Process, 0) // Process describes all relevant aspects of a sample for plotting
  };
}
#endif
