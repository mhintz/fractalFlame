#include "ofApp.h"
#include "types.h"

int ofApp::sideLength = 700;
int ofApp::fieldStride = 4;
int ofApp::fieldLength = ofApp::fieldStride * ofApp::sideLength * ofApp::sideLength;
int ofApp::numSamples = 1000000;
int ofApp::ignoreSamples = 20;

vector<float> generateProbDist(int numValues) {
	int count;
	float sum = 0.0f;
	vector<float> storage;

	storage.resize(numValues);

	for (count = 0; count < numValues; count++) {
		float randVal = ofRandom(0.f, 1.f);
		storage.at(count) = randVal;
		sum += randVal;
	}

	// Smooth to averages so they sum to 1
	for (count = 0; count < numValues; count++) {
		storage.at(count) /= sum;
	}

	return storage;
}

float normalizePos(float val) {
	return ofClamp((val + 1) / 2, 0.0f, 1.0f);
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0, 0, 0);

	resetField();
}

void ofApp::resetField() {
	TransformParameters params = TransformParameters();
	int numTransforms = round(ofRandom(2.f, 5.f));

	// Clear existing transforms
	transforms.clear();

	// Generate random transforms
	for (int tNum = 0; tNum < numTransforms; tNum++) {
		transforms.push_back(TransformFunction(params));
	}

	// Add rotations and conjugates here

	// Clear existing probabilities
	transformProbabilities.clear();

	// Randomly generate the transform's probabilities according to a random distribution that sums to 1
	vector<float> probabilities = generateProbDist(transforms.size());


	// Creates a cumulative probability distribution
	float probSum = 0.0f;
	for (int tNum = 0; tNum < transforms.size(); tNum++) {
		probSum += probabilities.at(tNum);
		transformProbabilities.push_back(probSum);
	}

	// Setup the field, starting from a random place
	generateField();
}

Sample ofApp::applyRandomTransform(const Sample & input) {
	int randVal = (int) floor((double) ofRandom(0.0, transforms.size()));
	if (randVal == transforms.size()) {
		cout << "Had a value that was too high :(" << endl;
		randVal = randVal - 1;
	}
	return transforms.at(randVal).apply(input);
}

Sample ofApp::applyWeightedTransform(const Sample & input) {
	float randVal = ofRandom(0.f, 1.f);
	for (int tNum = 0; tNum < transformProbabilities.size(); ++tNum) {
		if (randVal <= transformProbabilities.at(tNum)) {
			return transforms.at(tNum).apply(input);
		}
	}

	// If the probabilities don't sum to 1 and you ended up higher than the max,
	// just return the input.
	return input;
}

void ofApp::generateField() {
	// Allocate the image with empty components
	flameImage.clear();
	flameImage.allocate(ofApp::sideLength, ofApp::sideLength, OF_IMAGE_COLOR_ALPHA);

	// Start sampling at a random place
	Sample iterationSample(ofVec2f(ofRandom(-1.f, 1.f), ofRandom(-1.f, 1.f)));

	float * pixels = flameImage.getPixels();

	int maxPixelHits = 0;

	for (int sampleNum = 0; sampleNum < ofApp::numSamples; ++sampleNum) {
		iterationSample = applyRandomTransform(iterationSample);
		if (sampleNum > ofApp::ignoreSamples) {
			int fieldIndex = getFieldIndex(iterationSample.position);
			if (fieldIndex >= 0 && fieldIndex < ofApp::fieldLength) {
				int pixCount = pixels[fieldIndex + 3];
				int newPixCount = pixCount + 1;
				if (newPixCount > maxPixelHits) {
					maxPixelHits = newPixCount;
				}

				float avgFactor = 1.f / float(newPixCount);
				pixels[fieldIndex] = (pixels[fieldIndex] * pixCount + iterationSample.color.r) * avgFactor;
				pixels[fieldIndex + 1] = (pixels[fieldIndex + 1] * pixCount + iterationSample.color.g) * avgFactor;
				pixels[fieldIndex + 2] = (pixels[fieldIndex + 2] * pixCount + iterationSample.color.b) * avgFactor;
				pixels[fieldIndex + 3] = newPixCount;
			}
		}
	}

	for (int iter = 0; iter < ofApp::fieldLength; iter += ofApp::fieldStride) {
		if (pixels[iter + 3] != 0) {
			pixels[iter + 3] = scaleLocusAlpha(pixels[iter + 3] / maxPixelHits);
		}
	}

	flameImage.update();
}

int ofApp::getFieldIndex(const ofVec2f & position) {
	float yPos = normalizePos(position.y) * ofApp::sideLength;
	float xPos = normalizePos(position.x) * ofApp::sideLength;
	int index = floor(yPos) * ofApp::sideLength + floor(xPos);
	return index * ofApp::fieldStride;
}

float ofApp::scaleLocusAlpha(float alpha) {
	static float gammaValue = 0.3;
	static float oneOverLN2 = 1.f / log(2.f);
	return pow(log(1 + alpha) * oneOverLN2, gammaValue);
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
