//
//  Boid.cpp
//  oSitioHORIZONTES
//
//  Created by Tiago Brizolara on 10/3/17.
//
//

#include "Boid.h"
#include "ofMain.h" // apenas para ofGetMouseX e Y...

Boid::Boid(float x, float y, ofPoint& p_leftStop, ofPoint& p_rightStop, float p_yThreshold) {
    acc = ofVec3f::ofVec3f(0,0);
    vel = ofVec3f::ofVec3f(ofRandom(-1,1),ofRandom(-1,1));
    pos = ofVec3f::ofVec3f(x,y);
    r = 2.0;
    
    //  [ADi]
    state = stateChase;
    rEnterFlee_sq = 120*120;
    rExitFlee_sq  = 500*500;
    yThreshold = p_yThreshold;//0.6*height;
    leftStop  = ofVec3f::ofVec3f(p_leftStop.x, p_leftStop.y);//width/10.0, height/10.0);
    rightStop = ofVec3f::ofVec3f(p_rightStop.x, p_rightStop.y);//9.0*width/10.0, height/10.0);

}

void Boid::run(vector<Boid>& boids, float a_targetX, float a_targetY)
{
    target.x = a_targetX;
    target.y = a_targetY;
    //flock(boids);
    quero_quero();  //  ADi
    update();
    //borders();
    //render();
}
    
void Boid::applyForce(ofPoint& force) {
    // We could add mass here if we want A = F / M
    acc += force;
}
    
    // We accumulate a new acceleration each time based on three rules
void Boid::flock(vector<Boid>& boids) {
    calcSeparate(boids);   // Separation
    calcAlign(boids);      // Alignment
    calcCohesion(boids);   // Cohesion
    // Arbitrarily weight these forces
    this->separate *= swt;
    this->align    *= awt;
    this->cohere   *= cwt;
    // Add the force vectors to acceleration
    applyForce(this->separate);
    applyForce(this->align);
    applyForce(this->cohere);
}
    
//
//  [ADi]
void Boid::quero_quero()
{
    
    float sqrDist = (pos.x-target.x)*(pos.x-target.x) + (pos.y-target.y)*(pos.y-target.y);
    //  Inside enter flee area
    if( sqrDist < rEnterFlee_sq )
    {
        state = stateFlee;
        //cout << "flee"<< endl;
        //applyForce( new PVector(0.0,-10.0*maxforce).mult(chaseWt) );
        calcSeek(target,this->seek);
        //cout << this->seek << endl;
        this->seek *= -chaseWt;
    }
    /*else if(sqrDist < rEnterFlee_sq) ?
    {
        //  Minor than exit flee distance, but still in flee state
        applyForce( seek(target).mult(-chaseWt) );
    }*/
    else if(state == stateFlee){  //  Great than exit flee distance --> go to a stop point...
        if(vel.x < 0)  {
            float sqrDist2 = (pos.x-leftStop.x)*(pos.x-leftStop.x) + (pos.y-leftStop.y)*(pos.y-leftStop.y);
            calcSeek(leftStop,this->seek);
            //cout << this->seek << endl;
            this->seek *= chaseWt;
            if(sqrDist2 < 40*40) {
                state = stateChase;
                //cout << "chase" << endl;
            }
        }
        else {
            //applyForce( seek(rightStop).mult(chaseWt) );?
            
            float sqrDist2 = (pos.x-rightStop.x)*(pos.x-rightStop.x) + (pos.y-rightStop.y)*(pos.y-rightStop.y);
            calcSeek(rightStop,this->seek);
            //cout << this->seek << endl;
            this->seek *= chaseWt;
            if(sqrDist2 < 40*40) {
                state = stateChase;
                //cout << "chase"<< endl;
            }
        }
    }
    else
    {  //  ... or return to seek target
        state = stateChase;
        //cout << "chase..."<< endl;
        calcSeek(target,this->seek);
        //cout << this->seek << endl;
        this->seek *= chaseWt;
        //cout << this->seek << endl;
    }
    
    //cout << this->seek << endl;
    
    applyForce( this->seek );
    
    //  Always avoids ground
    if(pos.y > yThreshold) {
        state = stateFlee;
        ofPoint vUp(0.0, -0.1);
        vUp *= (this->pos.y-yThreshold);
        applyForce( vUp );
    }
}
    
    // Method to update position
void Boid::update() {
    // Update velocity
    vel += acc;
    // Limit speed
    vel.limit(maxspeed);
    pos += vel;
    // Reset accelertion to 0 each cycle
    acc *= 0.0f;
}
    
    // A method that calculates and applies a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
void Boid::calcSeek(ofPoint& target, ofPoint& out_steer)
{
    ofPoint desired = target - this->pos;  // A vector pointing from the position to the target
    
    // Normalize desired and scale to maximum speed
    desired.normalize();
    desired *= maxspeed;
    // Steering = Desired minus Velocity
    out_steer = desired - vel;
    out_steer.limit(maxforce);  // Limit to maximum steering force
    
    //return out_steer;
}
    
void Boid::render() {
    // Draw a triangle rotated in the direction of velocity
    /*float theta = vel.heading2D() + radians(90);
    fill(175);
    stroke(0);
    pushMatrix();
    translate(pos.x,pos.y);
    rotate(theta);
    beginShape(TRIANGLES);
    vertex(0, -r*2);
    vertex(-r, r*2);
    vertex(r, r*2);
    endShape();
    popMatrix();*/
}

// Wraparound
/*void borders() {
    if (pos.x < -r) pos.x = width+r;
    if (pos.y < -r) pos.y = height+r;
    if (pos.x > width+r) pos.x = -r;
    if (pos.y > height+r) pos.y = -r;
}*/

// Separation
// Method checks for nearby boids and steers away
void Boid::calcSeparate (vector<Boid>& boids)
{
    float desiredseparation = 25.0;
    this->separate *= 0.0f;
    int count = 0;
    // For every boid in the system, check if it's too close
    for (Boid other : boids) {
        float d = this->pos.distance(other.pos);
        // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
        if ((d > 0) && (d < desiredseparation)) {
            // Calculate vector pointing away from neighbor
            ofPoint diff = this->pos - other.pos;
            diff.normalize();
            diff /= d;        // Weight by distance
            this->separate += diff;
            count++;            // Keep track of how many
        }
    }
    // Average -- divide by how many
    if (count > 0) {
        this->separate /= (float)count;
        // Implement Reynolds: Steering = Desired - Velocity
        this->separate.normalize();
        this->separate *= maxspeed;
        this->separate -= vel;
        this->separate.limit(maxforce);
    }
    //return steer;
}

// Alignment
// For every nearby boid in the system, calculate the average velocity
void Boid::calcAlign (vector<Boid>& boids) {
    float neighbordist_sqr = 50.0*50.0f;
    this->align *= 0.0f;
    int count = 0;
    for (Boid other : boids) {
        float d2 = this->pos.squareDistance(other.pos);
        if ((d2 > 0) && (d2 < neighbordist_sqr)) {
            this->align += other.vel;
            count++;
        }
    }
    if (count > 0) {
        this->align /= (float)count;
        // Implement Reynolds: Steering = Desired - Velocity
        this->align.normalize();
        this->align *= maxspeed;
        this->align -= vel;
        this->align.limit(maxforce);
    }
    //return steer;
}
    
// Cohesion
// For the average position (i.e. center) of all nearby boids, calculate steering vector towards that position
void Boid::calcCohesion (vector<Boid>& boids) {
    float neighbordist_sqr = 50.0*50.0;
    this->cohere *= 0.0f;   // Start with empty vector to accumulate all positions
    int count = 0;
    for (Boid other : boids) {
        float d2 = this->pos.squareDistance(other.pos);
        if ((d2 > 0) && (d2 < neighbordist_sqr)) {
            this->cohere += other.pos; // Add position
            count++;
        }
    }
    if (count > 0) {
        this->cohere /= (float)count;
        calcSeek(this->cohere, this->cohere);
        //return seek(sum);  // Steer towards the position
    }
    //return sum;
}

ofPoint Boid::getTarget()
{
    return this->target;
}

