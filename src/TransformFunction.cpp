#include "TransformFunction.h"

TransformFunction::TransformFunction() :
probability(0.0f), pA(ofRandomf()), pB(ofRandomf()), pC(ofRandomf()), pD(ofRandomf()), pE(ofRandomf()), pF(ofRandomf()), color(ofFloatColor(ofRandomf(), ofRandomf(), ofRandomf())) {

}

TransformFunction::TransformFunction(float _probability, float _A, float _B, float _C, float _D, float _E, float _F, ofFloatColor _color) :
probability(_probability), pA(_A), pB(_B), pC(_C), pD(_D), pE(_E), pF(_F), color(_color) {

};

Sample TransformFunction::apply(const Sample & input) {
		ofVec2f returnPosition = ofVec2f(pA * input.position.x + pB * input.position.y + pC, pD * input.position.x + pE * input.position.y + pF);
		ofFloatColor returnColor = color;
		return Sample(returnPosition, returnColor);
}