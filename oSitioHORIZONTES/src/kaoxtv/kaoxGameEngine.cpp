//
//  kaoxGameEngine.cpp
//  ZappaMAPP
//
//  Created by Kaue Costa on 22/03/13.
//  Copyright 2013 kaox.tv. All rights reserved.
//
#include "kaoxGameEngine.h"



kaoxGameEngine::kaoxGameEngine(){}

void kaoxGameEngine::setup(){
    
    
    ofVec2f mouseP = ofVec2f(0, 0);
    
    cursorsFounded.push_back(mouseP);
    
    
    setupBox2d();
    
}

void kaoxGameEngine::update(){
    
    
    cursorsFounded[0].x = ofGetMouseX();
    cursorsFounded[0].y = ofGetMouseY();
    
    
    box2d.update();
	box2d2.update();
    
}

void kaoxGameEngine::draw(){
    
    
}


void kaoxGameEngine::applyRepulsionForce(float x, float y, float amount){
    
    
    for(int i=0; i<boxes.size(); i++) {

			boxes[i].addRepulsionForce(x, y, amount);
	}
    
	
	for(int i=0; i<circles.size(); i++) {
			circles[i].addRepulsionForce(x, y, amount);
	}
    
}

void kaoxGameEngine::applyAttractionPoint(float x, float y, float amount){
    
    for(int i=0; i<boxes.size(); i++) {
        boxes[i].addAttractionPoint(x, y, amount);
	}
    
	
	for(int i=0; i<circles.size(); i++) {
        circles[i].addAttractionPoint(x, y, amount);
	}
    
}


void kaoxGameEngine::drawLinesTo(float x, float y, ofColor cor){
    
    for(int i=0; i<circles.size(); i++) {
        
        ofPushStyle();
        ofSetColor(cor);
        ofLine(circles[i].getPosition().x, circles[i].getPosition().y, x, y);
        ofPopStyle();
	}
}


////BOX2D//--------------------------------------------------------------Box2D //DEPRECATED !!!
void kaoxGameEngine::setupBox2d() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetLogLevel(OF_LOG_NOTICE);
    
    
    hasWalls = false;
    
    width = ofGetWidth();
	height = ofGetHeight();
    
	//Box2D
    gravityX = 0;			//xy
    gravityY = 0;			//xy

    density = 3.6;			//rot
    bounce = 0.5;			//rot
    friction = 0.3;			//rot
    triang = 5;				//fad


    //	//forces
    power = 9.0;			//fad
    atractionMode = true;		//tog	


    //emitters
    qtdEmitter = 100;		//fad
    freqEmitter = 3;		//rot

    particleType = 1;			//tog
    emitterMode = false;		//tog
    
        
	
	box2d.init();
	box2d.setGravity(gravityX, gravityY);
	box2d.setFPS(60.0);
    
	box2d2.init();
	box2d2.setGravity(0, 0);
	box2d2.setFPS(60.0);
	
	
    //initDemo();
	//makeWalls();
}
//--------------------------------------------------------------
void kaoxGameEngine::updateBox2d() {
	
	box2d.update();	
	box2d2.update();	
    
    float minDis = ofGetMousePressed() ? 300 : 200;
	
	//aqui adiciono forças para cada objeto box2d principal > 1 
	
	for(int i=0; i<boxes.size(); i++) {
        
        
		for (int k=0; k<cursorsFounded.size(); k++) {
            
			
			float dis = cursorsFounded[k].distance(boxes[i].getPosition());
			
			if(k%2==0)
			{
				if(dis < minDis) boxes[i].addRepulsionForce(cursorsFounded[k], 4.0);
				else boxes[i].addAttractionPoint(cursorsFounded[k], 9.0);
			}else {
				if(dis < minDis*2) boxes[i].addRepulsionForce(cursorsFounded[k], 9.0);
				else boxes[i].addAttractionPoint(cursorsFounded[k], 4.0);
			}
            
		}
	}
    
	
	for(int i=0; i<circles.size(); i++) {
		
		for (int k=0; k<cursorsFounded.size(); k++) {
			
			float dis = cursorsFounded[k].distance(circles[i].getPosition());
			
			if(dis < minDis) circles[i].addRepulsionForce(cursorsFounded[k], 12.0);
			else circles[i].addAttractionPoint(cursorsFounded[k], 7.0);
		}
	}
	
	
	
	
	//Metodos Para Aplicar as Forças
	// ler deque > forcas e aplicar conforme configuração
	
	for(int i=0; i<boxes.size(); i++) {
		for(int f=0; f<forces.size(); f++)
		{
			if(forces[f].atractionMode) boxes[i].addRepulsionForce(pts[forces[f].idx], forces[f].power);
			else boxes[i].addAttractionPoint(pts[forces[f].idx], forces[f].power);			
		}
	}
	
	
	//EMITTER
	//Mode True > freq mode > a cada X freq > solta qtd
	//mode False > solta qtd / freq até acabar
	
	
	
	
	for(int e=0; e<emitters.size(); e++){
		
		if(emitters[e].emitterMode) {
			
			if(ofGetFrameNum() % emitters[e].freq == 0) { //inicio emit
				
				for(int i=0; i < emitters[e].qtd; i++){
					
					if(emitters[e].particleType == 1){
						
						float w = ofRandom(4, 16);	
						float h = ofRandom(4, 16);	
						
						ofxBox2dRect rect;
						rect.setPhysics(emitters[e].density, emitters[e].bounce, emitters[e].friction);
						rect.setup(box2d.getWorld(), pts[emitters[e].idx].x, pts[emitters[e].idx].y, w, h);
						boxes.push_back(rect);
						
					}else{
						
						float r = ofRandom(3, 15);		
						ofxBox2dCircle circle;
						circle.setPhysics(emitters[e].density, emitters[e].bounce, emitters[e].friction);
						circle.setup(box2d2.getWorld(), pts[emitters[e].idx].x, pts[emitters[e].idx].y, r);
						circles.push_back(circle);
						
					}
				}//fim emit 1
			}
			
		}else{
			
			if(emitters[e].life > 0){ //inicio emit
				
				for(int i=0; i < emitters[e].freq; i++){
					
					if(emitters[e].particleType == 1) {
						
						float w = ofRandom(4, 16);	
						float h = ofRandom(4, 16);	
						
						ofxBox2dRect rect;
						rect.setPhysics(emitters[e].density, emitters[e].bounce, emitters[e].friction);
						rect.setup(box2d.getWorld(), pts[emitters[e].idx].x, pts[emitters[e].idx].y, w, h);
						boxes.push_back(rect);
						
					}else {
						
						float r = ofRandom(3, 15);		
						ofxBox2dCircle circle;
						circle.setPhysics(emitters[e].density, emitters[e].bounce, emitters[e].friction);
						circle.setup(box2d2.getWorld(), pts[emitters[e].idx].x, pts[emitters[e].idx].y, r);
						circles.push_back(circle);
						
					}
				}//fim emit 1
				
				emitters[e].life -= emitters[e].freq;
			}
		}	
	}	
    
}
//--------------------------------------------------------------
void kaoxGameEngine::drawBox2d() {
	
    
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	
	ofFill();
	
	for(int i=0; i<circles.size(); i++) {
		      
//        if (colorList.size()>0) {
//            ofSetColor(colorList[i%colorList.size()]);
//        }else{
//            ofSetColor(0, 0, 0);
//        }
        
        ofSetColor(circles[i].getColor());

		ofPushMatrix();
		ofTranslate(circles[i].getPosition().x, circles[i].getPosition().y, 0);
		ofRotate(circles[i].getRotation(), 0, 0, 1);
		ofCircle(0,0, circles[i].getRadius());
		ofPopMatrix();
	}
	
	
	for(int i=0; i<boxes.size(); i++) {
		
		
        if (colorList.size()>0) {
            ofSetColor(colorList[i%colorList.size()]);
        }else{
            ofSetColor(0, 0, 0);
        }
        
        
		ofPushMatrix();
		ofTranslate(boxes[i].getPosition().x, boxes[i].getPosition().y, 0);
		ofRotate(boxes[i].getRotation(), 0, 0, 1);
		drawZ(-boxes[i].getWidth(), -boxes[i].getWidth(), boxes[i].getWidth()*2.5);		
		ofPopMatrix();
	}
//	ofDisableBlendMode();
    
//	ofEnableBlendMode(OF_BLENDMODE_SCREEN);
	
//	for(int i=0; i<circles.size(); i++) {
//		
//        if (colorList.size()>0) {
//            ofSetColor(colorList[i%colorList.size()]);
//        }else{
//            ofSetColor(0, 0, 0);
//        }
//		
//		ofPushMatrix();
//		ofTranslate(circles[i].getPosition().x, circles[i].getPosition().y, 0);
//		ofRotate(circles[i].getRotation(), 0, 0, 1);
//		ofCircle(5-ofRandom(10),5-ofRandom(10), circles[i].getRadius());		
//		ofPopMatrix();
//	}
    
    ofDisableBlendMode();
	
	
	// draw the ground
    //	box2d.drawGround();
	
    
	ofFill(); // <- OF not working here...	
	ofSetHexColor(0xFFFFFF);
	for (int i=0; i<polys.size(); i++) {
		polys[i].drawFill();
		polys[i].draw();
		polys[i].getPosition();
	}
    
	
}
//--------------------------------------------------------------


void kaoxGameEngine::initDemo(int num){
    
    for (int i=0; i<num; i++) {
		
		float r = ofRandom(3, 15);
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.9);
		circle.setup(box2d2.getWorld(), 0, 0, r);
		circles.push_back(circle);
		
		float w = ofRandom(4, 16);
		float h = ofRandom(4, 16);
		
		w = h;
		ofxBox2dRect rect;
		rect.setPhysics(3.0, 0.53, 0.9);
		rect.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, w, h);
		boxes.push_back(rect);
	}
    
    
    colorList.push_back(ofColor(0, 150, 150)); //verde
    colorList.push_back(ofColor(240, 120, 150)); //rosa
    colorList.push_back(ofColor(200, 120, 70)); //laranja
}


void kaoxGameEngine::initINTUITVdemo(int num){
    
    
    makeWalls();
    
    for (int i=0; i<num; i++) {

		
		float r = ofRandom(80, 30);
		ofxBox2dCircle circle;
		circle.setPhysics(1.0, 0.53, 1.5);
		circle.setup(box2d.getWorld(), 300, 300, r/2);
        
        circle.setColor(ofColor(255, 200));
		circles.push_back(circle);
        
        //camada sup
		r = ofRandom(30, 18);
		circle.setPhysics(2.0, 0.03, 0.2);
		circle.setup(box2d.getWorld(), 300, 300, r/2);
        
        circle.setColor(ofColor(255, 152));
		circles.push_back(circle);
        
		r = ofRandom(18, 9);
        circle.setPhysics(5.0, 0.53, 0.3);
		circle.setup(box2d.getWorld(), 300, 300, r/2);
        
        circle.setColor(ofColor(255, 52));
		circles.push_back(circle);
	}
    
    
    colorList.push_back(ofColor(255)); //turquesa
    colorList.push_back(ofColor(255, 152)); //verde
    colorList.push_back(ofColor(255, 26)); //preto claro
    
    
//    makeSolidEllipse(ofGetWidth()/2, ofGetHeight()/2, ofGetHeight()/6, 0.0, 0.0, 3.0);
    
    // makeForce(int pt, float _power, bool _atraction){
}

void kaoxGameEngine::loadPoly() {
	
	
	
	// load the shape we saved...
	vector <ofVec2f> pts = loadPoints("shape.dat");
	ofxBox2dPolygon poly;
	
	// loop and add vertex points
	for (int i=0; i<pts.size(); i++) {
		poly.addVertex(pts[i]);
	}
	poly.setAsEdge(false);
	poly.triangulate(15);
	poly.setPhysics(1.0, 0.3, 0.3);
	poly.create(box2d.getWorld());
	polys.push_back(poly);
	
}
//--------------------------------------------------------------
void kaoxGameEngine::createPolyZ() {
	
	
	// load the shape we saved...
	vector <ofVec2f> pts = loadPointsZ(ofGetWidth()/2, ofGetHeight()/2, 300);
    
	// loop and add vertex points
	for (int i=0; i<pts.size(); i++) {
		zBox2d.addVertex(pts[i]);
	}
	zBox2d.setAsEdge(false);
	zBox2d.triangulate(15);
	zBox2d.setPhysics(1.0, 0.3, 0.3);
	
    
    
}

//--------------------------------------------------------------
vector <ofVec2f> kaoxGameEngine::loadPoints(string file) {
	ifstream f;
	vector <ofVec2f> pts;
	f.exceptions ( ifstream::failbit | ifstream::badbit );
	try {
		f.open(ofToDataPath(file).c_str());
		string strLines;
		while (!f.eof()) {
			string ptStr;
			getline(f, ptStr);
			strLines+=ptStr;
		}
		f.close();
		
		vector <string>  ptsStr = ofSplitString(strLines, ",");
		for (int i=0; i<ptsStr.size(); i+=2) {
			float x = ofToFloat(ptsStr[i]);
			float y = ofToFloat(ptsStr[i+1]);
			pts.push_back(ofVec2f(x, y));
		}
	}
	catch (ifstream::failure e) {
		printf("no file to load...\n");
	}
	return pts;
}
//--------------------------------------------------------------
vector <ofVec2f> kaoxGameEngine::loadPointsZ(float x, float y, float size) {
    
	x = x - size/2;
	y = y - size/2;
	
	vector <ofVec2f> pts;
    
	pts.push_back(ofVec2f(x+(21*size/100), y+(10*size/100))); //1
	pts.push_back(ofVec2f(x+(5*size/100), y+(27*size/100))); //1
	pts.push_back(ofVec2f(x+(21*size/100), y+(43*size/100))); //1
	pts.push_back(ofVec2f(x+(34*size/100), y+(43*size/100))); //1
	
	
	pts.push_back(ofVec2f(x+(11*size/100), y+(66*size/100))); //2          
	pts.push_back(ofVec2f(x+(11*size/100), y+(89*size/100))); //2          
	
	
	pts.push_back(ofVec2f(x+(78*size/100), y+(89*size/100)));//3
	pts.push_back(ofVec2f(x+(94*size/100), y+(74*size/100)));//3
	pts.push_back(ofVec2f(x+(78*size/100), y+(57*size/100)));//3
	pts.push_back(ofVec2f(x+(67*size/100), y+(57*size/100)));//3          
	
	pts.push_back(ofVec2f(x+(89*size/100), y+(35*size/100)));//4       
	pts.push_back(ofVec2f(x+(89*size/100), y+(10*size/100)));//4       
	
	pts.push_back(ofVec2f(x+(21*size/100), y+(10*size/100))); //1
    
	return pts;
}
//--------------------------------------------------------------
void kaoxGameEngine::drawZ(float x, float y, float size){
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(x+(21*size/100), y+(10*size/100)); //1
	ofVertex(x+(5*size/100), y+(27*size/100)); //1
	ofVertex(x+(21*size/100), y+(43*size/100)); //1
	ofVertex(x+(34*size/100), y+(43*size/100)); //1
	
	
	ofVertex(x+(11*size/100), y+(66*size/100)); //2          
	ofVertex(x+(11*size/100), y+(89*size/100)); //2          
	
	
	ofVertex(x+(78*size/100), y+(89*size/100));//3
	ofVertex(x+(94*size/100), y+(74*size/100));//3
	ofVertex(x+(78*size/100), y+(57*size/100));//3
	ofVertex(x+(67*size/100), y+(57*size/100));//3          
	
	ofVertex(x+(89*size/100), y+(35*size/100));//4       
	ofVertex(x+(89*size/100), y+(10*size/100));//4       
	
	ofVertex(x+(21*size/100), y+(10*size/100)); //1
	ofEndShape();
}




//--------------------------------------------------------------
void kaoxGameEngine::createPolyBox2D(vector <ofVec2f> pts) {
    
	ofxBox2dPolygon poly;
	
	// loop and add vertex points
	for (int i=0; i<pts.size(); i++) {
		poly.addVertex(pts[i]);
	}
	poly.addVertex(pts[0]);
	
	poly.setAsEdge(false);
	poly.triangulate(55);
	poly.setPhysics(0.0, 0.3, 0.3);
	poly.create(box2d.getWorld());
	
	polys.push_back(poly);
}
void kaoxGameEngine::makePoly(vector <int> _pts, int _nPoints, float _density, float _bounce, float _friction, float _triang, ofColor _cor) {
	
	//box2d
	ofxBox2dPolygon poly;
	
	//oxB2DPoly
	oxB2DPoly polyTemp;
	
	polyTemp.nPoints = _nPoints;
	polyTemp.density = _density;
	polyTemp.bounce = _bounce;
	polyTemp.friction = _friction;
	polyTemp.triang = _triang;
	polyTemp.cor = _cor;
	
	
	for(int i=0; i < _nPoints; i++)
	{
		poly.addVertex(pts[_pts[i]]);
		polyTemp.idx[i] = _pts[i];
	}
	poly.setAsEdge(false);
	poly.triangulate(_triang);
	poly.setPhysics(_density, _bounce, _friction);
	poly.setColor(_cor);	//ox	
	poly.create(box2d.getWorld());
	
	if(_density == 0.0) st_polys.push_back(polyTemp);   //ox
	else dy_polys.push_back(polyTemp);
	polys.push_back(poly);			//box2d
}
void kaoxGameEngine::makePoly(oxB2DPoly polyTemp) {
	
	//box2d
	ofxBox2dPolygon poly;
	
	for(int i=0; i < polyTemp.nPoints; i++)
	{
		poly.addVertex(pts[polyTemp.idx[i]]);
	}
	
    
	poly.setColor(polyTemp.cor);
	
	poly.setAsEdge(false);
	poly.triangulate(polyTemp.triang);
	poly.setPhysics(polyTemp.density, polyTemp.bounce, polyTemp.friction);
	poly.create(box2d.getWorld());
	
	polys.push_back(poly);			//box2d
}
void kaoxGameEngine::makeEmitter(int pt, int _qtd, int _freq, bool _emitterMode,  float _density, float _bounce, float _friction, int _particleType){
	
	oxB2DEmitter emit;
	
	emit.idx = pt;
	emit.qtd = _qtd;
	emit.life = _qtd;
	emit.freq = _freq;
	emit.emitterMode = _emitterMode;
	emit.particleType = _particleType;
	emit.density = _density;
	emit.bounce = _bounce;
	emit.friction = _friction;
	
	emitters.push_back(emit);
	
}
void kaoxGameEngine::makeForce(int pt, float _power, bool _atraction){
	
	oxB2DForce force;
	
	force.idx = pt;
	force.power = _power;
	force.atractionMode = _atraction;
	
	forces.push_back(force);
}
void kaoxGameEngine::makeWalls(){
	
	if(hasWalls) { //se já tem walls reinicio sem walls
        
		hasWalls = false;
		walls.clear();
		resetBodies();
		return;
        
	}
    
	hasWalls = true;
    
	
	ofxBox2dPolygon polyN;
	ofxBox2dPolygon polyE;
	ofxBox2dPolygon polyS;
	ofxBox2dPolygon polyW;	
	
    
	polyN.addVertex(-20,0,0);
	polyN.addVertex(width+20,0,0);
	polyN.addVertex(width+20,-20,0);
	polyN.addVertex(-20,-20,0);	
	
	polyN.setAsEdge(true);
	polyN.triangulate(85);
	polyN.setPhysics(0.0, bounce, friction);
	polyN.create(box2d.getWorld());
	
	walls.push_back(polyN);
    
	//polyE
	polyE.addVertex(width,-20,0);
	polyE.addVertex(width+20,-20,0);
	polyE.addVertex(width+20,height+20,0);
	polyE.addVertex(width,height+20,0);	
	
	polyE.setAsEdge(true);
	polyE.triangulate(85);
	polyE.setPhysics(0.0, bounce, friction);
	polyE.create(box2d.getWorld());
	
	walls.push_back(polyE);
	
	polyS.addVertex(width+20,height,0);	
	polyS.addVertex(width+20,height+20,0);		
	polyS.addVertex(-20,height+20,0);
	polyS.addVertex(-20,height,0);
	
	polyS.setAsEdge(true);
	polyS.triangulate(85);
	polyS.setPhysics(0.0, bounce, friction);
	polyS.create(box2d.getWorld());
	
	walls.push_back(polyS);
	
	
	polyW.addVertex(-1,height+20,0);
	polyW.addVertex(-21,height+20,0);
	polyW.addVertex(-21,-20,0);
	polyW.addVertex(-1,-20,0);
	
	polyW.setAsEdge(true);
	polyW.triangulate(85);
	polyW.setPhysics(0.0, bounce, friction);
	polyW.create(box2d.getWorld());
	
	walls.push_back(polyW);
}

void kaoxGameEngine::makeSolidEllipse(float posX, float posY, int radius, float _density, float _bounce, float _friction){
    
    /*
     
     void setup(b2World * b2dworld, float x, float y, float radius);
     void setup(b2World * b2dworld, ofVec2f &pts, float radius);
     
     //------------------------------------------------
     float getRadius();
     void  setRadius(float r);
     
     //------------------------------------------------
     void addAttractionPoint(float x, float y, float amt=1);
     void addRepulsionForce(float x, float y, float amt);
     
     */
    
    ofxBox2dCircle circle;
    
	circle.setPhysics(_density, _bounce, _friction);
    circle.setup(box2d.getWorld(), posX, posY, radius);
    
}



int kaoxGameEngine::makeEllipse(float posX, float posY, int radius, float _density, float _bounce, float _friction, ofColor cor){
    

    ofxBox2dCircle circle;
    
	circle.setPhysics(_density, _bounce, _friction);
    circle.setup(box2d.getWorld(), posX, posY, radius);
    circle.setColor(cor);
    
    circles.push_back(circle);
    
    return circles.size()-1;
}

void kaoxGameEngine::moveEllipse(int id, float x, float y){
    
    circles[id].setPosition(x, y);
    
}

void kaoxGameEngine::loadSet(int idx){
	
}
void kaoxGameEngine::resetAll(){
	
	st_polys.clear();
	emitters.clear();
	forces.clear();
	
	polys.clear();
	dy_polys.clear();	
	circles.clear();
	boxes.clear();
	
	box2d.init();
	
	
}
void kaoxGameEngine::resetBodies(){
	
	
	walls.clear();
	polys.clear();
	circles.clear();
	boxes.clear();
	
	box2d.init();
	reloadStatics();
	reloadDynamics();
	
	if(hasWalls) {
		hasWalls = false;
		makeWalls();
	}
}
void kaoxGameEngine::resetEmitters(){
	
	for(int i=0; i < emitters.size(); i++){
        
		emitters[i].life = emitters[i].qtd;
	}
}


void kaoxGameEngine::reloadStatics(){
    
	for(int st=0; st < st_polys.size(); st++)
	{
		makePoly(st_polys[st]);
	}
}
void kaoxGameEngine::reloadDynamics(){
    
	for(int i=0; i < dy_polys.size(); i++)
	{
		makePoly(dy_polys[i]);
	}
}
void kaoxGameEngine::reloadSet(){
    
	
	
}

void kaoxGameEngine::transformStatics(){
    
	for(int i=0; i < st_polys.size(); i++)
	{
		if(st_polys[i].density == 0.0) st_polys[i].density = density; //transforma estaticos em dinamicos
		else st_polys[i].density = 0.0;
	}
	resetBodies();
	
}
void kaoxGameEngine::transformDynamics(){
	for(int i=0; i < dy_polys.size(); i++)
	{
		if(dy_polys[i].density > 0.0) dy_polys[i].density = 0.0;
		else dy_polys[i].density = density;
	}	
	resetBodies();	
}

