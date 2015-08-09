#include "variations.h"

ofVec2f Sierp1::transform(const ofVec2f & pos) {
		return ofVec2f(pos.x / 2, pos.y / 2);
}

ofVec2f Sierp2::transform(const ofVec2f & pos) {
		return ofVec2f((pos.x + 1) / 2, pos.y / 2);
}

ofVec2f Sierp3::transform(const ofVec2f & pos) {
		return ofVec2f(pos.x / 2, (pos.y + 1) / 2);
}

Sample Transformer::applyTransform(const ofVec2f & pos) {
		int index = (int) ofRandom(3);
		Sample theSample;

		switch (index) {
				case 0:
						theSample.position = ofVec2f(t0.transform(pos));
						theSample.color = t0.transformColor;
						break;
				case 1:
						theSample.position = ofVec2f(t1.transform(pos));
						theSample.color = t1.transformColor;
						break;
				case 2:
						theSample.position = ofVec2f(t2.transform(pos));
						theSample.color = t2.transformColor;
						break;
				default:
						throw std::logic_error("Invalid function index requested");
		}

		theSample.position = runVariation(theSample.position);

		return theSample;
}

ofVec2f variationSinusoidal(const ofVec2f & pos) {
		return ofVec2f(sin(pos.x), sin(pos.y));
}

ofVec2f runVariation(const ofVec2f & pos) {
		return variationSinusoidal(pos);
}