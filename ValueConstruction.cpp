#include "ConstructionTools.h"
using namespace llvm;
CLIPSValueBuilder::CLIPSValueBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSObjectBuilder(nm, ty, namer) { }
void CLIPSValueBuilder::setType(KnowledgeConstruction &kc, Type* t) {
	PointerAddress ptr = (PointerAddress)t;
   FunctionNamer& namer = getNamer();
   if(namer.pointerRegistered(ptr)) {
      addField("Type", namer.nameFromPointer(ptr));
   } else {
      addField("Type", kc->route(t, namer));
   }
}

void CLIPSValueBuilder::addFields(Value* val, KnowledgeConstruction &kc, char* parent) {
	CLIPSObjectBuilder::addFields((PointerAddress)val, parent);
	setType(kc, val->getType());
	addField("Name", val->getName());
	if(val->isDereferenceablePointer()) addTrueField("IsDereferenceablePointer");
	if(val->hasValueHandle()) addTrueField("HasValueHandle"); 
	if(val->hasOneUse()) {
      addTrueField("HasOneUse");
      addField("NumberOfUses", 1);
   } else {
      addField("NumberOfUses", val->getNumUses());
   }
}

void CLIPSValueBuilder::build(Value* val, KnowledgeConstruction &kc, char* parent) {
	open();
	addFields(val, kc, parent);
	close();
	std::string &str = getCompletedString();
	kc.addToKnowledgeBase((PointerAddress)val, str);
}
