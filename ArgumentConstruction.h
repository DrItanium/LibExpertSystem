#ifndef _argument_builder_h
#define _argument_builder_h
#include "ConstructionTools.h"
class CLIPSArgumentBuilder : public CLIPSValueBuilder {
   public:
      CLIPSArgumentBuilder(std::string nm, FunctionNamer& namer) : CLIPSValueBuilder(nm, "Argument", namer) { }
      void addFields(Argument* arg, KnowledgeConstruction &kc, char* parent) {
         CLIPSValueBuilder::addFields((Value*)arg, kc, parent);
         addField("Index", arg->getArgNo());
         if(arg->hasNestAttr()) addTrueField("HasNestAttribute");
         if(arg->hasNoAliasAttr()) addTrueField("HasNoAliasAttribute");
         if(arg->hasNoCaptureAttr()) addTrueField("HasNoCaptureAttribute");
         if(arg->hasStructRetAttr()) addTrueField("HasStructRetAttribute");
      }
     void build(Argument* arg, KnowledgeConstruction &kc, char* parent) {
		  open();
		  addFields(arg, kc, parent);
		  close();
		  std::string &str = getCompletedString();
		  kc->addToKnowledgeBase((PointerAddress)arg, str);
	  }
};
#endif
