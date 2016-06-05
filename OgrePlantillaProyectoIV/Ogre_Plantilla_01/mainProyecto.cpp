#include "Ogre\ExampleApplication.h"
#include <ctime> 

float test;

Ogre::AnimationState* animationStateTurret401;

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

		float movSpeed=10.0f;
		Ogre::Vector3 tmov(0,0,0);
		Ogre::Vector3 tcam(0,0,0);

		//Camara
		if(_key->isKeyDown(OIS::KC_LSHIFT))
			cam_speed += 30;

		if (_key->isKeyDown(OIS::KC_ESCAPE))
			return false;

		if(_key->isKeyDown(OIS::KC_W))
			tcam += Ogre::Vector3(0,0,-cam_speed);
		
		if(_key->isKeyDown(OIS::KC_S))
			tcam += Ogre::Vector3(0,0,cam_speed);

		if(_key->isKeyDown(OIS::KC_A))
			tcam += Ogre::Vector3(-cam_speed,0,0);
		
		if(_key->isKeyDown(OIS::KC_D))
			tcam += Ogre::Vector3(cam_speed,0,0);

		
		

		

		//camara control
		float rotX = _mouse->getMouseState().X.rel * evt.timeSinceLastFrame*-1;
		float rotY = _mouse->getMouseState().Y.rel * evt.timeSinceLastFrame*-1;
		_cam->yaw(Ogre::Radian(rotX));
		_cam->pitch(Ogre::Radian(rotY));
		_cam->moveRelative(tcam*movSpeed*evt.timeSinceLastFrame);
		
		animationStateTurret401->addTime(evt.timeSinceLastFrame);

		//Laser movements
		updateLaser(laser01,evt,200,100);
		updateLaser(laser02,evt,400,200);
		updateLaser(laser03,evt,1500,350);
		updateLaser(laser04,evt,800,300);
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

	void createScene()
	{
		 
		mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		

		Ogre::Entity* ent01 = mSceneMgr->createEntity("MyEntity1","ejes01.mesh");
		Ogre::SceneNode* node01 = mSceneMgr->createSceneNode("Node01");
		mSceneMgr->getRootSceneNode()->addChild(node01);
		node01->attachObject(ent01);
		//node01->setPosition(0,0,20);

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
		//subnodeTurret401->setScale(1,2.5,1);
	  
	  

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
		

		//Animacion Turreta 4
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



		////Laser animation
		//float duration3 = 3.0;
		//Ogre::Animation* animationLaser01 = mSceneMgr->createAnimation("AnimLaser01",duration3);
		//animationLaser01->setInterpolationMode(Animation::IM_SPLINE);
		//Ogre::NodeAnimationTrack* Laser01Track = animationLaser01->createNodeTrack(0,subNodeLaser01);
		//Ogre::TransformKeyFrame* keyLaser01;

		//keyLaser01 = Laser01Track->createNodeKeyFrame(0.0);
		//keyLaser01->setRotation(Quaternion (Degree(0), Vector3::UNIT_Z));
		//keyLaser01->setScale(Vector3(0.8,0.8,0.8));
		//keyLaser01->setTranslate(Vector3(0,4.7,3.5));

		//keyLaser01 = Laser01Track->createNodeKeyFrame(1.0);
		//keyLaser01->setRotation(Quaternion (Degree(90), Vector3::UNIT_Z));
		//keyLaser01->setScale(Vector3(0.8,0.8,0.8));
		//keyLaser01->setTranslate(Vector3(0,4.7,3.5));

		//keyLaser01 = Laser01Track->createNodeKeyFrame(2.0);
		//keyLaser01->setRotation(Quaternion (Degree(180), Vector3::UNIT_Z));
		//keyLaser01->setScale(Vector3(0.8,0.8,0.8));
		//keyLaser01->setTranslate(Vector3(0,4.7,3.5));
	}

};


int main (void)
{

	Example1 app;
	app.go();
	return 0;
}