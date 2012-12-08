#include "KnowledgeConstructionPass.h"
#include "KnowledgeConstructionEngine.h"
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
			vb.build(val, this, parent);
			return name;
		}
	}
}

std::string KnowledgeConstruction::route(Value* val, FunctionNamer& namer) {
	if(Instruction* inst = dyn_cast<Instruction>(val)) {
		return route(inst, namer, (char*)inst->getParent()->getName().data());
	} else {
		return route(val, namer, "nil");
	}
}

std::string KnowledgeConstruction::route(User* user, FunctionNamer& namer, char* parent) {
   if(namer.pointerRegistered((PointerAddress)user)) {
      return namer.nameFromPointer((PointerAddress)user);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
   //   CLIPSLoopBuilder l(name, namer);
	//	l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(Constant* cnst, FunctionNamer& namer) {
   if(namer.pointerRegistered((PointerAddress)cnst)) {
      return namer.nameFromPointer((PointerAddress)cnst);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
   //   CLIPSLoopBuilder l(name, namer);
	//	l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(Constant* cnst, FunctionNamer& namer, char* parent) {
   if(namer.pointerRegistered((PointerAddress)cnst)) {
      return namer.nameFromPointer((PointerAddress)cnst);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
   //   CLIPSLoopBuilder l(name, namer);
	//	l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(Instruction* inst, FunctionNamer& namer, char* parent) {
   if(namer.pointerRegistered((PointerAddress)inst)) {
      return namer.nameFromPointer((PointerAddress)inst);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
   //   CLIPSLoopBuilder l(name, namer);
	//	l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(Type* t, FunctionNamer& namer) {
   if(namer.pointerRegistered((PointerAddress)t)) {
      return namer.nameFromPointer((PointerAddress)t);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
   //   CLIPSLoopBuilder l(name, namer);
	//	l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(Operator* op, FunctionNamer& namer, char* parent) {
   if(namer.pointerRegistered((PointerAddress)op)) {
      return namer.nameFromPointer((PointerAddress)op);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
   //   CLIPSLoopBuilder l(name, namer);
	//	l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(BasicBlock* bb, FunctionNamer& namer, char* parent, bool constructInstructions) {
   if(namer.pointerRegistered((PointerAddress)bb)) {
      return namer.nameFromPointer((PointerAddress)bb);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
   //   CLIPSLoopBuilder l(name, namer);
	//	l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(Region* region, FunctionNamer& namer, char* parent) {
   if(namer.pointerRegistered((PointerAddress)region)) {
      return namer.nameFromPointer((PointerAddress)region);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
   //   CLIPSLoopBuilder l(name, namer);
	//	l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(Argument* arg, FunctionNamer& namer, char* parent) {
	if(namer.pointerRegistered((PointerAddress)arg)) {
		return namer.nameFromPointer((PointerAddress)arg);
	} else {
		char* gensymBuffer = (char*)calloc(64, sizeof(char));
		namer.makeGensymID(gensymBuffer);
		std::string name(gensymBuffer);
		free(gensymBuffer);
		CLIPSArgumentBuilder ab(name, namer);
		ab.build(arg, kc, parent);
		return name;
	}
}
std::string KnowledgeConstruction::route(Loop* loop, FunctionNamer& namer, char* parent) {
   if(namer.pointerRegistered((PointerAddress)loop)) {
      return namer.nameFromPointer((PointerAddress)loop);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
      CLIPSLoopBuilder l(name, namer);
		l.build(loop, this, parent);
      return name;
   }

}
std::string KnowledgeConstruction::route(MDString* mds, FunctionNamer& namer, char* parent) {
	if(namer.pointerRegistered((PointerAddress)mds)) {
		return namer.nameFromPointer((PointerAddress)mds);
	} else {
		char* gensymBuffer = (char*)calloc(64, sizeof(char));
		namer.makeGensymID(gensymBuffer);
		std::string name(gensymBuffer);
		free(gensymBuffer);
		CLIPSValueBuilder vb(name, "LLVMMDString", namer);
		vb.open();
		vb.addFields(val, parent);
		vb.addStringField("String", mds->getString());
		vb.close();
		std::string str = vb.getCompletedString();
		addToKnowledgeBase((PointerAddress)mds, str);
		return name;
	}

}
std::string KnowledgeConstruction::route(MDNode* mdn, FunctionNamer& namer, char* parent) {
	if(namer.pointerRegistered((PointerAddress)mdn)) {
		return namer.nameFromPointer((PointerAddress)mdn);
	} else {
		char* gensymBuffer = (char*)calloc(64, sizeof(char));
		namer.makeGensymID(gensymBuffer);
		std::string name(gensymBuffer);
		free(gensymBuffer);
		CLIPSValueBuilder vb (name, "LLVMMDNode", namer);
		vb.open();
		vb.addFields(val, parent);
		if(mdn->isFunctionLocal()) vb.addTrueField("IsFunctionLocal");
		const Function* fn = mdn->getFunction();
		if(fn != 0) {
			vb.addField("TargetFunction", fn->getName());
		}
		unsigned total = mdn->getNumOperands();
		if(total > 0) {
			char* cName = (char*)name.c_str();
			vb.openField("Operands");
			for(unsigned i = 0; i < total; ++i) {
				vb.appendValue(route(mdn->getOperand(i), namer, cName));
			}
			vb.closeField();
		}
		vb.close();
		std::string str = vb.getCompletedString();
		addToKnowledgeBase((PointerAddress)mdn, str);
		return name;
	}
}
std::string KnowledgeConstruction::route(InlineAsm* iasm, FunctionNamer& namer, char* parent) {
	if(namer.pointerRegistered((PointerAddress)iasm)) {
		return namer.nameFromPointer((PointerAddress)iasm);
	} else {
		char* gensymBuffer = (char*)calloc(64, sizeof(char));
		namer.makeGensymID(gensymBuffer);
		std::string name(gensymBuffer);
		free(gensymBuffer);
		CLIPSValueBuilder vb (name, "LLVMInlineAsm", namer);
		vb.open();
		vb.addFields(val, parent);
		if(iasm->hasSideEffects()) vb.addTrueField("HasSideEffects");
		if(iasm->isAlignStack()) vb.addTrueField("IsAlignStack");
		const std::string& aStr = iasm->getAsmString();
		const std::string& cnStr = iasm->getConstraintString();
		vb.addStringField("AsmString", aStr);
		vb.addStringField("ConstraintString", cnStr);
		vb.close();
		std::string str = vb.getCompletedString();
		addToKnowledgeBase((PointerAddress)iasm, str);
		return name;
	}
}
void KnowledgeConstruction::route(RegionInfo& ri, FunctionNamer& namer, char* parent) {

}
void KnowledgeConstruction::route(LoopInfo& li, FunctionNamer& namer, char* parent) {

}
void KnowledgeConstruction::updateFunctionContents(Function& fn, FunctionNamer& namer) {

}

char KnowledgeConstruction::ID = 0;
static RegisterPass<KnowledgeConstruction> kc("knowledge", "Knowledge constructor", false, false);
INITIALIZE_PASS(KnowledgeConstruction, "knowledge", "Knowledge constructor", false, false)
//for opt
