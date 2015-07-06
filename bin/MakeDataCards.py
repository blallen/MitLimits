###======================================================================================
### Script to Run Limit Task to Generate DataCards from Existing Root Files of 1D Histos
###======================================================================================
from subprocess import Popen, PIPE
import os
from optparse import OptionParser
from ROOT import *

###======================================================================================
### Set Up Input Options
###======================================================================================

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

###======================================================================================
### Initialize the variables!
###======================================================================================

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


###======================================================================================
### Make Data Cards
###======================================================================================

gSystem.Load('libMitLimitsInput.so')
gSystem.Load('libMitLimitsLimit.so')

gSystem.Setenv("MIT_LMT_DIR",LimitDir)
gSystem.Setenv("MIT_PROD_CFG",ProdConfig) 
gSystem.Setenv("MIT_ANA_HIST",AnaHist)
gSystem.Setenv("MIT_LMT_CFG",LimitConfig) 

limitTask = mithep.LimitTask(0)
limitTask.SetCutVariable(Xname)
limitTask.SetRootFileName(RootFileName)

#temp = mithep.CardType(1)
if Type == 'Integral': temp = 0
elif Type == 'Binned': temp = 1
elif Type == 'Unbinned': temp = 2

for Xbin in Xbins:
    limitTask.SetCutValue(Xbin)
    limitTask.WriteDataCard(temp)
