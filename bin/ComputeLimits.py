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

mandatories = ['RunName','Type','Xaxis','Xname','Yaxis','Yname']
for m in mandatories:
    if not opts.__dict__[m]:
        print "Mandatory option is missing\n"
        parser.print_help()
        exit(-1)

if (opts.Yname and not opts.Yaxis) or (opts.Yaxis and not opts.Yname):
    print "For 2D plots, you must both define the Y axis and provide a name for the Y variable\n"
    parser.print_help()
    exit(-1)
    

(opts, args) = parser.parse_args()

###==================================================================================================
### Initialize the variables!
###==================================================================================================

os.environ['MIT_LMT_PYT']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/bin')
os.environ['MIT_ROOT_DIR'] = os.path.join(os.environ['HOME'],"cms/root")
os.environ['MIT_MCR_DIR']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitMonoJet/macros')
os.environ['MIT_CFG_DIR']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitMonoJet/config')
os.environ['MIT_LMT_TOOL'] = os.path.join(os.environ['HOME'],'cms/cmssw/032/CMSSW_6_1_1/src/HiggsAnalysis/CombinedLimit')

os.environ['MIT_PROD_CFG'] = 'boostedv-v5'
os.environ['MIT_ANA_HIST'] = '/scratch4/dimatteo/cms/hist/boostedv-v5/merged-p1/'
os.environ['MIT_ANA_CFG']  = 'boostedv-plots-datadriven'

os.environ['MIT_LMT_CFG']  = 'boostedv-limits-datadriven'

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
### Make Data Cards
###==================================================================================================

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
RootFileName = 'DataCard_'+RunName+'_'+Yname
for Ybin in Ybins:
    tmpFileName = RootFileName+"_"+str(Ybin)
    limitTask = Popen(['python','./MakeDataCards.py',
                       '-R',tmpFileName,'-T',Type,'-x',
                       str(Xmin),str(Xmax),str(Xstep),'-X',Xname],
                      stdout=PIPE,stderr=PIPE,
                      cwd=os.environ['MIT_LMT_PYT'])
    (stdout, stderr) = limitTask.communicate()
    print stdout

###==================================================================================================
### Plot Limits
###==================================================================================================
cardStorage = os.path.join(os.environ['MIT_LMT_TOOL'],'data',RunName)
#print cardStorage

for Ybin in Ybins:
    for Xbin in Xbins:
        cardName = 'DataCard_'+RunName+'_'+Yname+'_'+str(Ybin)+'_'+Xname+'_'+str(Xbin)+'_'+Type+'.txt'
        #print cardName
        cardStart = os.path.join(os.environ['MIT_ROOT_DIR'],cardName)
        cardEnd   = os.path.join(cardStorage,cardName)
        os.renames(cardStart,cardEnd)
        #os.remove(cardLocation)

plotLimits = Popen(['python','./PlotLimits.py',
                    '-R',RunName,'-T',Type,
                    '-x',str(Xmin),str(Xmax),str(Xstep),'-X',Xname,
                    '-y',str(Ymin),str(Ymax),str(Ystep),'-Y',Yname],
                   cwd=os.environ['MIT_LMT_PYT'])
#plotLimits.communicate()


exit()
