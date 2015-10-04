#include "TransformFunction.h"

int randPlusMinus() {
	return ofRandom(0.f, 1.f) < 0.5f ? -1 : 1;
}

TransformFunction::TransformFunction(const TransformParameters & params) {
	float rotationAngle = params.maxRotAngle * ofRandom(-1.f, 1.f);
	float skewAngle = params.maxSkewAngle * (1.f - 2 * pow(ofRandom(0.f, 1.f), 3.f));
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
	xt = pow(ofRandom(0.f, 1.f), 0.5f) * randPlusMinus() * (1.f - abs(xx) - abs(xy));
	yt = pow(ofRandom(0.f, 1.f), 0.5f) * randPlusMinus() * (1.f - abs(yy) - abs(yx));

	// Generate the color here
	color = ofFloatColor(ofRandom(params.colorMin, params.colorMax), ofRandom(params.colorMin, params.colorMax), ofRandom(params.colorMin, params.colorMax));

	float maxComponent = max(color.r, max(color.g, color.b));

	// Brigten the colors
	color.r = color.r / maxComponent;
	color.g = color.g / maxComponent;
	color.b = color.b / maxComponent;
}

TransformFunction::TransformFunction() :
xx(ofRandom(-1.f, 1.f)), xy(ofRandom(-1.f, 1.f)), xt(ofRandom(-1.f, 1.f)), yx(ofRandom(-1.f, 1.f)), yy(ofRandom(-1.f, 1.f)), yt(ofRandom(-1.f, 1.f)), color(ofFloatColor(ofRandom(0.f, 1.f), ofRandom(0.f, 1.f), ofRandom(0.f, 1.f))) {

}

TransformFunction::TransformFunction(float _A, float _B, float _C, float _D, float _E, float _F, ofFloatColor _color) :
xx(_A), xy(_B), xt(_C), yx(_D), yy(_E), yt(_F), color(_color) {

}

Sample TransformFunction::apply(const Sample & input) {
		ofVec2f returnPosition = ofVec2f(xx * input.position.x + xy * input.position.y + xt, yx * input.position.x + yy * input.position.y + yt);
		ofFloatColor returnColor = color;
		return Sample(returnPosition, returnColor);
}