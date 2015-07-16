###======================================================================================
### Script to Run Higgs Combination Tool on Pre-existing Data Cards to Produce a plot of 
### expected Limits
###======================================================================================
from subprocess import Popen, PIPE
import os
import re
from pprint import pprint
from optparse import OptionParser
from ROOT import *

###======================================================================================
### Function to Run Higgs Tool and Get Expected Limit for a DataCard
###======================================================================================

def RunHiggsTool(DataCardPath,LimitToolDir):
    TextPath = DataCardPath+'.txt'
    HiggsTool = Popen(['combine','-M','Asymptotic',TextPath],
                      stdout=PIPE,stderr=PIPE,cwd=LimitToolDir)    

    find = Popen(['grep','Expected 50.0%'],stdin=HiggsTool.stdout,stdout=PIPE,stderr=PIPE)
    
    lines = [line for line in find.stdout]

    for line in lines:
        tmp = line.split()
        if tmp:
            return tmp[4]
        
###======================================================================================
### Function to Write Limits to a 2D Plotting Macro
###======================================================================================
        
def MakePlot2D(limits,plotName,Xstep,Xbins,Xname,Ystep,Ybins,Yname):
    canvas = TCanvas()
    hExpectedLimits = TH2D("ExpLimit", "Expected Model Independent Limits",len(Xbins),float(Xbins[0]),float(Xbins[-1]+Xstep),len(Ybins),float(Ybins[-1]),float(Ybins[0]+Ystep))
    hExpectedLimits.SetMarkerSize(2.5)
    hExpectedLimits.GetZaxis().SetTitle("Expected Limit")
    hExpectedLimits.GetYaxis().SetTitle(Yname)
    hExpectedLimits.GetXaxis().SetTitle(Xname)

    for row, Xbin in zip(limits,Xbins):
        for limit, Ybin in zip(row,Ybins):
            hExpectedLimits.Fill(float(Xbin),float(Ybin),float(limit))

    hExpectedLimits.Draw("colz")
    hExpectedLimits.Draw("sameTEXT")
    canvas.SaveAs(plotName)

###======================================================================================
### Function to Write Limits to a 1D Plotting Macro
###======================================================================================

def MakePlot1D(limits,plotName,Xstep,Xbins,Xname):
    canvas = TCanvas()
    hExpectedLimits = TH1D("ExpLimit", "Expected Model Independent Limits",len(Xbins),float(Xbins[0]),float(Xbins[-1]+Xstep))
    hExpectedLimits.SetMarkerSize(2.5)
    hExpectedLimits.GetYaxis().SetTitle("Expected Limit")
    hExpectedLimits.GetXaxis().SetTitle(Xname)
    
    for row, Xbin in zip(limits,Xbins):
        for limit in row:
            hExpectedLimits.Fill(float(Xbin),float(limit))
            
    hExpectedLimits.Draw("hist")
    canvas.SaveAs(plotName)

###======================================================================================
### Set Up Input Options
###======================================================================================

parser = OptionParser()
parser.add_option('-R', help='Unique Name to Specify this Run', dest='RunName', action='store', metavar='<name>')
parser.add_option('-T', help='type of datacard to make (Integral, Binned, or Unbinned)', dest='Type', action='store', metavar='<type>')
parser.add_option('-x', help='define X axis', dest='Xaxis', action='store', metavar='<min> <max> <step>', nargs=3, type='int')
parser.add_option('-X', help='set X variable name', dest='Xname', action='store', metavar='<name>')
parser.add_option('-y', help='define Y axis', dest='Yaxis', action='store', metavar='<min> <max> <step>', nargs=3, type='int')
parser.add_option('-Y', help='set Y variable name', dest='Yname', action='store', metavar='<name>')

(opts, args) = parser.parse_args()

mandatories = ['RunName','Type', 'Xaxis', 'Xname']
for m in mandatories:
    if not opts.__dict__[m]:
        print "\nMandatory option is missing\n"
        parser.print_help()
        exit(-1)

if (opts.Yname and not opts.Yaxis) or (opts.Yaxis and not opts.Yname):
    print "\nFor 2D plots, you must both define the Y axis and provide a name for the Y variable\n"
    parser.print_help()
    exit(-1)
        
###======================================================================================
### Initialize the variables!
###======================================================================================

LimitToolDir = os.getenv('MIT_LMT_TOOL', os.path.join(os.environ['HOME'],'cms/cmssw/034/CMSSW_7_1_5/src/HiggsAnalysis/CombinedLimit'))
MacroDir     = os.getenv('MIT_MCR_DIR',  os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/macros'))
RootDir      = os.getenv('MIT_ROOT_DIR', os.path.join(os.environ['HOME'],"cms/root"))

limits = []
RunName = opts.RunName
Type = opts.Type
Xname = opts.Xname
Xmin = opts.Xaxis[0]
Xmax = opts.Xaxis[1]
Xstep = opts.Xaxis[2]
Xbins = range(Xmin, Xmax+Xstep, Xstep)
if opts.Yname:
    Yname = opts.Yname
if opts.Yaxis:
    Ymin = opts.Yaxis[0]
    Ymax = opts.Yaxis[1]
    Ystep = opts.Yaxis[2]
    Ybins = range(Ymax, Ymin-Ystep, -Ystep)


###======================================================================================
### Run Higgs Combination Tool to Find Limits
###======================================================================================

print 'Using Higgs Combination Tool in Directory: '+LimitToolDir

### Run Combination Tool on the data cards
for Xbin in Xbins:
    limits.append([])
    rowNumber = (Xbin - Xmin) / Xstep
    if opts.Yaxis:
        for Ybin in Ybins:
            DataCardName = RunName+'_'+Yname+'_'+str(Ybin)+'_'+Xname+'_'+str(Xbin)+'_'+Type
            DataCardPath = os.path.join('data',RunName,DataCardName)
            print 'Using DataCard: '+DataCardPath
            limits[rowNumber].append(RunHiggsTool(DataCardPath,LimitToolDir))
    else:
        DataCardName = RunName+'_'+Xname+'_'+str(Xbin)+'_'+Type
        DataCardPath = os.path.join('data',RunName+'_'+Xname,DataCardName)
        print 'Using DataCard: '+DataCardPath
        limits[rowNumber].append(RunHiggsTool(DataCardPath,LimitToolDir))

print '\nExpected Limits'
pprint(limits)


###======================================================================================
### Plot Limits
###======================================================================================
if opts.Yaxis:
    plotName = 'ExpLimits_'+RunName+'_'+Yname+'_'+Xname+'_'+Type+'.png'
    MakePlot2D(limits,plotName,Xstep,Xbins,Xname,Ystep,Ybins,Yname)

else:
    plotName = 'ExpLimits_'+RunName+'_'+Xname+'_'+Type+'.png'
    MakePlot1D(limits,plotName,Xstep,Xbins,Xname)
