#ifndef _instruction_construction_h
#define _instruction_construction_h
#include "ConstructionTools.h"
class CLIPSInstructionBuilder : public CLIPSUserBuilder {
	public:
		CLIPSInstructionBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSUserBuilder(nm, ty, namer) { }
		void addFields(Instruction* instruction, KnowledgeConstruction *kc, char* parent, bool addDestinationRegisters = true);
		void build(Instruction* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSPHINodeBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSPHINodeBuilder(std::string nm, FunctionNamer& namer) : CLIPSInstructionBuilder(nm, "PhiNode", namer) { }
		void addFields(PHINode* instruction, KnowledgeConstruction *kc, char* parent);
		void build(PHINode* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSStoreInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSStoreInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSInstructionBuilder(nm, "StoreInstruction", namer) { }
		void addFields(StoreInst* target, KnowledgeConstruction *kc, char* parent);
		void build(StoreInst* instruction, KnowledgeConstruction *kc, char* parent);
};

class CLIPSBinaryOperatorBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSBinaryOperatorBuilder(std::string nm, FunctionNamer& namer) : CLIPSInstructionBuilder(nm, "BinaryOperator", namer) { }
		void addFields(BinaryOperator* target, KnowledgeConstruction *kc, char* parent);
		void build(BinaryOperator* instruction, KnowledgeConstruction *kc, char* parent);
};

class CLIPSCallInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSCallInstructionBuilder(std::string nm, FunctionNamer& namer, std::string ty = "CallInstruction") : CLIPSInstructionBuilder(nm, ty, namer) { }
		void addFields(CallInst* target, KnowledgeConstruction *kc, char* parent);
		void build(CallInst* instruction, KnowledgeConstruction *kc, char* parent);
};

class CLIPSCompareInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSCompareInstructionBuilder(std::string nm, FunctionNamer& namer, std::string ty = "CompareInstruction") : CLIPSInstructionBuilder(nm, ty, namer) { }
		void addFields(CmpInst* target, KnowledgeConstruction *kc, char* parent);
		void build(CmpInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSFPCompareInstructionBuilder : public CLIPSCompareInstructionBuilder {
	public:
		CLIPSFPCompareInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSCompareInstructionBuilder(nm, namer, "FloatingPointCompareInstruction") { }
		void addFields(FCmpInst* target, KnowledgeConstruction *kc, char* parent);
		void build(FCmpInst* instruction, KnowledgeConstruction *kc, char* parent);
};

class CLIPSIntCompareInstructionBuilder : public CLIPSCompareInstructionBuilder {
	public:
		CLIPSIntCompareInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSCompareInstructionBuilder(nm, namer, "IntegerCompareInstruction") { }
		void addFields(ICmpInst* target, KnowledgeConstruction *kc, char* parent);
		void build(ICmpInst* instruction, KnowledgeConstruction *kc, char* parent);
};

class CLIPSGetElementPtrInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSGetElementPtrInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSInstructionBuilder(nm, "GetElementPointerInstruction", namer) { }
		void addFields(GetElementPtrInst* target, KnowledgeConstruction *kc, char* parent);
		void build(GetElementPtrInst* instruction, KnowledgeConstruction *kc, char* parent);
};

class CLIPSTerminatorInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSTerminatorInstructionBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSInstructionBuilder(nm, ty, namer) { }
		void addFields(TerminatorInst* target, KnowledgeConstruction *kc, char* parent);
		void build(TerminatorInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSBranchInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSBranchInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSTerminatorInstructionBuilder(nm, "BranchInstruction", namer) { }
		void addFields(BranchInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(BranchInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSIndirectBranchInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSIndirectBranchInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSTerminatorInstructionBuilder(nm, "IndirectBranchInstruction", namer) { }
		void addFields(IndirectBrInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(IndirectBrInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSInvokeInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	//this is different from a call instruction because there are two extra
	//arguments that define the normal and exception jump points
	public:
		CLIPSInvokeInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSTerminatorInstructionBuilder(nm, "InvokeInstruction", namer) { }
		void addFields(InvokeInst* target, KnowledgeConstruction *kc, char* parent);
		void build(InvokeInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSResumeInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSResumeInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSTerminatorInstructionBuilder(nm, "ResumeInstruction", namer) { }
		void addFields(ResumeInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(ResumeInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSReturnInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSReturnInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSTerminatorInstructionBuilder(nm, "ReturnInstruction", namer) { }
		void addFields(ReturnInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(ReturnInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSSwitchInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSSwitchInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSTerminatorInstructionBuilder(nm, "SwitchInstruction", namer) { }
		void addFields(SwitchInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(SwitchInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSUnreachableInstructionBuilder : public CLIPSTerminatorInstructionBuilder {
	public:
		CLIPSUnreachableInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSTerminatorInstructionBuilder(nm, "UnreachableInstruction", namer) { }
		void addFields(UnreachableInst* instruction, KnowledgeConstruction *kc, char* parent);
		void build(UnreachableInst* instruction, KnowledgeConstruction *kc, char* parent);
};

class CLIPSSelectInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSSelectInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSInstructionBuilder(nm, "SelectInstruction", namer) { }
		void addFields(SelectInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(SelectInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSUnaryInstructionBuilder : public CLIPSInstructionBuilder {
	public:
		CLIPSUnaryInstructionBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSInstructionBuilder(nm, ty, namer) { }
		void addFields(UnaryInstruction* inst, KnowledgeConstruction *kc, char* parent);
		void build(UnaryInstruction* instruction, KnowledgeConstruction *kc, char* parent);
};

class CLIPSAllocaInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public:
		CLIPSAllocaInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSUnaryInstructionBuilder(nm, "AllocaInstruction", namer) { }
		void addFields(AllocaInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(AllocaInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSCastInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public:
		CLIPSCastInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSUnaryInstructionBuilder(nm, "CastInstruction", namer) { }
		void addFields(CastInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(CastInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSExtractValueInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public: 
		CLIPSExtractValueInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSUnaryInstructionBuilder(nm, "ExtractValueInstruction", namer) { }
		void addFields(ExtractValueInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(ExtractValueInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSLoadInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public: 
		CLIPSLoadInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSUnaryInstructionBuilder(nm, "LoadInstruction", namer) { }
		void addFields(LoadInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(LoadInst* instruction, KnowledgeConstruction *kc, char* parent);
};
class CLIPSVAArgInstructionBuilder : public CLIPSUnaryInstructionBuilder {
	public: 
		CLIPSVAArgInstructionBuilder(std::string nm, FunctionNamer& namer) : CLIPSUnaryInstructionBuilder(nm, "VAArgInstruction", namer) { }
		void addFields(VAArgInst* inst, KnowledgeConstruction *kc, char* parent);
		void build(VAArgInst* instruction, KnowledgeConstruction *kc, char* parent);
};
#endif
