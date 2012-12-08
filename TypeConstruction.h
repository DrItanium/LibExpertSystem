#ifndef _clips_type_builder_h
#define _clips_type_builder_h
#include "ConstructionTools.h"
class CLIPSTypeBuilder : public CLIPSObjectBuilder {
	public:
		CLIPSTypeBuilder(std::string nm, FunctionNamer& namer, std::string ty = "LLVMType") : CLIPSObjectBuilder(nm, ty, namer) { }
		void addFields(Type* type, KnowledgeConstruction* kc) {
         FunctionNamer& namer = getNamer();
			PointerAddress pointer = (PointerAddress)type;
			if(!namer.pointerRegistered(pointer)) {
				std::pair<PointerAddress, std::string> pair (pointer, getName());
				namer.getTranslationTable().insert(pair);
			}
			setPointer(pointer);
			if(type->isVoidTy()) addTrueField("IsVoidType");
			if(type->isFunctionTy()) addTrueField("IsFunctionType"); 
			if(type->isFPOrFPVectorTy()) {
				addTrueField("IsFPOrFPVectorType");
				if(type->isFloatingPointTy()) addTrueField("IsFloatingPointType");
				if(type->isFloatTy()) addTrueField("IsFloatType");
				if(type->isDoubleTy()) addTrueField("IsDoubleType");
				if(type->isX86_FP80Ty()) addTrueField("IsX86FP80Type"); 
				if(type->isFP128Ty()) addTrueField("IsFP128Type");
				if(type->isPPC_FP128Ty()) addTrueField("IsPPCFP128Type");
				addField("FPMantissaWidth", type->getFPMantissaWidth());
			}
			if(type->isX86_MMXTy()) addTrueField("IsX86MMXType");
			if(type->isLabelTy()) addTrueField("IsLabelType");
			if(type->isMetadataTy()) addTrueField("IsMetadataType");
			if(type->isIntOrIntVectorTy()) {
				addTrueField("IsIntOrIntVectorType");
				if(type->isIntegerTy()) addTrueField("IsIntegerType");
			}
			if(type->isStructTy()) addTrueField("IsStructType"); 
			if(type->isArrayTy()) addTrueField("IsArrayType"); 
			if(type->isPointerTy()) addTrueField("IsPointerType"); 
			if(type->isVectorTy()) {
				addTrueField("IsVectorType"); 
				addField("ScalarSizeInBits", type->getScalarSizeInBits());
			}
			if(type->isEmptyTy()) addTrueField("IsEmptyType"); 
			if(type->isPrimitiveType()) {
				addTrueField("IsPrimitiveType");
				addField("PrimitiveSizeInBits", type->getPrimitiveSizeInBits());
			}
			if(type->isDerivedType()) addTrueField("IsDerivedType"); 
			if(type->isFirstClassType()) addTrueField("IsFirstClassType");
			if(type->isSingleValueType()) addTrueField("IsSingleValueType"); 
			if(type->isAggregateType()) addTrueField("IsAggregateType");
			if(type->isSized()) addTrueField("IsSized");
			if(type->getNumContainedTypes() > 0) {
				openField("Subtypes");
				for(Type::subtype_iterator i = type->subtype_begin(),
						e = type->subtype_end(); i != e; ++i) {
					Type* t = (*i);
					appendValue(kc->route(t, namer));
				}
				closeField();
			}
		}
		void build(Type* st, KnowledgeConstruction* kc) {
			open();
			addFields(st, kc);
			close();
			std::string str = getCompletedString();
			kc->addToKnowledgeBase((PointerAddress)st, str);
		}
};
class CLIPSFunctionTypeBuilder : public CLIPSTypeBuilder {
	public:
		CLIPSFunctionTypeBuilder(std::string nm, FunctionNamer& namer, std::string ty = "FunctionType") : CLIPSTypeBuilder(nm, namer, ty) { }
};
class CLIPSCompositeTypeBuilder : public CLIPSTypeBuilder {
	public:
		CLIPSCompositeTypeBuilder(std::string nm, FunctionNamer& namer, std::string ty = "CompositeType") : CLIPSTypeBuilder(nm, namer, ty) { }
};
class CLIPSSequentialTypeBuilder : public CLIPSCompositeTypeBuilder {
	public:
		CLIPSSequentialTypeBuilder(std::string nm, FunctionNamer& namer, std::string ty = "SequentialType") : CLIPSCompositeTypeBuilder(nm, namer, ty) { }
};
class CLIPSArrayTypeBuilder : public CLIPSSequentialTypeBuilder {
	public:
		CLIPSArrayTypeBuilder(std::string nm, FunctionNamer& namer, std::string ty = "ArrayType") : CLIPSSequentialTypeBuilder(nm, namer, ty) { }
};
class CLIPSPointerTypeBuilder : public CLIPSSequentialTypeBuilder {
	public:
		CLIPSPointerTypeBuilder(std::string nm, FunctionNamer& namer, std::string ty = "PointerType") : CLIPSSequentialTypeBuilder(nm, namer, ty) { }

};
class CLIPSVectorTypeBuilder : public CLIPSSequentialTypeBuilder {
	public:
		CLIPSVectorTypeBuilder(std::string nm, FunctionNamer& namer, 
				std::string ty = "VectorType") : CLIPSSequentialTypeBuilder(nm, namer, ty) { }

};
class CLIPSStructTypeBuilder : public CLIPSCompositeTypeBuilder {
	public:
		CLIPSStructTypeBuilder(std::string nm, FunctionNamer& namer, std::string ty = "StructType") : CLIPSCompositeTypeBuilder(nm, namer, ty) { }
		void addFields(StructType* st, KnowledgeConstruction* kc, char* parent) 
		{
			CLIPSTypeBuilder::addFields(st, kc);
			setParent(parent);
			addField("Name", st->getName());
		}
		void build(StructType* st, KnowledgeConstruction* kc, char* parent) {
			open();
			addFields(st, kc, parent);
			close();
			std::string str = getCompletedString();
			kc->addToKnowledgeBase((PointerAddress)st, str);
		}
};

class CLIPSIntegerTypeBuilder : public CLIPSTypeBuilder {
	public:
		CLIPSIntegerTypeBuilder(std::string nm, FunctionNamer& namer, std::string ty = "IntegerType") : CLIPSTypeBuilder(nm, namer, ty) { }
};
#endif
