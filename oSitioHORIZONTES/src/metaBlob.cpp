//
//  metaBlob.cpp
//  expoYARA
//
//  Created by Kaue Costa on 9/13/17.
//
//

#include "metaBlob.h"



Blob::Blob(float _x, float _y, float _vx, float _vy, float _bs) {
    
    x = _x;
    y = _y;
    vx = _vx;
    vy = _vy;
    blobsize=_bs;

    
}


float Blob::getx(){
    
    return x;
    
}


float Blob::gety(){
    
    return y;
    
}


float Blob::getbs(){
    
    return blobsize;
    
}

void Blob::update(int num, float tx, float ty, float _pX, float _pY){
    
    
    x+=vx;
    y+=vy;
    vx*=.995;
    vy*=.99;
    scale = .5;
    
    pX = _pX;
    pY = _pY;
    
    
    if (x<blobsize*.5 || x>w-blobsize*.5) vx=-vx*.9;
    if (y<blobsize*.5 || y>h-blobsize*.5) vy=-vy*.9;
    
    
    
//    for (int i=0; i<w; i++) bx[i] = int(sqrt(x-i));
//
//    for (int i=0; i<h; i++) by[i] = int(sqrt(y-i));
    
    
    
    
    if (pressed) {
        float dis = ofDist(x, y, pX*scale, pY*scale);
        if (dis < forcerad) {
            float ang = atan2(pY*scale-y, pX*scale-x);
            if (direction) {
                vx += dis * force * cos(ang);
                vy += dis * force * sin(ang);
            } else {
                vx -= dis * force * cos(ang);
                vy -= dis * force * sin(ang);
            }
        }
    }
    
    
    float radius = ofDist(x, y, tx, ty);
    
    if (radius < blobsize*11) {
        
        float ang = atan2(ty-y, tx-x);
        
        vx += radius * fluid * cos(ang);
        vy += radius * fluid * sin(ang);
        
        if (radius < blobsize*.5) {
            vx -= radius * fluid*3 * cos(ang);
            vy -= radius * fluid*3 * sin(ang);
        }
    }
}