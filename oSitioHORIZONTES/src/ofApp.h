#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"
#include "ofxXmlSettings.h"
#include "ofxKinect.h"
#include "ofxOpenCV.h"
#include <deque>


///kaox.tv libs
#include "kaoxControlPanel.h"
#include "kaoxGameEngine.h"


#include "metaBlob.h"

#include "Boid.h"


const string HOST = "localhost";
const int PORT_RECEIVE = 5552;
const int PORT_SEND_ARDOUR = 3819; //  Porta padrao de envio de OSC para o Ardour
const int PORT_RECEIVE_FEEDBACK_ARDOUR = 8000; //  Porta padrao de recebimento de OSC de feedback

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
    
    
    
    void updateKinect();
    
    
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
	void receiveOSC1();
    void receiveOSC_Ardour();
    void sendOSC_Ardour();
    int current_msg_string;
    string msg_strings[20];
    float timers[20];
    
    //  Ardour current state, updated via OSC
    struct {
        int bar = 0;
        int beat = 0;
        float meter = -40.0f;
    } Ardour_state;
    
    //  Utilizando momentos da musica para modificar o estado da aplicacao
    void obbeyMusicTimeCues();
    
    //  reinicia kaoxGameEngine, estado e posicao do audio
    void reset();
    
    void updateBoids();
    
    void updateBlobs_doKaue();   //  nao estao sendo usados no momento...
	
    
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
    ofxOscSender   sender_Ardour;
	ofxOscReceiver receiver1;
    ofxOscReceiver receiver_Ardour;
    
    bool audioPlaying;  //  TODO - Essa info tem que vir do Ardour
    
    bool state_music;   //  true: acompanha a musica
    
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
    
    //  [Brizo] - TODO: fazer isso de maneira mais elegante
    //  Note que eles ainda dependem de sua camada estar habilitada
    bool box2dEllipses_enabled;
    bool userCentroidEllipse_enabled;
    
    //  Agentes
    struct
    {
        vector<Boid> boids;
        bool enabled = false;
    } QueroQueros;
    
    
    
    //  ofxKinect
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    //  OpenCV
    
    ofxCvContourFinder contourFinder;
    
    //  Analise da depth image
    struct {
        ofPoint centroid;
        ofRectangle rectCoverage;
        float totalBlobsArea;
    } kinDepthAnalysis;
    
    //  Audio provisorio
    /*ofSoundPlayer player;
    ofSoundBuffer lastBuffer;
    ofSoundStream soundStream;
    float rms;
    void audioOut(ofSoundBuffer &outBuffer);    //  callback do audio*/
    
};

#endif
