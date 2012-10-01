
#ifndef _operator_construction_h
#define _operator_construction_h
#include "ConstructionTools.h"
class CLIPSOperatorBuilder : public CLIPSUserBuilder {
   public:
      CLIPSOperatorBuilder(std::string nm, std::string ty, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSUserBuilder(nm, ty, namer, tl) { }
      void addFields(Operator* v, char* parent);
};
class CLIPSOverflowingBinaryOperatorBuilder : public CLIPSOperatorBuilder {
   public:
      CLIPSOverflowingBinaryOperatorBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSOperatorBuilder(nm, "OverflowingBinaryOperator", namer, tl) { }
      void addFields(OverflowingBinaryOperator* v, char* parent);
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
      CLIPSPossiblyExactOperatorBuilder(std::string nm, FunctionNamer& namer, TypeLibrarian& tl) : CLIPSOperatorBuilder(nm, "PossiblyExactOperator", namer, tl) { }
      void addFields(PossiblyExactOperator* v, char* parent);
};

#endif
