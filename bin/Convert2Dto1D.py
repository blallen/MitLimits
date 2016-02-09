###======================================================================================
### Script to Convert Root Files with 2D Histograms to Root Files with 1D Histograms
###======================================================================================
from subprocess import Popen, PIPE
import os
from pprint import pprint
from argparse import ArgumentParser
from ROOT import *

###======================================================================================
### Set Up Input Options
###======================================================================================

parser = ArgumentParser()
parser.add_argument('-R', '--run', help='Unique Name to Specify this Run', dest='RunName', action='store', metavar='<name>')
parser.add_argument('-X', '--Xname', help='set X variable name', dest='Xname', action='store', metavar='<name>')
parser.add_argument('-y', '--Yaxis', help='define Y axis', dest='Yaxis', action='store', metavar='<min> <max> <step>', nargs=3, type=int)
parser.add_argument('-Y', '--Yname', help='set Y variable name', dest='Yname', action='store', metavar='<name>')
parser.add_argument('-C', '--config', help='specify config file name', dest='config', action='store', metavar='<name>', nargs='+', default=[] )

opts = parser.parse_args()

mandatories = ['RunName','config', 'Xname','Yaxis','Yname']
#mandatories = []
for m in mandatories:
    if not opts.__dict__[m]:
        print "Mandatory option is missing\n"
        parser.print_help()
        exit(-1)    

opts = parser.parse_args()

###======================================================================================
### Initialize the variables!
###======================================================================================

RootDir     = os.getenv('MIT_ROOT_DIR', os.path.join(os.environ['HOME'],"cms/root"))
ConfigDir   = os.getenv('MIT_CFG_DIR',  os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/config'))
LimitConfig = os.getenv('MIT_LMT_CFG',  opts.config[0])

RunName = opts.RunName
Xname = opts.Xname
Yname = opts.Yname
Ymin = opts.Yaxis[0]
Ymax = opts.Yaxis[1]
Ystep = opts.Yaxis[2]
Ybins = range(Ymax, Ymin-1, -Ystep)

###======================================================================================
### Find Sample Names
###======================================================================================

samples = []
for config in opts.config:
    configFileName = os.path.join(ConfigDir,config+'.txt')
    configFile = open(configFileName, 'r')
    line = configFile.readline()
    tmp = line.split()
    if tmp:
        if len(tmp) > 2:
            if not tmp[0].startswith("#"):
                for sample in tmp[1:]:
                    samples.append(sample)
pprint(samples)

###======================================================================================
### Make 1D Histo Root Files
###======================================================================================

inFileName = os.path.join(RootDir,RunName+'.root')
file2D = TFile(inFileName)

for Ybin in Ybins:
    outFileName = os.path.join(RootDir, RunName+'_'+Yname+'_'+str(Ybin)+'_'+Xname+'.root')
    file1D = TFile(outFileName, "RECREATE")
    for sample in samples:
        # print sample
        histo2D = file2D.Get(sample)
        # print histo2D
        Ydiff = (Ymin - histo2D.GetYaxis().GetBinLowEdge(1))
        lowBin = ( Ydiff / histo2D.GetYaxis().GetBinWidth(1)) + 1
        histo1D = histo2D.ProjectionX(sample, int(lowBin), histo2D.GetNbinsY()+1, "o")
        # print Yname, Ybin, Ydiff, lowBin, histo1D.Integral()
        file1D.cd()
        histo1D.Write()
    file1D.Close()
file2D.Close()
