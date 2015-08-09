//
//  Header.h
//  fractalFlame
//
//  Created by Mark Hintz on 9/8/15.
//
//

#pragma once

ofVec3f func0(const ofVec3f & pos) {
		return ofVec3f(pos.x / 2, pos.y / 2);
}

ofVec3f func1(const ofVec3f & pos) {
		return ofVec3f((pos.x + 1) / 2, pos.y / 2);
}

ofVec3f func2(const ofVec3f & pos) {
		return ofVec3f(pos.x / 2, (pos.y + 1) / 2);
}

ofVec3f runFunc(ofVec3f pos) {
		int index = (int) ofRandom(3);
		switch (index) {
				case 0:
						return func0(pos);
				case 1:
						return func1(pos);
				case 2:
						return func2(pos);
				default:
						throw std::logic_error("Invalid function index requested");
		}
}

ofVec3f variationSinusoidal(ofVec3f pos) {
		return ofVec3f(sin(pos.x), sin(pos.y));
}

ofVec3f runVariation(ofVec3f pos) {
		return variationSinusoidal(pos);
}