#include "ConstructionTools.h"
using namespace llvm;
CLIPSValueBuilder::CLIPSValueBuilder(std::string nm, std::string ty, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSObjectBuilder(nm, ty, namer, tl) { }
void CLIPSValueBuilder::setType(Type* t) {
	PointerAddress ptr = (PointerAddress)t;
   FunctionNamer& namer = getNamer();
   TypeLibrarian& tl = getLibrarian();
   if(namer.pointerRegistered(ptr)) {
      addField("Type", namer.nameFromPointer(ptr));
   } else {
      addField("Type", Route(t, namer, tl));
   }
}

void CLIPSValueBuilder::setFields(Value* val, char* parent) {
	CLIPSObjectBuilder::setFields((PointerAddress)val, parent);
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
