#include "llvm/InlineAsm.h"
#include "llvm/Metadata.h"
#include "KnowledgeConstructionEngine.h"
#include "TypeLibrarian.h"
#define DefaultBuilderAction(obj, name, parent) \
obj.setFields(name, parent); 

#define BuildUpFullExpression(type, ty, tgt) \
   type vb (name, ty, namer, tl); \
vb.setFields(tgt, parent); 

#define BuildUpExpression(type, tgt) \
   type vb (name, namer, tl); \
vb.setFields(tgt, parent);

#define PointerFoundBody(v) std::string result(namer.getTranslationTable().lookup((PointerAddress)v)); return result;

#define makeGensym(name)  \
   char* n = CharBuffer(64); \
namer.makeGensymID(n); \
std::string name(n); \
free(n)
#define nested_dyn_cast(Type, name, in) Type * name = dyn_cast<Type>(in)
#define simple_dyn_cast(Type, in) nested_dyn_cast(Type, op, in)

char nil[4] = "nil";

std::string Route(Value* val, char* parent, FunctionNamer& namer, TypeLibrarian& tl) {
   if(namer.pointerRegistered((PointerAddress)val)) {
      return namer.nameFromPointer((PointerAddress)val);
   } else {
      if(User* u0 = dyn_cast<User>(val)) return Route(u0, parent, namer, tl);
      else if(BasicBlock* bb = dyn_cast<BasicBlock>(val)) return Route(bb, parent, namer, tl);
      else if(Argument* arg = dyn_cast<Argument>(val)) return Route(arg, parent, namer, tl); 
      else if(InlineAsm* iasm = dyn_cast<InlineAsm>(val)) {
         makeGensym(name);
         CLIPSValueBuilder vb (name, "LLVMInlineAsm", namer, tl);
         //vb.open();
         vb.setFields(val, parent);
         if(iasm->hasSideEffects()) vb.setFieldTrue("HasSideEffects");
         if(iasm->isAlignStack()) vb.setFieldTrue("IsAlignStack");
         const std::string& aStr = iasm->getAsmString();
         const std::string& cnStr = iasm->getConstraintString();
         vb.setField("AsmString", aStr, true);
         vb.setField("ConstraintString", cnStr, true);
         //vb.close();
         //vb.convertToKnowledge();
         return name;
      } else if(MDNode* mdn = dyn_cast<MDNode>(val)) {
         makeGensym(name);
         CLIPSValueBuilder vb (name, "LLVMMDNode", namer, tl);
         //vb.open();
         vb.setFields(val, parent);
         if(mdn->isFunctionLocal()) vb.setFieldTrue("IsFunctionLocal");
         const Function* fn = mdn->getFunction();
         if(fn != 0) {
            vb.setField("TargetFunction", fn->getName());
         }
         unsigned total = mdn->getNumOperands();
         if(total > 0) {
            char* cName = (char*)name.c_str();
            MultifieldBuilder mb (total);
            //vb.openField("Operands");
            for(unsigned i = 0, index = 1; i < total; ++i, ++index) {
               mb.setSlot(index, Route(mdn->getOperand(i), cName, namer, tl));
            }
            vb.setField("Operands", &mb);
            //vb.closeField();
         }
         //vb.close();
         //vb.convertToKnowledge();
         return name;

      } else if(MDString* mds = dyn_cast<MDString>(val)) {
         makeGensym(name);
         CLIPSValueBuilder vb(name, "LLVMMDString", namer, tl);
         //vb.open();
         vb.setFields(val, parent);
         vb.setField("String", mds->getString(), true);
         //vb.close();
         //vb.convertToKnowledge();
         return name;
      } else {
         llvm::errs() << "WARNING: Found an unimplemented value " << *val << '\n';
         makeGensym(name);
         CLIPSValueBuilder vb (name, "LLVMValue", namer, tl);
         //vb.open();
         vb.setFields(val, parent);
         //vb.close();
         //vb.convertToKnowledge();
         return name;
      }
   }
}
std::string Route(Argument* val, char* parent, FunctionNamer& namer, TypeLibrarian& tl) {
   if(namer.pointerRegistered((PointerAddress)val)) {
      return namer.nameFromPointer((PointerAddress)val);
   } else {
      makeGensym(name);
      CLIPSArgumentBuilder ab (name, namer, tl);
      DefaultBuilderAction(ab, val, parent);
      return name;
   }
}
std::string Route(Value* val, FunctionNamer& namer, TypeLibrarian& tl) {
   if(Instruction* inst = dyn_cast<Instruction>(val)) {
      return Route(inst, (char*)inst->getParent()->getName().data(), namer, tl);
   } else {
      return Route(val, nil, namer, tl);
   }
}
std::string Route(Operator* oper, FunctionNamer& namer, TypeLibrarian& tl) {
   return Route(oper, nil, namer, tl);
}

std::string Route(Operator* val, char* parent, FunctionNamer& namer, TypeLibrarian& tl) {
   if(namer.pointerRegistered((PointerAddress)val)) {
      return namer.nameFromPointer((PointerAddress)val);
   } else { 
      if(OverflowingBinaryOperator* op = dyn_cast<OverflowingBinaryOperator>(val)) {
         makeGensym(name);
         CLIPSOverflowingBinaryOperatorBuilder vb (name, namer, tl);
         DefaultBuilderAction(vb, op, parent);
         return name;
      } else if(PossiblyExactOperator* op = dyn_cast<PossiblyExactOperator>(val)) {
         makeGensym(name);
         CLIPSPossiblyExactOperatorBuilder vb (name, namer, tl);
         DefaultBuilderAction(vb, op, parent);
         return name;
      } else {
         makeGensym(name);
         CLIPSOperatorBuilder vb (name, "Operator", namer, tl);
         DefaultBuilderAction(vb, val, parent);
         return name;
      }
   }
}
std::string Route(Instruction* val, char* parent, FunctionNamer& namer, TypeLibrarian& tl) {
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
std::string Route(Constant* val, char* parent, FunctionNamer& namer, TypeLibrarian& tl) {
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
std::string Route(Constant* cnst, FunctionNamer& namer, TypeLibrarian& tl) {
   return Route(cnst, nil, namer, tl);
}
std::string Route(Type* t, FunctionNamer& namer, TypeLibrarian& tl) {
   if(namer.pointerRegistered((PointerAddress)t)) {
      return namer.nameFromPointer((PointerAddress)t);
   } else {
      char* n = CharBuffer(64);
      namer.makeGensymID(n);
      std::string id(n);
      free(n);
      if(FunctionType* ft = dyn_cast<FunctionType>(t)) {
         CLIPSFunctionTypeBuilder a(id, namer, tl);
         a.setFields(ft);
      } else if(IntegerType* it = dyn_cast<IntegerType>(t)) {
         CLIPSIntegerTypeBuilder b(id, namer, tl);
         b.setFields(it);
      } else if(CompositeType* ct = dyn_cast<CompositeType>(t)) {
         if(StructType* st = dyn_cast<StructType>(ct)) {
            CLIPSIntegerTypeBuilder c(id, namer, tl);
            c.setFields(st);
         } else if(SequentialType* qt = dyn_cast<SequentialType>(ct)) {
            if(ArrayType* at = dyn_cast<ArrayType>(qt)) {
               CLIPSArrayTypeBuilder d(id, namer, tl);
               d.setFields(at);
            } else if(PointerType* pt = dyn_cast<PointerType>(qt)) {
               CLIPSPointerTypeBuilder e(id, namer, tl);
               e.setFields(pt);
            } else if(VectorType* vt = dyn_cast<VectorType>(qt)) {
               CLIPSVectorTypeBuilder f(id, namer, tl);
               f.setFields(vt);
            } else {
               CLIPSSequentialTypeBuilder typ(id, namer, tl);
               typ.setFields(qt); 
            }
         } else {
            CLIPSCompositeTypeBuilder typ(id, namer, tl);
            typ.setFields(ct); 
         }
      } else {
         CLIPSTypeBuilder typ(id, namer, tl);
         typ.setFields(t); 
      }
      return id;
   }
}
std::string Route(User* user, char* parent, FunctionNamer& namer, TypeLibrarian& tl) {
   if(Instruction* inst = dyn_cast<Instruction>(user)) {
      return Route(inst, parent, namer, tl);
   } else if(Constant* cnst = dyn_cast<Constant>(user)) {
      return Route(cnst, parent, namer, tl);
   } else if(Operator* op = dyn_cast<Operator>(user)) {
      return Route(op, parent, namer, tl);
   } else {
      llvm::errs() << "WARNING: Found an unimplemented user " << *user << '\n';
      char* n = CharBuffer(64);
      namer.makeGensymID(n);
      std::string name(n);
      free(n);
      CLIPSUserBuilder vb (name, "LLVMUser", namer, tl);
      vb.setFields(user, parent);
      return name;
   }
}
void ModifyFunctionContents(Function& fn, FunctionNamer& namer, TypeLibrarian& tl) {
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
      Route(a, fnName, namer, tl);
   }
}
std::string Route(BasicBlock* val, char* parent, FunctionNamer& namer, TypeLibrarian& tl, bool constructInstructions) {
   if(namer.pointerRegistered((PointerAddress)val)) {
      return namer.nameFromPointer((PointerAddress)val);
   } else {
      std::string name(val->getName().data());
      CLIPSBasicBlockBuilder builder (name, namer, tl);
      builder.setFields(val, parent, constructInstructions);
      return name;
   }
}
std::string Route(Region* region, char* parent, FunctionNamer& namer, TypeLibrarian& tl, LoopInfo* li) {
   if(namer.pointerRegistered((PointerAddress)region)) {
      return namer.nameFromPointer((PointerAddress)region);
   } else {
      char* buf = CharBuffer(128);
      namer.makeRegionID(buf);
      std::string name(buf);
      free(buf);
      CLIPSRegionBuilder rb (name, namer, tl, li);
      rb.setFields(region, parent);
      return name;
   }
}
std::string Route(Loop* loop, char* parent, FunctionNamer& namer, TypeLibrarian& tl) {
   if(namer.pointerRegistered((PointerAddress)loop)) {
      return namer.nameFromPointer((PointerAddress)loop);
   } else {
      char* buf = CharBuffer(128);
      namer.makeLoopID(buf);
      std::string name(buf);
      free(buf);
      CLIPSLoopBuilder l(name, namer, tl);
      l.setFields(loop, parent);
      //l.build(loop, parent);
      return name;
   }
}
void RouteLoopInfo(LoopInfo& li, char* parent, FunctionNamer& namer, TypeLibrarian& tl) {
   for(LoopInfo::iterator b = li.begin(), e = li.end(); b != e; ++b) {
      Loop* l = *b;
      Route(l, parent, namer, tl);
   }
}
#undef nested_dyn_cast
#undef simple_dyn_cast
#undef DefaultBuilderAction
#undef BuildUpFullExpression
#undef BuildUpExpression
#undef PointerFoundBody
#undef makeGensym
