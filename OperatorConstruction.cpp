#include "OperatorConstruction.h"

void CLIPSOperatorBuilder::addFields(Operator* v, char* parent) {
	CLIPSUserBuilder::setFields((User*)v, parent);
}
void CLIPSOverflowingBinaryOperatorBuilder::addFields(OverflowingBinaryOperator* v, char* parent) {
	CLIPSOperatorBuilder::addFields((Operator*)v, parent);
	if(v->hasNoUnsignedWrap()) addTrueField("HasNoUnsignedWrap");
	if(v->hasNoSignedWrap()) addTrueField("HasNoSignedWrap");
}

void CLIPSPossiblyExactOperatorBuilder::addFields(PossiblyExactOperator* v, char* parent) {
	CLIPSOperatorBuilder::addFields((Operator*)v, parent);
	if(v->isExact()) addTrueField("IsExact");
}
