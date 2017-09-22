#include "ofApp.h"

//#__________________________________#//
//#                                  #//
//#    ZappaMAPP 0.1                 #//
//#    ########################      #//
//#    13/03/13                      #//
//#    Kaue Costa   www.kaox.tv      #//
//#----------------------------------#//



//----------------------------------------------------------------------------KORE
void ofApp::setup() {

	isSendingHands	= false;
	isSendingOSC	= false;
	isKinectOn		= false;
	isSyphonOn		= true;
	isDebugging		= false;
    isEditorOff      = true;
	
	
	ofSetVerticalSync(true);
	
//	nearThreshold = 500;
//	farThreshold  = 1000;
//	filterFactor = 0.1f;

	ofBackground(15, 15, 15);

	//OSC
	receiver.setup(PORT);
	// open an outgoing connection to HOST:PORT
	//sender.setup(HOST, PORT);	
	
    ofSetWindowTitle("oSitio HORIZONTES");
    
	//Syphon
	if(isSyphonOn) mainOutputSyphonServer.setName("oSitio HORIZONTES // HUB");
    
    
    
    //setup UI
    setupControlPanel();

    
    setupShaders();
    
    //setb("StartKinect", true);
    ofHideCursor();
    
    layer1.load("fundos/Azul1.jpg");
    layer2.load("fundos/Amarelo2.jpg");
    layer3.load("fundos/teste.jpg");
    
    
    int w = 1920;
    int h = 1080;
    
    
    for (int i=0; i<numBlobs; i++) {
        
        
        Blob bi = *new Blob(ofRandom(w), ofRandom(h), ofRandom(-100, 100), ofRandom(-100, 100), 1 + ofRandom(15));
        
        b.push_back(bi);
    }
    
    metaPixels.allocate(1920, 1080, 4);
    
}



void ofApp::setupControlPanel() {
	panel.setup("oSitio HORIZONTES", ofGetWidth()-340, 10, 320, 900);
	panel.msg = "dev:www.kaox.tv // tab hides the panel.";
    
    
    // ------------------------------------------------------- Kinect
    panel.addPanel("Painel de Controle");
    
    panel.addToggle("StartKinect", true);

    panel.addToggle("DebugKinect", false);
    
    panel.addToggle("IR", false);
    
    panel.addMultiToggle("KinectSource", 0, variadic("live")("recent")("calib")("swap")("menuDir")("menuInf")("misc"));    
    
    panel.addSlider("NearThreshold", 500, 100, 3000);
    panel.addSlider("FarThreshold", 1000, 200, 6000);
    panel.addSlider("FilterFactor", .1, -2, 2);

    panel.addToggle("TrackingHands", true);
    panel.addToggle("TrackingUser", true);   

    panel.addMultiToggle("KinectDraw", 1, variadic("none")("depth")("cam")("mask")("mask1")("mask2")("range"));

    
    panel.addSlider("FadeEffect", 50, 5, 255);
    
    panel.addToggle("ShowPanel", true);
    
    panel.reloadSettings();
    panel.hide();
    
}
//--------------------------------------------------------------
void ofApp::update(){
	
    
    //kinect setup
    
    if(getb("StartKinect")){
        
//        kin.isInfrared = getb("IR");
//        kin.startKinect(geti("KinectSource"));
       
//        isKinectOn = true;
        setb("StartKinect", false);
    }
    
    if(isKinectOn)
    {
//        kin.filterFactor = getf("FilterFactor");
//        kin.nearThreshold = geti("NearThreshold");
//        kin.farThreshold = geti("FarThreshold");         
//
//        kin.isTracking = getb("TrackingUser");
//        kin.isTrackingHands = getb("TrackingHands");
//        
//        if(geti("KinectDraw") > 2)  kin.isMasking = true;
//        else                        kin.isMasking = false;
    }
    
	
	if(isKinectOn){
        //chamar kaoxNI updateNI
//        kin.updateNI();
	}
	
	counter = counter + 0.033f;
    

    //updateBlobs
    for (int i=0; i<numBlobs-1; ++i) {
        b[i].update(i,b[i+1].getx(), b[i+1].gety(), ofGetMouseX(), ofGetMouseY());
    }
    
    
    updateShaders();
	
}
//--------------------------------------------------------------
void ofApp::draw(){
	
	
	ofBackground(0);
	
    
//	glEnable (GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();

    
    //videoPB.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    
    layer1.draw(0,0);

    
    glEnable(GL_BLEND_SRC_ALPHA);
    
    fbo1.draw(0,0);
    
    
    
    glEnable(GL_BLEND_SRC_ALPHA);

    
    fbo2.draw(0,0);
    
//    if(isKinectOn){
//        
//        //chamar kaoxNI updateNI
//        mask = kin.getUserMask();
//        mask.draw(0, 0, ofGetWidth(), ofGetHeight());
//        
//    }
    
	
	ofDisableAlphaBlending();

    
    if(isSyphonOn) mainOutputSyphonServer.publishScreen();
	
	
    if(getb("ShowPanel")) ofShowCursor();
    else                  ofHideCursor();

    
    
}


//UI
void ofApp::setb(string name, bool value) {
	panel.setValueB(name, value);
}
void ofApp::seti(string name, int value) {
	panel.setValueI(name, value);
}
void ofApp::setf(string name, float value) {
	panel.setValueF(name, value);
}
bool ofApp::getb(string name) {
	return panel.getValueB(name);
}
int ofApp::geti(string name) {
	return panel.getValueI(name);
}
float ofApp::getf(string name) {
	return panel.getValueF(name);
}


////OSC//--------------------------------------------------------------OSC

void ofApp::receiveOSC(){
	
	
}
//--------------------------------------------------------------

///EVENTOS//--------------------------------------------------------------Eventos
void ofApp::keyPressed(int key){

	
//	if(key == 't') ofToggleFullscreen();
	
	float smooth;
	ofxOscMessage mm;

	
		switch (key) {    
            case '1':
                setb("Toggle01", !getb("Toggle01"));
                break;
            case '2':
                setb("Toggle02", ! getb("Toggle02"));
                break;
            case '3':
                setb("Toggle03", ! getb("Toggle03"));
                break;				
            case '4':
                setb("Toggle04", ! getb("Toggle04"));
                break;
            case '5':
                setb("Toggle05", ! getb("Toggle05"));
                break;
            case '6':
                setb("Toggle06", ! getb("Toggle06"));				
                break;
            case '7':
                setb("Toggle07", ! getb("Toggle07"));
                break;
            case '8':
                setb("Toggle08", ! getb("Toggle08"));				
                break;
                
            case '0':
                setb("ShowPanel", ! getb("ShowPanel"));
                break;
                
            case 's':
                panel.saveSettings();
                break;
        }
    
    
    
    cout << "frame rate:" << ofGetFrameRate() << "   mouseX:" << ofGetMouseX() << endl;

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



void ofApp::setupShaders(){
    
    ofEnableAlphaBlending();
    
    brushImg.load("brush.png");
    
    int width = ofGetWidth();
    int height = ofGetHeight();
    
    maskFbo1.allocate(width,height, GL_RGBA);
    fbo1.allocate(width,height, GL_RGBA);
    maskFbo2.allocate(width,height, GL_RGBA);
    fbo2.allocate(width,height, GL_RGBA);
    maskFbo3.allocate(width,height, GL_RGBA);
    fbo3.allocate(width,height, GL_RGBA);
    
    // There are 3 of ways of loading a shader:
    //
    //  1 - Using just the name of the shader and ledding ofShader look for .frag and .vert:
    //      Ex.: shader.load( "myShader");
    //
    //  2 - Giving the right file names for each one:
    //      Ex.: shader.load( "myShader.vert","myShader.frag");
    //
    //  3 - And the third one it�s passing the shader programa on a single string;
    //
    
    
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/alphamask.vert","shaders_gles/alphamask.frag");
#else
    if(ofIsGLProgrammableRenderer()){
        string vertex = "#version 150\n\
        \n\
        uniform mat4 projectionMatrix;\n\
        uniform mat4 modelViewMatrix;\n\
        uniform mat4 modelViewProjectionMatrix;\n\
        \n\
        \n\
        in vec4  position;\n\
        in vec2  texcoord;\n\
        \n\
        out vec2 texCoordVarying;\n\
        \n\
        void main()\n\
        {\n\
        texCoordVarying = texcoord;\
        gl_Position = modelViewProjectionMatrix * position;\n\
        }";
        string fragment = "#version 150\n\
        \n\
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        in vec2 texCoordVarying;\n\
        \
        out vec4 fragColor;\n\
        void main (void){\
        vec2 pos = texCoordVarying;\
        \
        vec3 src = texture(tex0, pos).rgb;\
        float mask = texture(maskTex, pos).r;\
        \
        fragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
        shader.bindDefaults();
        shader.linkProgram();
    }else{
        string shaderProgram = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        \
        void main (void){\
        vec2 pos = gl_TexCoord[0].st;\
        \
        vec3 src = texture2DRect(tex0, pos).rgb;\
        float mask = texture2DRect(maskTex, pos).r;\
        \
        gl_FragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
        shader.linkProgram();
    }
#endif
    
    // Let�s clear the FBO�s
    // otherwise it will bring some junk with it from the memory    
    maskFbo1.begin();
    ofClear(0,0,0,0);
    maskFbo1.end();
    
    fbo1.begin();
    ofClear(0,0,0,255);
    fbo1.end();

    maskFbo2.begin();
    ofClear(0,0,0,0);
    maskFbo2.end();
    
    fbo2.begin();
    ofClear(0,0,0,255);
    fbo2.end();
    
    maskFbo3.begin();
    ofClear(0,0,0,0);
    maskFbo3.end();
    
    fbo3.begin();
    ofClear(0,0,0,255);
    fbo3.end();
    
}


void ofApp::updateShaders(){
    
    // MASK (frame buffer object)
    //
    
    ofSetCircleResolution(16);
    
    
    maskFbo1.begin();
    
    ofEnableAlphaBlending();
    
    ofSetColor(255,255,255, 5); //geti("FadeEffect"));
    
    if(isKinectOn){

        //chamar kaoxNI updateNI
        
        ////
//        mask = kin.getUserMask();
//        mask.draw(0, 0, ofGetWidth(), ofGetHeight());

    }
    
    ofSetColor(0,5);
    ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
    
    
    for(int i = 10; i > 0; i--){
        
        float factor = i;
        
        ofPushStyle();
        
        ofSetColor(255, (int)25);
        
//        ofDrawEllipse(b[0].getx(),b[0].gety(), 200-factor,200-factor);
//        ofDrawEllipse(b[1].getx(),b[1].gety(), 200-factor,200-factor);
        
        ofDrawEllipse(ofGetMouseX(),ofGetMouseY(), ofRandom(200),ofRandom(200));
        
//        ofDrawEllipse(sp[0].x,b[0].y, 200-factor,200-factor);
//        ofDrawEllipse(sp[0].x,b[0].y, 200-factor,200-factor);
        
        ofPopStyle();
        
        
    }
    
    
   // maskFbo.readToPixels(metaPixels);
    
    /*
    int w = 1920;
    int h = 1080;
    
    float scale = .5;
    
    
    for (int y=0; y<h; y++) {
        
        for (int x=0; x<w; x++) {
            
            int m = 1;
            for (int i=0; i <numBlobs-1; i++) {
                // Increase this number to make your blobs bigger
                m += b[i].getbs()*10000/(b[i].bx[x] + b[i].by[y] + 1);
            }
            
            ofColor cor = ofColor(255-ofDist(x, y, w*.5, h*.5), m+ofDist(x, y, w*.5, h*.5)*.3, (x+m+y)*scale); //in HSB mode: color((m+x+y),255,255);
            
            metaPixels.setColor(x, y, cor);
        }
    }
    
    ofTexture texture1;
    */
    
    //texture1.loadData(metaPixels);
    //texture1.draw(0,0);
    maskFbo1.end();
    
    
    
    // HERE the shader-masking happends
    //
    fbo1.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent for default
    ofClear(0, 0, 0, 255-geti("FadeEffect"));
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo1.getTextureReference(), 1 );
    
    //videoCor.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    layer3.draw(0,0);
//    srcImg.draw(0,0);
    
    shader.end();
    fbo1.end();
    
    
    
    maskFbo2.begin();
    
    ofEnableAlphaBlending();
    
    
    ofSetColor(0,25);
    ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
    
    
    float def = 120;
    
    for(int i = 0; i < def; i++){
        
        float px = ofGetWidth()/def*i;
        float fx = ofDist(px, 0, ofGetMouseX(), 0)/ofGetWidth() * ofGetWidth()/def;
        
        ofSetColor(255, 255, 255, 20);
        ofDrawRectangle(px, 0, fx*2, ofGetHeight());
        
    }
  
    maskFbo2.end();
    
    
    
    // HERE the shader-masking happends
    //
    fbo2.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent for default
    ofClear(0, 0, 0, 255-geti("FadeEffect"));
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo2.getTextureReference(), 1 );
    
    //videoCor.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    layer2.draw(0,0);
    //    srcImg.draw(0,0);
    
    shader.end();
    fbo2.end();
    
}