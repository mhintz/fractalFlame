#include "ofApp.h"
#include "types.h"

int ofApp::sideLength = 700;
int ofApp::fieldStride = 4;
int ofApp::fieldLength = ofApp::fieldStride * ofApp::sideLength * ofApp::sideLength;
int ofApp::numSamples = 1000000;
int ofApp::ignoreSamples = 40;

void ofApp::setup(){
	ofBackground(0, 0, 0);

	resetField();
}

void ofApp::update(){

}

void ofApp::draw(){
//		 Draw the field
	flameImage.draw(0, 0);
}

void ofApp::keyPressed(int key){
	if (key == 'r') {
		cout << "reset" << endl;
		resetField();
	}
}

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

void ofApp::resetField() {
	TransformParameters params = TransformParameters();
	int numTransforms = round(ofRandom(2.f, 5.f));

	// Clear existing transforms
	transforms.clear();

	// Generate random transforms
	for (int tNum = 0; tNum < numTransforms; tNum++) {
		transforms.push_back(TransformFunction(params));
	}

	// TODO: Add rotations and conjugates here

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

// Chooses a transform from the list randomly, using a uniform distribution
Sample ofApp::applyRandomTransform(const Sample & input) {
	int randVal = (int) floor((double) ofRandom(0.0, transforms.size()));
	if (randVal == transforms.size()) {
		cout << "Had a value that was too high :(" << endl;
		randVal = randVal - 1;
	}
	return transforms.at(randVal).apply(input);
}

// Chooses a transform from the list using the weights in transformProbabilities
// Uses a simple iteration over a probability distribution to sample the transform
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

vector<float> getViewportCoordinates(const vector<TransformFunction> & tfmFuncs) {
	// Work out optimal window coordinates for the fractal
	float minX = INFINITY;
	float maxX = -INFINITY;
	float minY = INFINITY;
	float maxY = -INFINITY;

	for (vector<TransformFunction>::const_iterator outerTfm = tfmFuncs.begin(); outerTfm != tfmFuncs.end(); outerTfm++) {
		for (vector<TransformFunction>::const_iterator innerTfm = tfmFuncs.begin(); innerTfm != tfmFuncs.end(); innerTfm++) {
			// Compute composition
			float cXX =  outerTfm->xx * innerTfm->xx +  outerTfm->xy * innerTfm->yx;
			float cXY =  outerTfm->xx * innerTfm->xy +  outerTfm->xy * innerTfm->yy;
			float cYX =  outerTfm->yx * innerTfm->xx +  outerTfm->yy * innerTfm->yx;
			float cYY =  outerTfm->yx * innerTfm->xy +  outerTfm->yy * innerTfm->yy;
			float cTX = outerTfm->xx * innerTfm->xt + outerTfm->xy * innerTfm->yt + outerTfm->xt;
			float cTY = outerTfm->yx * innerTfm->xt + outerTfm->yy * innerTfm->yt + outerTfm->yt;

			minX = min(minX, -abs(cXX) - abs(cXY) + cTX);
			maxX = max(maxX, abs(cXX) + abs(cXY) + cTX);
			minY = min(minY, -abs(cYX) - abs(cYY) + cTY);
			maxY = max(maxY, abs(cYX) + abs(cYY) + cTY);
		}
	}

	// Generate positions for the min and max x/y values of the viewport which preserve a 1:1 aspect ratio
	float dWinX = maxX - minX;
	float dWinY = maxY - minY;
	if (dWinX < dWinY) {
		maxX += 0.5f * (dWinY - dWinX);
		minX -= 0.5f * (dWinY - dWinX);
	} else if (dWinX > dWinY) {
		maxY += 0.5f * (dWinX - dWinY);
		minY -= 0.5f * (dWinX - dWinY);
	}

	// Add a little margin
	float margin = dWinX * 0.03f;
	minX -= margin;
	maxX += margin;
	minY -= margin;
	maxY += margin;

	// Probably won't happen, but worth checking
	if (minX == maxX) {
		minX = -1.f;
		maxX = 1.f;
	}
	if (minY == maxY) {
		minY = -1.f;
		maxY = 1.f;
	}

	// Prep the return value
	vector<float> retVal;
	retVal.push_back(minX);
	retVal.push_back(maxX);
	retVal.push_back(minY);
	retVal.push_back(maxY);

	return retVal;
}

void ofApp::generateField() {
	// Allocate the image with empty components
	flameImage.clear();
	flameImage.allocate(ofApp::sideLength, ofApp::sideLength, OF_IMAGE_COLOR_ALPHA);

	// Get the pointer to the raw pixels array;
	float * pixels = flameImage.getPixels();

	// Start sampling at a random place
	Sample iterationSample(ofVec2f(ofRandom(-1.f, 1.f), ofRandom(-1.f, 1.f)));

	// Get viewport coordinates for this flame based on the transforms array
	vector<float> viewportCoords = getViewportCoordinates(transforms);

	// The base and slope in each dimension are used to find the window position of a given
	// x/y sample location. This allows the viewport to be lopsided while preserving the
	// square display.
	float xBase = viewportCoords.at(0);
	float xSlope = ofApp::sideLength / (viewportCoords.at(1) - viewportCoords.at(0));
	float yBase = viewportCoords.at(2);
	float ySlope = ofApp::sideLength / (viewportCoords.at(3) - viewportCoords.at(2));

	// Keep track of the maximum pixel hits, used for log-normalizing colors later
	int maxPixelHits = 0;

	// Run all the transform samples
	for (int sampleNum = 0; sampleNum < ofApp::numSamples; ++sampleNum) {
		// Chooses a transform function randomly and applies it
		iterationSample = applyRandomTransform(iterationSample);

		// Do the pixel addition and computation after a certain number of stabilizing samples
		if (sampleNum > ofApp::ignoreSamples) {
			int xPos = round(xSlope * (iterationSample.position.x - xBase));
			int yPos = round(ySlope * (iterationSample.position.y - yBase));
			int fieldIndex = (yPos * ofApp::sideLength + xPos) * ofApp::fieldStride;

			// Only do pixel manipulation if the index falls within the image bounds
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

float ofApp::scaleLocusAlpha(float alpha) {
	static float gammaValue = 0.3;
	static float oneOverLN2 = 1.f / log(2.f);
	return pow(log(1 + alpha) * oneOverLN2, gammaValue);
}