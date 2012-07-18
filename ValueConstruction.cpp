#include "ConstructionTools.h"
using namespace llvm;
CLIPSValueBuilder::CLIPSValueBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSObjectBuilder(nm, ty, namer) { }
void CLIPSValueBuilder::setType(Type* t) {
	PointerAddress ptr = (PointerAddress)t;
   FunctionNamer& namer = getNamer();
   if(namer.pointerRegistered(ptr)) {
      addField("Type", namer.nameFromPointer(ptr));
   } else {
      addField("Type", Route(t, namer));
   }
}

void CLIPSValueBuilder::addFields(Value* val, char* parent) {
	CLIPSObjectBuilder::addFields((PointerAddress)val, parent);
	setType(val->getType());
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
