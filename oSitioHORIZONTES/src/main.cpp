//#__________________________________#//
//#                                  #//
//#    ZappaKinTrackerALPHA 0.1      #//
//#    ########################      #//
//#    25/07/12 out of time day      #//
//#    Kaue Costa   www.kaox.tv      #//
//#----------------------------------#//


#include "ofApp.h"
#include "ofMain.h"
#include "ofAppGlutWindow.h"


//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1920, 1080, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
