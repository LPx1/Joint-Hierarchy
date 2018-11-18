
//
//  Starter file for Project 3 - Skeleton Builder
//
//  This file includes functionality that supports selection and translate/rotation
//  of scene objects using the mouse.
//
//  Modifer keys for rotatation are x, y and z keys (for each axis of rotation)
//
//  (c) Kevin M. Smith  - 24 September 2018
//
//
//   Student : Luis Pamintuan - 17 November 2018
//

#include "ofMain.h"
#include "box.h"
#include "Primitives.h"
#include "stdio.h"
#include <stdio.h>

class Joint : public Sphere {
public:
	Joint(glm::vec3 p, float r) {
		position = p;
		radius = r; 
		diffuseColor = ofColor::blue; 

	}
	Joint() {}

	void draw();
	glm::vec3 childP;
	Ray r = Ray(getPosition(),getPosition());
	
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		static void drawAxis(glm::mat4 transform = glm::mat4(1.0), float len = 1.0);
		bool mouseToDragPlane(int x, int y, glm::vec3 &point);
		void printChannels(SceneObject *);
		bool objSelected() { return (selected.size() ? true : false ); };
		void deleteJoint();
		void saveToFile();
		void loadFromFile();
		
		// Lights
		//
		ofLight light1;
	
		// Cameras
		//
		ofEasyCam  mainCam;
		ofCamera sideCam;
		ofCamera topCam;
		ofCamera  *theCam;    // set to current camera either mainCam or sideCam

		// Materials
		//
		ofMaterial material;


		// scene components
		//
		vector<SceneObject *> scene;
		vector<SceneObject *> selected;
		ofPlanePrimitive plane;
		

		// state
		bool bDrag = false;
		bool bHide = true;
		bool bAltKeyDown = false;
		bool bRotateX = false;
		bool bRotateY = false;
		bool bRotateZ = false;
		bool child = false;
		glm::vec3 lastPoint;
		
};
