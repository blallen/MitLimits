###======================================================================================
### Script to Run Limit Task to Generate DataCards from Existing Root Files of 1D Histos
###======================================================================================
from subprocess import Popen, PIPE
import os
from argparse import ArgumentParser
from ROOT import *

###======================================================================================
### Set Up Input Options
###======================================================================================

parser = ArgumentParser()
parser.add_argument('-R', help='Root File to extract limits from', dest='RootFileName', action='store', metavar='<name>')
parser.add_argument('-T', help='type of datacard to make (Integral, Binned, or Unbinned)', dest='Type', action='store', metavar='<type>')
parser.add_argument('-x', help='define X axis', dest='Xaxis', action='store', metavar='<min> <max> <step>', nargs=3, type=int)
parser.add_argument('-X', help='set X variable name', dest='Xname', action='store', metavar='<name>')
parser.add_argument('-C', help='specify config file name', dest='config', action='store', metavar='<name>', nargs='+', default=[])

opts = parser.parse_args()

mandatories = ['RootFileName','config','Type','Xaxis','Xname']
for m in mandatories:
    if not opts.__dict__[m]:
        print "Mandatory option is missing\n"
        parser.print_help()
        exit(-1)

opts = parser.parse_args()

###======================================================================================
### Initialize the variables!
###======================================================================================

LimitDir    = os.getenv('MIT_LMT_DIR', os.path.join(os.environ['CMSSW_BASE'], 'src/MitLimits'))
MacroDir    = os.getenv('MIT_MCR_DIR',  os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/macros'))
LimitConfig = os.getenv('MIT_LMT_CFG',  opts.config[0])
RootDir     = os.getenv('MIT_ROOT_DIR', os.path.join(os.environ['HOME'],"cms/root"))
LimitTool   = os.getenv('MIT_LMT_TOOL', os.path.join(os.environ['CMSSW_BASE'],'src/HiggsAnalysis/CombinedLimit'))


cardStorage = os.path.join(LimitTool,'data',opts.RootFileName)
RootFileName = opts.RootFileName
Type = opts.Type
Xname = opts.Xname
Xmin = opts.Xaxis[0]
Xmax = opts.Xaxis[1]
Xstep = opts.Xaxis[2]
Xbins = range(Xmin, Xmax+1, Xstep)


###======================================================================================
### Make Data Cards
###======================================================================================

gSystem.Load('libMitLimitsInput.so')
gSystem.Load('libMitLimitsLimit.so')

gSystem.Setenv("MIT_LMT_DIR",LimitDir)
gSystem.Setenv("MIT_LMT_CFG",LimitConfig) 

if Type == 'Integral': temp = 0
elif Type == 'Binned': temp = 1
elif Type == 'Unbinned': temp = 2


for Xbin in Xbins:
    CardsToAdd = []
    ShapesToAdd = []
    baseName = RootFileName+'_'+str(Xbin)+'_'+Type
    cardName = baseName+'.txt'
    cardStart = os.path.join(RootDir,cardName)
        

    for config in opts.config:
        os.environ['MIT_LMT_CFG']  = config
        LimitConfig = os.getenv('MIT_LMT_CFG',  config)
        
        limitTask = mithep.LimitTask(0)
        limitTask.SetCutVariable(Xname)
        limitTask.SetRootFileName(RootFileName)
        limitTask.SetCutValue(Xbin)
        limitTask.WriteDataCard(temp)
    
        cardEnd   = os.path.join(cardStorage,cardName+'_'+LimitConfig)
        print 'Moving datacard from '+cardStart+' to '+cardEnd
        os.system('mv %s %s'%(cardStart,cardEnd))
#        os.renames(cardStart,cardEnd)
        cardToAdd = cardEnd #config+'='+cardEnd
        CardsToAdd.append(cardToAdd)
        if Type == 'Binned':
            shapeName = baseName+'.root'+'_'+LimitConfig
            shapeStart = os.path.join(RootDir,shapeName)
            shapeEnd   = os.path.join(cardStorage,shapeName)
            print 'Moving datacard from '+shapeStart+ ' to '+shapeEnd
            os.renames(shapeStart,shapeEnd)
            ShapesToAdd.append(shapeEnd)

    CombinePath = os.path.join(os.environ['CMSSW_BASE'],'bin',os.environ['SCRAM_ARCH'],'combineCards.py')
    finalCardPath = os.path.join(cardStorage,cardName)
    finalCard = open(finalCardPath, 'w')
    print 'Combining the following cards: \n', CardsToAdd
    CombineCards = Popen(['python',CombinePath]+CardsToAdd, stdout=PIPE, stderr=PIPE,cwd=LimitTool)
    (cout, cerr) = CombineCards.communicate()
    finalCard.write(cout)
    print cerr
    finalCard.close()
