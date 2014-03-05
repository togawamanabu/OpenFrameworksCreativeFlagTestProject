#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "RockShape.h"
#include "ofxSvg.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    ofShader shader;	//Shader
	ofFbo fbo;			//First buffer for input image
	ofFbo fbo2;			//Second buffer for mask
    
    ofxBox2dCircle fingercircle;
    int fingerR;
	
	ofImage backgroundImage;
    ofImage testImage;

    vector <ofImage> images;
    
    vector <ofPtr<RockShape> > rocks;
    vector <ofPtr<ofxBox2dRect> >	boxes;
    vector <ofPtr<ofxBox2dRect> >	walls;
    
    vector <ofxSVG> rockSvgs;
    
    ofxBox2d box2d;
        
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
};


