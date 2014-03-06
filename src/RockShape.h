//
//  RockShape.h
//  mySketch
//
//  Created by togawa manabu on 2014/02/27.
//
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxSvg.h"

class RockShape {
    
public:
    RockShape() {
    }
    
    ofImage    *    image;
    ofMesh          mesh;
    ofColor         color;
    ofxBox2dPolygon polyShape;
    ofxSVG     *    svg;
    vector <ofPoint> points;
    vector <ofPoint> tex_points;
    ofImage img;
    
    ofTexture texture;
    
    float min_x, min_y, max_x, max_y;
    float center_diff_x, center_diff_y;
    
	void setup(ofxBox2d &world, float cx, float cy, ofxSVG * _svg, ofImage * _image) {
        svg = _svg;
        image = _image;
        texture.allocate(_image->width, _image->height, GL_RGB);
        texture.loadData(_image->getPixels(), _image->width, _image->height, GL_RGB);
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        min_x = 100000;
        min_y = 100000;
        max_x = -1;
        max_y = -1;
        
        for (int i = 0;i < svg->getNumPath();i++){
            ofPath p = svg->getPathAt(i);
            p.setPolyWindingMode(OF_POLY_WINDING_ODD);
            
            vector<ofPolyline> vpl = p.getOutline();
            
            for(int z = 0; z < vpl.size(); z++) {
                ofPolyline pl = vpl[z];
                vector<ofPoint> vp = pl.getVertices();
                
                int step = vp.size() / 8; //TODO:20
                
                for(int x=0; x<vp.size(); x = x+step) {
                    float d = ofRandom(0, 20);
                    ofPoint point = ofPoint(vp[x].x + cx , vp[x].y + cy);
                    
                    polyShape.addVertex(point);
                    
                    points.push_back(point);
                    
                    if(point.x < min_x ) min_x = point.x;
                    if(point.y < min_y ) min_y = point.y;
                    if(max_x < point.x) max_x = point.x;
                    if(max_y < point.y) max_y = point.y;
                }
            }
        }
        
        float iw = texture.getWidth();
        float ih = texture.getHeight();
        float rw = max_x - min_x;
        float rh = max_y - min_y;
        
        
        float image_start_pos_x = 0;
        float image_start_pos_y = 0;
        float image_end_pos_x = iw;
        float image_end_pos_y = ih;
        
        float scaled_image_w;
        float scaled_image_h;
        
        float scale;
        if(rh < rw) {
            float scale = rh / rw;
            if(iw * scale < ih) {
                scaled_image_h = iw * scale;
                scaled_image_w = iw;
                image_start_pos_x = 0;
                image_start_pos_y = (ih - scaled_image_h) / 2.0;
            } else {
                scaled_image_h = ih;
                scaled_image_w = ih / scale;
                image_start_pos_x = (iw - scaled_image_w) / 2.0;
                image_start_pos_y = 0;
            }
        } else {
            float scale = rw / rh;
            if(ih * scale < iw) {
                scaled_image_w = ih * scale;
                scaled_image_h = ih;
                image_start_pos_x = (iw - scaled_image_w) / 2.0;
                image_start_pos_y = 0;
            } else {
                scaled_image_w = iw;
                scaled_image_h = iw / scale;
                image_start_pos_x = 0;
                image_start_pos_y = (ih - scaled_image_h) / 2.0;
            }
        }
    
        
        ofPoint center = polyShape.getCentroid2D();
        ofPoint center_tex = ofPoint(iw/2.0, ih/2.0);
        
        vector<ofPoint> &pts = polyShape.getPoints();
        
        for(int i=0; i<pts.size(); i++) {
            ofPoint p = pts[i];
            float x1 = ofMap(p.x - min_x, 0, max_x - min_x, image_start_pos_x, image_start_pos_x + scaled_image_w);
            float y1 = ofMap(p.y - min_y, 0, max_y - min_y, image_start_pos_y, image_start_pos_y + scaled_image_h);
            ofPoint o = ofPoint(x1,  y1);
            
            mesh.addVertex(center);
            mesh.addVertex(p);
            
            mesh.addTexCoord(center_tex);
            mesh.addTexCoord(o);

        }
        mesh.addVertex(center);
        mesh.addVertex(points.front());
        
        mesh.addTexCoord(center_tex);
        ofPoint p = points.front();
        float x1 = ofMap(p.x - min_x, 0, max_x - min_x, image_start_pos_x, image_start_pos_x + scaled_image_w);
        float y1 = ofMap(p.y - min_y, 0, max_y - min_y, image_start_pos_y, image_start_pos_y + scaled_image_h);
        mesh.addTexCoord(ofPoint(x1, y1));
        
        polyShape.setPhysics(10, 0.3, 0.1);
    	polyShape.create(world.getWorld());
    }
    
    void draw(int pos_x, int pos_y) {
        mesh.clearVertices();
        
        vector<ofPoint> &pts = polyShape.getPoints();
        
        min_x = 100000;
        min_y = 100000;
        max_x = -1;
        max_y = -1;
        for(int i=0;i<pts.size(); i++) {
            ofPoint point = pts[i];
            if(point.x < min_x ) min_x = point.x;
            if(point.y < min_y ) min_y = point.y;
            if(max_x < point.x) max_x = point.x;
            if(max_y < point.y) max_y = point.y;
        }
        
        ofPoint center =  polyShape.getCentroid2D(); // ofPoint(min_x + (max_x - min_x) / 2.0, min_y + (max_y - min_y) / 2.0);
        
        for (int i=0; i<pts.size(); i++) {
            mesh.addVertex(center);
            mesh.addVertex(pts[i]);
        }
        mesh.addVertex(center);
        mesh.addVertex(pts.front());
        
        
        
        ofSetColor(255, 255, 255);
        texture.bind();
        mesh.draw();
        texture.unbind();
        mesh.drawWireframe();
        
        if(polyShape.inside(pos_x, pos_y)) {
            
            
            // texture test
            texture.draw(0, 0);
            
            float iw = texture.getWidth();
            float ih = texture.getHeight();
            
            float rw = max_x - min_x;
            float rh = max_y - min_y;
            
            
            float image_start_pos_x = 0;
            float image_start_pos_y = 0;
            
            float scaled_image_w;
            float scaled_image_h;
            
            float scale;
            if(rh < rw) {
                float scale = rh / rw;
            } else {
                float scale = rw / rh;
            }
            
            if(ih < iw) {
                scaled_image_h = ih;
                scaled_image_w = iw * scale;
            } else {
                scaled_image_w = iw;
                scaled_image_h = ih * scale;
            }

            
            ofPoint center_tex = ofPoint(scaled_image_w/2.0, scaled_image_h/2.0);
            ofRect(0, 0, scaled_image_w, scaled_image_h);
            
            for(int i=0;i<points.size();i++) {
                ofPoint p = ofPoint(points[i].x, points[i].y);
                ofPoint o = ofPoint(ofMap(p.x, min_x, max_x, image_start_pos_x, image_start_pos_x + scaled_image_w) , ofMap(p.y, min_y, max_y, image_start_pos_y, image_start_pos_y + scaled_image_h) );
                
                ofLine(center_tex.x, center_tex.y, o.x, o.y);
            }
            
            
            
            
            
            ofVec2f prevPoint;
            bool isFirst = true;
            for(int i=0; i < mesh.getTexCoords().size();i++) {
                ofVec2f p = mesh.getTexCoords()[i];

                if(!isFirst) {
                    ofLine(prevPoint.x, prevPoint.y, p.x, p.y);
                } 
                
                prevPoint = p;
                isFirst = false;
            }
            
            ofCircle(center.x, center.y, 10);
        }
        
//        ofPoint screenCenter = ofPoint(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
//        ofLine(screenCenter.x, screenCenter.y, center.x, center.y);
        
    }
    
    void destroy() {
        polyShape.destroy();
    }
};

