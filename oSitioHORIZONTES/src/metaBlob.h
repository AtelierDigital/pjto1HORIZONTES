//
//  metaBlob.h
//  expoYARA
//
//  Created by Kaue Costa on 9/13/17.
//
//

#ifndef __expoYARA__metaBlob__
#define __expoYARA__metaBlob__

#include <stdio.h>
#include "ofMain.h"

class Blob {
    
    
public:
    
    
    Blob(float _x, float _y, float _vx, float _vy, float _bs);
    
    float getx();
    float gety();
    float getbs();
    
    void update(int num, float tx, float ty, float pX, float pY);
    
    
    
    float blobsize;
    float force=.001;
    float fluid=.005;
    float forcerad=120;
    float x, y, vx, vy;
    
    float w, h;
    
    float pX, pY;
    float scale;
    bool pressed;
    bool direction;
    
    int bx[1920];
    int by[1080];
    
};








#endif /* defined(__expoYARA__metaBlob__) */
