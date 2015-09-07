#include "ofApp.h"

int ofApp::sideLength = 700;
int ofApp::fieldStride = 4;
int ofApp::fieldLength = ofApp::fieldStride * ofApp::sideLength * ofApp::sideLength;

void generateProbDist(int numValues, float * storage) {
	int count;
	float sum = 0.0f;

	for (count = 0; count < numValues; count++) {
		float randVal = ofRandomf();
		storage[count] = randVal;
		sum += randVal;
	}

	// Smooth to averages so they sum to 1
	for (count = 0; count < numValues; count++) {
		storage[count] /= sum;
	}
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0, 0, 0);

	resetField();
}

void ofApp::resetField() {
	// Allocate the image with empty components
	flameImage.clear();
	flameImage.allocate(ofApp::sideLength, ofApp::sideLength, OF_IMAGE_COLOR_ALPHA);

	// Initialize the transforms with random parameters
	transforms.clear();
	transforms.resize(numTransforms);

	// Randomly generate the transform's probabilities according to a random distribution that sums to 1
	float probabilities[numTransforms];
	generateProbDist(numTransforms, probabilities);
	for (int tNum = 0; tNum < numTransforms; tNum++) {
		transforms[tNum] = TransformFunction();
		transforms[tNum].probability = probabilities[tNum];
	}

	// Creates a cumulative probability distribution
	transformProbabilities.resize(numTransforms);
	float probSum = 0.0f;
	for (int tNum = 0; tNum < numTransforms; tNum++) {
		probSum += transforms[tNum].probability;
		transformProbabilities[tNum] = probSum;
	}

	// Setup the field, starting from a random place
	generateField(ofVec2f(ofRandomf(), ofRandomf()));
}

Sample ofApp::applyRandomTransform(const Sample & input) {
	float randVal = ofRandomf();
	for (int tNum = 0; tNum < numTransforms; ++tNum) {
		if (randVal <= transformProbabilities[tNum]) {
			return transforms[tNum].apply(input);
		}
	}

	// If the probabilities don't sum to 1 and you ended up higher than the max,
	// just return the input.
	return input;
}

void ofApp::generateField(ofVec2f position) {
	Sample iterationSample(position);

	float * pixels = flameImage.getPixels();

	for (int sampleNum = 0; sampleNum < numSamples; ++sampleNum) {
		iterationSample = applyRandomTransform(iterationSample);
		if (sampleNum > 20) {
			int fieldIndex = getFieldIndex(iterationSample.position);
			if (fieldIndex >= 0 && fieldIndex < ofApp::fieldLength) {
				pixels[fieldIndex] = (pixels[fieldIndex] + iterationSample.color.r) / 2;
				pixels[fieldIndex + 1] = (pixels[fieldIndex + 1] + iterationSample.color.g) / 2;
				pixels[fieldIndex + 2] = (pixels[fieldIndex + 2] + iterationSample.color.b) / 2;
				pixels[fieldIndex + 3]++;
			}
		}
	}

	for (int iter = 0; iter < ofApp::fieldLength; iter += ofApp::fieldStride) {
		pixels[iter + 3] = scaleLocusAlpha(pixels[iter + 3]);
	}

	flameImage.update();
}

float normalizePos(float val) {
	return ofClamp((val + 1) / 2, 0.0f, 1.0f);
}

int ofApp::getFieldIndex(const ofVec2f & position) {
	float yPos = position.y * ofApp::sideLength;
	float xPos = position.x * ofApp::sideLength;
	int index = floor(yPos) * ofApp::sideLength + floor(xPos);
	return index * ofApp::fieldStride;
}

ofVec2f ofApp::getPosFromIndex(int index) {
	float x = index / ofApp::sideLength;
	float y = index % ofApp::sideLength;
	return ofVec2f(x, y);
}

float ofApp::scaleLocusAlpha(float alpha) {
		return log(alpha) / 3.5f;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
//		 Draw the field
	flameImage.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r') {
		cout << "reset" << endl;
		resetField();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
