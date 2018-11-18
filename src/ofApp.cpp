
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
static int num = 0;

#include "ofApp.h"
void Joint::draw() {
	//   get the current transformation matrix for this object
   //
	glm::mat4 m = getMatrix();

	//   push the current stack matrix and multiply by this object's
	//   matrix. now all vertices dran will be transformed by this matrix
	//
	ofPushMatrix();
	ofMultMatrix(m);
	ofDrawSphere(radius);
	ofPopMatrix();

	

	for (int i = 0; i < childList.size(); i++) {
		ofSetColor(ofColor::gold);
		//r = Ray(getPosition(), childList[i]->getPosition());
		//r.draw(10);
		childP = childList[i]->getPosition();
		ofDrawArrow(getPosition(), childP);


		//glm::vec3 PtoC = getPosition() - childList[i]->getPosition();
		//float len = glm::length(PtoC);
		//ofDrawCone(radius *.75f, len - (radius * 2));
		//ofDrawCone(getPosition() * .75f, radius, len - (radius + radius));
	}


	// draw axis
	//
	ofApp::drawAxis(m, 1.5);

}


//--------------------------------------------------------------
//
void ofApp::setup() {


	ofSetBackgroundColor(ofColor::black);
	ofEnableDepthTest();
	mainCam.setDistance(15);
	mainCam.setNearClip(.1);
	
	sideCam.setPosition(40, 0, 0);
	sideCam.lookAt(glm::vec3(0, 0, 0));
	topCam.setNearClip(.1);
	topCam.setPosition(0, 16, 0);
	topCam.lookAt(glm::vec3(0, 0, 0));
	ofSetSmoothLighting(true);


	// setup one point light
	//
	light1.enable();
	light1.setPosition(5, 5, 0);
	light1.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	light1.setSpecularColor(ofColor(255.f, 255.f, 255.f));

	theCam = &mainCam;

	//  create a scene consisting of a ground plane with 2x2 blocks
	//  arranged in semi-random positions, scales and rotations
	//
	// ground plane
	//
	scene.push_back(new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0)));   
	//scene.push_back(new Joint(glm::vec3(0, 2, 0), 1)); //test creation of joint

}

 
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	theCam->begin();
	ofNoFill();
	drawAxis();
	ofEnableLighting();

	//  draw the objects in scene
	//
	material.begin();
	ofFill();

	// Draws parent object and highlights them when selected
	//
	for (int i = 0; i < scene.size(); i++) {
		if (objSelected() && scene[i] == selected[0])
			ofSetColor(ofColor::white);
		else ofSetColor(scene[i]->diffuseColor);
		scene[i]->draw();

		// Highlights child object selected
		//
		for (int j = 0; j < scene[i]->childList.size(); j++)
		{
			if (objSelected() && scene[i]->childList[j] == selected[0])
				ofSetColor(ofColor::white);
			else ofSetColor(scene[i]->childList[j]->diffuseColor);
			//scene[i]->childList[j]->draw();

		}
	}

	material.end();
	ofDisableLighting();
	theCam->end();
}

void ofApp::saveToFile() {
	FILE *fp = fopen("data/myscene.cmd", "w");

	for (int i = 0; i < scene.size(); i++) {
		glm::vec3 r = scene[i]->rotation;
		glm::vec3 t = scene[i]->getPosition();
		string name = scene[i]->name.c_str();
		
		//printf("%s \n", scene[i]->name.c_str());
		printf("<%d %d %d> \n",
			scene[i]->rotation.x, scene[i]->rotation.y, scene[i]->rotation.z);

		if (scene[i]->parent != NULL) {
			//"-create -joint %s -rotate <%f, %f, %f> -translate <%f, %f, %f> -parent %s\n

			fprintf_s(fp, "-create -joint %s -rotate <%f, %f, %f> -translate <%f, %f, %f> -parent %s\n",
				name.c_str() , r.x, r.y, r.z, t.x, t.y, t.z, scene[i]->parent->name.c_str());
		}
		else {
			//-create -joint %s -rotate <%f, %f, %f> -translate <%f, %f, %f> -parent %s\n

			fprintf_s(fp, "-create -joint %s -rotate <%f, %f, %f> -translate <%f, %f, %f> -parent %s\n",
				name.c_str() , r.x, r.y, r.z, t.x, t.y, t.z, NULL);
		}

	}

	fclose(fp);
}

void ofApp::loadFromFile() {
	FILE *fp = fopen("data/myscene.cmd", "r");
	vector<SceneObject *> sceneCopy;
	char p[100];
	char c[100];
	float r1;
	float r2;
	float r3;
	float t1;
	float t2;
	float t3;

	num = 0;
	sceneCopy.push_back(new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0)));

	//rewind(fp); 

//	for (int i = 0; i < scene.size(); i++) {
	//fscanf(fp, "%s", test);
	while (fscanf(fp, "-create -joint %s -rotate <%f, %f, %f> -translate <%f, %f, %f> -parent %s \n"
		, &c, &r1, &r2, &r3, &t1, &t2, &t3, &p) != EOF) 
	{
		printf_s("-create -joint %s -rotate <%f, %f, %f> -translate <%f, %f, %f> -parent %s\n",
			c, r1, r2, r3, t1, t2, t3, p);

		sceneCopy.push_back(new Joint(glm::vec3(t1, t2, t3), 1)); //Initialize position

		//Sleep(100);
		sceneCopy[sceneCopy.size() - 1]->name = c; //Initialize name 
		sceneCopy[sceneCopy.size() - 1]->rotation = glm::vec3(r1, r2, r3); // Initialize rotated value
		
		//for (int i = 0; i < sceneCopy.size() - 1; i++) {
		//	if (sceneCopy[i]->parent->name == sceneCopy[sceneCopy.size() - 1]->parent->name) {
		//		sceneCopy[sceneCopy.size() - 1]->parent = sceneCopy[i]->parent;
		//	}

		//}
		
		num++;
	}

//	}

	fclose(fp);

	
	for (int i = 1; i < scene.size(); i++) {
		scene[i] = sceneCopy[i];
	}

	/* Load each joint's rotate and translate values in pushback and set the name */
	// num = 0;
	//scene.push_back(new Joint(glm::vec3(0, 2, 0), 1));
	//scene[scene.size() - 1]->name = "Joint" + std::to_string(num);
	
}

// 
// Draw an XYZ axis in RGB at transform
//
void ofApp::drawAxis(glm::mat4 m, float len) {

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(len, 0, 0, 1)));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(0, len, 0, 1)));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(0, 0, len, 1)));
}

// print C++ code for obj tranformation channels. (for debugging);
//
void ofApp::printChannels(SceneObject *obj) {
	cout << "position = glm::vec3(" << obj->position.x << "," << obj->position.y << "," << obj->position.z << ");" << endl;
	cout << "rotation = glm::vec3(" << obj->rotation.x << "," << obj->rotation.y << "," << obj->rotation.z << ");" << endl;
	cout << "scale = glm::vec3(" << obj->scale.x << "," << obj->scale.y << "," << obj->scale.z << ");" << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	switch (key) {
	case OF_KEY_ALT:
		bAltKeyDown = false;
		mainCam.disableMouseInput();
		break;
	case 'A':
	case 'a':
		bRotateX = false;
		break;
	case 'S':
	case 's':
		bRotateY = false;
		break;
	case 'D':
	case 'd':
		bRotateZ = false;
		break;
	case 'J':
	case 'j':
		child = false;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'C':
	case 'c':
		if (mainCam.getMouseInputEnabled()) mainCam.disableMouseInput();
		else mainCam.enableMouseInput();
		break;
	case 'F':
	case 'b':
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 'h':
		bHide = !bHide;
		break;
	case 'j':
		child = true;
		break;
	case 'n':
		break;
	case 'p':
		if (objSelected()) printChannels(selected[0]);
		break;
	case 'r':
		break;
	case 'A':
	case 'a':
		bRotateX = true;
		break;
	case 'S':
	case 's':
		bRotateY = true;
		break;
	case 'D':
	case 'd':
		bRotateZ = true;
		break;
	case 'O':
	case 'o':
		saveToFile();
		break;
	case 'I':
	case 'i':
		loadFromFile();
		break;
	case OF_KEY_F1: 
		theCam = &mainCam;
		break;
	case OF_KEY_F2:
		theCam = &sideCam;
		break;
	case OF_KEY_F3:
		theCam = &topCam;
		break;
	case OF_KEY_ALT:
		bAltKeyDown = true;
		if (!mainCam.getMouseInputEnabled()) mainCam.enableMouseInput();
		break;
	case ' ': //Create new joints dynamically 
		num++;
//		name = "Joint " + std::to_string(num);
//		printf_s("%s \n", name.c_str());

		if (objSelected() && child ) {
			selected[0]->addChild(new Joint(glm::vec3(0, 2, 0), 1));
			selected[0]->childList[selected[0]->childList.size() - 1]->name = "Joint" + std::to_string(num);
			scene.push_back(selected[0]->childList[selected[0]->childList.size() - 1]);
		}
		else {
			scene.push_back(new Joint(glm::vec3(0, 2, 0), 1));
			scene[scene.size() - 1]->name = "Joint" + std::to_string(num);
		}
		break;
	case OF_KEY_BACKSPACE:
		deleteJoint();
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::deleteJoint() {

	//Deletes parent joint
	for (int i = 0; i < scene.size(); i++) {
		if (objSelected() && scene[i] == selected[0]) {

			if (scene[i]->parent != NULL) {

				/* If selected joint has a parent then parent should inherit
				the selected joint's children */
				for (int j = 0; j < scene[i]->childList.size(); j++) {
					scene[i]->parent->addChild(scene[i]->childList[j]);
				}
				//scene[i]->parent->childList = scene[i]->childList;		
			}
			scene.erase(scene.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	if (objSelected() && bDrag) {
		glm::vec3 point; 
		mouseToDragPlane(x, y, point);
		if (bRotateX) {
			selected[0]->rotation += glm::vec3((point.x - lastPoint.x) * 20.0, 0, 0);
		}
		else if (bRotateY) {
			selected[0]->rotation += glm::vec3(0, (point.x - lastPoint.x) * 20.0, 0);
		}
		else if (bRotateZ) {
			selected[0]->rotation += glm::vec3(0, 0, (point.x - lastPoint.x) * 20.0);
		}
		else {
			selected[0]->position += (point - lastPoint);
		}
		lastPoint = point;
	}

}

//  This projects the mouse point in screen space (x, y) to a 3D point on a plane
//  normal to the view axis of the camera passing through the point of the selected object.
//  If no object selected, the plane passing through the world origin is used.
//
bool ofApp::mouseToDragPlane(int x, int y, glm::vec3 &point) {
	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	float dist;
	glm::vec3 pos;
	if (objSelected()) {
		pos = selected[0]->position;
	}
	else pos = glm::vec3(0, 0, 0);
	if (glm::intersectRayPlane(p, dn, pos, glm::normalize(theCam->getZAxis()), dist)) {
		point = p + dn * dist;
		return true;
	}
	return false;
}

//--------------------------------------------------------------
//
// Provides functionality of single selection and if something is already selected,
// sets up state for translation/rotation of object using mouse.
//
void ofApp::mousePressed(int x, int y, int button){

	// if we are moving the camera around, don't allow selection
	//
	if (mainCam.getMouseInputEnabled()) return;

	// clear selection list
	//
	selected.clear();

	//
	// test if something selected
	//
	vector<SceneObject *> hits;

	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	// check for selection of scene objects
	//
	for (int i = 0; i < scene.size(); i++) {
		
		glm::vec3 point, norm;
		
		//  We hit an object
		//
		if (scene[i]->isSelectable && scene[i]->intersect(Ray(p, dn), point, norm)) {
			hits.push_back(scene[i]);
		}
	}


	// if we selected more than one, pick nearest
	//
	SceneObject *selectedObj = NULL;
	if (hits.size() > 0) {
		selectedObj = hits[0];
		float nearestDist = std::numeric_limits<float>::infinity();
		for (int n = 0; n < hits.size(); n++) {
			float dist = glm::length(hits[n]->position - theCam->getPosition());
			if (dist < nearestDist) {
				nearestDist = dist;
				selectedObj = hits[n];
			}	
		}
	}
	if (selectedObj) {
		selected.push_back(selectedObj);
		bDrag = true;
		mouseToDragPlane(x, y, lastPoint);
	}
	else {
		selected.clear();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bDrag = false;

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

