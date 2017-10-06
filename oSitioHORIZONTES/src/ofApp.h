#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"
#include "ofxXmlSettings.h"
#include <deque>


///kaox.tv libs
#include "kaoxControlPanel.h"
#include "kaoxGameEngine.h"


#include "metaBlob.h"


#define HOST "localhost"
#define PORT 5552

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
    
    ofFbo       maskFbo1, maskFbo2, maskFbo3, maskFbo4, maskFbo5, maskFbo6;
    ofFbo       fbo1, fbo2, fbo3, fbo4, fbo5, fbo6;
    
    ofShader    shader;
    
    
    ofImage layer0;
    ofImage layer1;
    ofImage layer2;
    ofImage layer3;
    ofImage layer4;
    ofImage layer5;
    ofImage layer6;
    
    
    int numBlobs = 12;
    
    vector<Blob> b;
    
    
    vector<ofPoint> sp;
    
    
    
    ofPixels metaPixels;
    
    kaoxGameEngine ge;
};

#endif
