#include "Ogre\ExampleApplication.h"

float test;
Ogre::AnimationState* animationStateTurret301;

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
			cam_speed += 10;

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
		

		animationStateTurret301->addTime(evt.timeSinceLastFrame);
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
	  manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_STRIP);
	  
	  
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

		Ogre::ManualObject *man = mSceneMgr->createManualObject();
      man->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_TRIANGLE_LIST);
      man->position(0,1,0);
      man->colour(1,0,0);
      man->position(-1,-1,0);
      man->colour(0,1,0);
      man->position(1,-1,0);
      man->colour(0,0,1); 
      man->triangle(0,1,2);
      man->end();
      man->convertToMesh("CustomMesh");

	  // Turret 1.
	  Ogre::SceneNode *nodeTurret01 = mSceneMgr->createSceneNode();
	  mSceneMgr->getRootSceneNode()->addChild(nodeTurret01);

	  Ogre::Entity *subEntityTorret101 = mSceneMgr->createEntity("subEntityTorret101", "usb_cubomod01.mesh");
      Ogre::SceneNode *subNodeTurret101 = mSceneMgr->createSceneNode();
      subNodeTurret101->attachObject(subEntityTorret101);
	  subNodeTurret101->setPosition(0,0,0);

	  Ogre::Entity *subEntityTorret102 = mSceneMgr->createEntity("subEntityTorret102", "cube.mesh");
      Ogre::SceneNode *subNodeTurret102 = mSceneMgr->createSceneNode();
      subNodeTurret102->attachObject(subEntityTorret102);
	  subNodeTurret102->setPosition(0,5,0);
	  subNodeTurret102->setScale(0.05,0.05,0.05);

	  Ogre::Entity *subEntityTorret103 = mSceneMgr->createEntity("subEntityTorret103", "usb_cilindro02.mesh");
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

	  // Turret 2.
	  Ogre::SceneNode *nodeTurret02 = mSceneMgr->createSceneNode();
	  mSceneMgr->getRootSceneNode()->addChild(nodeTurret02);

	  Ogre::Entity *subEntityTorret201 = mSceneMgr->createEntity("subEntityTorret201", "usb_pipe.mesh");
      Ogre::SceneNode *subnodeTurret201 = mSceneMgr->createSceneNode();
      subnodeTurret201->attachObject(subEntityTorret201);
	  subnodeTurret201->setPosition(0,0,0);
	  subnodeTurret201->setScale(0.6,5.2,0.6);

	  Ogre::Entity *subEntityTorret202 = mSceneMgr->createEntity("subEntityTorret202", "usb_pipe.mesh");
      Ogre::SceneNode *subNodeTurret202 = mSceneMgr->createSceneNode();
      subNodeTurret202->attachObject(subEntityTorret202);
	  subNodeTurret202->setPosition(0,5,0);
	  subNodeTurret202->setScale(0.9,1,0.9);

	  Ogre::Entity *subEntityTorret203 = mSceneMgr->createEntity("subEntityTorret203", "usb_pipe.mesh");
      Ogre::SceneNode *subNodeTurret203 = mSceneMgr->createSceneNode();
      subNodeTurret203->attachObject(subEntityTorret203);
	  subNodeTurret203->setPosition(0,-2,0);
	  subNodeTurret203->setScale(0.9,1,0.9);

	  Ogre::Entity *subEntityTorret204 = mSceneMgr->createEntity("subEntityTorret204", "usb_cilindro02.mesh");
      Ogre::SceneNode *subNodeTurret204 = mSceneMgr->createSceneNode();
      subNodeTurret204->attachObject(subEntityTorret204);
	  subNodeTurret204->setPosition(0,5.5,7);
	  subNodeTurret204->setScale(0.4,0.7,0.4);
	  subNodeTurret204->pitch(Ogre::Degree(90.0f));
	  
	  
	  nodeTurret02->addChild(subnodeTurret201);
	  nodeTurret02->addChild(subNodeTurret202);
	  nodeTurret02->addChild(subNodeTurret203);
	  nodeTurret02->addChild(subNodeTurret204);
	 
	  nodeTurret02->setPosition(22,-7,-407);
	  //nodeTurret02->setScale(1.2,1.2,1.2);

	  // Turret 3.
	  Ogre::SceneNode *nodeTurret03 = mSceneMgr->createSceneNode();
	  mSceneMgr->getRootSceneNode()->addChild(nodeTurret03);

	  Ogre::Entity *subEntityTorret301 = mSceneMgr->createEntity("subEntityTorret301", "usb_dodecaedro.mesh");
      Ogre::SceneNode *subnodeTurret301 = mSceneMgr->createSceneNode();
      subnodeTurret301->attachObject(subEntityTorret301);
	  subnodeTurret301->setPosition(0,0,0);
	  subnodeTurret301->setScale(1,2.5,1);
	  
	  

	  Ogre::Entity *subEntityTorret302 = mSceneMgr->createEntity("subEntityTorret302", "usb_torus.mesh");
      Ogre::SceneNode *subNodeTurret302 = mSceneMgr->createSceneNode();
      subNodeTurret302->attachObject(subEntityTorret302);
	  subNodeTurret302->setPosition(0,0,0);
	  subNodeTurret302->setScale(3,3,3);

	  //Animacion esfera
		float duration = 4.0;
		Ogre::Animation* animationTurret301 = mSceneMgr->createAnimation("AnimTurret301",duration);
		animationTurret301->setInterpolationMode(Animation::IM_SPLINE);
		Ogre::NodeAnimationTrack* Turret301Track = animationTurret301->createNodeTrack(0,subNodeTurret302);
		Ogre::TransformKeyFrame* key;

		key = Turret301Track->createNodeKeyFrame(0.0);
		key->setTranslate(Vector3(0,0,0));
		key->setScale(Vector3(3,3,3));
		

		key = Turret301Track->createNodeKeyFrame(1.0);
		key->setTranslate(Vector3(0,8,0));
		key->setScale(Vector3(3,3,3));

		key = Turret301Track->createNodeKeyFrame(2.0);
		key->setTranslate(Vector3(0,0,0));
		key->setScale(Vector3(3,3,3));

		key = Turret301Track->createNodeKeyFrame(3.0);
		key->setTranslate(Vector3(0,-8,0));
		key->setScale(Vector3(3,3,3));

		key = Turret301Track->createNodeKeyFrame(4.0);
		key->setTranslate(Vector3(0,0,0));
		key->setScale(Vector3(3,3,3));

		animationStateTurret301 = mSceneMgr->createAnimationState("AnimTurret301");
		animationStateTurret301->setEnabled(true);
		animationStateTurret301->setLoop(true);

	  Ogre::Entity *subEntityTorret303 = mSceneMgr->createEntity("subEntityTorret303", "sphere.mesh");
      Ogre::SceneNode *subNodeTurret303 = mSceneMgr->createSceneNode();
      subNodeTurret303->attachObject(subEntityTorret303);
	  subNodeTurret303->setPosition(0,25,0);
	  subNodeTurret303->setScale(0.06,0.06,0.06);

	  /*Ogre::Entity *subEntityTorret304 = mSceneMgr->createEntity("subEntityTorret304", "usb_cilindro02.mesh");
      Ogre::SceneNode *subNodeTurret304 = mSceneMgr->createSceneNode();
      subNodeTurret304->attachObject(subEntityTorret304);
	  subNodeTurret304->setPosition(0,5.5,7);
	  subNodeTurret304->setScale(0.4,0.7,0.4);
	  subNodeTurret304->pitch(Ogre::Degree(90.0f));*/
	  
	  
	  nodeTurret03->addChild(subnodeTurret301);
	  nodeTurret03->addChild(subNodeTurret302);
	  nodeTurret03->addChild(subNodeTurret303);
	  /*nodeTurret03->addChild(subNodeTurret304);*/
	 
	  nodeTurret03->setPosition(-22,-3,-637);
	  nodeTurret03->setScale(0.4,0.4,0.4);
	  
	  //Spaceship

      Ogre::SceneNode *nodeSpaceship = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodeSpaceship");
	  Ogre::SceneNode *subNodeSpaceshipLeftWing = nodeSpaceship->createChildSceneNode("subnodeSpaceshipLeftWing");
	  Ogre::SceneNode *subNodeSpaceshipRightWing = nodeSpaceship->createChildSceneNode("subNodeSpaceshipRightWing");

	  ManualObject* body1;
	  body1 = generateParallelepiped("body1",0.0,-1.2,0.0,0.0,1.2,1.2,2.3,1.2,1.2,2.3,3.0);
	  ManualObject* body2;
	  body2 = generateParallelepiped("body2",0.0,1.2,0.0,0.0,1.2,2.3,1.2,1.2,2.3,1.2,3.0);
	  ManualObject* body3;
	  body3 = generateParallelepiped("body3",0.0,-1.2,-9.5,0.9,0.3,0.3,0.8,1.2,1.2,2.3,6.5);
	  ManualObject* body4;
	  body4 = generateParallelepiped("body4",0.0,1.2,-9.5,-0.9,0.3,0.8,0.3,1.2,2.3,1.2,6.5);
	  ManualObject* right_upper_wing;
	  right_upper_wing = generateParallelepiped("right_upper_wing",6.3,0.3,-1.0,0.0,  0.1, 4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* right_down_wing;
	  right_down_wing = generateParallelepiped("right_down_wing",6.3,-0.3,-1.0,0.0,    0.1 ,4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* left_upper_wing;
	  left_upper_wing = generateParallelepiped("left_upper_wing",-6.3,0.3,-1.0,0.0,  0.1, 4.0, 4.0,  0.1,4.0,4.0,  1.0);
	  ManualObject* left_down_wing;
	  left_down_wing = generateParallelepiped("left_down_wing",-6.3,-0.3,-1.0,0.0,    0.1 ,4.0, 4.0,  0.1,4.0,4.0,  1.0);

	  nodeSpaceship->attachObject(body1);
	  nodeSpaceship->attachObject(body2);
	  nodeSpaceship->attachObject(body3);
	  nodeSpaceship->attachObject(body4);
	  subNodeSpaceshipRightWing->attachObject(right_upper_wing);
	  subNodeSpaceshipRightWing->attachObject(right_down_wing);
	  subNodeSpaceshipLeftWing->attachObject(left_upper_wing);
	  subNodeSpaceshipLeftWing->attachObject(left_down_wing);
	  
	  subNodeSpaceshipLeftWing->setPosition(0,0,0);
	  subNodeSpaceshipRightWing->setPosition(0,0,0);
	  nodeSpaceship->setPosition(0, 0, 0);


	}

};


int main (void)
{

	Example1 app;
	app.go();
	return 0;
}