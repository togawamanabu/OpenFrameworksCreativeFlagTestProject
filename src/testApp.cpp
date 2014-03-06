#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    //background image
    
    // find all images
    ofDirectory dir;

    int n = dir.listDir("creaters");
    for (int i=0; i<n; i++) {
        ofDirectory subdir;
        int m = subdir.listDir(dir.getPath(i));
        for(int j=0; j<m; j++) {
            images.push_back(ofImage(subdir.getPath(j)));
        }
    }
    printf("%i Textures Loaded\n", (int)images.size());
    
    n= dir.listDir("rocks");
    for (int i=0; i<n; i++) {
        ofxSVG svg;
        svg.load(dir.getPath(i));
        rockSvgs.push_back(svg);
    }
    printf("%i SVG Loaded\n", (int)rockSvgs.size());
    
   
    
    // Box2d
	box2d.init();
	box2d.setGravity(0, 5);
	box2d.createGround();
	box2d.setFPS(30.0);
    box2d.registerGrabbing();
    
    //create walls
    float w = 1;
    float h = 250;
    walls.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
    walls.back().get()->setPhysics(0.0, 0.50, 0.50);
    walls.back().get()->setup(box2d.getWorld(), 0, ofGetHeight()-h / 2, w, h);
    
    walls.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
    walls.back().get()->setPhysics(0.0, 0.50, 0.50);
    walls.back().get()->setup(box2d.getWorld(), ofGetWidth(), ofGetHeight()-h / 2, w, h);
    
    
    //draw first rocks
    for(int i=0; i<20; i++) {
        int x = ofRandom(ofGetWidth()/2.0, ofGetWidth()-50);
        rocks.push_back(ofPtr<RockShape>(new RockShape));
        rocks.back().get()->setup(box2d, x, 0, &rockSvgs[(int)ofRandom(rockSvgs.size())], &images[(int)ofRandom(images.size())]);
    }
    
    fingerR = 30;
    fingercircle = *new ofxBox2dCircle();
    fingercircle.setup(box2d.getWorld(), 0, 0, fingerR);
    fingercircle.setPhysics(0, 0.1, 0.1);    
}

//--------------------------------------------------------------
void testApp::update(){
    box2d.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient( ofColor( 255 ), ofColor( 128 ) );
    ofSetColor(255, 255, 255);
    backgroundImage.draw(ofGetWidth()/2.0, ofGetHeight()/2.0 );
    
    //draw wall
    for(int i=0; i<walls.size(); i++) {
		walls[i].get()->draw();
	}
    
    for(int i=0; i<rocks.size(); i++) {
		rocks[i].get()->draw(mouseX, mouseY);
	}
    
    // draw the ground
	box2d.drawGround();


    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
   
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
