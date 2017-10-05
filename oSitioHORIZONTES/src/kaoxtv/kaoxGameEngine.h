//
//  kaoxGameEngine.h
//  ZappaMAPP
//
//  Created by Kaue Costa on 22/03/13.
//  Copyright 2013 kaox.tv. All rights reserved.
//
#include "ofMain.h"
#include "ofxBox2d.h"
#include "kaoxGameObjects.h"
#include <deque>


#ifndef _H_KAOXGAMEENGINE
#define _H_KAOXGAMEENGINE

class kaoxGameEngine{
    
public:
    kaoxGameEngine();

    void setup();
    void update();
    void draw();
    
	//Box2D OLD
	void setupBox2d();
	void updateBox2d();
	void drawBox2d();
	
    void initDemo(int num);
    void initINTUITVdemo(int num);
    
 	void loadPoly();
	void createPolyZ();
    void drawZ(float x, float y, float size);
    
    void applyRepulsionForce(float x, float y, float amount);
    void applyAttractionPoint(float x, float y, float amount);
    
    void drawLinesTo(float x, float y, ofColor cor=ofColor(0,0,0));
    
    
    vector <ofVec2f> loadPoints(string file);
	vector <ofVec2f> loadPointsZ(float x, float y, float size);
    
    
    void createPolyBox2D(vector <ofVec2f> pts);
	void makePoly(vector <int> _pts, int _nPoints, 
				  float _density, float _bounce, float _friction, float _triang, ofColor _cor=ofColor(0,0,0,0));
	void makePoly(oxB2DPoly poly);	
	void makeEmitter(int pt, int _qtd, int _freq, bool _emitterMode,  
					 float _density, float _bounce, float _friction, int _particleType);
	void makeForce(int pt, float _power, bool _atraction);
    
    void makeSolidEllipse(float posX, float posY, int radius, float _density, float _bounce, float _friction);
    

    
    //NOVAS FUNCOES PARA CRIAR ELLIPSES DINAMICAS E DEPOIS CONTROLA_LAS // REFAZER PARA BOXES
    int makeEllipse(float posX, float posY, int radius, float _density, float _bounce, float _friction, ofColor cor=ofColor(0,0,0,0));
    void moveEllipse(int id, float x, float y);
    
    
	void makeWalls();
	
    void loadSet(int idx);
	
    void resetAll();
	void resetBodies();
	void resetEmitters();
	
	void reloadStatics();
	void reloadDynamics();
	void reloadSet();
	
	void transformStatics();
	void transformDynamics();
    
    
    
    //BOX 2D      <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	//Box2d
	ofxBox2d						box2d;			  //	the box2d world
	ofxBox2d						box2d2;			  //	the box2d world
	vector		<ofxBox2dCircle>	circles;		  //	default box2d circles
	vector		<ofxBox2dRect>		boxes;			  //	defalut box2d rects
	vector		<ofxBox2dPolygon>	polys;
	vector		<ofxBox2dPolygon>	walls;	
	
    //	float gravityX, gravityY, density, bounce, friction, power;
    //	int triang, qtdEmitter, particleType, freqEmitter;
    //	bool atractionMode, emitterMode;
    bool hasWalls;
	
    
	
    //	int corAtualH, corAtualS, corAtualB, corAtualA, corFundoH, corFundoS, corFundoB, corFundoA;
	
	
	ofxBox2dPolygon zBox2d;
	vector <ofVec2f> tempPolyBox2d;
    
    deque<ofPoint> pts; //copy of editor points
    deque<oxMPERect> rects;
	deque<oxMPETriangle> tris;
	deque<oxB2DPoly> st_polys;
	deque<oxB2DPoly> dy_polys;
	deque<oxB2DEmitter> emitters;
	deque<oxB2DForce> forces;
    
    float gravityX, gravityY, density, bounce, friction, triang, power;
    
    bool atractionMode, emitterMode;
    int qtdEmitter, freqEmitter, particleType;
    int width, height;
    
    vector <ofVec2f> cursorsFounded;

    vector <ofColor> colorList;
};

#endif