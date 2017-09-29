#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"
#include "kaoxControlPanel.h"
#include "ofxXmlSettings.h"
#include "ofxKinect.h"
#include "ofxOpenCV.h"
#include <deque>

#include "metaBlob.h"


#define HOST "localhost"
#define PORT 5552

//UI-Editor
#define MOUSE_STAT_DEFAULT 0
#define MOUSE_STAT_HOVER 1


#define PHASE_POINT 0
#define PHASE_RECT 1
#define PHASE_TRIANGLE 2
#define PHASE_STATIC_POLY 3
#define PHASE_DYNAMIC_POLY 4
#define PHASE_EMITTER 5
#define PHASE_FORCE 6

#define MOVEVIEW_FRAME 90


class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
    
    
    //UI / Eventos
    void setb(string name, bool value);
	void seti(string name, int value);
	void setf(string name, float value);
	bool getb(string name);
	int geti(string name);
	float getf(string name);
    kaoxControlPanel panel;
    
    void setupControlPanel();

    //OSC
	void receiveOSC();
	
	
    
    ///new SHADERS!!!
    void setupShaders();
    void updateShaders();
	
		

	//EVENTOS
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);


	
	bool				isKinectOn, isSyphonOn, isEditorOff;

	bool				isSendingOSC, isSendingHands, isSendingSkeleton, isDebugging; //kaox
		
    //  Kinect...
    ofxKinect kinect;
	
//÷÷÷÷÷÷÷  Syphon:

	float 	counter;
	bool	bSmooth;
	
    ofTexture tex;
    
	ofxSyphonServer mainOutputSyphonServer;
	ofxSyphonServer individualTextureSyphonServer;

    //OSC
    ofxOscSender   sender;
	ofxOscReceiver receiver;
    
    ofImage mask;

    ofVideoPlayer videoCor, videoPB;
    
    
    ///shaders
    
    ofImage     srcImg;
    ofImage     dstImg;
    ofImage     brushImg;
    
    ofFbo       maskFbo1, maskFbo2, maskFbo3;
    ofFbo       fbo1, fbo2, fbo3;
    
    ofShader    shader;
    
    ofImage layer1;
    ofImage layer2;
    ofImage layer3;
    
    int numBlobs = 12;
    
    vector<Blob> b;
    
    
    vector<ofPoint> sp;
    
    
    
    ofPixels metaPixels;
    
    //
    //  [Brizo] Inspired by ofxKinect's KinectExample
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
};

#endif
