// $Id: Process.cc,v 1.2 2014/06/27 11:30:30 paus Exp $

#include "MitLimits/Input/interface/Process.h"

ClassImp(mithep::Process)

using namespace std;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
Process::Process(const char *name, const char* type) :
  fName     (name),
  fType     (type),
  fNSystematics(0)
{
  // Constructor
}

//--------------------------------------------------------------------------------------------------
void Process::Show() const
{
  // Show information of this sample
  printf(" %-10s %-6s Number of Systematics: %-3u",fName.Data(),fType.Data(),fNSystematics);
  for (UInt_t i0 = 0; i0 < fNSystematics; i0++) {
    printf(" %-6s", fSystematics[i0].Data());
  }
  printf("\n");
}

//--------------------------------------------------------------------------------------------------
// UInt_t *Process::AddSystematic(TString syst)
void Process::AddSystematic(TString syst)
{
  // Adding another systematic (vector takes care of memory management)

  TString *Syst = new TString(syst);
  fSystematics.push_back(*Syst);
  fNSystematics++;
  delete Syst;

  // return &fSystematics[fSystematics.size()-1];
  return;
}


//--------------------------------------------------------------------------------------------------
const TString *Process::GetSystematic(UInt_t iSystematic) const
{
  // Get sample corresponding to given sample number. Return NULL pointer if index out of range.

  if (iSystematic >= fNSystematics)
    return 0;

  return &fSystematics[iSystematic];
}
