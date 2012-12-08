#include "KnowledgeConstructionPass.h"
KnowledgeConstruction::~KnowledgeConstruction() {
	delete instances;
	delete instanceStream;
}
void KnowledgeConstruction::addToInstanceStream(std::string &instance) {
	(*instanceStream) << instance << " ";
}
void KnowledgeConstruction::registerInstance(PointerAddress ptrAdr, std::string &instance) {
	std::pair<PointerAddress, std::string&> pair (ptrAdr, instance);
	instances->insert(pair);
}
void KnowledgeConstruction::addToKnowledgeBase(PointerAddress ptrAddress, std::string &instance) {
	addToInstanceStream(instance);
	registerInstance(ptrAddress, instance);
}
std::string KnowledgeConstruction::route(Value* val, FunctionNamer& namer, char* parent) {
	if(namer.pointerRegistered((PointerAddress)val)) {
		return namer.nameFromPointer((PointerAddress)val);
	} else {
		if(User* u0 = dyn_cast<User>(val)) {
			return route(u0, namer, parent);
		} else if(BasicBlock* bb = dyn_cast<BasicBlock>(val)) {
			return route(bb, namer, parent);
		} else if(Argument* arg = dyn_cast<Argument>(val)) {
			return route(arg, namer, parent);
		} else if(InlineAsm* iasm = dyn_cast<InlineAsm>(val)) {
			return route(iasm, namer, parent);
		} else if(MDNode* mdn = dyn_cast<MDNode>(val)) {
			return route(mdn, namer, parent);
		} else if(MDString* mds = dyn_cast<MDString>(val)) {
			return route(mds, namer, parent);
		} else {
         llvm::errs() << "WARNING: Found an unimplemented value " << *val << '\n';
			char* gensymBuffer = (char*)calloc(64, sizeof(char));
			namer.makeGensymID(gensymBuffer);
			std::string name(gensymBuffer);
			free(gensymBuffer);
         CLIPSValueBuilder vb (name, "LLVMValue", namer);
         vb.open();
         vb.addFields(val, parent);
         vb.close();
			std::string& str = vb.getCompletedString();
			addToKnowledgeBase((PointerAddress)val, str);
         return name;
		}
	}
}

std::string route(Value* val, FunctionNamer& namer) {

}
char KnowledgeConstruction::ID = 0;
static RegisterPass<KnowledgeConstruction> kc("knowledge", "Knowledge constructor", false, false);
INITIALIZE_PASS(KnowledgeConstruction, "knowledge", "Knowledge constructor", false, false)
//for opt
