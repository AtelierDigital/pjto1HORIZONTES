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
    receiver.setup(PORT_RECEIVE_FEEDBACK_ARDOUR);//PORT_RECEIVE);
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT_SEND_ARDOUR);
	// open an outgoing connection to HOST:PORT
	//sender.setup(HOST, PORT);	
	
    ofSetWindowTitle("oSitio HORIZONTES");
    
	//Syphon
	if(isSyphonOn) mainOutputSyphonServer.setName("oSitio HORIZONTES // HUB");
    
    
    
    //setup UI
    setupControlPanel();

    
    setupShaders();
    
    setb("StartKinect", true);
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
    
    //
    //  Agentes
    
    ofPoint leftStop(ofGetWidth()/10, ofGetHeight()/10);
    ofPoint rightStop(9*ofGetWidth()/10, ofGetHeight()/10);
    for(int i =0; i < 6; i++)
    {
        Boid boid(ofGetWidth()/2, ofGetHeight()/2, leftStop, rightStop, 0.6 * ofGetHeight());
        boids.push_back(boid);
    }
    
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
    panel.addSlider("FarThreshold", 6000, 200, 8000);
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
	
	//counter = counter + 0.033f;   tempo entre frames? Tem uma funcao millis() ou algo do tipo...
    
    sendOSC_Ardour();
    //receiveOSC_Ardour();
    
    updateBoids();
    
    //updateBlobs_doKaue();
    
    /* NOTE - [Brizo] retirei a updateShaders abaixo, aparentemente duplicada.
    o for de update dos blobs estah agora em updateBlobs_doKaue(); comentada acima
    (nao estao sendo usados). "_doKaue" para nao confundir com os blobs da analise do OpenCV
    updateShaders();

    //updateBlobs
    for (int i=0; i<numBlobs-1; ++i) {
        b[i].update(i,b[i+1].getx(), b[i+1].gety(), ofGetMouseX(), ofGetMouseY());
    }*/
    
    
    updateShaders();
    
    ge.update(this->kinDepthAnalysis.centroid.x, this->kinDepthAnalysis.centroid.y);
    ge.updateBox2d();
	
}
//--------------------------------------------------------------
void ofApp::updateBoids()
{
    //centroid.x = ofGetMouseX();   //  test
    //centroid.y = ofGetMouseY();   //  test
    for(int i=0; i<boids.size(); i++)
    {
        boids[i].run(boids, this->kinDepthAnalysis.centroid.x, this->kinDepthAnalysis.rectCoverage.y);  // passing the entire list of boids to each boid
    }
}
//--------------------------------------------------------------
void ofApp::updateBlobs_doKaue()
{
    for (int i=0; i<numBlobs-1; ++i) {
        b[i].update(i,b[i+1].getx(), b[i+1].gety(), ofGetMouseX(), ofGetMouseY());
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
	
	
	ofBackground(0);
	
    glPushMatrix();
    
    
//	glEnable (GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();
    
    ofSetColor(255);
    
    
    float pllx = (-this->kinDepthAnalysis.centroid.x + ofGetWidth()/2);
    float plly = (-this->kinDepthAnalysis.centroid.y + ofGetHeight()/2);
    
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
    
    //  ---- DEBUG da analise ([Brizo] nao achei a flag de Debug...)
    ofPushMatrix();
    ofSetColor(255);
    ofDrawBitmapString(std::to_string(this->kinDepthAnalysis.totalBlobsArea), this->kinDepthAnalysis.centroid.x, this->kinDepthAnalysis.rectCoverage.y-10);
    ofDrawRectangle(this->kinDepthAnalysis.centroid.x, this->kinDepthAnalysis.rectCoverage.y, 20, 20);
    ofPopMatrix();
    //  -----
    
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
        
        //  ofxKinect
        
        // enable depth->video image calibration
        kinect.setRegistration(true);
        
        kinect.init();
        //kinect.init(true); // shows infrared instead of RGB video image
        //kinect.init(false, false); // disable video image (faster fps)
        
        kinect.open();		// opens first available kinect
        //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
        //kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
        
        // print the intrinsic IR sensor values
        if(kinect.isConnected()) {
                        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
                        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
                        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
                        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
        }
        
        //  OpenCV
        grayImage.allocate(kinect.width, kinect.height);
        //grayThreshNear.allocate(kinect.width, kinect.height);
        //grayThreshFar.allocate(kinect.width, kinect.height);
        
        mask.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
        
        isKinectOn = true;
        setb("StartKinect", false);
    }
    
    //
    //  * Armazenando a depth image, a espelhando e descartando tudo que esta fora de near e far
    //  * Determinando qtd de pixels da mask da depth image; retangulo de cobertura; centroide
    //  * Atualizando agentes
    
    kinect.setDepthClipping((float)getf("NearThreshold"), (float)getf("FarThreshold"));
    
    kinect.update();
    
    this->kinDepthAnalysis.centroid = ofVec3f::ofVec3f(-1.0f, -1.0f);
    this->kinDepthAnalysis.rectCoverage.set(-1.0f, -1.0f, -1.0f, -1.0f);
    this->kinDepthAnalysis.totalBlobsArea = 0.0f;
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // Lendo e espelhando a depth image do kinect
        grayImage.setFromPixels(kinect.getDepthPixels());
        grayImage.mirror(false, true);
        
        grayImage.getCvImage();
        
        //ofLogNotice() << "count = " << grayImage.countNonZeroInRegion(0, 0, kinect.width, kinect.height);
        
        /*
         // we do two thresholds - one for the far plane and one for the near plane
         // we then do a cvAnd to get the pixels which are a union of the two thresholds
         if(bThreshWithOpenCV) {
         grayThreshNear = grayImage;
         grayThreshFar = grayImage;
         //grayThreshNear.threshold(kinect.getNearClipping(), true);
         //grayThreshFar.threshold(kinect.getFarClipping());
         grayThreshNear.threshold(230, true);    //  hard-coding so para testes, usando o kinect bem perto de mim
         grayThreshFar.threshold(70, trye); //  hard-coding so para testes, usando o kinect bem perto de mim
         cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
         } else {*/
        
        // Estou mantendo os valores intermediarios ao inves de satura-los (TODO - vamos saturar ou nao?)
        ofPixels & pix = grayImage.getPixels();
        int numPixels = pix.size();
        for(int i = 0; i < numPixels; i++) {
            if(pix[i] > kinect.getFarClipping() || pix[i] < (kinect.getNearClipping())) {
                //if(pix[i] > 230 || pix[i] < 70) {    //  hard-coding so para testes, usando o kinect bem perto de mim
                pix[i] = 0;
            }
            else
            {
                pix[i] *= 10.0f;
            }
        }
        //}
        
        //  Comentado - Nao estamos mais editando os pixels da grayImage individualmente...
        //grayImage.flagImageChanged(); // updateTexture(); ?
        
        
        //  Convertendo para ofImage
        mask.setFromPixels(grayImage.getPixels());
        mask.update();
        
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        float nblobs = (float)contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 10, false);
        
        if(nblobs > 0) {
            this->kinDepthAnalysis.rectCoverage   = contourFinder.blobs[0].boundingRect;
            this->kinDepthAnalysis.centroid       = contourFinder.blobs[0].centroid;
            this->kinDepthAnalysis.totalBlobsArea = contourFinder.blobs[0].area;
        }
        for(int i=1; i < nblobs; i++)
        {
            this->kinDepthAnalysis.centroid += contourFinder.blobs[i].centroid;
            this->kinDepthAnalysis.rectCoverage.growToInclude( contourFinder.blobs[i].boundingRect );
            this->kinDepthAnalysis.totalBlobsArea = contourFinder.blobs[i].area;
        }
        this->kinDepthAnalysis.centroid /= nblobs;
        
        //
        //  Migrando da escala das dimensoes da depth image do kinect para as dimensoes da tela do nosso app
        
        this->kinDepthAnalysis.centroid.x = ofMap(this->kinDepthAnalysis.centroid.x, 0, kinect.width, 0, ofGetWidth());
        this->kinDepthAnalysis.centroid.y = ofMap(this->kinDepthAnalysis.centroid.y, 0, kinect.height, 0, ofGetHeight());
        this->kinDepthAnalysis.rectCoverage.x = ofMap(this->kinDepthAnalysis.rectCoverage.x, 0, kinect.width, 0, ofGetWidth());
        this->kinDepthAnalysis.rectCoverage.y = ofMap(this->kinDepthAnalysis.rectCoverage.y, 0, kinect.height, 0, ofGetHeight());
        this->kinDepthAnalysis.rectCoverage.width  = ofMap(this->kinDepthAnalysis.rectCoverage.width, 0, kinect.width, 0, ofGetWidth());
        this->kinDepthAnalysis.rectCoverage.height = ofMap(this->kinDepthAnalysis.rectCoverage.height, 0, kinect.height, 0, ofGetHeight());
        
        //ofLogNotice() << "centroid = " << centroid << ", rectCoverage = " << rectCoverage;
    }
    
    /*SimpleOpenNI
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
    */
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
//--------------------------------------------------------------
void ofApp::sendOSC_Ardour()
{
    ofxOscMessage mm;
    mm.setAddress("/set_transport_speed");
    mm.addFloatArg(ofMap(fabs(boids[0].vel[0]), 0.0f, boids[0].getMaxSpeed(), 1.0f, 1.5f));
    sender.sendMessage(mm, false);
}
//--------------------------------------------------------------
void ofApp::receiveOSC_Ardour()
{
    // hide old messages
    for(int i = 0; i < 20; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        {
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ": ";
            for(int i = 0; i < m.getNumArgs(); i++){
                // get the argument type
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msg_string += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msg_string += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msg_string += m.getArgAsString(i);
                }
                else{
                    msg_string += "unknown";
                }
            }
            // add to the list of strings to display
            msg_strings[current_msg_string] = msg_string;
            timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
            current_msg_string = (current_msg_string + 1) % 20;
            // clear the next line
            msg_strings[current_msg_string] = "";
        }
        
    }
}

///EVENTOS//--------------------------------------------------------------Eventos
void ofApp::keyPressed(int key){

	
//	if(key == 't') ofToggleFullscreen();
	
	float smooth;
    
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
void ofApp::keyReleased(int key)
{
    ofxOscMessage mm;
    mm.setAddress("/transport_play");
    /*mm.addIntArg(1);
     mm.addFloatArg(3.5f);
     mm.addStringArg("hello");
     mm.addFloatArg(ofGetElapsedTimef());*/
    sender.sendMessage(mm, false);
    
    
    
    /*  Anotando comandos OSC para o Ardour...
     goto_start
     goto_end
     
     /next_marker
     /prev_marker
     /locate spos roll	spos is the target position in samples; roll is a bool/integer: keep rolling or not
     /loop_toggle	Toggle loop mode on and off
     /loop_location start end	(frame position)
     /midi_panic	Ardour will send an all notes off to all midi tracks
     /cancel_all_solos	Cancel All Solos/PFLs/AFLs
     
     /transport_frame	Ardour sends /transport_frame current_frame
     /transport_speed	Ardour sends /transport_speed speed
     
     
     
     /master/gain dB	dB is a float indicating the desired gain in dB
     /master/fader position	position is a float between 0 and 1 setting the desired position of the fader
     /master/db_delta delta	where delta is a float that will increase or decrease the gain of master by the amount of the delta. (Ardour 5.11+)
     /master/trimdB dB	dB is a float from -20 to +20 representing the desired trim gain in dB
     /master/pan_stereo_position position	position is a float from 0 to 1 representing the desired pan position
     /master/mute state	state is an int of o or 1 representing the desired mute state
     
     
     
     /bank_up	Change bank to the next higher bank.
     /bank_up delta	Where delta is a float of 1 to bank up and -1 is bank down for use with an encoder (Ardour 5.11+)
     /bank_down	Change bank to the next lower bank.
     /strip/mute ssid mute_st	where mute_st is a bool/int representing the desired mute state of the track
     /strip/solo ssid solo_st	where solo_st is a bool/int representing the desired solo state of the track
     /strip/solo_iso ssid state	where state is a bool/int representing the desired solo isolate state of the track
     /strip/gain ssid gain	where gain is a float ranging from -193 to 6 representing the desired gain of the track in dB.
     /strip/fader ssid position	where position is a float ranging from 0 to 1 representing the fader control position.
     /strip/db_delta ssid delta	where delta is a float that will increase or decrease the gain of a track by the amount of the delta. (Ardour 5.11+)
     
     /strip/pan_stereo_position ssid position	where position is a float ranging from 0 to 1 representing the desired pan position of the track
     /strip/pan_stereo_width ssid width	where width is a float ranging from 0 to 1 representing the desired pan width of the track
     
     /strip/plugin/activate ssid piid	where piid = nth Plugin, will set the plugin's state to active
     /strip/plugin/deactivate ssid piid	where piid = nth Plugin, will set the plugin's state to inactive
     /strip/plugin/parameter ssid piid param value	where piid = nth Plugin, param = nth param, value is a float ranging from 0 to 1 representing the desired parameter value
     */
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
    ofLogNotice() << "target = " << boids[0].getTarget() << "frame rate:" << ofGetFrameRate();
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
        
        //  ofxKinect + OpenCV
        mask.draw(0, 0, ofGetWidth(), ofGetHeight());

    }
    
        //  Agentes
        
        ofPushStyle();
        ofSetColor(255, (int)25);
        for(int i=0; i<boids.size(); i++)
        {
            for(int e = 0; e < 10; e++){
                ofDrawEllipse(boids[i].pos.x, boids[i].pos.y, ofRandom(100),ofRandom(100));
            }
        }
        ofPopStyle();
    
    ofSetColor(0,5);
    ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());

    ofSetColor(255, 25);
    ge.drawEllipses();
    
    for(int i = 10; i > 0; i--){
        
        float factor = i;
        
        ofPushStyle();
        ofSetColor(255, (int)25);
        ofDrawEllipse(this->kinDepthAnalysis.centroid.x, this->kinDepthAnalysis.centroid.y, ofRandom(200),ofRandom(200));
        ofPopStyle();
        
    }
    maskFbo1.end();
    
    //////// FBO 1
    
    fbo1.begin();
    ofClear(0, 0, 0, 255-geti("FadeEffect"));
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo1.getTexture(), 1 );
    
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
        
        brushImg.draw(this->kinDepthAnalysis.centroid.x + ofRandom(-20,20), this->kinDepthAnalysis.centroid.y + ofRandom(-20,20));
        
        
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
    shader.setUniformTexture("maskTex", maskFbo3.getTexture(), 1 );
    
    
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
    shader.setUniformTexture("maskTex", maskFbo4.getTexture(), 1 );
    
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
        
        ofTranslate(this->kinDepthAnalysis.centroid.x + ofRandom(-20,20), this->kinDepthAnalysis.centroid.y + ofRandom(-20,20));
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
    shader.setUniformTexture("maskTex", maskFbo5.getTexture(), 1 );
    
    
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