#include "OperatorConstruction.h"

void CLIPSOperatorBuilder::addFields(Operator* v, KnowledgeConstruction* kc, char* parent) {
	CLIPSUserBuilder::addFields((User*)v, kc, parent);
}
void CLIPSOperatorBuilder::build(Operator* v, KnowledgeConstruction* kc, char* parent) {
	open();
	addFields(v,kc,parent);
	close();
	std::string str = getCompletedString();
	kc->addToKnowledgeBase((PointerAddress)v, str);
}
void CLIPSOverflowingBinaryOperatorBuilder::addFields(OverflowingBinaryOperator* v, KnowledgeConstruction* kc, char* parent) {
	CLIPSOperatorBuilder::addFields((Operator*)v, kc, parent);
	if(v->hasNoUnsignedWrap()) addTrueField("HasNoUnsignedWrap");
	if(v->hasNoSignedWrap()) addTrueField("HasNoSignedWrap");
}
void CLIPSOverflowingBinaryOperatorBuilder::build(OverflowingBinaryOperator* v, KnowledgeConstruction* kc, char* parent) {
	open();
	addFields(v,kc,parent);
	close();
	std::string str = getCompletedString();
	kc->addToKnowledgeBase((PointerAddress)v, str);
}

void CLIPSPossiblyExactOperatorBuilder::addFields(PossiblyExactOperator* v, KnowledgeConstruction* kc, char* parent) {
	CLIPSOperatorBuilder::addFields((Operator*)v, kc, parent);
	if(v->isExact()) addTrueField("IsExact");
}
void CLIPSPossiblyExactOperatorBuilder::build(PossiblyExactOperator* v, KnowledgeConstruction* kc, char* parent) {
	open();
	addFields(v,kc,parent);
	close();
	std::string str = getCompletedString();
	kc->addToKnowledgeBase((PointerAddress)v, str);
}
