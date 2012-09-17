#ifndef _argument_builder_h
#define _argument_builder_h
#include "ConstructionTools.h"
class CLIPSArgumentBuilder : public CLIPSValueBuilder {
   public:
      CLIPSArgumentBuilder(std::string nm, FunctionNamer& namer) : CLIPSValueBuilder(nm, "Argument", namer) { }
      void setFields(Argument* arg, char* parent) {
         CLIPSValueBuilder::setFields((Value*)arg, parent);
         setField("Index", arg->getArgNo());
         if(arg->hasNestAttr()) setFieldTrue("HasNestAttribute");
         if(arg->hasNoAliasAttr()) setFieldTrue("HasNoAliasAttribute");
         if(arg->hasNoCaptureAttr()) setFieldTrue("HasNoCaptureAttribute");
         if(arg->hasStructRetAttr()) setFieldTrue("HasStructRetAttribute");
      }
      
};
#endif
