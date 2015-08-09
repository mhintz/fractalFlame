#include "ofApp.h"

int ofApp::sideLength = 700;
int ofApp::fieldStride = 4;
int ofApp::fieldLength = ofApp::fieldStride * ofApp::sideLength * ofApp::sideLength;

//--------------------------------------------------------------
void ofApp::setup(){
		// Initialize the field with empty components
		field = new float[ofApp::fieldLength]();

		position = ofVec2f(ofRandomf(), ofRandomf());
		color = ofVec3f(ofRandomf(), ofRandomf(), ofRandomf());


		setupField(position, color);

		thePoints.setMode(OF_PRIMITIVE_POINTS);
}

void ofApp::setupField(ofVec2f position, ofVec3f color) {
		for (int sampleNum = 0; sampleNum < numSamples; ++sampleNum) {
				Sample theSample = transformer.applyTransform(position);
				position = theSample.position;
				if (sampleNum > 20) {
						ofVec2f indexes = getCoordinates(position);
						int fieldIndex = indexes.x * sideLength + indexes.y;
						field[fieldIndex] += theSample.color.r;
						field[fieldIndex + 1] += theSample.color.g;
						field[fieldIndex + 2] += theSample.color.b;
						field[fieldIndex + 3]++;
				}
		}

		thePoints.clear();
		for (int iter = 0; iter != ofApp::fieldLength; iter += ofApp::fieldStride) {
				thePoints.addVertex(getPosFromIndex(iter));
				float count = field[iter + 3];
				float rVal = field[iter] / count * 255.0f;
				float gVal = field[iter + 1] / count * 255.0f;
				float bVal = field[iter + 2] / count * 255.0f;
				float alphaVal = scaleLocusAlpha(count);
				thePoints.addColor(ofColor(rVal, gVal, bVal, alphaVal));
		}
}

float normalizePos(float val) {
		return (val + 1) / 2;
}

ofVec2f ofApp::getCoordinates(const ofVec2f & position) {
		int xPos = (int) (normalizePos(position.x) * ofApp::sideLength);
		int yPos = (int) (normalizePos(position.y) * ofApp::sideLength);
		return ofVec2f(xPos, yPos);
}

ofVec2f ofApp::getPosFromIndex(int index) {
		// Note to self: the floor operation might not be necessary, since the numerator and
		// denominator of the expression are both ints, but it's good to have it in there anyway,
		// for clarity.
		return ofVec2f(floor(index / ofApp::sideLength), index % ofApp::sideLength);
}

float ofApp::scaleLocusAlpha(float alpha) {
		return log(alpha) / 3.5f * 255.0f;
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
