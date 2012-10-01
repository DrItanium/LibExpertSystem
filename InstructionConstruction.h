#ifndef _instruction_construction_h
#define _instruction_construction_h
#include "ConstructionTools.h"
class CLIPSInstructionBuilder : public CLIPSUserBuilder {
	public:
		CLIPSInstructionBuilder(std::string nm, std::string ty, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSUserBuilder(nm, ty, namer, tl) { }
		void setFields(Instruction* instruction, char* parent, bool addDestinationRegisters = true);
};
class CLIPSPHINodeBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSPHINodeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSInstructionBuilder(nm, "PhiNode", namer, tl) { }
		void setFields(PHINode* instruction, char* parent);
};
class CLIPSStoreInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSStoreInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSInstructionBuilder(nm, "StoreInstruction", namer, tl) { }
		void setFields(StoreInst* target, char* parent);
};

class CLIPSBinaryOperatorBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSBinaryOperatorBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSInstructionBuilder(nm, "BinaryOperator", namer, tl) { }
		void setFields(BinaryOperator* target, char* parent);
};

class CLIPSCallInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSCallInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl, std::string ty = "CallInstruction") : CLIPSInstructionBuilder(nm, ty, namer, tl) { }
		void setFields(CallInst* target, char* parent);
};

class CLIPSCompareInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSCompareInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl, std::string ty = "CompareInstruction") : CLIPSInstructionBuilder(nm, ty, namer, tl) { }
		void setFields(CmpInst* target, char* parent);

};
class CLIPSFPCompareInstructionBuilder : public CLIPSCompareInstructionBuilder {
	public:
		CLIPSFPCompareInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSCompareInstructionBuilder(nm, namer, tl, "FloatingPointCompareInstruction") { }
		void setFields(FCmpInst* target, char* parent);
};

class CLIPSIntCompareInstructionBuilder : public CLIPSCompareInstructionBuilder {
	public:
		CLIPSIntCompareInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSCompareInstructionBuilder(nm, namer, tl, "IntegerCompareInstruction") { }
		void setFields(ICmpInst* target, char* parent);
};

class CLIPSGetElementPtrInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSGetElementPtrInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSInstructionBuilder(nm, "GetElementPointerInstruction", namer, tl) { }
		void setFields(GetElementPtrInst* target, char* parent);
};

class CLIPSTerminatorInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSTerminatorInstructionBuilder(std::string nm, std::string ty, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSInstructionBuilder(nm, ty, namer, tl) { }
		void setFields(TerminatorInst* target, char* parent);
};
class CLIPSBranchInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSBranchInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSTerminatorInstructionBuilder(nm, "BranchInstruction", namer, tl) { }
		void setFields(BranchInst* inst, char* parent);
};
class CLIPSIndirectBranchInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSIndirectBranchInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSTerminatorInstructionBuilder(nm, "IndirectBranchInstruction", namer, tl) { }
		void setFields(IndirectBrInst* inst, char* parent);
};
class CLIPSInvokeInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	//this is different from a call instruction because there are two extra
	//arguments that define the normal and exception jump points
	public:
		CLIPSInvokeInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSTerminatorInstructionBuilder(nm, "InvokeInstruction", namer, tl) { }
		void setFields(InvokeInst* target, char* parent);
};
class CLIPSResumeInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSResumeInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSTerminatorInstructionBuilder(nm, "ResumeInstruction", namer, tl) { }
		void setFields(ResumeInst* inst, char* parent);
};
class CLIPSReturnInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSReturnInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSTerminatorInstructionBuilder(nm, "ReturnInstruction", namer, tl) { }
		void setFields(ReturnInst* inst, char* parent);
};
class CLIPSSwitchInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSSwitchInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSTerminatorInstructionBuilder(nm, "SwitchInstruction", namer, tl) { }
		void setFields(SwitchInst* inst, char* parent);
};
class CLIPSUnreachableInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSUnreachableInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSTerminatorInstructionBuilder(nm, "UnreachableInstruction", namer, tl) { }
		void setFields(UnreachableInst* instruction, char* parent);
};

class CLIPSSelectInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSSelectInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSInstructionBuilder(nm, "SelectInstruction", namer, tl) { }
		void setFields(SelectInst* inst, char* parent);
};
class CLIPSUnaryInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSUnaryInstructionBuilder(std::string nm, std::string ty, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSInstructionBuilder(nm, ty, namer, tl) { }
		void setFields(UnaryInstruction* inst, char* parent);
};

class CLIPSAllocaInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public:
		CLIPSAllocaInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSUnaryInstructionBuilder(nm, "AllocaInstruction", namer, tl) { }
		void setFields(AllocaInst* inst, char* parent);
};
class CLIPSCastInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public:
		CLIPSCastInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSUnaryInstructionBuilder(nm, "CastInstruction", namer, tl) { }
		void setFields(CastInst* inst, char* parent);
};
class CLIPSExtractValueInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public: 
		CLIPSExtractValueInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSUnaryInstructionBuilder(nm, "ExtractValueInstruction", namer, tl) { }
		void setFields(ExtractValueInst* inst, char* parent);
};
class CLIPSLoadInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public: 
		CLIPSLoadInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSUnaryInstructionBuilder(nm, "LoadInstruction", namer, tl) { }
		void setFields(LoadInst* inst, char* parent);
};
class CLIPSVAArgInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public: 
		CLIPSVAArgInstructionBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSUnaryInstructionBuilder(nm, "VAArgInstruction", namer, tl) { }
		void setFields(VAArgInst* inst, char* parent);
};
#endif
