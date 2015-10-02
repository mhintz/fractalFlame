#include "TransformFunction.h"

int randPlusMinus() {
	return ofRandomf() < 0.5f ? -1 : 1;
}

TransformFunction::TransformFunction(TransformParameters params) {
	float rotationAngle = params.maxRotAngle * ofRandom(-1, 1);
	float skewAngle = params.maxSkewAngle * ofRandom(-1, 1);
	float angle1 = rotationAngle + skewAngle;
	float angle2 = rotationAngle - skewAngle;

	float radius1 = randPlusMinus() * ofRandom(params.minScale, params.maxScale);
	float radius2 = randPlusMinus() * ofRandom(params.minScale, params.maxScale);

	xx = radius1 * cos(angle1);
	xy = -radius2 * sin(angle2);
	yy = radius1 * sin(angle1);
	yx = radius2 * cos(angle2);

	// Ensure matrix has reasonable scale
	float maxX = max(xx, xy);
	if (maxX > 1) {
		xx /= maxX;
		xy /= maxX;
		yy /= maxX;
		yx /= maxX;
	}

	float maxY = max(yy, yx);
	if (maxY > 1) {
		xx /= maxY;
		xy /= maxY;
		yy /= maxY;
		yx /= maxY;
	}

	// Transformation values
	xt = pow(ofRandomf(), 0.5f) * randPlusMinus() * (1 - abs(xx) - abs(xy));
	yt = pow(ofRandomf(), 0.5f) * randPlusMinus() * (1 - abs(yy) - abs(yx));

	// Generate the color here
}

TransformFunction::TransformFunction() :
probability(0.0f), xx(ofRandomf()), xy(ofRandomf()), xt(ofRandomf()), yx(ofRandomf()), yy(ofRandomf()), yt(ofRandomf()), color(ofFloatColor(ofRandomf(), ofRandomf(), ofRandomf())) {

}

TransformFunction::TransformFunction(float _probability, float _A, float _B, float _C, float _D, float _E, float _F, ofFloatColor _color) :
probability(_probability), xx(_A), xy(_B), xt(_C), yx(_D), yy(_E), yt(_F), color(_color) {

}

Sample TransformFunction::apply(const Sample & input) {
		ofVec2f returnPosition = ofVec2f(xx * input.position.x + xy * input.position.y + xt, yx * input.position.x + yy * input.position.y + yt);
		ofFloatColor returnColor = color;
		return Sample(returnPosition, returnColor);
}