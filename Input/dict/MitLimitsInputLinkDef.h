// $Id: MitLimitsInputLinkDef.h,v 1.1.2.1 2010/10/03 04:08:48 paus Exp $

#ifndef MITLIMITS_INPUT_LINKDEF_H
#define MITLIMITS_INPUT_LINKDEF_H
#include "MitLimits/Input/interface/Process.h"
#include "MitLimits/Input/interface/TaskProcesses.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::Process+;
#pragma link C++ class std::vector<mithep::Process>+;
#pragma link C++ class mithep::TaskProcesses+;
#endif
