###==================================================================================================
### Script to Go From nTuples to Datacards and Calculate Expected Limits
###==================================================================================================
from subprocess import Popen, PIPE
import os
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

mandatories = ['RunName','Type','Xaxis','Xname']
for m in mandatories:
    if not opts.__dict__[m]:
        print "\nMandatory option is missing\n"
        parser.print_help()
        exit(-1)

if (opts.Yname and not opts.Yaxis) or (opts.Yaxis and not opts.Yname):
    print "\nFor 2D plots, you must both define the Y axis and provide a name for the Y variable\n"
    parser.print_help()
    exit(-1)
    

(opts, args) = parser.parse_args()

###==================================================================================================
### Initialize the variables!
###==================================================================================================

os.environ['MIT_LMT_DIR']  = os.path.join(os.environ['CMSSW_BASE'], 'src/MitLimits')
os.environ['MIT_ROOT_DIR'] = os.path.join(os.environ['HOME'],"cms/root")

os.environ['MIT_LMT_PYT']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/bin')
os.environ['MIT_MCR_DIR']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/macros')
os.environ['MIT_CFG_DIR']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/config')
os.environ['MIT_LMT_TOOL'] = os.path.join(os.environ['HOME'],'cms/cmssw/032/CMSSW_6_1_1/src/HiggsAnalysis/CombinedLimit')

os.environ['MIT_LMT_CFG']  = 'boostedv-limits-datadriven'

os.environ['MIT_PROD_CFG'] = 'boostedv-v5'
os.environ['MIT_ANA_HIST'] = '/scratch4/dimatteo/cms/hist/boostedv-v5/merged-p1/'



if opts.Yaxis:
    RunName = opts.RunName
else:
    RunName = opts.RunName #+"_"+opts.Xname
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
### Convert 2D Plots to 1D Plots
###==================================================================================================
if opts.Yaxis:
    convertHistos = Popen(['python','./Convert2Dto1D.py',
                           '-R',RunName,'-X',Xname,
                           '-y',str(Ymin),str(Ymax),str(Ystep),'-Y',Yname],
                          stdout=PIPE,stderr=PIPE,
                          cwd=os.environ['MIT_LMT_PYT'])
    (stdout, stderr) = convertHistos.communicate()
    print stdout
    
###==================================================================================================
### Make Data Cards
###==================================================================================================
if opts.Yaxis:
    tmpFileName = 'DataCard_'+RunName+'_'+Yname
    for Ybin in Ybins:
        RootFileName = tmpFileName+"_"+str(Ybin)+"_"+Xname
        limitTask = Popen(['python','./MakeDataCards.py',
                           '-R',RootFileName,'-T',Type,'-x',
                           str(Xmin),str(Xmax),str(Xstep),'-X',Xname],
                          stdout=PIPE,stderr=PIPE,
                          cwd=os.environ['MIT_LMT_PYT'])
        (stdout, stderr) = limitTask.communicate()
        print stdout
else:
    RootFileName = 'DataCard_'+RunName+'_'+Xname
    limitTask = Popen(['python','./MakeDataCards.py',
                       '-R',RootFileName,'-T',Type,'-x',
                       str(Xmin),str(Xmax),str(Xstep),'-X',Xname],
                      stdout=PIPE,stderr=PIPE,
                      cwd=os.environ['MIT_LMT_PYT'])
    (stdout, stderr) = limitTask.communicate()
    print stdout
    print stderr
    
###==================================================================================================
### Plot Limits
###==================================================================================================
cardStorage = os.path.join(os.environ['MIT_LMT_TOOL'],'data',opts.RunName)
#print cardStorage

for Xbin in Xbins:
    if opts.Yaxis:
        for Ybin in Ybins:
            baseName = 'DataCard_'+RunName+'_'+Yname+'_'+str(Ybin)+'_'+Xname+'_'+str(Xbin)+'_'+Type
            cardName = baseName+'.txt'
            cardStart = os.path.join(os.environ['MIT_ROOT_DIR'],cardName)
            cardEnd   = os.path.join(cardStorage,cardName)
            os.renames(cardStart,cardEnd)
            if Type == 'Binned':
                print Type
                shapeName = baseName+'.root'
                shapeStart = os.path.join(os.environ['MIT_ROOT_DIR'],shapeName)
                shapeEnd   = os.path.join(cardStorage,shapeName)
                os.renames(shapeStart,shapeEnd)
    else:
        baseName = 'DataCard_'+RunName+'_'+Xname+'_'+str(Xbin)+'_'+Type
        cardName = baseName+'.txt'
        cardStart = os.path.join(os.environ['MIT_ROOT_DIR'],cardName)
        cardEnd   = os.path.join(cardStorage,cardName)
        os.renames(cardStart,cardEnd)
        print Type
        if Type == 'Binned':
            print Type
            shapeName = baseName+'.root'
            shapeStart = os.path.join(os.environ['MIT_ROOT_DIR'],shapeName)
            shapeEnd   = os.path.join(cardStorage,shapeName)
            os.renames(shapeStart,shapeEnd)
        
            
if opts.Yaxis:
    plotLimits = Popen(['python','./PlotLimits.py',
                        '-R',RunName,'-T',Type,
                        '-x',str(Xmin),str(Xmax),str(Xstep),'-X',Xname,
                        '-y',str(Ymin),str(Ymax),str(Ystep),'-Y',Yname],
                       cwd=os.environ['MIT_LMT_PYT'])
else:
    plotLimits = Popen(['python','./PlotLimits.py',
                        '-R',RunName,'-T',Type,
                        '-x',str(Xmin),str(Xmax),str(Xstep),'-X',Xname],
                       cwd=os.environ['MIT_LMT_PYT'])
#plotLimits.communicate()


exit()
