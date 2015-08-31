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
		thePoints.setMode(OF_PRIMITIVE_POINTS);

		resetField();
}

void ofApp::resetField() {
	// Initialize the field with empty components
	delete[] field;
	field = new float[ofApp::fieldLength]();

	// Initialize the transforms with random parameters
	delete[] transforms;
	transforms = new TransformFunction[numTransforms]();

	// Randomly generate the transform's probabilities according to a random distribution that sums to 1
	float probabilities[numTransforms];
	generateProbDist(numTransforms, probabilities);
	for (int tNum = 0; tNum < numTransforms; tNum++) {
		transforms[tNum].probability = probabilities[tNum];
	}

	// For the Sierpinski example, set parameters and probabilities by hand
	// transforms[0] = TransformFunction(0.33f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, ofFloatColor(0.0f, 0.5f, 0.75f));
	// transforms[1] = TransformFunction(0.33f, 0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.0f, ofFloatColor(0.5f, 0.1f, 0.05f));
	// transforms[2] = TransformFunction(0.34f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, ofFloatColor(0.5f, 0.05f, 0.8f));

	// Creates a cumulative probability distribution
	delete[] transformProbabilities;
	transformProbabilities = new float[numTransforms]();
	float probSum = 0.0f;
	for (int tNum = 0; tNum < numTransforms; tNum++) {
		probSum += transforms[tNum].probability;
		transformProbabilities[tNum] = probSum;
	}

	// Setup the field, starting from a random place
	generateField(ofVec2f(ofRandomf(), ofRandomf()), ofFloatColor(ofRandomf(), ofRandomf(), ofRandomf()));
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

void ofApp::generateField(ofVec2f position, ofFloatColor color) {
		Sample iterationSample(position, color);

		for (int sampleNum = 0; sampleNum < numSamples; ++sampleNum) {
				iterationSample = applyRandomTransform(iterationSample);
				if (sampleNum > 20) {
						ofVec2f indexes = getCoordinates(iterationSample.position);
						int fieldIndex = indexes.x * sideLength + indexes.y;
						field[fieldIndex] = (field[fieldIndex] + iterationSample.color.r) / 2;
						field[fieldIndex + 1] = (field[fieldIndex + 1] + iterationSample.color.g) / 2;
						field[fieldIndex + 2] = (field[fieldIndex + 2] + iterationSample.color.b) / 2;
						field[fieldIndex + 3]++;
				}
		}

		thePoints.clear();
		for (int iter = 0; iter != ofApp::fieldLength; iter += ofApp::fieldStride) {
				thePoints.addVertex(getPosFromIndex(iter));
				float count = field[iter + 3];
				int grayScale = count > 0 ? 1 : 0;
				float rVal = field[iter];
				float gVal = field[iter + 1];
				float bVal = field[iter + 2];
				float alphaVal = scaleLocusAlpha(count);
				// thePoints.addColor(ofColor(255, 255, 255));
				// thePoints.addColor(ofFloatColor(alphaVal, alphaVal, alphaVal));
				thePoints.addColor(ofFloatColor(rVal, gVal, bVal, alphaVal));
		}
}

float normalizePos(float val) {
		return ofClamp((val + 1) / 2, 0.0f, 1.0f);
}

ofVec2f ofApp::getCoordinates(const ofVec2f & position) {
		int xPos = (int) (normalizePos(position.x) * ofApp::sideLength);
		int yPos = (int) (normalizePos(position.y) * ofApp::sideLength);
		return ofVec2f(xPos, yPos);
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
		ofBackground(8);

		thePoints.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r') {
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
