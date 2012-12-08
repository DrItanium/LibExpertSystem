#include "ConstructionTools.h"
using namespace llvm;

CLIPSUserBuilder::CLIPSUserBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSValueBuilder(nm, ty, namer) { }
void CLIPSUserBuilder::addFields(User* user, KnowledgeConstruction *kc, char* parent) {
	CLIPSValueBuilder::addFields((Value*)user, kc, parent);
   unsigned opCount = user->getNumOperands();
	if(opCount > 0) {
		openField("Operands");
      FunctionNamer& namer = getNamer();
	   for (User::op_iterator i0 = user->op_begin(), e0 = user->op_end(); i0 != e0; ++i0) {
         Value* target = *i0;
			if(isa<Function>(target) || isa<Instruction>(target) || isa<BasicBlock>(target)) {
				appendValue(target->getName());
         } else {
				appendValue(kc->route(target, namer));
         }
		}
		closeField();
	}
}

void CLIPSUserBuilder::build(User* user, KnowledgeConstruction *kc, char* parent) {
	open();
	addFields(user, kc, parent);
	close();
	std::string str = getCompletedString();
	kc->addToKnowledgeBase((PointerAddress)user, str);
}
