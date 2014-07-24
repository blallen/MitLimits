//--------------------------------------------------------------------------------------------------
// Basic limits for the boostedV analysis.
//
// Authors: B.Allen                                                                        (July 2014)
//--------------------------------------------------------------------------------------------------
#include <TSystem.h>
#include "MitLimits/Limit/interface/LimitTask.h"
using namespace std;
using namespace mithep;
//==================================================================================================
void basicBoostedVLimits()
{
  // set the folder containing the input ntuples properly
  // here you can change the plot sources, these are the defaults
  gSystem->Setenv("MIT_PROD_CFG","boostedv-v5");
  gSystem->Setenv("MIT_ANA_HIST","/scratch4/dimatteo/cms/hist/boostedv-v5/merged-p1/");

  // our plot task
  LimitTask *limitTask = 0;
  
  // Set ENV vars for Limit task
  gSystem->Setenv("MIT_LMT_CFG","boostedv-limits-Wlnu");
  
  // raw met with basic cuts
  limitTask = new LimitTask(0);
  limitTask->SetCutVariable("metRaw");
  limitTask->SetRootFileName("DataCard_WP_Wlnu_CR");

  limitTask->SetCutValue(250);
  limitTask->WriteDataCard(Integral);
  
  /*

  limitTask->SetCutValue(260);
  limitTask->WriteDataCard(Integral);

  limitTask->SetCutValue(270);
  limitTask->WriteDataCard(Integral);

  limitTask->SetCutValue(280);
  limitTask->WriteDataCard(Integral);

  limitTask->SetCutValue(290);
  limitTask->WriteDataCard(Integral);

  limitTask->SetCutValue(300);
  limitTask->WriteDataCard(Integral);
  */

  //limitTask->SetCutValue(350);
  //limitTask->WriteDataCard(Integral);
  


  printf("FinishedWriteDataCard!\n");
  delete limitTask;
 
  return;
}
