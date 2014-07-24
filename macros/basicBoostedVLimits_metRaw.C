#include <TSystem.h> 
#include "MitLimits/Limit/interface/LimitTask.h" 
using namespace std; 
using namespace mithep; 
//==================================================================================================

void basicBoostedVLimits_metRaw() 
{

gSystem->Setenv("MIT_PROD_CFG","boostedv-v5"); 
gSystem->Setenv("MIT_ANA_HIST","/scratch4/dimatteo/cms/hist/boostedv-v5/merged-p1/"); 
gSystem->Setenv("MIT_ANA_CFG","boostedv-limits-datadriven"); 

LimitTask *limitTask = 0; 
limitTask = new LimitTask(0); 
limitTask->SetCutVariable("metRaw"); 
limitTask->SetRootFileName("DataCard_BoostedV_250");

limitTask->SetCutValue(250);
limitTask->WriteDataCard(Integral);

limitTask->SetCutValue(260);
limitTask->WriteDataCard(Integral);

delete limitTask; 
return; 
}