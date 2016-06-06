#include "Ogre\ExampleApplication.h"
#include <ctime> 




struct Laser
{
	Ogre::SceneNode *nodeLaser;
	bool isActive;
	float timeCounter;
	float waitTime;
	Vector3 initialPosition;
}laser01,laser02,laser03,laser04;

void updateLaser(Laser &laser,const Ogre::FrameEvent &evt,float travelDistance,float speed){
	Vector3 laserPosition = laser.nodeLaser->getPosition();
	if (laser.isActive){
		laser.nodeLaser->setPosition(laserPosition.x,laserPosition.y,laserPosition.z + speed *  evt.timeSinceLastFrame);
		if (laserPosition.z > travelDistance){
			laser.isActive = false;
			laser.waitTime = rand() % 3;
			laser.timeCounter = 0;
			laser.nodeLaser->setVisible(false);
		}
	}
	else if (laser.timeCounter >= laser.waitTime){ 
	
		laser.isActive = true;
		laser.nodeLaser->setVisible(true);
		laser.nodeLaser->setPosition(laser.initialPosition);
		
	}

	laser.timeCounter += evt.timeSinceLastFrame;
	
};

Ogre::AnimationState* animationStateTurret301;
Ogre::AnimationState* animationStateTurret401;
Ogre::AnimationState* animationStateOpenWingsSpaceship1;
Ogre::AnimationState* animationStateOpenWingsSpaceship2;
Ogre::AnimationState* animationStateOpenWingsSpaceship3;
Ogre::AnimationState* animationStateOpenWingsSpaceship4;
Ogre::AnimationState* animationStateCloseWingsSpaceship1;
Ogre::AnimationState* animationStateCloseWingsSpaceship2;
Ogre::AnimationState* animationStateCloseWingsSpaceship3;
Ogre::AnimationState* animationStateCloseWingsSpaceship4;
Ogre::AnimationState* animationStateTurnSpaceshipRight;
Ogre::AnimationState* animationStateTurnSpaceshipLeft;
Ogre::AnimationState* animationStateTurnSpaceshipCenterFR;
Ogre::AnimationState* animationStateTurnSpaceshipCenterFL;

float x_pos = 0.0;
float z_pos = 0.0;
Ogre::SceneNode *nodePlayer;
Ogre::Real tr_counter = 0.0;
Ogre::Real tl_counter = 0.0;
float spaceship_speed = 0.8;

/*
	state: 0 for centered spaceship
		   1 for spaceship turned right
		   2 for spaceship turned left
*/
int state = 0;

bool open = false;
bool turnLeftNextFrame = false;
bool turnRightNextFrame = false;


class FrameListenerClase : public Ogre::FrameListener{

private:
	Ogre::SceneNode* _nodoF01;
	


	OIS::InputManager* _man;
	OIS::Keyboard* _key;
	OIS::Mouse* _mouse;
	Ogre::Camera* _cam;
	

public:
	FrameListenerClase(Ogre::Camera* cam, RenderWindow* win){

		//Configuracion para captura de teclado y mouse 
		size_t windowHnd = 0;
		std::stringstream windowHndStr;
		win->getCustomAttribute("WINDOW",&windowHnd);
		windowHndStr << windowHnd;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));

		//eventos
		_man = OIS::InputManager::createInputSystem(pl);
		_key = static_cast<OIS::Keyboard*>(_man->createInputObject(OIS::OISKeyboard,false));
		_mouse = static_cast<OIS::Mouse*>(_man->createInputObject(OIS::OISMouse,false));
		_cam = cam;
		nodePlayer->attachObject(_cam);
		
		
	}


	~FrameListenerClase(){
		_man->destroyInputObject(_key);
		_man->destroyInputObject(_mouse);
		OIS::InputManager::destroyInputSystem(_man);
	}

	bool frameStarted(const Ogre::FrameEvent &evt){
		float cam_speed = 10;
		
		_key->capture();
		_mouse->capture();

		if (_key->isKeyDown(OIS::KC_ESCAPE))
			return false;

		//Spaceship navigation
		if(_key->isKeyDown(OIS::KC_W)){
			if(z_pos > -1280) z_pos -= spaceship_speed;
			else {
				z_pos = 0.0;
				x_pos = 0.0;
			}

		}
		if(_key->isKeyDown(OIS::KC_A)){
			if(state == 0){
				if(x_pos > -23.8) x_pos -= 0.1;
				state = 2;
				tl_counter = 0.0;
				animationStateTurnSpaceshipLeft->setEnabled(true);
				animationStateTurnSpaceshipLeft->setTimePosition(0);
			} else if (state == 2){
				if(x_pos > -23.8) x_pos -= 0.1;
				tl_counter = 1.15;
			} else if (state == 1){
				animationStateTurnSpaceshipRight->setEnabled(false);
				animationStateTurnSpaceshipCenterFR->setEnabled(true);
				animationStateTurnSpaceshipCenterFR->setTimePosition(0);
				turnLeftNextFrame = true;
				state = -1;
				tl_counter = 0.0;
			}
		}
		if(_key->isKeyDown(OIS::KC_D)){
			if(state == 0){
				if(x_pos < 23.8) x_pos += 0.1;
				state = 1;
				tr_counter = 0.0;
				animationStateTurnSpaceshipRight->setEnabled(true);
				animationStateTurnSpaceshipRight->setTimePosition(0);
			} else if (state == 1){
				if(x_pos < 23.8) x_pos += 0.1;
				tr_counter = 1.15;
			} else if (state == 2){
				animationStateTurnSpaceshipLeft->setEnabled(false);
				animationStateTurnSpaceshipCenterFL->setEnabled(true);
				animationStateTurnSpaceshipCenterFL->setTimePosition(0);
				turnRightNextFrame = true;
				state = -1;
				tr_counter = 0.0;
			}
		}
		if(_key->isKeyDown(OIS::KC_E)){
			if(!open){
				animationStateCloseWingsSpaceship1->setEnabled(false);
				animationStateCloseWingsSpaceship2->setEnabled(false);
				animationStateCloseWingsSpaceship3->setEnabled(false);
				animationStateCloseWingsSpaceship4->setEnabled(false);
				animationStateOpenWingsSpaceship1->setEnabled(true);
				animationStateOpenWingsSpaceship2->setEnabled(true);
				animationStateOpenWingsSpaceship3->setEnabled(true);
				animationStateOpenWingsSpaceship4->setEnabled(true);
				animationStateOpenWingsSpaceship1->setTimePosition(0);
				animationStateOpenWingsSpaceship2->setTimePosition(0);
				animationStateOpenWingsSpaceship3->setTimePosition(0);
				animationStateOpenWingsSpaceship4->setTimePosition(0);
				open = true;
			}
		}
		if(_key->isKeyDown(OIS::KC_R)){
			if(open){
				animationStateOpenWingsSpaceship1->setEnabled(false);
				animationStateOpenWingsSpaceship2->setEnabled(false);
				animationStateOpenWingsSpaceship3->setEnabled(false);
				animationStateOpenWingsSpaceship4->setEnabled(false);
				animationStateCloseWingsSpaceship1->setEnabled(true);
				animationStateCloseWingsSpaceship2->setEnabled(true);
				animationStateCloseWingsSpaceship3->setEnabled(true);
				animationStateCloseWingsSpaceship4->setEnabled(true);
				animationStateCloseWingsSpaceship1->setTimePosition(0);
				animationStateCloseWingsSpaceship2->setTimePosition(0);
				animationStateCloseWingsSpaceship3->setTimePosition(0);
				animationStateCloseWingsSpaceship4->setTimePosition(0);
				open = false;
			}
		}

		if(turnRightNextFrame){
			animationStateTurnSpaceshipCenterFL->setEnabled(false);
			animationStateTurnSpaceshipRight->setEnabled(true);
			animationStateTurnSpaceshipRight->setTimePosition(0);
			turnRightNextFrame = false;
			state = 1;
			tr_counter = 0.0;
		}

		if(turnLeftNextFrame){
			animationStateTurnSpaceshipCenterFR->setEnabled(false);
			animationStateTurnSpaceshipLeft->setEnabled(true);
			animationStateTurnSpaceshipLeft->setTimePosition(0);
			turnLeftNextFrame = false;
			state = 2;
			tl_counter = 0.0;
		}

		//Turret 4 animation
		animationStateTurret401->addTime(evt.timeSinceLastFrame);

		//Laser movements
		updateLaser(laser01,evt,200,100);
		updateLaser(laser02,evt,400,200);
		updateLaser(laser03,evt,1500,350);
		updateLaser(laser04,evt,800,300);

		
		if (animationStateOpenWingsSpaceship1->getEnabled()){
			animationStateOpenWingsSpaceship1->addTime(evt.timeSinceLastFrame);
			animationStateOpenWingsSpaceship2->addTime(evt.timeSinceLastFrame);
			animationStateOpenWingsSpaceship3->addTime(evt.timeSinceLastFrame);
			animationStateOpenWingsSpaceship4->addTime(evt.timeSinceLastFrame);
		}
		if (animationStateCloseWingsSpaceship1->getEnabled()){
			animationStateCloseWingsSpaceship1->addTime(evt.timeSinceLastFrame);
			animationStateCloseWingsSpaceship2->addTime(evt.timeSinceLastFrame);
			animationStateCloseWingsSpaceship3->addTime(evt.timeSinceLastFrame);
			animationStateCloseWingsSpaceship4->addTime(evt.timeSinceLastFrame);
		}
		if(animationStateTurnSpaceshipRight->getEnabled()) animationStateTurnSpaceshipRight->addTime(evt.timeSinceLastFrame);
		if(animationStateTurnSpaceshipLeft->getEnabled()) animationStateTurnSpaceshipLeft->addTime(evt.timeSinceLastFrame);
		if(animationStateTurnSpaceshipCenterFR->getEnabled()) animationStateTurnSpaceshipCenterFR->addTime(evt.timeSinceLastFrame);
		if(animationStateTurnSpaceshipCenterFL->getEnabled()) animationStateTurnSpaceshipCenterFL->addTime(evt.timeSinceLastFrame);

		if(state == 0){
			
		} else if (state == 1) {
			if(tr_counter >= 1.3){
				state = 0;
				animationStateTurnSpaceshipRight->setEnabled(false);
				animationStateTurnSpaceshipCenterFR->setEnabled(true);
				animationStateTurnSpaceshipCenterFR->setTimePosition(0);
			} else {
				tr_counter += evt.timeSinceLastFrame;
			}
		} else if (state == 2) {
			if(tl_counter >= 1.3){
				state = 0;
				animationStateTurnSpaceshipLeft->setEnabled(false);
				animationStateTurnSpaceshipCenterFL->setEnabled(true);
				animationStateTurnSpaceshipCenterFL->setTimePosition(0);
			} else {
				tl_counter += evt.timeSinceLastFrame;
			}
		}

		nodePlayer->setPosition(x_pos, 0, z_pos);

		return true;
	}

};
class Example1 : public ExampleApplication
{

public:
	Ogre::FrameListener* FrameListener01;

	void createCamera() {

		mCamera = mSceneMgr->createCamera("MyCamera1");
		mCamera->setPosition(0,10,50);
		mCamera->lookAt(0,0,-50);
		mCamera->setNearClipDistance(5);

	}
	void createFrameListener(){
		FrameListener01 = new FrameListenerClase(mCamera,mWindow);
		mRoot->addFrameListener(FrameListener01);

	}

	ManualObject* generateFigureRightWing(String name, float g_x, float g_y, float g_z, float r_y, float f_h, float f_w1, float f_w2, float b_h, float b_w1,float b_w2, float d){

	 ManualObject* manual = mSceneMgr->createManualObject(name);
	  manual->begin("DarkGrey", RenderOperation::OT_TRIANGLE_STRIP);
	  
	  
	  //      position(  R ,    G    , B  )

	  manual->position( g_x+ -b_w1, g_y+ -b_h, g_z+ d);       //1
	  manual->position( g_x+ -b_w2, g_y+  b_h, g_z+ d);       //2 

	  manual->position( g_x+ -f_w1, r_y+ g_y+ -f_h, g_z+ -d); //3 
	  manual->position( g_x+ -f_w2,  r_y+ g_y+ f_h, g_z+ -d); //4 
	  manual->position( g_x+ f_w1,  r_y+ g_y+ -f_h, g_z+ -d); //5 
	  manual->position( g_x+ f_w2, r_y+ g_y+ f_h, g_z+ -d);   //6 

	  manual->position( g_x+ -b_w1, g_y+ -b_h, g_z+ d);       //1
	  manual->position( g_x+ -b_w2, g_y+  b_h, g_z+ d);       //2 
	  manual->position( g_x+ -f_w2,  r_y+ g_y+ f_h, g_z+ -d); //4 
	  manual->position( g_x+ f_w2, r_y+ g_y+ f_h, g_z+ -d);   //6 
	  manual->position( g_x+ f_w1,  r_y+ g_y+ -f_h, g_z+ -d); //5 
	  manual->position( g_x+ -b_w1, g_y+ -b_h, g_z+ d);       //1
	  manual->position( g_x+ -f_w1, r_y+ g_y+ -f_h, g_z+ -d); //3 

 
	  manual->end();
	  return manual;
	}

	ManualObject* generateFigureLeftWing(String name, float g_x, float g_y, float g_z, float r_y, float f_h, float f_w1, float f_w2, float b_h, float b_w1,float b_w2, float d){

	 ManualObject* manual = mSceneMgr->createManualObject(name);
	  manual->begin("DarkGrey", RenderOperation::OT_TRIANGLE_STRIP);
	  

	  //      position(  R ,    G    , B  )

	  manual->position( g_x+ b_w1, g_y+ -b_h, g_z+ d);        //1 
	  manual->position( g_x+ b_w2, g_y+ b_h, g_z+ d);         //2 

	  manual->position( g_x+ -f_w1, r_y+ g_y+ -f_h, g_z+ -d); //3 
	  manual->position( g_x+ -f_w2,  r_y+ g_y+ f_h, g_z+ -d); //4 
	  manual->position( g_x+ f_w1,  r_y+ g_y+ -f_h, g_z+ -d); //5 
	  manual->position( g_x+ f_w2, r_y+ g_y+ f_h, g_z+ -d);   //6 

	  manual->position( g_x+ b_w1, g_y+ -b_h, g_z+ d);        //1 
	  manual->position( g_x+ b_w2, g_y+ b_h, g_z+ d);         //2 
	  manual->position( g_x+ -f_w2,  r_y+ g_y+ f_h, g_z+ -d); //4 
	  manual->position( g_x+ f_w2, r_y+ g_y+ f_h, g_z+ -d);   //6 
	  manual->position( g_x+ f_w1,  r_y+ g_y+ -f_h, g_z+ -d); //5 
	  manual->position( g_x+ b_w1, g_y+ -b_h, g_z+ d);        //1 
	  manual->position( g_x+ -f_w1, r_y+ g_y+ -f_h, g_z+ -d); //3 

 
	  manual->end();

	  return manual;
	}

	ManualObject* generateParallelepiped(String name, float g_x, float g_y, float g_z, float r_y, float f_h, float f_w1, float f_w2, float b_h, float b_w1,float b_w2, float d){

	 /*
		Generates a manual object of some kind of parallelepiped that consists on two rectangles
		with different sizes and positions, but parallel that generates a volumetric figure

		input is
			name: name of the ManualObject object
			g_x, g_y, g_z: General x,y,z positions for the whole figure
			r_y: Relative position in the y axis of the front rectangle
			f_h, f_w1, f_w2: height, upper and down width of the front rectangle
			b_h, b_w1, b_w2: height, upper and down width of the back rectangle
			d: depth of the figure (on the Z axis)

		output is
			ManualOject object

	 */

	 ManualObject* manual = mSceneMgr->createManualObject(name);
	  manual->begin("DarkGrey", RenderOperation::OT_TRIANGLE_STRIP);
	  
	  
	  //      position(  R ,    G    , B  )

	  manual->position( g_x+ b_w1, g_y+ -b_h, g_z+ d);        //1 
	  manual->position( g_x+ b_w2, g_y+ b_h, g_z+ d);         //2 
	  manual->position( g_x+ -b_w1, g_y+ -b_h, g_z+ d);       //3 
	  manual->position( g_x+ -b_w2, g_y+  b_h, g_z+ d);       //4 

	  manual->position( g_x+ -f_w1, r_y+ g_y+ -f_h, g_z+ -d); //5 
	  manual->position( g_x+ -f_w2,  r_y+ g_y+ f_h, g_z+ -d); //6 
	  manual->position( g_x+ f_w1,  r_y+ g_y+ -f_h, g_z+ -d); //7 
	  manual->position( g_x+ f_w2, r_y+ g_y+ f_h, g_z+ -d);   //8 

	  manual->position( g_x+ b_w1, g_y+ -b_h, g_z+ d);        //1 
	  manual->position( g_x+ b_w2, g_y+ b_h, g_z+ d);         //2 
	  manual->position( g_x+ -b_w2, g_y+  b_h, g_z+ d);       //4 
	  manual->position( g_x+ f_w2, r_y+ g_y+ f_h, g_z+ -d);   //8 
	  manual->position( g_x+ -f_w2,  r_y+ g_y+ f_h, g_z+ -d); //6 
	  manual->position( g_x+ -f_w1, r_y+ g_y+ -f_h, g_z+ -d); //5 
	  manual->position( g_x+ -b_w1, g_y+ -b_h, g_z+ d);       //3 
	  manual->position( g_x+ f_w1,  r_y+ g_y+ -f_h, g_z+ -d); //7 
	  manual->position( g_x+ b_w1, g_y+ -b_h, g_z+ d);        //1 


 
	  manual->end();
	  return manual;
	}

	void createScene()
	{

		nodePlayer = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodePlayer");
		 
		mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		

		Ogre::Entity* ent01 = mSceneMgr->createEntity("MyEntity1","ejes01.mesh");
		Ogre::SceneNode* node01 = mSceneMgr->createSceneNode("Node01");
		mSceneMgr->getRootSceneNode()->addChild(node01);
		node01->attachObject(ent01);

		Ogre::Entity* entEscena01 = mSceneMgr->createEntity("ogre_base01.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena01);

		Ogre::Entity* entEscena02 = mSceneMgr->createEntity("ogre_fondo01.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena02);

		Ogre::Entity* entEscena03 = mSceneMgr->createEntity("ogre_pared01.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena03);

		Ogre::Entity* entEscena04 = mSceneMgr->createEntity("ogre_pared02.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena04);

		Ogre::Entity* entEscena05 = mSceneMgr->createEntity("ogre_pared03.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena05);

		Ogre::Entity* entEscena06 = mSceneMgr->createEntity("ogre_torretas01.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena06);

		mSceneMgr->setSkyDome(true, "StarSky", 5, 8);

		// Turret 1.
		Ogre::SceneNode *nodeTurret01 = mSceneMgr->createSceneNode();
		mSceneMgr->getRootSceneNode()->addChild(nodeTurret01);

		Ogre::Entity *subEntityTorret101 = mSceneMgr->createEntity("subEntityTorret101", "usb_cubomod01.mesh");
		subEntityTorret101->setMaterialName("DarkGrey");
		Ogre::SceneNode *subNodeTurret101 = mSceneMgr->createSceneNode();
		subNodeTurret101->attachObject(subEntityTorret101);
		subNodeTurret101->setPosition(0,0,0);

		Ogre::Entity *subEntityTorret102 = mSceneMgr->createEntity("subEntityTorret102", "cube.mesh");
		subEntityTorret102->setMaterialName("Grey");
		Ogre::SceneNode *subNodeTurret102 = mSceneMgr->createSceneNode();
		subNodeTurret102->attachObject(subEntityTorret102);
		subNodeTurret102->setPosition(0,5,0);
		subNodeTurret102->setScale(0.05,0.05,0.05);

		Ogre::Entity *subEntityTorret103 = mSceneMgr->createEntity("subEntityTorret103", "usb_cilindro02.mesh");
		subEntityTorret103->setMaterialName("Cannon");
		Ogre::SceneNode *subNodeTurret103 = mSceneMgr->createSceneNode();
		subNodeTurret103->attachObject(subEntityTorret103);
		subNodeTurret103->setPosition(0,5,4);
		subNodeTurret103->setScale(0.5,0.5,0.5);
		subNodeTurret103->pitch(Ogre::Degree(90.0f));
	  
		nodeTurret01->addChild(subNodeTurret101);
		nodeTurret01->addChild(subNodeTurret102);
		nodeTurret01->addChild(subNodeTurret103);
	 
		nodeTurret01->setPosition(-21,-7,-227);
		nodeTurret01->setScale(1.2,1.2,1.2);
		nodeTurret01->rotate(Quaternion (Degree(10), Vector3::UNIT_Y));

		// Turret 2.
		Ogre::SceneNode *nodeTurret02 = mSceneMgr->createSceneNode();
		mSceneMgr->getRootSceneNode()->addChild(nodeTurret02);

		Ogre::Entity *subEntityTorret201 = mSceneMgr->createEntity("subEntityTorret201", "usb_pipe.mesh");
		subEntityTorret201->setMaterialName("Grey");
		Ogre::SceneNode *subnodeTurret201 = mSceneMgr->createSceneNode();
		subnodeTurret201->attachObject(subEntityTorret201);
		subnodeTurret201->setPosition(0,0,0);
		subnodeTurret201->setScale(0.6,5.2,0.6);

		Ogre::Entity *subEntityTorret202 = mSceneMgr->createEntity("subEntityTorret202", "usb_pipe.mesh");
		subEntityTorret202->setMaterialName("DarkGrey");
		Ogre::SceneNode *subNodeTurret202 = mSceneMgr->createSceneNode();
		subNodeTurret202->attachObject(subEntityTorret202);
		subNodeTurret202->setPosition(0,5,0);
		subNodeTurret202->setScale(0.9,1,0.9);

		Ogre::Entity *subEntityTorret203 = mSceneMgr->createEntity("subEntityTorret203", "usb_pipe.mesh");
		subEntityTorret203->setMaterialName("DarkGrey");
		Ogre::SceneNode *subNodeTurret203 = mSceneMgr->createSceneNode();
		subNodeTurret203->attachObject(subEntityTorret203);
		subNodeTurret203->setPosition(0,-2,0);
		subNodeTurret203->setScale(0.9,1,0.9);

		Ogre::Entity *subEntityTorret204 = mSceneMgr->createEntity("subEntityTorret204", "usb_cilindro02.mesh");
		subEntityTorret204->setMaterialName("Cannon");
		Ogre::SceneNode *subNodeTurret204 = mSceneMgr->createSceneNode();
		subNodeTurret204->attachObject(subEntityTorret204);
		subNodeTurret204->setPosition(0,5.3,7);
		subNodeTurret204->setScale(0.4,0.7,0.4);
		subNodeTurret204->pitch(Ogre::Degree(90.0f));
	  
	  
		nodeTurret02->addChild(subnodeTurret201);
		nodeTurret02->addChild(subNodeTurret202);
		nodeTurret02->addChild(subNodeTurret203);
		nodeTurret02->addChild(subNodeTurret204);
		nodeTurret02->setPosition(22,-7,-407);
		nodeTurret02->rotate(Quaternion (Degree(-2.5), Vector3::UNIT_Y));

		// Turret 3.
		Ogre::SceneNode *nodeTurret03 = mSceneMgr->createSceneNode();
		mSceneMgr->getRootSceneNode()->addChild(nodeTurret03);

		Ogre::Entity *subEntityTorret301 = mSceneMgr->createEntity("subEntityTorret301", "usb_dodecaedro.mesh");
		subEntityTorret301->setMaterialName("Grey");
		Ogre::SceneNode *subnodeTurret301 = mSceneMgr->createSceneNode();
		subnodeTurret301->attachObject(subEntityTorret301);
		subnodeTurret301->setPosition(0,-5,0);
		subnodeTurret301->setScale(1,2.5,1);
	  
	  

		Ogre::Entity *subEntityTorret302 = mSceneMgr->createEntity("subEntityTorret302", "usb_torus.mesh");
		subEntityTorret302->setMaterialName("DarkBlue");
		Ogre::SceneNode *subNodeTurret302 = mSceneMgr->createSceneNode();
		subNodeTurret302->attachObject(subEntityTorret302);
		subNodeTurret302->setPosition(0,20,0);
		subNodeTurret302->setScale(2.5,2.5,2.5);
		subNodeTurret302->pitch(Ogre::Degree(90));

		Ogre::Entity *subEntityTorret303 = mSceneMgr->createEntity("subEntityTorret303", "sphere.mesh");
		subEntityTorret303->setMaterialName("LightBlue");
		Ogre::SceneNode *subNodeTurret303 = mSceneMgr->createSceneNode();
		subNodeTurret303->attachObject(subEntityTorret303);
		subNodeTurret303->setPosition(0,20,0);
		subNodeTurret303->setScale(0.06,0.06,0.06);

		Ogre::Entity *subEntityTorret304 = mSceneMgr->createEntity("subEntityTorret304", "usb_cubomod01.mesh");
		subEntityTorret304->setMaterialName("DarkGrey");
		Ogre::SceneNode *subNodeTurret304 = mSceneMgr->createSceneNode();
		subNodeTurret304->attachObject(subEntityTorret304);
		subNodeTurret304->setPosition(0,-10,0);
		subNodeTurret304->setScale(3,3,3);

		nodeTurret03->addChild(subnodeTurret301);
		nodeTurret03->addChild(subNodeTurret302);
		nodeTurret03->addChild(subNodeTurret303);
		nodeTurret03->addChild(subNodeTurret304);
	 
		nodeTurret03->setPosition(-22,-3,-637);
		nodeTurret03->setScale(0.4,0.4,0.4);
		nodeTurret03->rotate(Quaternion (Degree(2.5), Vector3::UNIT_Y));


		// Turret 4.
		Ogre::SceneNode *nodeTurret04 = mSceneMgr->createSceneNode();
		mSceneMgr->getRootSceneNode()->addChild(nodeTurret04);

		Ogre::Entity *subEntityTorret401 = mSceneMgr->createEntity("subEntityTorret401", "usb_cubomod01.mesh");
		subEntityTorret401->setMaterialName("DarkGrey");
		Ogre::SceneNode *subnodeTurret401 = mSceneMgr->createSceneNode();
		subnodeTurret401->attachObject(subEntityTorret401);
		subnodeTurret401->setPosition(0,0,0);  

		Ogre::Entity *subEntityTorret402 = mSceneMgr->createEntity("subEntityTorret402", "cube.mesh");
		subEntityTorret402->setMaterialName("Grey");
		Ogre::SceneNode *subNodeTurret402 = mSceneMgr->createSceneNode();
		subNodeTurret402->attachObject(subEntityTorret402);
		subNodeTurret402->setPosition(0,5,0);
		subNodeTurret402->setScale(0.035,0.05,0.035);

		Ogre::Entity *subEntityTorret403 = mSceneMgr->createEntity("subEntityTorret403", "usb_cilindro02.mesh");
		subEntityTorret403->setMaterialName("Cannon");
		Ogre::SceneNode *subNodeTurret403 = mSceneMgr->createSceneNode();
		subNodeTurret403->attachObject(subEntityTorret403);
		subNodeTurret403->setPosition(0,5,4);
		subNodeTurret403->setScale(0.5,0.5,0.5);
		subNodeTurret403->pitch(Ogre::Degree(90.0f));

		Ogre::Entity *subEntityTorret404 = mSceneMgr->createEntity("subEntityTorret404", "sphere.mesh");
		subEntityTorret404->setMaterialName("Laser");
		Ogre::SceneNode *subNodeTurret404 = mSceneMgr->createSceneNode();
		subNodeTurret404->attachObject(subEntityTorret404);
		subNodeTurret404->setPosition(0,5,7);
		subNodeTurret404->setScale(0.008,0.008,0.008);

		Ogre::Entity *subEntityTorret405 = mSceneMgr->createEntity("subEntityTorret405", "usb_planocurvo02.mesh");
		subEntityTorret405->setMaterialName("DarkGrey");
		Ogre::SceneNode *subNodeTurret405 = mSceneMgr->createSceneNode();
		subNodeTurret405->attachObject(subEntityTorret405);
		subNodeTurret405->setPosition(0,5,0);
		subNodeTurret405->setScale(0.08,0.5,0.5);
		subNodeTurret405->pitch(Ogre::Degree(70.0f));

		Ogre::Entity *subEntityTorret406 = mSceneMgr->createEntity("subEntityTorret406", "usb_planocurvo02.mesh");
		subEntityTorret406->setMaterialName("DarkGrey");
		Ogre::SceneNode *subNodeTurret406 = mSceneMgr->createSceneNode();
		subNodeTurret406->attachObject(subEntityTorret406);
		subNodeTurret406->setPosition(0,-7,2);
		subNodeTurret406->setScale(0.08,0.5,0.5);
		subNodeTurret406->pitch(Ogre::Degree(165.0f));
		subNodeTurret406->rotate(Quaternion (Degree(180), Vector3::UNIT_X));	
		
		Ogre::SceneNode *subNodeTurret407 = mSceneMgr->createSceneNode();
		subNodeTurret407->addChild(subNodeTurret405);
		subNodeTurret407->addChild(subNodeTurret406);
		subNodeTurret407->setPosition(0,4.7,3.5);
		subNodeTurret407->setScale(0.8,0.8,0.8);
		

		//Animacion Torreta 4
		float duration2 = 2.0;
		Ogre::Animation* animationTurret401 = mSceneMgr->createAnimation("AnimTurret401",duration2);
		animationTurret401->setInterpolationMode(Animation::IM_SPLINE);
		Ogre::NodeAnimationTrack* Turret401Track = animationTurret401->createNodeTrack(0,subNodeTurret407);
		Ogre::TransformKeyFrame* keyTurret04;

		keyTurret04 = Turret401Track->createNodeKeyFrame(0.0);
		keyTurret04->setRotation(Quaternion (Degree(0), Vector3::UNIT_Z));
		keyTurret04->setScale(Vector3(0.8,0.8,0.8));
		keyTurret04->setTranslate(Vector3(0,4.7,3.5));

		keyTurret04 = Turret401Track->createNodeKeyFrame(1.0);
		keyTurret04->setRotation(Quaternion (Degree(90), Vector3::UNIT_Z));
		keyTurret04->setScale(Vector3(0.8,0.8,0.8));
		keyTurret04->setTranslate(Vector3(0,4.7,3.5));

		keyTurret04 = Turret401Track->createNodeKeyFrame(2.0);
		keyTurret04->setRotation(Quaternion (Degree(180), Vector3::UNIT_Z));
		keyTurret04->setScale(Vector3(0.8,0.8,0.8));
		keyTurret04->setTranslate(Vector3(0,4.7,3.5));

		animationStateTurret401 = mSceneMgr->createAnimationState("AnimTurret401");
		animationStateTurret401->setEnabled(true);
		animationStateTurret401->setLoop(true);

		nodeTurret04->addChild(subnodeTurret401);
		nodeTurret04->addChild(subNodeTurret402);
		nodeTurret04->addChild(subNodeTurret403);
		nodeTurret04->addChild(subNodeTurret404);
		nodeTurret04->addChild(subNodeTurret407);

		nodeTurret04->setPosition(20,-7,-883);
		nodeTurret04->setScale(1.2,1.2,1.2);
		nodeTurret04->rotate(Quaternion (Degree(-2.5), Vector3::UNIT_Y));

		//Laser 1
		Ogre::Entity *EntityLaser01 = mSceneMgr->createEntity("EntityLaser01", "usb_laser.mesh");
		EntityLaser01->setMaterialName("Laser");
		laser01.nodeLaser = mSceneMgr->createSceneNode();
		laser01.nodeLaser->attachObject(EntityLaser01);
		nodeTurret01->addChild(laser01.nodeLaser);
		laser01.nodeLaser->setPosition(0,5,4);
		laser01.initialPosition = Vector3(0,5,4);
		laser01.nodeLaser->setScale(0.5,0.5,0.5);
		laser01.nodeLaser->pitch(Ogre::Degree(90.0f));

		//Laser 2
		Ogre::Entity *EntityLaser02 = mSceneMgr->createEntity("EntityLaser02", "usb_laser.mesh");
		EntityLaser02->setMaterialName("Laser");
		laser02.nodeLaser = mSceneMgr->createSceneNode();
		laser02.nodeLaser->attachObject(EntityLaser02);
		nodeTurret02->addChild(laser02.nodeLaser);
		laser02.nodeLaser->setPosition(0,5.5,6);
		laser02.initialPosition = Vector3(0,5.5,6);
		laser02.nodeLaser->setScale(0.5,0.5,0.5);
		laser02.nodeLaser->pitch(Ogre::Degree(90.0f));
		
		//Laser 3
		Ogre::Entity *EntityLaser03 = mSceneMgr->createEntity("EntityLaser03", "usb_laser.mesh");
		EntityLaser03->setMaterialName("LightBlue");
		laser03.nodeLaser = mSceneMgr->createSceneNode();
		laser03.nodeLaser->attachObject(EntityLaser03);
		nodeTurret03->addChild(laser03.nodeLaser);
		laser03.nodeLaser->setPosition(0,20,6);
		laser03.initialPosition = Vector3(0,20,6);
		laser03.nodeLaser->setScale(0.8,0.8,0.8);
		laser03.nodeLaser->pitch(Ogre::Degree(90.0f));

		//Laser 4
		Ogre::Entity *EntityLaser04 = mSceneMgr->createEntity("EntityLaser04", "usb_laser.mesh");
		EntityLaser04->setMaterialName("Laser");
		laser04.nodeLaser = mSceneMgr->createSceneNode();
		laser04.nodeLaser->attachObject(EntityLaser04);
		nodeTurret04->addChild(laser04.nodeLaser);
		laser04.nodeLaser->setPosition(0,5,4);
		laser04.initialPosition = Vector3(0,5,4);
		laser04.nodeLaser->setScale(0.5,0.5,0.5);
		laser04.nodeLaser->pitch(Ogre::Degree(90.0f));
		
	  
	  
	  /*
	  
		Spaceship
	  
	  */
	  
      Ogre::SceneNode *nodeSpaceship = nodePlayer->createChildSceneNode("nodeSpaceship");
	  Ogre::SceneNode *subNodeSpaceshipLeftWingUpper = nodeSpaceship->createChildSceneNode("subnodeSpaceshipLeftWingUpper");
	  Ogre::SceneNode *subNodeSpaceshipLeftWingDown = nodeSpaceship->createChildSceneNode("subnodeSpaceshipLeftWingDown");
	  Ogre::SceneNode *subNodeSpaceshipRightWingUpper = nodeSpaceship->createChildSceneNode("subNodeSpaceshipRightWingUpper");
	  Ogre::SceneNode *subNodeSpaceshipRightWingDown = nodeSpaceship->createChildSceneNode("subNodeSpaceshipRightWingDown");

	  ManualObject* body1;
	  body1 = generateParallelepiped("body1",0.0,-1.2,0.0,0.0,1.2,1.2,2.3,1.2,1.2,2.3,3.0);
	  ManualObject* body2;
	  body2 = generateParallelepiped("body2",0.0,1.2,0.0,0.0,1.2,2.3,1.2,1.2,2.3,1.2,3.0);
	  ManualObject* body3;
	  body3 = generateParallelepiped("body3",0.0,-1.2,-9.5,0.9,0.3,0.3,0.8,1.2,1.2,2.3,6.5);
	  ManualObject* body4;
	  body4 = generateParallelepiped("body4",0.0,1.2,-9.5,-0.9,0.3,0.8,0.3,1.2,2.3,1.2,6.5);
	  ManualObject* right_upper_wing1;
	  right_upper_wing1 = generateParallelepiped("right_upper_wing1",6.3,0.3,-1.0,0.0,  0.1, 4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* right_down_wing1;
	  right_down_wing1 = generateParallelepiped("right_down_wing1",6.3,-0.3,-1.0,0.0,    0.1 ,4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* left_upper_wing1;
	  left_upper_wing1 = generateParallelepiped("left_upper_wing1",-6.3,0.3,-1.0,0.0,  0.1, 4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* left_down_wing1;
	  left_down_wing1 = generateParallelepiped("left_down_wing1",-6.3,-0.3,-1.0,0.0,    0.1 ,4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* right_upper_wing2;
	  right_upper_wing2 = generateFigureRightWing("right_upper_wing2",6.3,0.3,1.0,0.0,    0.1 ,4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* right_down_wing2;
	  right_down_wing2 = generateFigureRightWing("right_down_wing2",6.3,0.3,1.0,0.0,    0.1 ,4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* left_upper_wing2;
	  left_upper_wing2 = generateFigureLeftWing("left_upper_wing2",-6.3,0.3,1.0,0.0,    0.1 ,4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* left_down_wing2;
	  left_down_wing2 = generateFigureLeftWing("left_down_wing2",-6.3,0.3,1.0,0.0,    0.1 ,4.0, 4.0,  0.1,4.0,4.0,  1.0);


	  nodeSpaceship->attachObject(body1);
	  nodeSpaceship->attachObject(body2);
	  nodeSpaceship->attachObject(body3);
	  nodeSpaceship->attachObject(body4);
	  subNodeSpaceshipRightWingUpper->attachObject(right_upper_wing1);
	  subNodeSpaceshipRightWingDown->attachObject(right_down_wing1);
	  subNodeSpaceshipLeftWingUpper->attachObject(left_upper_wing1);
	  subNodeSpaceshipLeftWingDown->attachObject(left_down_wing1);
	  subNodeSpaceshipRightWingUpper->attachObject(right_upper_wing2);
	  subNodeSpaceshipRightWingDown->attachObject(right_down_wing2);
	  subNodeSpaceshipLeftWingUpper->attachObject(left_upper_wing2);
	  subNodeSpaceshipLeftWingDown->attachObject(left_down_wing2);

	  subNodeSpaceshipLeftWingUpper->setPosition(0,0,0);
	  subNodeSpaceshipLeftWingDown->setPosition(0,0,0);
	  subNodeSpaceshipRightWingUpper->setPosition(0,0,0);
	  subNodeSpaceshipRightWingDown->setPosition(0,0,0);
	  nodeSpaceship->setPosition(0, 0, 0);
	  nodePlayer->setPosition(x_pos, 0, z_pos);


	  //1 left upper wing
	  Ogre::SceneNode *subNodeGunLWU = subNodeSpaceshipLeftWingUpper->createChildSceneNode("subNodeGunLWU");
	  Ogre::Entity *subEntityNodeGunLWU = mSceneMgr->createEntity("subEntityNodeGunLWU", "usb_cilindro.mesh");
	  subEntityNodeGunLWU->setMaterialName("DarkGrey");
	  subEntityNodeGunLWU->setCastShadows(true);
	  subNodeGunLWU->attachObject(subEntityNodeGunLWU);
	  subNodeGunLWU->setPosition(-10.0,0.3,-6.0);
	  subNodeGunLWU->setScale(0.08,1.5,0.08);
	  subNodeGunLWU->pitch(Ogre::Degree(90.0f));

	  Ogre::SceneNode *subNodeTurbine1LWU = subNodeSpaceshipLeftWingUpper->createChildSceneNode("subNodeTurbine1LWU");
	  Ogre::Entity *subEntityNodeTurbine1LWU = mSceneMgr->createEntity("subEntityNodeTurbine1LWU", "usb_cilindro.mesh");
	  subEntityNodeTurbine1LWU->setMaterialName("DarkGrey");
	  subNodeTurbine1LWU->attachObject(subEntityNodeTurbine1LWU);
	  subNodeTurbine1LWU->setPosition(-3.5,1.3,-1.0);
	  subNodeTurbine1LWU->setScale(0.38,0.2,0.38);
	  subNodeTurbine1LWU->pitch(Ogre::Degree(90.0f));

	  Ogre::SceneNode *subNodeTurbine2LWU = subNodeSpaceshipLeftWingUpper->createChildSceneNode("subNodeTurbine2LWU");
	  Ogre::Entity *subEntityNodeTurbine2LWU = mSceneMgr->createEntity("subEntityNodeTurbine2LWU", "usb_cilindro02.mesh");
	  subEntityNodeTurbine2LWU->setMaterialName("DarkGrey");
	  subNodeTurbine2LWU->attachObject(subEntityNodeTurbine2LWU);
	  subNodeTurbine2LWU->setPosition(-3.5,1.3,0.0);
	  subNodeTurbine2LWU->setScale(0.38,0.2,0.38);
	  subNodeTurbine2LWU->pitch(Ogre::Degree(-90.0f));

	  //2 Left down wing
	  Ogre::SceneNode *subNodeGunLWD = subNodeSpaceshipLeftWingDown->createChildSceneNode("subNodeGunLWD");
	  Ogre::Entity *subEntityNodeGunLWD = mSceneMgr->createEntity("subEntityNodeGunLWD", "usb_cilindro.mesh");
	  subEntityNodeGunLWD->setMaterialName("DarkGrey");
	  subNodeGunLWD->attachObject(subEntityNodeGunLWD);
	  subNodeGunLWD->setPosition(-10.0,-0.3,-6.0);
	  subNodeGunLWD->setScale(0.08,1.5,0.08);
	  subNodeGunLWD->pitch(Ogre::Degree(90.0f));

	  Ogre::SceneNode *subNodeTurbine1LWD = subNodeSpaceshipLeftWingDown->createChildSceneNode("subNodeTurbine1LWD");
	  Ogre::Entity *subEntityNodeTurbine1LWD = mSceneMgr->createEntity("subEntityNodeTurbine1LWD", "usb_cilindro.mesh");
	  subEntityNodeTurbine1LWD->setMaterialName("DarkGrey");
	  subNodeTurbine1LWD->attachObject(subEntityNodeTurbine1LWD);
	  subNodeTurbine1LWD->setPosition(-3.5,-1.3,-1.0);
	  subNodeTurbine1LWD->setScale(0.38,0.2,0.38);
	  subNodeTurbine1LWD->pitch(Ogre::Degree(90.0f));

	  Ogre::SceneNode *subNodeTurbine2LWD = subNodeSpaceshipLeftWingDown->createChildSceneNode("subNodeTurbine2LWD");
	  Ogre::Entity *subEntityNodeTurbine2LWD = mSceneMgr->createEntity("subEntityNodeTurbine2LWD", "usb_cilindro02.mesh");
	  subEntityNodeTurbine2LWD->setMaterialName("DarkGrey");
	  subNodeTurbine2LWD->attachObject(subEntityNodeTurbine2LWD);
	  subNodeTurbine2LWD->setPosition(-3.5,-1.3,0.0);
	  subNodeTurbine2LWD->setScale(0.38,0.2,0.38);
	  subNodeTurbine2LWD->pitch(Ogre::Degree(-90.0f));

	  
	  //3 Right Up wing
	  Ogre::SceneNode *subNodeGunRWU = subNodeSpaceshipRightWingUpper->createChildSceneNode("subNodeGunRWU");
	  Ogre::Entity *subEntityNodeGunRWU = mSceneMgr->createEntity("subEntityNodeGunRWU", "usb_cilindro.mesh");
	  subEntityNodeGunRWU->setMaterialName("DarkGrey");
	  subNodeGunRWU->attachObject(subEntityNodeGunRWU);
	  subNodeGunRWU->setPosition(10.0,0.3,-6.0);
	  subNodeGunRWU->setScale(0.08,1.5,0.08);
	  subNodeGunRWU->pitch(Ogre::Degree(90.0f));

	  Ogre::SceneNode *subNodeTurbine1RWU = subNodeSpaceshipRightWingUpper->createChildSceneNode("subNodeTurbine1RWU");
	  Ogre::Entity *subEntityNodeTurbine1RWU = mSceneMgr->createEntity("subEntityNodeTurbine1RWU", "usb_cilindro.mesh");
	  subEntityNodeTurbine1RWU->setMaterialName("DarkGrey");
	  subNodeTurbine1RWU->attachObject(subEntityNodeTurbine1RWU);
	  subNodeTurbine1RWU->setPosition(3.5,1.3,-1.0);
	  subNodeTurbine1RWU->setScale(0.38,0.2,0.38);
	  subNodeTurbine1RWU->pitch(Ogre::Degree(90.0f));

	  Ogre::SceneNode *subNodeTurbine2RWU = subNodeSpaceshipRightWingUpper->createChildSceneNode("subNodeTurbine2RWU");
	  Ogre::Entity *subEntityNodeTurbine2RWU = mSceneMgr->createEntity("subEntityNodeTurbine2RWU", "usb_cilindro02.mesh");
	  subEntityNodeTurbine2RWU->setMaterialName("DarkGrey");
	  subNodeTurbine2RWU->attachObject(subEntityNodeTurbine2RWU);
	  subNodeTurbine2RWU->setPosition(3.5,1.3,0.0);
	  subNodeTurbine2RWU->setScale(0.38,0.2,0.38);
	  subNodeTurbine2RWU->pitch(Ogre::Degree(-90.0f));

	  //3 Right Down wing
	  Ogre::SceneNode *subNodeGunRWD = subNodeSpaceshipRightWingDown->createChildSceneNode("subNodeGunRWD");
	  Ogre::Entity *subEntityNodeGunRWD = mSceneMgr->createEntity("subEntityNodeGunRWD", "usb_cilindro.mesh");
	  subEntityNodeGunRWD->setMaterialName("DarkGrey");
	  subNodeGunRWD->attachObject(subEntityNodeGunRWD);
	  subNodeGunRWD->setPosition(10.0,-0.3,-6.0);
	  subNodeGunRWD->setScale(0.08,1.5,0.08);
	  subNodeGunRWD->pitch(Ogre::Degree(90.0f));

	  Ogre::SceneNode *subNodeTurbine1RWD = subNodeSpaceshipRightWingDown->createChildSceneNode("subNodeTurbine1RWD");
	  Ogre::Entity *subEntityNodeTurbine1RWD = mSceneMgr->createEntity("subEntityNodeTurbine1RWD", "usb_cilindro.mesh");
	  subEntityNodeTurbine1RWD->setMaterialName("DarkGrey");
	  subNodeTurbine1RWD->attachObject(subEntityNodeTurbine1RWD);
	  subNodeTurbine1RWD->setPosition(3.5,-1.3,-1.0);
	  subNodeTurbine1RWD->setScale(0.38,0.2,0.38);
	  subNodeTurbine1RWD->pitch(Ogre::Degree(90.0f));

	  Ogre::SceneNode *subNodeTurbine2RWD = subNodeSpaceshipRightWingDown->createChildSceneNode("subNodeTurbine2RWD");
	  Ogre::Entity *subEntityNodeTurbine2RWD = mSceneMgr->createEntity("subEntityNodeTurbine2RWD", "usb_cilindro02.mesh");
	  subEntityNodeTurbine2RWD->setMaterialName("DarkGrey");
	  subNodeTurbine2RWD->attachObject(subEntityNodeTurbine2RWD);
	  subNodeTurbine2RWD->setPosition(3.5,-1.3,0.0);
	  subNodeTurbine2RWD->setScale(0.38,0.2,0.38);
	  subNodeTurbine2RWD->pitch(Ogre::Degree(-90.0f));

	
	  //Wings animation
	  float wingsanimduration1 = 3.0;

	  //Open wings
	  Ogre::Animation* animationOpenWingsSpaceship1 = mSceneMgr->createAnimation("animOpenWingsSpaceship1",wingsanimduration1);
	  Ogre::Animation* animationOpenWingsSpaceship2 = mSceneMgr->createAnimation("animOpenWingsSpaceship2",wingsanimduration1);
	  Ogre::Animation* animationOpenWingsSpaceship3 = mSceneMgr->createAnimation("animOpenWingsSpaceship3",wingsanimduration1);
	  Ogre::Animation* animationOpenWingsSpaceship4 = mSceneMgr->createAnimation("animOpenWingsSpaceship4",wingsanimduration1);
	  animationOpenWingsSpaceship1->setInterpolationMode(Animation::IM_SPLINE);
	  animationOpenWingsSpaceship2->setInterpolationMode(Animation::IM_SPLINE);
	  animationOpenWingsSpaceship3->setInterpolationMode(Animation::IM_SPLINE);
	  animationOpenWingsSpaceship4->setInterpolationMode(Animation::IM_SPLINE);
	  Ogre::NodeAnimationTrack* UpperLeftwingTrack1 = animationOpenWingsSpaceship1->createNodeTrack(0,subNodeSpaceshipLeftWingUpper);
	  Ogre::NodeAnimationTrack* DownLeftwingTrack1 = animationOpenWingsSpaceship2->createNodeTrack(0,subNodeSpaceshipLeftWingDown);
	  Ogre::NodeAnimationTrack* UpperRightwingTrack1 = animationOpenWingsSpaceship3->createNodeTrack(0,subNodeSpaceshipRightWingUpper);
	  Ogre::NodeAnimationTrack* DownRightwingTrack1 = animationOpenWingsSpaceship4->createNodeTrack(0,subNodeSpaceshipRightWingDown);

	  Ogre::TransformKeyFrame* key1;
	  Ogre::TransformKeyFrame* key2;
	  Ogre::TransformKeyFrame* key3;
	  Ogre::TransformKeyFrame* key4;

	  key1 = UpperLeftwingTrack1->createNodeKeyFrame(0.0);
	  key1->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));
	  key2 = DownLeftwingTrack1->createNodeKeyFrame(0.0);
	  key2->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));
	  key3 = UpperRightwingTrack1->createNodeKeyFrame(0.0);
	  key3->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));
	  key4 = DownRightwingTrack1->createNodeKeyFrame(0.0);
	  key4->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));

		
	
	  key1 = UpperLeftwingTrack1->createNodeKeyFrame(2.0);
	  key1->setRotation(Ogre::Quaternion(Ogre::Degree(-15), Ogre::Vector3(0,0,1)));
	  key2 = DownLeftwingTrack1->createNodeKeyFrame(2.0);
	  key2->setRotation(Ogre::Quaternion(Ogre::Degree(15), Ogre::Vector3(0,0,1)));
	  key3 = UpperRightwingTrack1->createNodeKeyFrame(2.0);
	  key3->setRotation(Ogre::Quaternion(Ogre::Degree(15), Ogre::Vector3(0,0,1)));
	  key4 = DownRightwingTrack1->createNodeKeyFrame(2.0);
	  key4->setRotation(Ogre::Quaternion(Ogre::Degree(-15), Ogre::Vector3(0,0,1)));

	  key1 = UpperLeftwingTrack1->createNodeKeyFrame(3.0);
	  key1->setRotation(Ogre::Quaternion(Ogre::Degree(-30), Ogre::Vector3(0,0,1)));
	  key2 = DownLeftwingTrack1->createNodeKeyFrame(3.0);
	  key2->setRotation(Ogre::Quaternion(Ogre::Degree(30), Ogre::Vector3(0,0,1)));
	  key3 = UpperRightwingTrack1->createNodeKeyFrame(3.0);
	  key3->setRotation(Ogre::Quaternion(Ogre::Degree(30), Ogre::Vector3(0,0,1)));
	  key4 = DownRightwingTrack1->createNodeKeyFrame(3.0);
	  key4->setRotation(Ogre::Quaternion(Ogre::Degree(-30), Ogre::Vector3(0,0,1)));


	  animationStateOpenWingsSpaceship1 = mSceneMgr->createAnimationState("animOpenWingsSpaceship1");
	  animationStateOpenWingsSpaceship2 = mSceneMgr->createAnimationState("animOpenWingsSpaceship2");
	  animationStateOpenWingsSpaceship3 = mSceneMgr->createAnimationState("animOpenWingsSpaceship3");
	  animationStateOpenWingsSpaceship4 = mSceneMgr->createAnimationState("animOpenWingsSpaceship4");
	  
	  animationStateOpenWingsSpaceship1->setLoop(false);
	  animationStateOpenWingsSpaceship2->setLoop(false);
	  animationStateOpenWingsSpaceship3->setLoop(false);
	  animationStateOpenWingsSpaceship4->setLoop(false);

	  //Close wings
	  float wingsanimduration2 = 3.0;
	  Ogre::Animation* animationCloseWingsSpaceship1 = mSceneMgr->createAnimation("animCloseWingsSpaceship1",wingsanimduration2);
	  Ogre::Animation* animationCloseWingsSpaceship2 = mSceneMgr->createAnimation("animCloseWingsSpaceship2",wingsanimduration2);
	  Ogre::Animation* animationCloseWingsSpaceship3 = mSceneMgr->createAnimation("animCloseWingsSpaceship3",wingsanimduration2);
	  Ogre::Animation* animationCloseWingsSpaceship4 = mSceneMgr->createAnimation("animCloseWingsSpaceship4",wingsanimduration2);
	  animationCloseWingsSpaceship1->setInterpolationMode(Animation::IM_SPLINE);
	  animationCloseWingsSpaceship2->setInterpolationMode(Animation::IM_SPLINE);
	  animationCloseWingsSpaceship3->setInterpolationMode(Animation::IM_SPLINE);
	  animationCloseWingsSpaceship4->setInterpolationMode(Animation::IM_SPLINE);
	  Ogre::NodeAnimationTrack* UpperLeftwingTrack2 = animationCloseWingsSpaceship1->createNodeTrack(0,subNodeSpaceshipLeftWingUpper);
	  Ogre::NodeAnimationTrack* DownLeftwingTrack2 = animationCloseWingsSpaceship2->createNodeTrack(0,subNodeSpaceshipLeftWingDown);
	  Ogre::NodeAnimationTrack* UpperRightwingTrack2 = animationCloseWingsSpaceship3->createNodeTrack(0,subNodeSpaceshipRightWingUpper);
	  Ogre::NodeAnimationTrack* DownRightwingTrack2 = animationCloseWingsSpaceship4->createNodeTrack(0,subNodeSpaceshipRightWingDown);

	  Ogre::TransformKeyFrame* keyClose1;
	  Ogre::TransformKeyFrame* keyClose2;
	  Ogre::TransformKeyFrame* keyClose3;
	  Ogre::TransformKeyFrame* keyClose4;

		
	
	  keyClose1 = UpperLeftwingTrack2->createNodeKeyFrame(0.0);
	  keyClose1->setRotation(Ogre::Quaternion(Ogre::Degree(-30), Ogre::Vector3(0,0,1)));
	  keyClose2 = DownLeftwingTrack2->createNodeKeyFrame(0.0);
	  keyClose2->setRotation(Ogre::Quaternion(Ogre::Degree(30), Ogre::Vector3(0,0,1)));
	  keyClose3 = UpperRightwingTrack2->createNodeKeyFrame(0.0);
	  keyClose3->setRotation(Ogre::Quaternion(Ogre::Degree(30), Ogre::Vector3(0,0,1)));
	  keyClose4 = DownRightwingTrack2->createNodeKeyFrame(0.0);
	  keyClose4->setRotation(Ogre::Quaternion(Ogre::Degree(-30), Ogre::Vector3(0,0,1)));

	  keyClose1 = UpperLeftwingTrack2->createNodeKeyFrame(2.0);
	  keyClose1->setRotation(Ogre::Quaternion(Ogre::Degree(-15), Ogre::Vector3(0,0,1)));
	  keyClose2 = DownLeftwingTrack2->createNodeKeyFrame(2.0);
	  keyClose2->setRotation(Ogre::Quaternion(Ogre::Degree(15), Ogre::Vector3(0,0,1)));
	  keyClose3 = UpperRightwingTrack2->createNodeKeyFrame(2.0);
	  keyClose3->setRotation(Ogre::Quaternion(Ogre::Degree(15), Ogre::Vector3(0,0,1)));
	  keyClose4 = DownRightwingTrack2->createNodeKeyFrame(2.0);
	  keyClose4->setRotation(Ogre::Quaternion(Ogre::Degree(-15), Ogre::Vector3(0,0,1)));


	  keyClose1 = UpperLeftwingTrack2->createNodeKeyFrame(3.0);
	  keyClose1->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));
	  keyClose2 = DownLeftwingTrack2->createNodeKeyFrame(3.0);
	  keyClose2->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));
	  keyClose3 = UpperRightwingTrack2->createNodeKeyFrame(3.0);
	  keyClose3->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));
	  keyClose4 = DownRightwingTrack2->createNodeKeyFrame(3.0);
	  keyClose4->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));



	  animationStateCloseWingsSpaceship1 = mSceneMgr->createAnimationState("animCloseWingsSpaceship1");
	  animationStateCloseWingsSpaceship2 = mSceneMgr->createAnimationState("animCloseWingsSpaceship2");
	  animationStateCloseWingsSpaceship3 = mSceneMgr->createAnimationState("animCloseWingsSpaceship3");
	  animationStateCloseWingsSpaceship4 = mSceneMgr->createAnimationState("animCloseWingsSpaceship4");
	  
	  animationStateCloseWingsSpaceship1->setLoop(false);
	  animationStateCloseWingsSpaceship2->setLoop(false);
	  animationStateCloseWingsSpaceship3->setLoop(false);
	  animationStateCloseWingsSpaceship4->setLoop(false);

	  //Turn animations

	  //Turn right 
	  float turnAnim1 = 1.0;
	  Ogre::Animation* animationTurnRightSpaceship = mSceneMgr->createAnimation("animTurnRightSpaceship",turnAnim1);
	  animationTurnRightSpaceship->setInterpolationMode(Animation::IM_SPLINE);
	  Ogre::NodeAnimationTrack* turnRightSpaceshipTrack = animationTurnRightSpaceship->createNodeTrack(0,nodeSpaceship);
	  Ogre::TransformKeyFrame* keyTurnRight;

	  keyTurnRight = turnRightSpaceshipTrack->createNodeKeyFrame(0.0);
	  keyTurnRight->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));

	  keyTurnRight = turnRightSpaceshipTrack->createNodeKeyFrame(0.5);
	  keyTurnRight->setRotation(Ogre::Quaternion(Ogre::Degree(-15.0), Ogre::Vector3(0,0,1)));

	  keyTurnRight = turnRightSpaceshipTrack->createNodeKeyFrame(1.0);
	  keyTurnRight->setRotation(Ogre::Quaternion(Ogre::Degree(-30.0), Ogre::Vector3(0,0,1)));
	  animationStateTurnSpaceshipRight = mSceneMgr->createAnimationState("animTurnRightSpaceship");
	  animationStateTurnSpaceshipRight->setLoop(false);

	  //Turn center from right
	  float turnAnim2 = 1.0;
	  Ogre::Animation* animationTurnCenterFRSpaceship = mSceneMgr->createAnimation("animTurnCenterFRSpaceship",turnAnim2);
	  animationTurnCenterFRSpaceship->setInterpolationMode(Animation::IM_SPLINE);
	  Ogre::NodeAnimationTrack* turnCenterFRSpaceshipTrack = animationTurnCenterFRSpaceship->createNodeTrack(0,nodeSpaceship);
	  Ogre::TransformKeyFrame* keyTurnCenterFR;

	  keyTurnCenterFR = turnCenterFRSpaceshipTrack->createNodeKeyFrame(0.0);
	  keyTurnCenterFR->setRotation(Ogre::Quaternion(Ogre::Degree(-30.0), Ogre::Vector3(0,0,1)));

	  keyTurnCenterFR = turnCenterFRSpaceshipTrack->createNodeKeyFrame(0.5);
	  keyTurnCenterFR->setRotation(Ogre::Quaternion(Ogre::Degree(-15.0), Ogre::Vector3(0,0,1)));

	  keyTurnCenterFR = turnCenterFRSpaceshipTrack->createNodeKeyFrame(1.0);
	  keyTurnCenterFR->setRotation(Ogre::Quaternion(Ogre::Degree(0.0), Ogre::Vector3(0,0,1)));
	  animationStateTurnSpaceshipCenterFR = mSceneMgr->createAnimationState("animTurnCenterFRSpaceship");
	  animationStateTurnSpaceshipCenterFR->setLoop(false);

	  //Turn left
	  float turnAnim3 = 1.0;
	  Ogre::Animation* animationTurnLeftSpaceship = mSceneMgr->createAnimation("animTurnLeftSpaceship",turnAnim3);
	  animationTurnLeftSpaceship->setInterpolationMode(Animation::IM_SPLINE);
	  Ogre::NodeAnimationTrack* turnLeftSpaceshipTrack = animationTurnLeftSpaceship->createNodeTrack(0,nodeSpaceship);
	  Ogre::TransformKeyFrame* keyTurnLeft;

	  keyTurnLeft = turnLeftSpaceshipTrack->createNodeKeyFrame(0.0);
	  keyTurnLeft->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(0,0,1)));

	  keyTurnLeft = turnLeftSpaceshipTrack->createNodeKeyFrame(0.5);
	  keyTurnLeft->setRotation(Ogre::Quaternion(Ogre::Degree(15.0), Ogre::Vector3(0,0,1)));

	  keyTurnLeft = turnLeftSpaceshipTrack->createNodeKeyFrame(1.0);
	  keyTurnLeft->setRotation(Ogre::Quaternion(Ogre::Degree(30.0), Ogre::Vector3(0,0,1)));
	  animationStateTurnSpaceshipLeft = mSceneMgr->createAnimationState("animTurnLeftSpaceship");
	  animationStateTurnSpaceshipLeft->setLoop(false);

	  //Turn center from left
	  float turnAnim4 = 1.0;
	  Ogre::Animation* animationTurnCenterFLSpaceship = mSceneMgr->createAnimation("animTurnCenterFLSpaceship",turnAnim4);
	  animationTurnCenterFLSpaceship->setInterpolationMode(Animation::IM_SPLINE);
	  Ogre::NodeAnimationTrack* turnCenterFLSpaceshipTrack = animationTurnCenterFLSpaceship->createNodeTrack(0,nodeSpaceship);
	  Ogre::TransformKeyFrame* keyTurnCenterFL;

	  keyTurnCenterFL = turnCenterFLSpaceshipTrack->createNodeKeyFrame(0.0);
	  keyTurnCenterFL->setRotation(Ogre::Quaternion(Ogre::Degree(30.0), Ogre::Vector3(0,0,1)));

	  keyTurnCenterFL = turnCenterFLSpaceshipTrack->createNodeKeyFrame(0.5);
	  keyTurnCenterFL->setRotation(Ogre::Quaternion(Ogre::Degree(15.0), Ogre::Vector3(0,0,1)));

	  keyTurnCenterFL = turnCenterFLSpaceshipTrack->createNodeKeyFrame(1.0);
	  keyTurnCenterFL->setRotation(Ogre::Quaternion(Ogre::Degree(0.0), Ogre::Vector3(0,0,1)));
	  animationStateTurnSpaceshipCenterFL = mSceneMgr->createAnimationState("animTurnCenterFLSpaceship");
	  animationStateTurnSpaceshipCenterFL->setLoop(false);

	}

};


int main (void)
{

	Example1 app;
	app.go();
	return 0;
}