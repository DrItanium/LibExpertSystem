#ifndef _argument_builder_h
#define _argument_builder_h
#include "ConstructionTools.h"
class CLIPSArgumentBuilder : public CLIPSValueBuilder {
   public:
      CLIPSArgumentBuilder(std::string nm, FunctionNamer& namer) : CLIPSValueBuilder(nm, "Argument", namer) { }
      void addFields(Argument* arg, char* parent) {
         CLIPSValueBuilder::addFields((Value*)arg, parent);
         addField("Index", arg->getArgNo());
         if(arg->hasNestAttr()) addTrueField("HasNestAttribute");
         if(arg->hasNoAliasAttr()) addTrueField("HasNoAliasAttribute");
         if(arg->hasNoCaptureAttr()) addTrueField("HasNoCaptureAttribute");
         if(arg->hasStructRetAttr()) addTrueField("HasStructRetAttribute");
      }
      
};
#endif
