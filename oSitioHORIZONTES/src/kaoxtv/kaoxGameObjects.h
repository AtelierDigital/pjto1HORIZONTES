#ifndef gameOBJECTS_H
#define gameOBJECTS_H

//
//  kaoxGameObjects.h
//  ZappaMAPP
//
//  Created by Kaue Costa on 22/03/13.
//  Copyright 2013 kaox.tv. All rights reserved.
//

//Estruturar os Objetos
struct oxMPERect{
	int idx[4];
};

struct oxMPETriangle{
	int idx[3];
};



struct oxB2DPoly{
	int idx[32];
	int nPoints;
	float density, bounce, friction, triang;
	ofColor cor;
};

struct oxB2DEmitter{
    
	int idx;
	int qtd;
	int freq;
	int life;
	bool emitterMode;
	float density, bounce, friction;
	int particleType;
	
};
struct oxB2DForce{
    
	int idx;
	bool atractionMode;
	float power;
};


#endif // galeriaVIEW_H