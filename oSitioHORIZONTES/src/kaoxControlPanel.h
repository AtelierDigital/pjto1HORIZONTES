//
//  kaoxControlPanel.h
//  
//
//  Modified by Kaue Costa on 25/03/13.
//
#pragma once

/*
 kaoxControlPanel makes ofxControlPanel a little easier to work with.
 
 the control panel is automatically toggle-able using the tab key.
 the panel is hooked up to mouse, draw, and update events automatically.
 addPanel() does an addPanel() followed by a setWhichPanel() to avoid redundancy and mistakes.
 the fps is autoamtically drawn at the lower right corner of the screen.
 */

#include "ofxControlPanel.h"
#include "ofEvents.h"
#include "simpleColor.h"

#include "ofxTouchOsc.h"


#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
template <typename T>
struct variadic_ : public vector<T> {
	variadic_(const T& t) {
		(*this)(t);
	}
	variadic_& operator()(const T& t) {
		this->push_back(t);
		return *this;
	}
};

typedef variadic_<string> variadic;



class kaoxControlPanel : public ofxControlPanel {
public:
	string msg;
    bool isOn;
    bool exportOsc = false;
    bool lastToggle = true;
    
    
    ofxTouchOsc touchOsc;
    ofxTouchOscPage *page;
    
    float px, py, pw, ph;
    
    
	kaoxControlPanel() {
		msg = "";
		setXMLFilename("settings.xml");
		ofAddListener(ofEvents().update, this, &kaoxControlPanel::update);
		ofAddListener(ofEvents().draw, this, &kaoxControlPanel::draw);
		ofAddListener(ofEvents().keyPressed, this, &kaoxControlPanel::keyPressed);
		ofAddListener(ofEvents().mousePressed, this, &kaoxControlPanel::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &kaoxControlPanel::mouseReleased);
		ofAddListener(ofEvents().mouseDragged, this, &kaoxControlPanel::mouseDragged);
        
        isOn = true;
	}
	void setup() {
		ofxControlPanel::setup("Control Panel", 5, 5, 280, 600);
	}
	void setup(int width, int height) {
		ofxControlPanel::setup("Control Panel", 5, 5, width, height);
	}
	void setup(string label, int posX, int posY, int width, int height) {
		ofxControlPanel::setup(label, posX, posY, width, height);
	}
    void update(ofEventArgs& event) {
		ofxControlPanel::update();
	}
	void draw(ofEventArgs& event) {
		if(!hidden) {
			ofPushMatrix();
			ofSetupScreenPerspective();
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);
			ofPushStyle();
			ofPushMatrix();
			ofTranslate(.5, .5);
			ofxControlPanel::draw();
			ofPopMatrix();
			if(msg != "") {
				ofSetColor(0);
				ofFill();
				int textWidth =  10 + msg.length() * 8;
				ofRect(5, ofGetHeight() - 22, textWidth, 20);
				ofSetColor(255, 255, 255);
				ofDrawBitmapString(msg, 10, ofGetHeight() - 10);
			}
			
			ofSetColor(255);
			ofFill();
			ofRect(ofGetWidth() - 45, ofGetHeight() - 25, 40, 20);
			ofSetColor(0);
			ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 40, ofGetHeight() - 10);
			ofPopStyle();
			glPopAttrib();
			ofPopMatrix();
		}
	}
    
    void draw() {
        
        ofPushMatrix();
        ofSetupScreenPerspective();
//        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_DEPTH_TEST);
//        ofPushStyle();
        ofPushMatrix();
        ofTranslate(.5, .5);
        ofxControlPanel::draw();
        ofPopMatrix();
//        ofPopStyle();
//        glPopAttrib();        
	}
    
	using ofxControlPanel::hasValueChanged;
	// usage: panel.hasValueChanged(variadic(1)(2)(3)(4)(5));
	bool hasValueChanged(const vector<string>& values) {
		for(int i = 0; i < values.size(); i++) {
			string cur = values[i];
			if(ofxControlPanel::hasValueChanged(cur)) {
				return true;
			}
		}
		return false;
	}
	void keyPressed(ofKeyEventArgs& event) {
		if(event.key == '\t') {
			if(hidden) {
				show();
			} else {
				hide();
			}
		}
	}
	void show() {
		ofxControlPanel::show();
        isOn = true;
		ofShowCursor();
	}
	void hide() {
		ofxControlPanel::hide();
        isOn = false;
		ofHideCursor();
	}
    
    //OX
    
    void removeListeners(){
        
 		ofRemoveListener(ofEvents().update, this, &kaoxControlPanel::update);
		ofRemoveListener(ofEvents().draw, this, &kaoxControlPanel::draw);
		ofRemoveListener(ofEvents().keyPressed, this, &kaoxControlPanel::keyPressed);
		ofRemoveListener(ofEvents().mousePressed, this, &kaoxControlPanel::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &kaoxControlPanel::mouseReleased);
		ofRemoveListener(ofEvents().mouseDragged, this, &kaoxControlPanel::mouseDragged);       
        
    }
    
    
	void mousePressed(ofMouseEventArgs& event) {
		ofxControlPanel::mousePressed(event.x, event.y, event.button);
	}
	void mouseReleased(ofMouseEventArgs& event) {
		ofxControlPanel::mouseReleased();
	}
	void mouseDragged(ofMouseEventArgs& event) {
		ofxControlPanel::mouseDragged(event.x, event.y, event.button);
	}
	
	// by default, make sliders float
	using ofxControlPanel::addSlider;
	void addSlider(string name, string xmlName, float value, float low, float high, bool isInt = false) {
		ofxControlPanel::addSlider(name, xmlName, value, low, high, isInt);
        
        if(exportOsc) addSliderOsc(name);
        
	}
	void addSlider(string name, float value, float low, float high, bool isInt = false) {
		ofxControlPanel::addSlider(name, name, value, low, high, isInt);
        
        if(exportOsc) addSliderOsc(name);
        
	}
	using ofxControlPanel::addToggle;
	void addToggle(string name, bool value = false) {
		ofxControlPanel::addToggle(name, name, value);
        
        if(exportOsc) addToggleOsc(name);
        
	}
	void addToggle(string label, string name, bool value = false) {
		ofxControlPanel::addToggle(label, name, value);
        
        if(exportOsc) addToggleOsc(name);
        
	}
    using ofxControlPanel::addMultiToggle;
	void addMultiToggle(string name, int initial, vector<string> values) {
		ofxControlPanel::addMultiToggle(name, name, initial, values);
        
        if(exportOsc) addMultiToggleOsc(name, values.size());
        
	}
	using ofxControlPanel::addPanel;
	void addPanel(string panelName, int columns = 1) {
		ofxControlPanel::addPanel(panelName, columns);
		ofxControlPanel::setWhichPanel(panelName);
        
        if(exportOsc) addPanelOsc(panelName);
        
	}
    
    
    //////////////////////////////////////
    //      + output: TOUCH OSC
    //////////////////////////////////////
    
    
    void setupOsc(int w=480, int h=320){
        
        
        touchOsc.setScale(w, h);
        touchOsc.setDefaultColor(GREEN);
        
        exportOsc = true;
     
        
    }
    
    void setupOscColor(TouchOscColor cor){
        
        touchOsc.setDefaultColor(cor);
        
    }
    
    
    void addPanelOsc(string panelName) {
        
        // create a new page
        page = touchOsc.addPage(panelName);
        
        // set the color for each widget blue
        page->setDefaultWidgetColor(GREEN);
     
        
        ofxTouchOscLabel *lbl = page->addLabel(panelName, 0.88, 0.01, 0.1, 0.1);
        lbl->setLabel("kaox.tv");
        lbl->setTextSize(12);
        lbl->setOutline(false);
        lbl->setBackground(false);
        
        px = 0.0;
        py = 0.0;
        
    }
    
    void addSliderOsc(string name) {
        
        
        py += 0.1;
        px = 0.1;
    
        // arguments for adding any new widget: name, x, y, width, height (0.0->1.0)
        ofxTouchOscFader *sld = page->addFader(name, px, py, 0.63, 0.08);
        sld->setCentered(false);
        sld->setInverted(false);
        sld->setResponseRelative(false);
        
        
        // faders and rotaries can be set vertical or horizontal.
        // by default, it's horizontal if width>height, and vice-versa
        sld->setHorizontal();
        
        sld->setOscAddress("/control/slider/"+name);
        
        lastToggle = false;
        
    }
    
    
    void addToggleOsc(string name) {
        
        if(!lastToggle) {
            px = 0.1;
            py+=0.1;
        }
        else px += 0.1;
    
        ofxTouchOscToggle *toggle = page->addToggle(name, px, py, 0.08, 0.08);
        toggle->setLocalOff(false);
        
        
        // override default osc address
        toggle->setOscAddress("/control/toggle/"+name);
        
        lastToggle = true;

        
    }
    
    void addMultiToggleOsc(string name, int number){

        py+=0.1;
        px = 0.1;
        
        ofxTouchOscMultiToggle *multitoggle = page->addMultiToggle(name, px, py, 0.4, 0.1);
        multitoggle->setLocalOff(false);
        multitoggle->setExclusive(true);
        multitoggle->setNumberX(number);
        multitoggle->setNumberY(1);
        
        
        // override default osc address
        multitoggle->setOscAddress("/control/multi/"+name);
        
        
        lastToggle = false;
        
    }
    
    
    ////// NOVOS MËTODOS > > > custom Controls
    
    
    void addColorSelector(string name){
        
        ofxControlPanel::addSlider(name+"_R", name+"_R", 0, 0, 255, true);
        ofxControlPanel::addSlider(name+"_G", name+"_G", 0, 0, 255, true);
        ofxControlPanel::addSlider(name+"_B", name+"_B", 0, 0, 255, true);
        ofxControlPanel::addSlider(name+"_A", name+"_A", 0, 0, 255, true);
        
        
        if(exportOsc) addColorSelectorOsc(name);
    }
    
    void addColorSelectorOsc(string name){
    
        
        py+=0.1;
        px = 0.1;
        
        
        ofxTouchOscMultiFader *multifader = page->addMultiFader(name, px, py, 0.6, 0.2);
        multifader->setCentered(false);
        multifader->setInverted(false);
        multifader->setNumber(4);
        
        py+=0.1;
        
        // override default osc address
        multifader->setOscAddress("/control/color/"+name);
        
        
        lastToggle = false;
        
    }

    
    void addVectorSelector(string name, float min, float max){
        
        
        ofxControlPanel::addSlider(name+"_X", name+"_X", (max+min)/2, min, max, true);
        if(exportOsc) addSliderOsc(name+"_X");
        ofxControlPanel::addSlider(name+"_Y", name+"_Y", (max+min)/2, min, max, true);
        if(exportOsc) addSliderOsc(name+"_Y");
        ofxControlPanel::addSlider(name+"_Z", name+"_Z", (max+min)/2, min, max, true);
        if(exportOsc) addSliderOsc(name+"_Z");
        
//        if(exportOsc) addVectorSelectorOsc(name);
//        
//        
//        if(exportOsc) addSliderOsc(name);
    }
    
    void addVectorSelectorOsc(string name){
        
        
        
        
        
        /*
        py+=0.1;
        px = 0.1;
        
        
        ofxTouchOscMultiFader *multifader = page->addMultiFader(name, px, py, 0.6, 0.2);
        multifader->setCentered(true);
        multifader->setInverted(false);
        multifader->setNumber(3);
        
        py+=0.1;
        
        // override default osc address
        multifader->setOscAddress("/control/vector/"+name);
        
        lastToggle = false;
         */
        
    }

    
    
    ///////
    
    void exportLayoutOsc(string layoutName){
        
        // save to file (appears in data folder).
        // it will automatically launch TouchOSC (if you have it installed)
        // and open the layout
        touchOsc.save(layoutName);
        
        // or grab the raw xml
//        string xml = touchOsc.getXml();
//        cout << xml << endl;
        
        
    }
    
};

