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
    
	void setup(ofxBox2d &world, float cx, float cy, ofxSVG * _svg, ofImage * _image) {
        svg = _svg;
        image = _image;
        texture.allocate(_image->width, _image->height, GL_RGB);
        texture.loadData(_image->getPixels(), _image->width, _image->height, GL_RGB);
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
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
                }
            }
        }
        
        float min_x, max_x, min_y, max_y;
        calcMinMaxPos(min_x, max_x, min_y, max_y);
        ofPoint center = getCenter(min_x, max_x, min_y, max_y);
        
        vector<ofPoint> &pts = polyShape.getPoints();
        
        for(int i=0; i<pts.size(); i++) {
            ofPoint p = pts[i];
            mesh.addVertex(center);
            mesh.addVertex(p);

        }
        mesh.addVertex(center);
        mesh.addVertex(pts.front());
        
        createTextureMap();
        
        polyShape.setPhysics(10, 0.3, 0.1);
    	polyShape.create(world.getWorld());
    }
    
    void draw(int pos_x, int pos_y) {
        mesh.clearVertices();
        
        float min_x, max_x, min_y, max_y;
        calcMinMaxPos(min_x, max_x, min_y, max_y);
        ofPoint center = getCenter(min_x, max_x, min_y, max_y);
        
        vector<ofPoint> &pts = polyShape.getPoints();
        
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
        
        
        //for debug
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
    }
    
    
    void createTextureMap() {
        mesh.clearTexCoords();
        
        vector<ofPoint> &pts = polyShape.getPoints();
        
        float min_x, max_x, min_y, max_y;
        calcMinMaxPos(min_x, max_x, min_y, max_y);
        ofPoint vecCenter = getCenter(min_x, max_x, min_y, max_y);
        
        float rw = max_x - min_x;
        float rh = max_y - min_y;
        
        float iw = texture.getWidth();
        float ih = texture.getHeight();
        
        float scaled_h, scaled_w;
        
        if(ih < iw) {
            float s = rh/rw;
            scaled_w = iw;
            scaled_h = iw * s;
            if(ih < scaled_h) {
                scaled_w = ih/s;
                scaled_h = ih;
            }
        } else {
            float s = rw/rh;
            scaled_h = ih;
            scaled_w = ih * s;
            if(iw < scaled_w) {
                scaled_w = iw;
                scaled_h = iw/s;
            }
        }
        
        printf("center_x %f, center_y %f, mm_center_x %f, mm_center_y %f\n ", vecCenter.x, vecCenter.y, (max_x - min_x)/2.0 + min_x, (max_y - min_y)/2.0 + min_y);
        
        bool isfirst = true;
        float first_tx, first_ty;
        ofPoint texCenter = ofPoint(iw/2.0, ih/2.0);
        for(int i=0; i<pts.size(); i++) {
            ofPoint p = pts[i];
            float dx = (p.x- vecCenter.x);
            float dy = (p.y - vecCenter.y);
            
            float tx = texCenter.x + ofMap(dx, -rw/2.0, rw/2.0, -scaled_w/2.0, scaled_w/2.0);
            float ty = texCenter.y + ofMap(dy, -rh/2.0, rh/2.0, -scaled_h/2.0, scaled_h/2.0);
            
            mesh.addTexCoord(texCenter);
            mesh.addTexCoord(ofPoint(tx, ty));
            
            if(isfirst) {
                first_tx = tx;
                first_ty = ty;
                isfirst = false;
            }
        }
        mesh.addTexCoord(texCenter);
        mesh.addTexCoord(ofPoint(first_tx, first_ty));
        
    }
    
    void calcMinMaxPos(float &min_x, float &max_x, float &min_y, float &max_y) {
        min_x = 10000;
        max_x = -1;
        min_y = 100000;
        max_y = -1;
        vector<ofPoint> &pts = polyShape.getPoints();
        
        for(int i=0; i<pts.size(); i++) {
            ofPoint p = pts[i];
            if(p.x < min_x ) min_x = p.x;
            if(p.y < min_y ) min_y = p.y;
            if(max_x < p.x) max_x = p.x;
            if(max_y < p.y) max_y = p.y;
        }
    }
    
    ofPoint getCenter(float min_x, float max_x, float min_y, float max_y) {
        return ofPoint(min_x + (max_x - min_x)/2.0, min_y + (max_y - min_y)/2.0);
    }

    
    void destroy() {
        polyShape.destroy();
    }
};

