###======================================================================================
### Script to Convert Root Files with 2D Histograms to Root Files with 1D Histograms
###======================================================================================
from subprocess import Popen, PIPE
import os
from pprint import pprint
from optparse import OptionParser
from ROOT import *

###======================================================================================
### Set Up Input Options
###======================================================================================

parser = OptionParser()
parser.add_option('-R', help='Unique Name to Specify this Run', dest='RunName', action='store', metavar='<name>')
parser.add_option('-X', help='set X variable name', dest='Xname', action='store', metavar='<name>')
parser.add_option('-y', help='define Y axis', dest='Yaxis', action='store', metavar='<min> <max> <step>', nargs=3, type='int')
parser.add_option('-Y', help='set Y variable name', dest='Yname', action='store', metavar='<name>')

(opts, args) = parser.parse_args()

mandatories = ['RunName','Xname','Yaxis','Yname']
#mandatories = []
for m in mandatories:
    if not opts.__dict__[m]:
        print "Mandatory option is missing\n"
        parser.print_help()
        exit(-1)    

(opts, args) = parser.parse_args()

###======================================================================================
### Initialize the variables!
###======================================================================================

RootDir     = os.getenv('MIT_ROOT_DIR', os.path.join(os.environ['HOME'],"cms/root"))
ConfigDir   = os.getenv('MIT_CFG_DIR',  os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/config'))
LimitConfig = os.getenv('MIT_LMT_CFG',  'boostedv-limits-datadriven')

RunName = opts.RunName
Xname = opts.Xname
Yname = opts.Yname
Ymin = opts.Yaxis[0]
Ymax = opts.Yaxis[1]
Ystep = opts.Yaxis[2]
Ybins = range(Ymax, Ymin-Ystep, -Ystep)

###======================================================================================
### Find Sample Names
###======================================================================================

samples = []
configFileName = os.path.join(ConfigDir,LimitConfig+'.txt')
configFile = open(configFileName, 'r')
lines = [line for line in configFile]
tmp = []
for line in lines:
    tmp = line.split()
    if tmp:
        if len(tmp) > 2:
            if not tmp[0].startswith("#"):
                samples.append(tmp[0])
# pprint(samples)

###======================================================================================
### Make 1D Histo Root Files
###======================================================================================

inFileName = os.path.join(RootDir,"DataCard_"+RunName+'.root')
file2D = TFile(inFileName)

for Ybin in Ybins:
    outFileName = os.path.join(RootDir, "DataCard_"+RunName+'_'+Yname+'_'+str(Ybin)+'_'+Xname+'.root')
    file1D = TFile(outFileName, "RECREATE")
    for sample in samples:
        histo2D = file2D.Get(sample)
        lowBin = ( Ybin / histo2D.GetYaxis().GetBinWidth(1)) + 1
        histo1D = histo2D.ProjectionX(sample, int(lowBin), histo2D.GetNbinsY()+1, "o")
        file1D.cd()
        histo1D.Write()
    file1D.Close()
file2D.Close()
