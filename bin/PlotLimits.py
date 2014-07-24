###==================================================================================================
### Script to Run Higgs Combination Tool on Pre-existing Data Cards to Produce a 2D plot of expected Limits
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
parser.add_option('-T', help='type of datacard to make (Integral, Binned, or Unbinned)', dest='Type', action='store', metavar='<type>')
parser.add_option('-x', help='define X axis', dest='Xaxis', action='store', metavar='<min> <max> <step>', nargs=3, type='int')
parser.add_option('-X', help='set X variable name', dest='Xname', action='store', metavar='<name>')
parser.add_option('-y', help='define Y axis', dest='Yaxis', action='store', metavar='<min> <max> <step>', nargs=3, type='int')
parser.add_option('-Y', help='set Y variable name', dest='Yname', action='store', metavar='<name>')

(opts, args) = parser.parse_args()

mandatories = ['RunName','Type', 'Xaxis', 'Yaxis', 'Xname', 'Yname']
for m in mandatories:
    if not opts.__dict__[m]:
        print "Mandatory option is missing\n"
        parser.print_help()
        exit(-1)
        
###==================================================================================================
### Initialize the variables!
###==================================================================================================

LimitToolDir = os.getenv('MIT_LMT_TOOL', os.path.join(os.environ['HOME'],'cms/cmssw/032/CMSSW_6_1_1/src/HiggsAnalysis/CombinedLimit'))
MacroDir     = os.getenv('MIT_MCR_DIR',  os.path.join(os.environ['CMSSW_BASE'],'src/MitMonoJet/macros'))
RootDir      = os.getenv('MIT_ROOT_DIR', os.path.join(os.environ['HOME'],"cms/root"))

limits = []
RunName = opts.RunName
Type = opts.Type
Xname = opts.Xname
Xmin = opts.Xaxis[0]
Xmax = opts.Xaxis[1]
Xstep = opts.Xaxis[2]
Xbins = range(Xmin, Xmax+Xstep, Xstep)
Yname = opts.Yname
Ymin = opts.Yaxis[0]
Ymax = opts.Yaxis[1]
Ystep = opts.Yaxis[2]
Ybins = range(Ymax, Ymin-Ystep, -Ystep)


###==================================================================================================
### Run Higgs Combination Tool to Find Limits
###==================================================================================================

print 'Using Higgs Combination Tool in Directory: '+LimitToolDir

### Run Combination Tool on the data cards
for Ybin in Ybins:
    limits.append([])
    rowNumber = (Ymax - Ybin) / Ystep
    for Xbin in Xbins:
        DataCardName = 'DataCard_'+RunName+'_'+Yname+'_'+str(Ybin)+'_'+Xname+'_'+str(Xbin)+'_'+Type+'.txt'
        DataCardPath = os.path.join('data',RunName,DataCardName)
        print 'Using DataCard: '+DataCardPath
        HiggsTool = Popen(['combine','-M','Asymptotic',DataCardPath],
                          stdout=PIPE,stderr=PIPE,cwd=LimitToolDir)
        #(HiggsOut, HiggsErr) = HiggsTool.communicate()
        
        find = Popen(['grep','Expected 50.0%'],stdin=HiggsTool.stdout,stdout=PIPE,stderr=PIPE)
                
        lines = [line for line in find.stdout]

        tmp = []
        for line in lines:
            tmp = line.split()
            if tmp:
                limits[rowNumber].append(tmp[4])

print '\nExpected Limits'
pprint(limits)


###==================================================================================================
### Write limits to plotting Macro
###==================================================================================================

### Open file
MacroName = "PlotExpectedLimits_"+Yname+"_"+Xname+"_"+Type
FileName = os.path.join(MacroDir,MacroName+".C")
plotMacro = open(FileName, 'w')

### write header
plotMacro.write('#include "MitPlots/Style/interface/MitStyle.h" \n #include "MitLimits/Limit/interface/LimitTask.h" \n #include "TLegend.h" \n #include "TH2.h" \n #include "TH2D.h" \n #include "TStyle.h" \n using namespace std; \n using namespace mithep; \n //================================================================================================== \n\n')
plotMacro.write('void '+MacroName+'() \n{\n')

### configure graphics stuff
plotMacro.write('// setup graphics stuff before starting \n MitStyle::Init(); \n TStyle *MitStyle = gStyle;// new TStyle("MIT-Style","The Perfect Style for Plots ;-)");\n\n MitStyle->SetCanvasDefW      (1000);\n MitStyle->SetPadBottomMargin(0.18);\n MitStyle->SetPadLeftMargin  (0.18);\n MitStyle->SetPadRightMargin (0.18);\n\n MitStyle->SetTitleFont  (42   ,"Z");\n MitStyle->SetTitleSize  (0.055,"Z");\n MitStyle->SetTitleOffset(1.700,"Z");\n MitStyle->SetLabelOffset(0.010,"Z");\n MitStyle->SetLabelSize  (0.050,"Z");\n MitStyle->SetLabelFont  (42   ,"Z");\n MitStyle->SetTickLength (-0.01,"Z");\n MitStyle->SetTitleFont  (42,"Z");\n\n')

### Set up histogram
plotMacro.write('TCanvas *canvas = new TCanvas;\n TH2D* hExpectedLimits = new TH2D("ExpLimit", "Expected Model Independent Limits",'+str(len(Xbins))+','+str(Xmin)+','+str(Xmax+Xstep)+','+str(len(Ybins))+','+str(Ymin)+','+str(Ymax+Ystep)+');\n MitStyle::InitHistWide(hExpectedLimits,"'+Xname+'","'+Yname+'",kBlack);\n hExpectedLimits->SetMarkerSize(2.5);\n hExpectedLimits->GetZaxis()->SetTitle("Expected Limit");\n\n')

### Fill histogram with limits
for row, Ybin in zip(limits,Ybins):
    for limit, Xbin in zip(row,Xbins):
        #print 'For '+Xname+' '+str(Xbin)+' and '+Yname+' '+str(Ybin)+' the expected limit is: '+str(limit)
        plotMacro.write('hExpectedLimits->Fill('+str(Xbin)+','+str(Ybin)+','+str(limit)+');\n')
    plotMacro.write('\n')
    
### Draw and Save Histogram
plotName = 'Plots/'+RunName+'_ExpLimits_'+Yname+'_'+Xname+'.png'
plotMacro.write('hExpectedLimits->Draw("colz");\n hExpectedLimits->Draw("sameTEXT");\n canvas->SaveAs("'+plotName+'");\n}\n')
plotMacro.close()

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
