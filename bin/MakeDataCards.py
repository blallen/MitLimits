###==================================================================================================
### Script to Run Limit Task to Generate DataCards from Existing Root Files of 1D Histograms
###==================================================================================================
from subprocess import Popen, PIPE
import os
from optparse import OptionParser

###==================================================================================================
### Set Up Input Options
###==================================================================================================

parser = OptionParser()
parser.add_option('-R', help='Root File to extract limits from', dest='RootFileName', action='store', metavar='<name>')
parser.add_option('-T', help='type of datacard to make (Integral, Binned, or Unbinned)', dest='Type', action='store', metavar='<type>')
parser.add_option('-x', help='define X axis', dest='Xaxis', action='store', metavar='<min> <max> <step>', nargs=3, type='int')
parser.add_option('-X', help='set X variable name', dest='Xname', action='store', metavar='<name>')

(opts, args) = parser.parse_args()

mandatories = ['RootFileName','Type','Xaxis','Xname']
for m in mandatories:
    if not opts.__dict__[m]:
        print "Mandatory option is missing\n"
        parser.print_help()
        exit(-1)

(opts, args) = parser.parse_args()

###==================================================================================================
### Initialize the variables!
###==================================================================================================

LimitDir    = os.getenv('MIT_LMT_DIR', os.path.join(os.environ['CMSSW_BASE'], 'src/MitLimits'))
MacroDir    = os.getenv('MIT_MCR_DIR',  os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/macros'))
ProdConfig  = os.getenv('MIT_PROD_CFG', 'boostedv-v5')
AnaHist     = os.getenv('MIT_ANA_HIST', '/scratch4/dimatteo/cms/hist/boostedv-v5/merged-p1/')
LimitConfig = os.getenv('MIT_LMT_CFG',  'boostedv-limits-datadriven')
RootDir     = os.getenv('MIT_ROOT_DIR', os.path.join(os.environ['HOME'],"cms/root"))

RootFileName = opts.RootFileName
Type = opts.Type
Xname = opts.Xname
Xmin = opts.Xaxis[0]
Xmax = opts.Xaxis[1]
Xstep = opts.Xaxis[2]
Xbins = range(Xmin, Xmax+Xstep, Xstep)

###==================================================================================================
### Write Macro File
###==================================================================================================

MacroName = 'basicBoostedVLimits_'+Xname+"_"+Type
FileName = os.path.join(MacroDir,MacroName+'.C')
limitMacro = open(FileName, 'w')

### Write Header ###
limitMacro.write('#include <TSystem.h> \n#include "MitLimits/Limit/interface/LimitTask.h" \nusing namespace std; \nusing namespace mithep; \n//==================================================================================================\n\n')
limitMacro.write('void '+MacroName+'() \n{\n\n')

### Set Sys Env Var ###
limitMacro.write('gSystem->Setenv("MIT_LMT_DIR","'+LimitDir+'"); \ngSystem->Setenv("MIT_PROD_CFG","'+ProdConfig+'"); \ngSystem->Setenv("MIT_ANA_HIST","'+AnaHist+'"); \ngSystem->Setenv("MIT_LMT_CFG","'+LimitConfig+'"); \n\n')

### Initialize limit task
limitMacro.write('LimitTask *limitTask = 0; \nlimitTask = new LimitTask(0); \nlimitTask->SetCutVariable("'+Xname+'"); \nlimitTask->SetRootFileName("'+RootFileName+'");\n\n')

### Generate Data Cards ###
for Xbin in Xbins:
    limitMacro.write('limitTask->SetCutValue('+str(Xbin)+');\n')
    limitMacro.write('limitTask->WriteDataCard('+Type+');\n\n')

### Close Macro ###
limitMacro.write('delete limitTask; \nreturn; \n}\n\n')
limitMacro.close()

###==================================================================================================
### Execute Macro File
###==================================================================================================

limitTask = Popen(['root','-b','-l','-q',MacroName+'.C+'],stdout=PIPE,stderr=PIPE,cwd=RootDir)
(stdout, stderr) = limitTask.communicate()
print stdout
#print stderr

exit(-1)
