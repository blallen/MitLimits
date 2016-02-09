// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIsnarayandIcmsdIcmsswdI041dICMSSW_7_4_4dItmpdIslc6_amd64_gcc491dIMitLimitsInput_LinkDefDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "MitLimits/Input/interface/Process.h"
#include "MitLimits/Input/interface/TaskProcesses.h"

// Header files passed via #pragma extra_include

namespace mithep {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *mithep_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("mithep", 0 /*version*/, "MitLimits/Input/interface/Process.h", 17,
                     ::ROOT::DefineBehavior((void*)0,(void*)0),
                     &mithep_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *mithep_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_mithepcLcLProcess(void *p = 0);
   static void *newArray_mithepcLcLProcess(Long_t size, void *p);
   static void delete_mithepcLcLProcess(void *p);
   static void deleteArray_mithepcLcLProcess(void *p);
   static void destruct_mithepcLcLProcess(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mithep::Process*)
   {
      ::mithep::Process *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mithep::Process >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mithep::Process", ::mithep::Process::Class_Version(), "MitLimits/Input/interface/Process.h", 19,
                  typeid(::mithep::Process), DefineBehavior(ptr, ptr),
                  &::mithep::Process::Dictionary, isa_proxy, 4,
                  sizeof(::mithep::Process) );
      instance.SetNew(&new_mithepcLcLProcess);
      instance.SetNewArray(&newArray_mithepcLcLProcess);
      instance.SetDelete(&delete_mithepcLcLProcess);
      instance.SetDeleteArray(&deleteArray_mithepcLcLProcess);
      instance.SetDestructor(&destruct_mithepcLcLProcess);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mithep::Process*)
   {
      return GenerateInitInstanceLocal((::mithep::Process*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::mithep::Process*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_mithepcLcLTaskProcesses(void *p);
   static void deleteArray_mithepcLcLTaskProcesses(void *p);
   static void destruct_mithepcLcLTaskProcesses(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mithep::TaskProcesses*)
   {
      ::mithep::TaskProcesses *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mithep::TaskProcesses >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mithep::TaskProcesses", ::mithep::TaskProcesses::Class_Version(), "MitLimits/Input/interface/TaskProcesses.h", 22,
                  typeid(::mithep::TaskProcesses), DefineBehavior(ptr, ptr),
                  &::mithep::TaskProcesses::Dictionary, isa_proxy, 4,
                  sizeof(::mithep::TaskProcesses) );
      instance.SetDelete(&delete_mithepcLcLTaskProcesses);
      instance.SetDeleteArray(&deleteArray_mithepcLcLTaskProcesses);
      instance.SetDestructor(&destruct_mithepcLcLTaskProcesses);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mithep::TaskProcesses*)
   {
      return GenerateInitInstanceLocal((::mithep::TaskProcesses*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::mithep::TaskProcesses*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace mithep {
//______________________________________________________________________________
atomic_TClass_ptr Process::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Process::Class_Name()
{
   return "mithep::Process";
}

//______________________________________________________________________________
const char *Process::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mithep::Process*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Process::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mithep::Process*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Process::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mithep::Process*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Process::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mithep::Process*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace mithep
namespace mithep {
//______________________________________________________________________________
atomic_TClass_ptr TaskProcesses::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TaskProcesses::Class_Name()
{
   return "mithep::TaskProcesses";
}

//______________________________________________________________________________
const char *TaskProcesses::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mithep::TaskProcesses*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TaskProcesses::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mithep::TaskProcesses*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TaskProcesses::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mithep::TaskProcesses*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TaskProcesses::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mithep::TaskProcesses*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace mithep
namespace mithep {
//______________________________________________________________________________
void Process::Streamer(TBuffer &R__b)
{
   // Stream an object of class mithep::Process.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mithep::Process::Class(),this);
   } else {
      R__b.WriteClassBuffer(mithep::Process::Class(),this);
   }
}

} // namespace mithep
namespace ROOT {
   // Wrappers around operator new
   static void *new_mithepcLcLProcess(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::mithep::Process : new ::mithep::Process;
   }
   static void *newArray_mithepcLcLProcess(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::mithep::Process[nElements] : new ::mithep::Process[nElements];
   }
   // Wrapper around operator delete
   static void delete_mithepcLcLProcess(void *p) {
      delete ((::mithep::Process*)p);
   }
   static void deleteArray_mithepcLcLProcess(void *p) {
      delete [] ((::mithep::Process*)p);
   }
   static void destruct_mithepcLcLProcess(void *p) {
      typedef ::mithep::Process current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mithep::Process

namespace mithep {
//______________________________________________________________________________
void TaskProcesses::Streamer(TBuffer &R__b)
{
   // Stream an object of class mithep::TaskProcesses.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mithep::TaskProcesses::Class(),this);
   } else {
      R__b.WriteClassBuffer(mithep::TaskProcesses::Class(),this);
   }
}

} // namespace mithep
namespace ROOT {
   // Wrapper around operator delete
   static void delete_mithepcLcLTaskProcesses(void *p) {
      delete ((::mithep::TaskProcesses*)p);
   }
   static void deleteArray_mithepcLcLTaskProcesses(void *p) {
      delete [] ((::mithep::TaskProcesses*)p);
   }
   static void destruct_mithepcLcLTaskProcesses(void *p) {
      typedef ::mithep::TaskProcesses current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mithep::TaskProcesses

namespace ROOT {
   static TClass *vectorlEmithepcLcLProcessgR_Dictionary();
   static void vectorlEmithepcLcLProcessgR_TClassManip(TClass*);
   static void *new_vectorlEmithepcLcLProcessgR(void *p = 0);
   static void *newArray_vectorlEmithepcLcLProcessgR(Long_t size, void *p);
   static void delete_vectorlEmithepcLcLProcessgR(void *p);
   static void deleteArray_vectorlEmithepcLcLProcessgR(void *p);
   static void destruct_vectorlEmithepcLcLProcessgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mithep::Process>*)
   {
      vector<mithep::Process> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mithep::Process>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mithep::Process>", -2, "vector", 214,
                  typeid(vector<mithep::Process>), DefineBehavior(ptr, ptr),
                  &vectorlEmithepcLcLProcessgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mithep::Process>) );
      instance.SetNew(&new_vectorlEmithepcLcLProcessgR);
      instance.SetNewArray(&newArray_vectorlEmithepcLcLProcessgR);
      instance.SetDelete(&delete_vectorlEmithepcLcLProcessgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmithepcLcLProcessgR);
      instance.SetDestructor(&destruct_vectorlEmithepcLcLProcessgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mithep::Process> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<mithep::Process>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmithepcLcLProcessgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mithep::Process>*)0x0)->GetClass();
      vectorlEmithepcLcLProcessgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmithepcLcLProcessgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmithepcLcLProcessgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<mithep::Process> : new vector<mithep::Process>;
   }
   static void *newArray_vectorlEmithepcLcLProcessgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<mithep::Process>[nElements] : new vector<mithep::Process>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmithepcLcLProcessgR(void *p) {
      delete ((vector<mithep::Process>*)p);
   }
   static void deleteArray_vectorlEmithepcLcLProcessgR(void *p) {
      delete [] ((vector<mithep::Process>*)p);
   }
   static void destruct_vectorlEmithepcLcLProcessgR(void *p) {
      typedef vector<mithep::Process> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mithep::Process>

namespace {
  void TriggerDictionaryInitialization_MitLimitsInput_LinkDefDict_Impl() {
    static const char* headers[] = {
"MitLimits/Input/interface/Process.h",
"MitLimits/Input/interface/TaskProcesses.h",
0
    };
    static const char* includePaths[] = {
"/home/snarayan/cms/cmssw/041/CMSSW_7_4_4/src",
"/cvmfs/cms.cern.ch/slc6_amd64_gcc491/lcg/root/6.02.00-odfocd4/include",
"/home/snarayan/cms/cmssw/041/CMSSW_7_4_4/src/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace mithep{class __attribute__((annotate("$clingAutoload$MitLimits/Input/dict/MitLimitsInputLinkDef.h")))  Process;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$string")))  allocator;
}
namespace mithep{class __attribute__((annotate(R"ATTRDUMP(TaskProcesses manages a list of samples used in an analysis task)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$MitLimits/Input/dict/MitLimitsInputLinkDef.h")))  TaskProcesses;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "MitLimits/Input/interface/Process.h"
#include "MitLimits/Input/interface/TaskProcesses.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"mithep::Process", payloadCode, "@",
"mithep::TaskProcesses", payloadCode, "@",
"vector<mithep::Process>", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MitLimitsInput_LinkDefDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MitLimitsInput_LinkDefDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MitLimitsInput_LinkDefDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MitLimitsInput_LinkDefDict() {
  TriggerDictionaryInitialization_MitLimitsInput_LinkDefDict_Impl();
}
