#include "ConstructionTools.h"
using namespace llvm;

CLIPSUserBuilder::CLIPSUserBuilder(std::string nm, std::string ty, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSValueBuilder(nm, ty, namer, tl) { }
void CLIPSUserBuilder::setFields(User* user, char* parent) {
	CLIPSValueBuilder::setFields((Value*)user, parent);
   unsigned opCount = user->getNumOperands();
	if(opCount > 0) {
      MultifieldBuilder mb (opCount);
		//openField("Operands");
      FunctionNamer& namer = getNamer();
      TypeLibrarian& tl = getLibrarian();
      unsigned index = 1;
	   for (User::op_iterator i0 = user->op_begin(), e0 = user->op_end(); i0 != e0; ++i0, ++index) {
         Value* target = *i0;
			if(isa<Function>(target) || isa<Instruction>(target) || isa<BasicBlock>(target)) {
            mb.setSlot(index, target->getName());
				//appendValue(target->getName());
         } else {
            mb.setSlot(index, Route(target, namer, tl));
            //appendValue(Route(target, namer));
         }
		}
		//closeField();
      setField("Operands", &mb);
	}
}
