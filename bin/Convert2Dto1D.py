###==================================================================================================
### Script to Convert Root Files with 2D Histograms to Root Files with 1D Histograms
###==================================================================================================
from subprocess import Popen, PIPE
import os
from pprint import pprint
from optparse import OptionParser

###==================================================================================================
### Set Up Input Options
###==================================================================================================

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

###==================================================================================================
### Initialize the variables!
###==================================================================================================

RootDir     = os.getenv('MIT_ROOT_DIR', os.path.join(os.environ['HOME'],"cms/root"))
MacroDir    = os.getenv('MIT_MCR_DIR',  os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/macros'))
ConfigDir   = os.getenv('MIT_CFG_DIR',  os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/config'))
LimitConfig = os.getenv('MIT_LMT_CFG',  'boostedv-limits-datadriven')

RunName = opts.RunName
Xname = opts.Xname
Yname = opts.Yname
Ymin = opts.Yaxis[0]
Ymax = opts.Yaxis[1]
Ystep = opts.Yaxis[2]
Ybins = range(Ymax, Ymin-Ystep, -Ystep)

###==================================================================================================
### Find Sample Names
###==================================================================================================

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
#pprint(samples)
    
    
###==================================================================================================
### Write Macro File
###==================================================================================================

MacroName = 'Convert2Dto1D_'+Yname
FileName = os.path.join(MacroDir,MacroName+'.C')
convertMacro = open(FileName, 'w')

### Write Header ###
convertMacro.write('#include <TROOT.h> \n#include <TMath.h> \n#include <TPad.h> \n#include <TFile.h> \n#include <TH1.h> \n#include <TH1D.h> \n#include <TH2.h> \n#include <TH2D.h> \n#include "MitPlots/Style/interface/MitStyle.h" \n\nusing namespace std; \nusing namespace mithep; \n\n')
convertMacro.write('void '+MacroName+'() \n{\n\n')

### Initialize the variables ###
convertMacro.write('MitStyle::Init(); \nTFile *f2D = new TFile("DataCard_'+RunName+'.root"); \nTFile *f1D; \nTH2D* h2Dtmp = 0; \nTH1D* h1Dtmp = 0; \nint Ybin = 0;\n\n')

### Create the 1D histo files
for Ybin in Ybins:
    convertMacro.write('f1D = new TFile(TString("DataCard_'+RunName+'_'+Yname+'_'+str(Ybin)+'_'+Xname+'.root"), "RECREATE");\n')
    for sample in samples:
        convertMacro.write('h2Dtmp = (TH2D*)f2D->Get("'+sample+'");\n')
        convertMacro.write('Ybin = ('+str(Ybin)+' / h2Dtmp->GetYaxis()->GetBinWidth(1)) + 1;\n')
        convertMacro.write('h1Dtmp = h2Dtmp->ProjectionX("'+sample+'", Ybin, h2Dtmp->GetNbinsY()+1, "o");\n')
        convertMacro.write('MitStyle::InitHist(h1Dtmp,"'+Xname+'","Number of Events",kBlack);\n')
        convertMacro.write('f1D->cd(); \nh1Dtmp->Write();\n\n')

convertMacro.write('return;\n}')
convertMacro.close()

###==================================================================================================
### Execute Macro File
###==================================================================================================

convertTask = Popen(['root','-b','-l','-q',FileName+'+'],stdout=PIPE,stderr=PIPE,cwd=RootDir)
(stdout, stderr) = convertTask.communicate()
#print stdout
print '1D Plots saved to '+os.path.join(RootDir,'DataCard_'+RunName+'_'+Yname+'_*.root')

exit(-1)
