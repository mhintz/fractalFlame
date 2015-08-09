#pragma once

#include <vector>

#include "ofMain.h"

struct Locus {
		int count;

		Locus() {
				count = 0;
		}
};

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
		int numSamples = 600000;

		typedef std::vector<Locus> LocusVec;
		std::vector<Locus> field;
		ofVec3f position;
		ofMesh thePoints = ofMesh();

		// Setting up the field of samples
		void setupField(ofVec3f);
		std::pair<int, int> getCoordinates(ofVec3f);
		ofVec3f getPosFromIndex(int);
		float getLocusAlpha(const Locus &);
};
