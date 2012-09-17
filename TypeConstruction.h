#ifndef _clips_type_builder_h
#define _clips_type_builder_h
#include "ConstructionTools.h"
class CLIPSTypeBuilder : public CLIPSObjectBuilder {
	public:
		CLIPSTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "LLVMType") : CLIPSObjectBuilder(nm, ty, namer, lib) { }
		void setFields(Type* type) {
         FunctionNamer& namer = getNamer();
			PointerAddress pointer = (PointerAddress)type;
			if(!namer.pointerRegistered(pointer)) {
				std::pair<PointerAddress, std::string> pair (pointer, getName());
				namer.getTranslationTable().insert(pair);
			}
			setPointer(pointer);
			if(type->isVoidTy()) setFieldTrue("IsVoidType");
			if(type->isFunctionTy()) setFieldTrue("IsFunctionType"); 
			if(type->isFPOrFPVectorTy()) {
				setFieldTrue("IsFPOrFPVectorType");
				if(type->isFloatingPointTy()) setFieldTrue("IsFloatingPointType");
				if(type->isFloatTy()) setFieldTrue("IsFloatType");
				if(type->isDoubleTy()) setFieldTrue("IsDoubleType");
				if(type->isX86_FP80Ty()) setFieldTrue("IsX86FP80Type"); 
				if(type->isFP128Ty()) setFieldTrue("IsFP128Type");
				if(type->isPPC_FP128Ty()) setFieldTrue("IsPPCFP128Type");
				setField("FPMantissaWidth", type->getFPMantissaWidth());
			}
			if(type->isX86_MMXTy()) setFieldTrue("IsX86MMXType");
			if(type->isLabelTy()) setFieldTrue("IsLabelType");
			if(type->isMetadataTy()) setFieldTrue("IsMetadataType");
			if(type->isIntOrIntVectorTy()) {
				setFieldTrue("IsIntOrIntVectorType");
				if(type->isIntegerTy()) setFieldTrue("IsIntegerType");
			}
			if(type->isStructTy()) setFieldTrue("IsStructType"); 
			if(type->isArrayTy()) setFieldTrue("IsArrayType"); 
			if(type->isPointerTy()) setFieldTrue("IsPointerType"); 
			if(type->isVectorTy()) {
				setFieldTrue("IsVectorType"); 
				setField("ScalarSizeInBits", type->getScalarSizeInBits());
			}
			if(type->isEmptyTy()) setFieldTrue("IsEmptyType"); 
			if(type->isPrimitiveType()) {
				setFieldTrue("IsPrimitiveType");
				setField("PrimitiveSizeInBits", type->getPrimitiveSizeInBits());
			}
			if(type->isDerivedType()) setFieldTrue("IsDerivedType"); 
			if(type->isFirstClassType()) setFieldTrue("IsFirstClassType");
			if(type->isSingleValueType()) setFieldTrue("IsSingleValueType"); 
			if(type->isAggregateType()) setFieldTrue("IsAggregateType");
			if(type->isSized()) setFieldTrue("IsSized");
         unsigned typeCount = type->getNumContainedTypes();
			if(typeCount > 0) {
            MultifieldBuilder b0(typeCount);
				//openField("Subtypes");
            unsigned index = 1;
				for(Type::subtype_iterator i = type->subtype_begin(),
						e = type->subtype_end(); i != e; ++i, ++index) {
					Type* t = (*i);
               b0.setSlot(index, Route(t, namer));
				}
				//closeField();
			}
		}
};
class CLIPSFunctionTypeBuilder : public CLIPSTypeBuilder {
	public:
		CLIPSFunctionTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "FunctionType") : CLIPSTypeBuilder(nm, namer, lib, ty) { }
};
class CLIPSCompositeTypeBuilder : public CLIPSTypeBuilder {
	public:
		CLIPSCompositeTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "CompositeType") : CLIPSTypeBuilder(nm, namer, lib, ty) { }
};
class CLIPSSequentialTypeBuilder : public CLIPSCompositeTypeBuilder {
	public:
		CLIPSSequentialTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "SequentialType") : CLIPSCompositeTypeBuilder(nm, namer, lib, ty) { }
};
class CLIPSArrayTypeBuilder : public CLIPSSequentialTypeBuilder {
	public:
		CLIPSArrayTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "ArrayType") : CLIPSSequentialTypeBuilder(nm, namer, lib, ty) { }
};
class CLIPSPointerTypeBuilder : public CLIPSSequentialTypeBuilder {
	public:
		CLIPSPointerTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "PointerType") : CLIPSSequentialTypeBuilder(nm, namer, lib, ty) { }

};
class CLIPSVectorTypeBuilder : public CLIPSSequentialTypeBuilder {
	public:
		CLIPSVectorTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "VectorType") : CLIPSSequentialTypeBuilder(nm, namer, lib, ty) { }

};
class CLIPSStructTypeBuilder : public CLIPSCompositeTypeBuilder {
	public:
		CLIPSStructTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "StructType") : CLIPSCompositeTypeBuilder(nm, namer, lib, ty) { }
		void setFields(StructType* st, char* parent) 
		{
			CLIPSTypeBuilder::setFields(st);
			setParent(parent);
			setField("Name", st->getName());
		}
};

class CLIPSIntegerTypeBuilder : public CLIPSTypeBuilder {
	public:
		CLIPSIntegerTypeBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& lib, std::string ty = "IntegerType") : CLIPSTypeBuilder(nm, namer, lib, ty) { }
};
#endif
