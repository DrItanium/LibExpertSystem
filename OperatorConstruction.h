
#ifndef _operator_construction_h
#define _operator_construction_h
#include "ConstructionTools.h"
class CLIPSOperatorBuilder : public CLIPSUserBuilder {
   public:
      CLIPSOperatorBuilder(std::string nm, std::string ty, FunctionNamer& namer) : CLIPSUserBuilder(nm, ty, namer) { }
      void addFields(Operator* v, KnowledgeConstruction* kc, char* parent);
		void build(Operator* v, KnowledgeConstruction* kc, char* parent);
};
class CLIPSOverflowingBinaryOperatorBuilder : public CLIPSOperatorBuilder {
   public:
      CLIPSOverflowingBinaryOperatorBuilder(std::string nm, FunctionNamer& namer) : CLIPSOperatorBuilder(nm, "OverflowingBinaryOperator", namer) { }
      void addFields(OverflowingBinaryOperator* v, KnowledgeConstruction* kc, char* parent);
		void build(OverflowingBinaryOperator* v, KnowledgeConstruction* kc, char* parent);
};
/* for some reason this doesn't exist and as such I have commented it out
 * - Joshua Scoggins 5/20/2012 @ 1909
 *
class CLIPSFPMathOperatorBuilder : public CLIPSOperatorBuilder
{
   public:
      CLIPSFPMathOperatorBuilder(std::string nm, FunctionNamer& namer) : CLIPSOperatorBuilder(nm, "FPMathOperator", namer) { }
      void addFields(FPMathOperator* v, char* parent)
      {
         CLIPSOperatorBuilder::addFields((Operator*)v, parent);
         addField("FPAccuracy", v->getFPAccuracy());
      }
};
*/
class CLIPSPossiblyExactOperatorBuilder : public CLIPSOperatorBuilder {
   public:
      CLIPSPossiblyExactOperatorBuilder(std::string nm, FunctionNamer& namer) : CLIPSOperatorBuilder(nm, "PossiblyExactOperator", namer) { }
      void addFields(PossiblyExactOperator* v, KnowledgeConstruction* kc, char* parent);
		void build(PossiblyExactOperator* v, KnowledgeConstruction* kc, char* parent);
};

#endif
