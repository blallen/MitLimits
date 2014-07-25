###==================================================================================================
### Script to Run Higgs Combination Tool on Pre-existing Data Cards to Produce a 2D plot of expected Limits
###==================================================================================================
from subprocess import Popen, PIPE
import os
from pprint import pprint
from optparse import OptionParser

###==================================================================================================
### Function to Run Higgs Tool and Get Expected Limit for a DataCard
###==================================================================================================

def RunHiggsTool(DataCardPath,LimitToolDir):
    HiggsTool = Popen(['combine','-M','Asymptotic',DataCardPath],
                      stdout=PIPE,stderr=PIPE,cwd=LimitToolDir)
    
    find = Popen(['grep','Expected 50.0%'],stdin=HiggsTool.stdout,stdout=PIPE,stderr=PIPE)
    
    lines = [line for line in find.stdout]
        
    for line in lines:
        tmp = line.split()
        if tmp:
            return tmp[4]
        
###==================================================================================================
### Function to Write Limits to a 2D Plotting Macro
###==================================================================================================
        
def WriteMacro2D(limits,MacroName,FileName,plotName,Xmin,Xmax,Xstep,Xbins,Ymin,Ymax,Ystep,Ybins):
### Open file
    plotMacro = open(FileName, 'w')
    
### write header
    plotMacro.write('#include "MitPlots/Style/interface/MitStyle.h" \n #include "MitLimits/Limit/interface/LimitTask.h" \n #include "TLegend.h" \n #include "TH2.h" \n #include "TH2D.h" \n #include "TStyle.h" \n using namespace std; \n using namespace mithep; \n //================================================================================================== \n\n')
    plotMacro.write('void '+MacroName+'() \n{\n')

### configure graphics stuff
    plotMacro.write('// setup graphics stuff before starting \n MitStyle::Init(); \n TStyle *MitStyle = gStyle;// new TStyle("MIT-Style","The Perfect Style for Plots ;-)");\n\n MitStyle->SetCanvasDefW      (1000);\n MitStyle->SetPadBottomMargin(0.18);\n MitStyle->SetPadLeftMargin  (0.18);\n MitStyle->SetPadRightMargin (0.18);\n\n MitStyle->SetTitleFont  (42   ,"Z");\n MitStyle->SetTitleSize  (0.055,"Z");\n MitStyle->SetTitleOffset(1.700,"Z");\n MitStyle->SetLabelOffset(0.010,"Z");\n MitStyle->SetLabelSize  (0.050,"Z");\n MitStyle->SetLabelFont  (42   ,"Z");\n MitStyle->SetTickLength (-0.01,"Z");\n MitStyle->SetTitleFont  (42,"Z");\n\n')
    
### Set up histogram
    plotMacro.write('TCanvas *canvas = new TCanvas;\n TH2D* hExpectedLimits = new TH2D("ExpLimit", "Expected Model Independent Limits",'+str(len(Xbins))+','+str(Xmin)+','+str(Xmax+Xstep)+','+str(len(Ybins))+','+str(Ymin)+','+str(Ymax+Ystep)+');\n MitStyle::InitHistWide(hExpectedLimits,"'+Xname+'","'+Yname+'",kBlack);\n hExpectedLimits->SetMarkerSize(2.5);\n hExpectedLimits->GetZaxis()->SetTitle("Expected Limit");\n\n')

### Fill histogram with limits
    for row, Xbin in zip(limits,Xbins):
        for limit, Ybin in zip(row,Ybins):
            plotMacro.write('hExpectedLimits->Fill('+str(Xbin)+','+str(Ybin)+','+str(limit)+');\n')
            plotMacro.write('\n')
        
### Draw and Save Histogram
    plotMacro.write('hExpectedLimits->Draw("colz");\n hExpectedLimits->Draw("sameTEXT");\n canvas->SaveAs("'+plotName+'");\n}\n')
    plotMacro.close()

###==================================================================================================
### Function to Write Limits to a 1D Plotting Macro
###==================================================================================================

def WriteMacro1D(limits,MacroName,FileName,plotName,Xmin,Xmax,Xstep,Xbins):
### Open file
    plotMacro = open(FileName, 'w')
        
### write header
    plotMacro.write('#include "MitPlots/Style/interface/MitStyle.h" \n #include "MitLimits/Limit/interface/LimitTask.h" \n #include "TLegend.h" \n #include "TH1.h" \n #include "TH1D.h" \n #include "TStyle.h" \n using namespace std; \n using namespace mithep; \n //================================================================================================== \n\n')
    plotMacro.write('void '+MacroName+'() \n{\n')
    
### configure graphics stuff
    plotMacro.write('// setup graphics stuff before starting \n MitStyle::Init(); \n TStyle *MitStyle = gStyle;// new TStyle("MIT-Style","The Perfect Style for Plots ;-)");\n\n MitStyle->SetCanvasDefW      (1000);\n MitStyle->SetPadBottomMargin(0.18);\n MitStyle->SetPadLeftMargin  (0.18);\n MitStyle->SetPadRightMargin (0.18);\n\n ')
    
### Set up histogram
    plotMacro.write('TCanvas *canvas = new TCanvas;\n TH1D* hExpectedLimits = new TH1D("ExpLimit", "Expected Model Independent Limits",'+str(len(Xbins))+','+str(Xmin)+','+str(Xmax+Xstep)+');\n MitStyle::InitHistWide(hExpectedLimits,"'+Xname+'","Expected Limits",kBlack);\n hExpectedLimits->SetMarkerSize(2.5);\n\n')
    
### Fill histogram with limits
    for row, Xbin in zip(limits,Xbins):
        for limit in row:
            plotMacro.write('hExpectedLimits->Fill('+str(Xbin)+','+str(limit)+');\n')
            plotMacro.write('\n')
            
### Draw and Save Histogram
        
    plotMacro.write('hExpectedLimits->Draw("hist");\n;\n canvas->SaveAs("'+plotName+'");\n}\n')
    plotMacro.close()


###==================================================================================================
### Set Up Input Options
###==================================================================================================

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
        
###==================================================================================================
### Initialize the variables!
###==================================================================================================

LimitToolDir = os.getenv('MIT_LMT_TOOL', os.path.join(os.environ['HOME'],'cms/cmssw/032/CMSSW_6_1_1/src/HiggsAnalysis/CombinedLimit'))
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


###==================================================================================================
### Run Higgs Combination Tool to Find Limits
###==================================================================================================

print 'Using Higgs Combination Tool in Directory: '+LimitToolDir

### Run Combination Tool on the data cards
for Xbin in Xbins:
    limits.append([])
    rowNumber = (Xbin - Xmin) / Xstep
    if opts.Yaxis:
        for Ybin in Ybins:
            DataCardName = 'DataCard_'+RunName+'_'+Yname+'_'+str(Ybin)+'_'+Xname+'_'+str(Xbin)+'_'+Type+'.txt'
            DataCardPath = os.path.join('data',RunName,DataCardName)
            print 'Using DataCard: '+DataCardPath
            limits[rowNumber].append(RunHiggsTool(DataCardPath,LimitToolDir))
    else:
        DataCardName = 'DataCard_'+RunName+'_'+str(Xbin)+'_'+Type+'.txt'
        DataCardPath = os.path.join('data',RunName,DataCardName)
        print 'Using DataCard: '+DataCardPath
        limits[rowNumber].append(RunHiggsTool(DataCardPath,LimitToolDir))

print '\nExpected Limits'
pprint(limits)


###==================================================================================================
### Write limits to plotting Macro
###==================================================================================================
if opts.Yaxis:
    MacroName = "PlotExpectedLimits_"+RunName+"_"+Yname+"_"+Xname+"_"+Type
    FileName = os.path.join(MacroDir,MacroName+".C")
    plotName = 'Plots/'+RunName+'_ExpLimits_'+Yname+'_'+Xname+'.png'
    WriteMacro2D(limits,MacroName,FileName,plotName,Xmin,Xmax,Xstep,Xbins,Ymin,Ymax,Ystep,Ybins)

else:
    MacroName = "PlotExpectedLimits_"+RunName+"_"+Type
    FileName = os.path.join(MacroDir,MacroName+".C")
    plotName = 'Plots/'+RunName+'_ExpLimits.png'
    WriteMacro1D(limits,MacroName,FileName,plotName,Xmin,Xmax,Xstep,Xbins)

###==================================================================================================
### Run Plotting Macro
###==================================================================================================
plot = Popen(['root','-b','-l','-q',FileName+'+'],stdout=PIPE,stderr=PIPE,cwd=RootDir)
(stdout, stderr) = plot.communicate()
#print stdout
#print stderr
plotPath = os.path.join(RootDir,plotName)
print 'Expected Limits Plot saved to '+plotPath
os.system('display '+plotPath+' &')

exit()
