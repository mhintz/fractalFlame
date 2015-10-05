#pragma once

#include "ofMain.h"
#include "TransformFunction.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

	// Not Implemented
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	static int sideLength;
	static int fieldStride;
	static int fieldLength;
	static int numSamples;
	static int ignoreSamples;

	vector<TransformFunction> transforms;
	vector<float> transformProbabilities;

	ofVec2f iterPosition;
	ofVec3f color;

	ofFloatImage flameImage;

	// Setting up the field of samples
	Sample applyRandomTransform(const Sample &);
	Sample applyWeightedTransform(const Sample &);
	void resetField();
	void generateField();
	int getFieldIndex(const ofVec2f &);
	float scaleLocusAlpha(float);
};
