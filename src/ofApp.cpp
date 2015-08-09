#include "ofApp.h"
#include "variations.h"

int ofApp::sideLength = 700;

//--------------------------------------------------------------
void ofApp::setup(){
		// Initialize the field with empty components
		field.resize(ofApp::sideLength * ofApp::sideLength);

		position = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1));

		std::cout << "Position: " << position << " - " << getCoordinates(position).first << ", " << getCoordinates(position).second << std::endl;

		setupField(position);

		thePoints.setMode(OF_PRIMITIVE_POINTS);
}

void ofApp::setupField(ofVec3f startPos) {
		for (int sampleNum = 0; sampleNum < numSamples; ++sampleNum) {
				position = runVariation(runFunc(position));
				if (sampleNum > 20) {
						std::pair<int, int> indexes = getCoordinates(position);
						// Note to self: this has to be a reference, or else assigning to its count will mutate a copy of it
						Locus & theLocus = field.at(indexes.first * sideLength + indexes.second);
						theLocus.count++;
				}
		}

		thePoints.clear();
		for (LocusVec::size_type iter = 0; iter != field.size(); ++iter) {
				Locus * theLocus = & field[iter];
				thePoints.addVertex(getPosFromIndex(iter));
				thePoints.addColor(ofColor(255.0f, getLocusAlpha(* theLocus)));
		}
}

float normalizePos(float val) {
		return (val + 1) / 2;
}

std::pair<int, int> ofApp::getCoordinates(ofVec3f position) {
		int xPos = (int) (normalizePos(position.x) * ofApp::sideLength);
		int yPos = (int) (normalizePos(position.y) * ofApp::sideLength);
		return std::pair<int, int>(xPos, yPos);
}

ofVec3f ofApp::getPosFromIndex(int index) {
		// Note to self: the floor operation might not be necessary, since the numerator and
		// denominator of the expression are both ints, but it's good to have it in there anyway,
		// for clarity.
		return ofVec3f(floor(index / ofApp::sideLength), index % ofApp::sideLength);
}

float ofApp::getLocusAlpha(const Locus & theLocus) {
		return log(theLocus.count) / 3.5f * 255.0f;
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
