#include "ofApp.h"

//#__________________________________#//
//#                                  #//
//#    KinectApp 0.1                 #//
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

    layer0.load("fundos/fundos0.jpg");
    layer1.load("fundos/fundos1.jpg");
    layer2.load("fundos/fundos2.jpg");
    layer3.load("fundos/fundos3.jpg");
    
    layer4.load("fundos/fundos4.jpg");
    layer5.load("fundos/fundos5.jpg");
    layer6.load("fundos/fundos6.jpg");
    
    brushImg.load("brush.png");
    
    int w = 1920;
    int h = 1080;
    
    
    for (int i=0; i<numBlobs; i++) {
        
        
        Blob bi = *new Blob(ofRandom(w), ofRandom(h), ofRandom(-100, 100), ofRandom(-100, 100), 1 + ofRandom(15));
        
        b.push_back(bi);
    }
    
    metaPixels.allocate(1920, 1080, 4);
    
    ge.setup();
    ge.initHorizontes(26);
    
    
    
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
    
    
    
    // ------------------------------------------------------- Camadas
    panel.addPanel("Camadas");
    
    panel.addToggle("Background", true);
    panel.addToggle("Layer1", true);
    panel.addToggle("Layer2", true);
    panel.addToggle("Layer3", true);
    panel.addToggle("Layer4", true);
    panel.addToggle("Layer5", true);
    panel.addToggle("Layer6", true);
    
    panel.addToggle("SendOSC", true);
    
    
    
    // ------------------------------------------------------- Camadas
    panel.addPanel("Noise Shader");
    
    
    panel.addSlider("noiseIntensity", 0, -4.0f, 4.0f);
    panel.addSlider("noiseVelocity", 0, -4.0f, 4.0f);
    
    
    
    
    panel.addSlider("shaderValue1", 0.5, 0.0, 1., 0);
    panel.addSlider("shaderValue2", 0.5, 0.0, 1., 0);
    panel.addSlider("shaderValue3", 0.5, 0.0, 1., 0);
    panel.addSlider("shaderValue4", 0.5, 0.0, 1., 0);
    
    
    panel.addSlider("shaderScale", 0, -4.0f, 4.0f);
    panel.addToggle("useNoiseShader", true);
    
    
    
    panel.reloadSettings();
    panel.hide();
    
}
//--------------------------------------------------------------
void ofApp::update(){
	
    updateKinect();
	
	counter = counter + 0.033f;
    

    //updateBlobs
    for (int i=0; i<numBlobs-1; ++i) {
        b[i].update(i,b[i+1].getx(), b[i+1].gety(), ofGetMouseX(), ofGetMouseY());
    }
    
    
    updateShaders();
    
    ge.update();
    ge.updateBox2d();
	
}
//--------------------------------------------------------------
void ofApp::draw(){
	
	
	ofBackground(0);
	
    glPushMatrix();
    
    
//	glEnable (GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();
    
    ofSetColor(255);
    
    
    float pllx = (-ofGetMouseX() + ofGetWidth()/2);
    float plly = (-ofGetMouseY() + ofGetHeight()/2);
    
    //videoPB.draw(0, 0, ofGetWidth(), ofGetHeight());
    layer0.draw((pllx/100),(plly/100));
    
    
    
    if (getb("Layer1")) {

        glEnable(GL_BLEND_SRC_ALPHA);
        fbo1.draw(0,0);
    
    }
    
    
    if (getb("Layer2")) {

        
        glEnable(GL_BLEND_SRC_ALPHA);

        ofSetColor(0, 0, 0, 120);
        fbo2.draw(50+pllx/80.0,50+(plly/80));
        
        
        ofSetColor(255);
        fbo2.draw(0+pllx/80.0,(plly/80));
        
    }

    
    
    if (getb("Layer3")) {
        
        glEnable(GL_BLEND_SRC_ALPHA);
        
        ofSetColor(0, 0, 0, 120);
        fbo3.draw(-50+pllx/50.0,40+(plly/50));
        
        
        ofSetColor(255);
        fbo3.draw(-10+pllx/50.0,-10+(plly/50));
        
    }
    
    
    if (getb("Layer5")) {

    
        glEnable(GL_BLEND_SRC_ALPHA);
        
        ofSetColor(0, 0, 0, 120);
        fbo5.draw(-60+pllx/10.0,40+(plly/20));
        
        
        ofSetColor(255);
        fbo5.draw(-20+pllx/10.0,-10+(plly/20));
                
    }
    
    
    if (getb("Layer4")) {

        
        glEnable(GL_BLEND_SRC_ALPHA);
        
        
        ofPushMatrix();
        ofTranslate(5+pllx/20.0, 4+(plly/20));
        
        
        ofSetColor(0, 0, 0, 120);
        fbo4.draw(0,0);
        
        
        ofPushStyle();
        ge.drawPolyLines();
        
        ofTranslate(-10+pllx/20.0,-10+(plly/20));
        
        ofSetColor(0, 255);
        ofSetLineWidth(2);
        ge.drawPolyLines();
        
        ofSetColor(255);
        fbo4.draw(0,0);
        
        
        ofPopMatrix();
        ofPopStyle();
    }
    
    
    
//    if(isKinectOn){
//        
//        //chamar kaoxNI updateNI
//        mask = kin.getUserMask();
//        mask.draw(0, 0, ofGetWidth(), ofGetHeight());
//        
//    }
    
    
    
	ofDisableAlphaBlending();
    
    glPopMatrix();
    
    
    if(isSyphonOn) mainOutputSyphonServer.publishScreen();
	
	
    if(getb("ShowPanel")) {

        ofShowCursor();
        panel.draw();
        
    }else    ofHideCursor();

    
    
    
}



///-----------------------updates

void ofApp::updateKinect(){
    
    
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
void ofApp::sendOSC(){
    
    
    
}

///EVENTOS//--------------------------------------------------------------Eventos
void ofApp::keyPressed(int key){

	
//	if(key == 't') ofToggleFullscreen();
	
	float smooth;
	ofxOscMessage mm;

	
		switch (key) {    
            case '1':
                setb("Layer1", !getb("Layer1"));
                break;
            case '2':
                setb("Layer2", ! getb("Layer2"));
                break;
            case '3':
                setb("Layer3", ! getb("Layer3"));
                break;				
            case '4':
                setb("Layer4", ! getb("Layer4"));
                break;
            case '5':
                setb("Layer5", ! getb("Layer5"));
                break;
            case '6':
                setb("Layer6", ! getb("Layer6"));				
                break;
            case ' ':
                
                ge.initHorizontes(26);
                
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
    
    maskFbo4.allocate(width,height, GL_RGBA);
    fbo4.allocate(width,height, GL_RGBA);
    maskFbo5.allocate(width,height, GL_RGBA);
    fbo5.allocate(width,height, GL_RGBA);
    maskFbo6.allocate(width,height, GL_RGBA);
    fbo6.allocate(width,height, GL_RGBA);
    
    
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
    
    maskFbo4.begin();
    ofClear(0,0,0,0);
    maskFbo4.end();
    
    fbo4.begin();
    ofClear(0,0,0,255);
    fbo4.end();
    
    maskFbo5.begin();
    ofClear(0,0,0,0);
    maskFbo5.end();
    
    fbo5.begin();
    ofClear(0,0,0,255);
    fbo5.end();
    
    maskFbo6.begin();
    ofClear(0,0,0,0);
    maskFbo6.end();
    
    fbo6.begin();
    ofClear(0,0,0,255);
    fbo6.end();
    
}


void ofApp::updateShaders(){
    
    // MASK (frame buffer object)
    //
    ofSetCircleResolution(16);
    
    float def = 120;
    
    
    //////// MASK FBO 1
    
    
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

    ofSetColor(255, 25);
    ge.drawEllipses();
    
    for(int i = 10; i > 0; i--){
        
        float factor = i;
        
        ofPushStyle();
        ofSetColor(255, (int)25);
        ofDrawEllipse(ofGetMouseX(),ofGetMouseY(), ofRandom(200),ofRandom(200));
        ofPopStyle();
        
    }
    maskFbo1.end();
    
    //////// FBO 1
    
    fbo1.begin();
    ofClear(0, 0, 0, 255-geti("FadeEffect"));
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo1.getTextureReference(), 1 );
    
    layer1.draw(0,0);
    
    shader.end();
    fbo1.end();
    
    ////////fim FBO 1
    
    
    ////////////////// FBO     2
    /// MASK
    maskFbo2.begin();
    
    ofEnableAlphaBlending();
    
    ofSetColor(0,5);
    ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
    
    ofSetColor(255, 25);
    
    for(int i = 1; i > 0; i--){
        
        float factor = i;
        
        ofPushStyle();
        ofSetColor(255, (int)26);
        
        brushImg.draw(ofGetMouseX()+ofRandom(-20,20),ofGetMouseY()+ofRandom(-20,20));
        
        
        //ofDrawEllipse(ofGetMouseX()+ofRandom(-200,200),ofGetMouseY()+ofRandom(-200,200), ofRandom(16),ofRandom(16));
        ofPopStyle();
        
    }
    
    maskFbo2.end();
    
    //FBO
    fbo2.begin();
    ofClear(0, 0, 0, 255-geti("FadeEffect"));
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo2.getTextureReference(), 1 );
    
    
    layer2.draw(0,0);
    
    shader.end();
    fbo2.end();
    
    /// fim 2
    
    
    
    
    ////////////////// FBO     3
    /// MASK
    maskFbo3.begin();
    
    ofEnableAlphaBlending();
    ofSetColor(0,55);
    ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
    
    
    ofSetColor(255,150);
    //ofDrawRectangle(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(40), ofRandom(40));
    
    ge.drawBoxes();
    
    
    maskFbo3.end();
    
    //FBO
    fbo3.begin();
    ofClear(0, 0, 0, 255-geti("FadeEffect"));
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo3.getTextureReference(), 1 );
    
    
    layer3.draw(0,0);
    
    shader.end();
    fbo3.end();
    
    /// fim 3
    
    
    
    ////////////////// FBO     4
    /// MASK
    maskFbo4.begin();
    
    ofEnableAlphaBlending();
    ofSetColor(0,25);
    ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
    
    ge.drawPolys();
    
    maskFbo4.end();
    
    //FBO
    fbo4.begin();
    ofClear(0, 0, 0, 255-geti("FadeEffect"));
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo4.getTextureReference(), 1 );
    
    layer4.draw(0,0);
    
    shader.end();
    fbo4.end();
    
    /// fim 4
    
    ////////////////// FBO     5
    /// MASK
    
    maskFbo5.begin();
    
    ofEnableAlphaBlending();
    
    ofSetColor(0,15);
    ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
    
    ofSetColor(255, 15);
    
    for(int i = 3; i > 0; i--){
        
        float factor = i;
        
        ofPushStyle();
        ofSetColor(255, (int)26);
        
        ofPushMatrix();
        
        ofTranslate(ofGetMouseX()+ofRandom(-20,20), ofGetMouseY()+ofRandom(-20,20));
        ofScale(2.5, 2.5);
        
        brushImg.draw(-125,-125);
        
        
        //ofDrawEllipse(ofGetMouseX()+ofRandom(-200,200),ofGetMouseY()+ofRandom(-200,200), ofRandom(16),ofRandom(16));
        ofPopStyle();
        
    }
    
    maskFbo5.end();
    
    
    //FBO
    fbo5.begin();
    ofClear(0, 0, 0, 255-geti("FadeEffect"));
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo5.getTextureReference(), 1 );
    
    
//    ofSetColor(0);
//    ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
    
    layer5.draw(0,0);
    
    shader.end();
    fbo5.end();
    
    /// fim 5
    
//    float totPix = ofGetHeight()*ofGetWidth();
//    
//    for (int p=0; p<totPix; p++) {
//        
//    }
    
    
}