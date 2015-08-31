#pragma once

#include "ofMain.h"
#include "TransformFunction.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	static int sideLength;
	static int fieldStride;
	static int fieldLength;

	const int numSamples = 600000;
	const int numTransforms = 3;

	vector<float> field;
	vector<TransformFunction> transforms;
	vector<float> transformProbabilities;

	ofVec2f iterPosition;
	ofVec3f color;

	ofTexture imageTex;

	// Setting up the field of samples
	Sample applyRandomTransform(const Sample &);
	void resetField();
	void generateField(ofVec2f, ofFloatColor);
	ofVec2f getCoordinates(const ofVec2f &);
	ofVec2f getPosFromIndex(int);
	float scaleLocusAlpha(float);
};
