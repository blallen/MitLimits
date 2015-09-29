###======================================================================================
### Script to Go From nTuples to Datacards and Calculate Expected Limits
###======================================================================================
from subprocess import Popen, PIPE
import os
from argparse import ArgumentParser

###======================================================================================
### Set Up Input Options
###======================================================================================

parser = ArgumentParser()
parser.add_argument('-R', '--run', help='Unique Name to Specify this Run', dest='RunName', action='store', metavar='<name>')
parser.add_argument('-T', '--type', help='type of datacard to make (Integral, Binned, or Unbinned)', dest='Type', action='store', metavar='<type>')
parser.add_argument('-x', '--Xaxis', help='define X axis', dest='Xaxis', action='store', metavar='<min> <max> <step>', nargs=3, type=int)
parser.add_argument('-X', '--Xname', help='set X variable name', dest='Xname', action='store', metavar='<name>')
parser.add_argument('-y', '--Yaxis', help='define Y axis', dest='Yaxis', action='store', metavar='<min> <max> <step>', nargs=3, type=int)
parser.add_argument('-Y', '--Yname', help='set Y variable name', dest='Yname', action='store', metavar='<name>')
parser.add_argument('-C', '--config', help='specify config file name', dest='config', action='store', metavar='<name>', nargs='+', default=[] )

opts = parser.parse_args()

mandatories = ['RunName','config','Type','Xaxis','Xname']
for m in mandatories:
    if not opts.__dict__[m]:
        print "\nMandatory option is missing\n"
        parser.print_help()
        exit(-1)

if (opts.Yname and not opts.Yaxis) or (opts.Yaxis and not opts.Yname):
    print "\nFor 2D plots, you must both define the Y axis and provide a name for the Y variable\n"
    parser.print_help()
    exit(-1)
    

opts = parser.parse_args()

###======================================================================================
### Initialize the variables!
###======================================================================================

os.environ['MIT_LMT_DIR']  = os.path.join(os.environ['CMSSW_BASE'], 'src/MitLimits')
os.environ['MIT_ROOT_DIR'] = os.path.join(os.environ['HOME'],"cms/root")

os.environ['MIT_LMT_PYT']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/bin')
os.environ['MIT_MCR_DIR']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/macros')
os.environ['MIT_CFG_DIR']  = os.path.join(os.environ['CMSSW_BASE'],'src/MitLimits/config')
os.environ['MIT_LMT_TOOL'] = os.path.join(os.environ['CMSSW_BASE'],'src/HiggsAnalysis/CombinedLimit')

os.environ['MIT_LMT_CFG']  = opts.config[0]

RunName = opts.RunName
Type = opts.Type
Xname = opts.Xname
Xmin = opts.Xaxis[0]
Xmax = opts.Xaxis[1]
Xstep = opts.Xaxis[2]
Xbins = range(Xmin, Xmax+1, Xstep) # old method would overshoot
print "Xbins: ",Xbins
if opts.Yname:
    Yname = opts.Yname
if opts.Yaxis:
    Ymin = opts.Yaxis[0]
    Ymax = opts.Yaxis[1]
    Ystep = opts.Yaxis[2]
    Ybins = range(Ymax, Ymin-1, -Ystep)

###======================================================================================
### Convert 2D Plots to 1D Plots
###======================================================================================
if opts.Yaxis:
    ConvertPath = os.path.join(os.environ['MIT_LMT_PYT'], 'Convert2Dto1D.py')
    convertHistos = Popen(['python',ConvertPath,
                           '-R',RunName,'-X',Xname,
                           '-y',str(Ymin),str(Ymax),str(Ystep),'-Y',Yname],
                          stdout=PIPE,stderr=PIPE,
                          cwd=os.environ['MIT_ROOT_DIR'])
    (stdout, stderr) = convertHistos.communicate()
    print stdout
    print stderr
    
###======================================================================================
### Make Data Cards
###======================================================================================
MakePath = os.path.join(os.environ['MIT_LMT_PYT'], 'MakeDataCards.py')
if opts.Yaxis:
    tmpFileName = RunName+'_'+Yname
    for Ybin in Ybins:
        RootFileName = tmpFileName+"_"+str(Ybin)+"_"+Xname
        limitTask = Popen(['python',MakePath,
                           '-R',RootFileName,'-T',Type,'-C']+opts.config+[
                           '-x',str(Xmin),str(Xmax),str(Xstep),'-X',Xname],
                          stdout=PIPE,stderr=PIPE,
                          cwd=os.environ['MIT_ROOT_DIR'])
        (stdout, stderr) = limitTask.communicate()
        print stdout
        print stderr
else:
    RootFileName = RunName+'_'+Xname
    limitTask = Popen(['python',MakePath,
                       '-R',RootFileName,'-T',Type,'-C']+opts.config+[
                       '-x',str(Xmin),str(Xmax),str(Xstep),'-X',Xname],
                      stdout=PIPE,stderr=PIPE,
                      cwd=os.environ['MIT_ROOT_DIR'])
    (stdout, stderr) = limitTask.communicate()
    print stdout
    print stderr
    
###======================================================================================
### Plot Limits
###======================================================================================
cardStorage = os.path.join(os.environ['MIT_LMT_TOOL'],'data',opts.RunName)
#print cardStorage            
PlotPath = os.path.join(os.environ['MIT_LMT_PYT'], 'PlotLimits.py')
if opts.Yaxis:
    plotLimits = Popen(['python',PlotPath,
                        '-R',RunName,'-T',Type,
                        '-x',str(Xmin),str(Xmax),str(Xstep),'-X',Xname,
                        '-y',str(Ymin),str(Ymax),str(Ystep),'-Y',Yname],
                       stderr = PIPE,
                       cwd=os.environ['MIT_ROOT_DIR'])
else:
    plotLimits = Popen(['python',PlotPath,
                        '-R',RunName,'-T',Type,
                        '-x',str(Xmin),str(Xmax),str(Xstep),'-X',Xname],
                       stderr = PIPE,
                       cwd=os.environ['MIT_ROOT_DIR'])
print plotLimits.communicate()[1]

exit()
