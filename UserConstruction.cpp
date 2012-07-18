#include "ConstructionTools.h"
using namespace llvm;

CLIPSUserBuilder::CLIPSUserBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSValueBuilder(nm, ty, namer) { }
void CLIPSUserBuilder::addFields(User* user, char* parent) {
	CLIPSValueBuilder::addFields((Value*)user, parent);
   unsigned opCount = user->getNumOperands();
	if(opCount > 0) {
		openField("Operands");
      FunctionNamer& namer = getNamer();
	   for (User::op_iterator i0 = user->op_begin(), e0 = user->op_end(); i0 != e0; ++i0) {
         Value* target = *i0;
			if(isa<Function>(target) || isa<Instruction>(target) || isa<BasicBlock>(target)) {
				appendValue(target->getName());
         } else {
            appendValue(Route(target, namer));
         }
		}
		closeField();
	}
}
