#include "KnowledgeConstructionPass.h"
#include "KnowledgeConstructionEngine.h"

#define BuildUpFullExpression(type, ty, tgt) \
	type vb (name, ty, namer); \
vb.build(tgt, this, parent)

#define BuildUpExpression(type, tgt) \
	type vb (name, namer); \
vb.build(tgt, this, parent)

#define makeGensym(name)  \
	char* n = CharBuffer(64); \
namer.makeGensymID(n); \
std::string name(n); \
free(n)

#define nested_dyn_cast(Type, name, in) Type * name = dyn_cast<Type>(in)
#define simple_dyn_cast(Type, in) nested_dyn_cast(Type, op, in)
KnowledgeConstruction::~KnowledgeConstruction() {
	delete instances;
	delete instanceStream;
	delete tmp;
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
	if(Instruction* inst = dyn_cast<Instruction>(user)) {
		return route(inst, namer, parent);
	} else if(Constant* cnst = dyn_cast<Constant>(user)) {
		return route(cnst, namer, parent);
	} else if(Operator* op = dyn_cast<Operator>(user)) {
		return route(op, namer, parent);
	} else {
		llvm::errs() << "WARNING: Found an unimplemented user " << *user << '\n';
		char* n = CharBuffer(64);
		namer.makeGensymID(n);
		std::string name(n);
		free(n);
		CLIPSUserBuilder vb (name, "LLVMUser", namer);
		vb.build(user, this, parent);
		return name;
	}
}
std::string KnowledgeConstruction::route(Constant* cnst, FunctionNamer& namer) {
	return route(cnst, namer, "nil");
}
std::string KnowledgeConstruction::route(Constant* val, FunctionNamer& namer, char* parent) {
	if(namer.pointerRegistered((PointerAddress)val)) {
		return namer.nameFromPointer((PointerAddress)val);
	} else if(isa<Function>(val)) {
		return val->getName().str();
	} else {
		std::string q;
		raw_string_ostream tmp(q);
		if(!val->hasName()) {
			namer.makeGensymID(tmp); 
		} else {
			tmp << val->getName();
		}
		std::string name = tmp.str();
		if(simple_dyn_cast(UndefValue, val)) {
			BuildUpExpression(CLIPSUndefValueBuilder, op);
			return name;
		} else if(simple_dyn_cast(BlockAddress, val)) {
			BuildUpExpression(CLIPSBlockAddressBuilder, op);
			return name;
		} else if(simple_dyn_cast(ConstantAggregateZero, val)) {
			BuildUpExpression(CLIPSConstantAggregateZeroBuilder, op);
			return name;
		} else if(simple_dyn_cast(ConstantArray, val)) {
			BuildUpExpression(CLIPSConstantArrayBuilder, op);
			return name;
		} else if(simple_dyn_cast(ConstantExpr, val)) {
			BuildUpExpression(CLIPSConstantExpressionBuilder, op);
			return name;
		} else if(simple_dyn_cast(ConstantFP, val)) {
			BuildUpExpression(CLIPSConstantFloatingPointBuilder, op);
			return name;
		} else if(simple_dyn_cast(ConstantInt, val)) {
			BuildUpExpression(CLIPSConstantIntegerBuilder, op);
			return name;
		} else if(simple_dyn_cast(ConstantPointerNull, val)) {
			BuildUpExpression(CLIPSConstantPointerNullBuilder, op);
			return name;
		} else if(simple_dyn_cast(ConstantStruct, val)) {
			ConstantStruct* cs = (ConstantStruct*)val;
			std::string name((char*)cs->getName().data());
			BuildUpExpression(CLIPSConstantStructBuilder, op);
			return name;
		} else if(simple_dyn_cast(ConstantVector, val)) {
			BuildUpExpression(CLIPSConstantVectorBuilder, op);
			return name;
		} else if(simple_dyn_cast(GlobalValue, val)) {
			if(nested_dyn_cast(GlobalAlias, gaop, op)) {
				BuildUpExpression(CLIPSGlobalAliasBuilder, gaop);
				return name;
			} else if(nested_dyn_cast(GlobalVariable, gvop, op)) {
				BuildUpExpression(CLIPSGlobalVariableBuilder, gvop);
				return name;
			} else {
				BuildUpFullExpression(CLIPSGlobalValueBuilder, "GlobalValue", op);
				return name;
			}
		} else {
			BuildUpFullExpression(CLIPSConstantBuilder, "Constant", val);
			return name;
		}
	}
}
std::string KnowledgeConstruction::route(Instruction* val, FunctionNamer& namer, char* parent) {
	if(namer.pointerRegistered((PointerAddress)val)) {
		return namer.nameFromPointer((PointerAddress)val);
	} else {
		//set the name if it doesn't have one and should
		if(!val->getType()->isVoidTy() && !val->hasName()) {
			char* buf = CharBuffer(128);
			namer.makeRegisterID(buf);
			val->setName(Twine(buf));
			free(buf);
		}
		std::string tmp;
		raw_string_ostream builder (tmp);
		if(!val->getType()->isVoidTy()) {
			builder << val->getName();
		} else {
			char* buf = CharBuffer(64);
			namer.makeGensymID(buf);
			builder << buf;	
			free(buf);
		}
		std::string name (builder.str());
		if(PHINode* op = dyn_cast<PHINode>(val)) {
			BuildUpExpression(CLIPSPHINodeBuilder, op);
			return name;
		} else if(StoreInst* op = dyn_cast<StoreInst>(val)) {
			BuildUpExpression(CLIPSStoreInstructionBuilder, op);
			return name;
		} else if(BinaryOperator* op = dyn_cast<BinaryOperator>(val)) {
			BuildUpExpression(CLIPSBinaryOperatorBuilder, op);
			return name;
		} else if(CallInst* op = dyn_cast<CallInst>(val)) {
			BuildUpExpression(CLIPSCallInstructionBuilder, op);
			return name;
		} else if(CmpInst* op = dyn_cast<CmpInst>(val)) {
			if(FCmpInst* fop = dyn_cast<FCmpInst>(op)) {
				BuildUpExpression(CLIPSFPCompareInstructionBuilder, fop);
				return name;
			} else if(ICmpInst* iop = dyn_cast<ICmpInst>(op)) {
				BuildUpExpression(CLIPSIntCompareInstructionBuilder, iop);
				return name;
			} else {
				BuildUpExpression(CLIPSCompareInstructionBuilder, op);
				return name;
			}
		} else if(GetElementPtrInst* op = dyn_cast<GetElementPtrInst>(val)) {
			BuildUpExpression(CLIPSGetElementPtrInstructionBuilder, op);
			return name;
		} else if(LoadInst* op = dyn_cast<LoadInst>(val)) {
			BuildUpExpression(CLIPSLoadInstructionBuilder, op);
			return name;
		} else if(TerminatorInst* op = dyn_cast<TerminatorInst>(val)) {
			if(BranchInst* bop = dyn_cast<BranchInst>(op)) {
				BuildUpExpression(CLIPSBranchInstructionBuilder, bop);
				return name;
			} else if(IndirectBrInst* ibop = dyn_cast<IndirectBrInst>(op)) {
				BuildUpExpression(CLIPSIndirectBranchInstructionBuilder, ibop);
				return name;
			} else if(InvokeInst* iiop = dyn_cast<InvokeInst>(op)) {
				BuildUpExpression(CLIPSInvokeInstructionBuilder, iiop);
				return name;
			} else if(ResumeInst* rop = dyn_cast<ResumeInst>(op)) {
				BuildUpExpression(CLIPSResumeInstructionBuilder, rop);
				return name;
			} else if(nested_dyn_cast(ReturnInst, riop, op)) {
				BuildUpExpression(CLIPSReturnInstructionBuilder, riop);
				return name;
			} else if(nested_dyn_cast(SwitchInst, swop, op)) {
				BuildUpExpression(CLIPSSwitchInstructionBuilder, swop);
				return name;
			} else if(nested_dyn_cast(UnreachableInst, uwop, op)) {
				BuildUpExpression(CLIPSUnreachableInstructionBuilder, uwop);
				return name;
			} else {
				BuildUpFullExpression(CLIPSTerminatorInstructionBuilder, "TerminatorInstruction", op);
				return name;
			}
		} else if(simple_dyn_cast(SelectInst, val)) {
			BuildUpExpression(CLIPSSelectInstructionBuilder, op);
			return name;
		} else if(simple_dyn_cast(UnaryInstruction, val)) {
			if(nested_dyn_cast(AllocaInst, aop, op)) {
				BuildUpExpression(CLIPSAllocaInstructionBuilder, aop);
				return name;
			} else if(nested_dyn_cast(CastInst, cop, op)) {
				BuildUpExpression(CLIPSCastInstructionBuilder, cop);
				return name;
			} else if(nested_dyn_cast(ExtractValueInst,eop, op)) {
				BuildUpExpression(CLIPSExtractValueInstructionBuilder, eop);
				return name;
			} else if(nested_dyn_cast(VAArgInst, vop, op)) {
				BuildUpExpression(CLIPSVAArgInstructionBuilder, vop);
				return name;
			} else {
				BuildUpFullExpression(CLIPSUnaryInstructionBuilder, "UnaryInstruction", op);
				return name;
			}
		} else {
			BuildUpFullExpression(CLIPSInstructionBuilder, "Instruction", val);
			return name;
		}
	}
}
std::string KnowledgeConstruction::route(Type* t, FunctionNamer& namer) {
	if(namer.pointerRegistered((PointerAddress)t)) {
		return namer.nameFromPointer((PointerAddress)t);
	} else {
		char* n = CharBuffer(64);
		namer.makeGensymID(n);
		std::string id(n);
		free(n);
		if(FunctionType* ft = dyn_cast<FunctionType>(t)) {
			CLIPSFunctionTypeBuilder a(id, namer);
			a.build(ft, this);
		} else if(IntegerType* it = dyn_cast<IntegerType>(t)) {
			CLIPSIntegerTypeBuilder b(id, namer);
			b.build(it, this);
		} else if(CompositeType* ct = dyn_cast<CompositeType>(t)) {
			if(StructType* st = dyn_cast<StructType>(ct)) {
				CLIPSStructTypeBuilder c(id, namer);
				c.build(st, this, "nil");
			} else if(SequentialType* qt = dyn_cast<SequentialType>(ct)) {
				if(ArrayType* at = dyn_cast<ArrayType>(qt)) {
					CLIPSArrayTypeBuilder d(id, namer);
					d.build(at, this);
				} else if(PointerType* pt = dyn_cast<PointerType>(qt)) {
					CLIPSPointerTypeBuilder e(id, namer);
					e.build(pt, this);
				} else if(VectorType* vt = dyn_cast<VectorType>(qt)) {
					CLIPSVectorTypeBuilder f(id, namer);
					f.build(vt, this);
				} else {
					CLIPSSequentialTypeBuilder typ(id, namer);
					typ.build(qt, this);
				}
			} else {
				CLIPSCompositeTypeBuilder typ(id, namer);
				typ.build(ct, this);
			}
		} else {
			CLIPSTypeBuilder typ(id, namer);
			typ.build(t, this);
		}
		return id;
	}

}
std::string KnowledgeConstruction::route(Operator* val, FunctionNamer& namer) {
	return route(val, namer, "nil");
}
std::string KnowledgeConstruction::route(Operator* val, FunctionNamer& namer, char* parent) {

	if(namer.pointerRegistered((PointerAddress)val)) {
		return namer.nameFromPointer((PointerAddress)val);
	} else { 
		if(OverflowingBinaryOperator* op = dyn_cast<OverflowingBinaryOperator>(val)) {
			makeGensym(name);
			CLIPSOverflowingBinaryOperatorBuilder vb (name, namer);
			vb.build(op, this, parent);
			return name;
		} else if(PossiblyExactOperator* op = dyn_cast<PossiblyExactOperator>(val)) {
			makeGensym(name);
			CLIPSPossiblyExactOperatorBuilder vb (name, namer);
			vb.build(op, this, parent);
			return name;
		} else {
			makeGensym(name);
			CLIPSOperatorBuilder vb (name, "Operator", namer);
			vb.build(val, this, parent);
			return name;
		}
	}
}
std::string KnowledgeConstruction::route(BasicBlock* bb, FunctionNamer& namer, char* parent, bool constructInstructions) {
	if(namer.pointerRegistered((PointerAddress)bb)) {
		return namer.nameFromPointer((PointerAddress)bb);
	} else {
		std::string name(bb->getName().data());
		CLIPSBasicBlockBuilder b(name, namer);
		b.build(bb, this, parent, constructInstructions);
		return name;
	}

}
std::string KnowledgeConstruction::route(Region* region, FunctionNamer& namer, char* parent) {
	if(namer.pointerRegistered((PointerAddress)region)) {
		return namer.nameFromPointer((PointerAddress)region);
	} else {
		char* buf = CharBuffer(128);
		namer.makeRegionID(buf);
		std::string name(buf);
		free(buf);
		CLIPSRegionBuilder r(name, namer);
		r.build(region, this, parent);
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
		ab.build(arg, this, parent);
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
		vb.addFields(mds, this, parent);
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
		vb.addFields(mdn, this, parent);
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
		vb.addFields(iasm, this, parent);
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
	route(ri.getTopLevelRegion(), namer, parent);
}
void KnowledgeConstruction::route(LoopInfo& li, FunctionNamer& namer, char* parent) {
	for(LoopInfo::iterator b = li.begin(), e = li.end(); b != e; ++b) {
		Loop* l = *b;
		route(l, namer, parent);
	}
}
void KnowledgeConstruction::updateFunctionContents(Function& fn, FunctionNamer& namer) {
	for(llvm::Function::iterator i = fn.begin() , e = fn.end(); i != e; ++i) {
		llvm::BasicBlock* bb = i;
		if(!bb->hasName()) {
			char* buf = CharBuffer(128);
			namer.makeBasicBlockID(buf);
			std::string name(buf);
			bb->setName(Twine(name));
			free(buf);
		}
		//aww hell let's go through and set the names of all instructions right
		//here and now :)
		for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
			Instruction* inst = i;
			if(!inst->getType()->isVoidTy()) {
				if(!inst->hasName()) {
					char* buf = CharBuffer(128);
					namer.makeRegisterID(buf);
					inst->setName(Twine(buf));
					free(buf);
				} else {
					StringRef name = inst->getName();
					if(name[0] == '.') {
						char* buf = CharBuffer(2 + name.size());
						sprintf(buf, "v%s", name.data());
						inst->setName(Twine(buf));
						free(buf);
					}
				}
			}
		}
	}
	char* fnName = (char*)fn.getName().data();
	for(Function::arg_iterator s = fn.arg_begin(), f = fn.arg_end(); s != f; ++s) {
		Argument* a = s;
		//build it if we need to :)
		route(a, namer, fnName);
	}
}
void KnowledgeConstruction::route(Function& fn, LoopInfo& li, RegionInfo& ri) {
	char* funcName;
	//get the function namer object
	FunctionNamer namer;
	funcName = (char*)fn.getName().data();
	namer.reset();
	std::string tmp("nil");
	namer.tryRegisterPointerToName(0L, tmp);
	instances->clear();
	updateFunctionContents(fn, namer);
	route(li, namer, funcName);
	route(ri, namer, funcName);
}

#undef nested_dyn_cast
#undef simple_dyn_cast
#undef BuildUpFullExpression
#undef BuildUpExpression
#undef makeGensym
