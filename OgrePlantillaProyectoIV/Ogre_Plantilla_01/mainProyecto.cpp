#include "Ogre\ExampleApplication.h"

float test;
Ogre::AnimationState* animationStateTurret301;
Ogre::AnimationState* animationStateOpenWingsSpaceship1;
Ogre::AnimationState* animationStateOpenWingsSpaceship2;
Ogre::AnimationState* animationStateOpenWingsSpaceship3;
Ogre::AnimationState* animationStateOpenWingsSpaceship4;
Ogre::AnimationState* animationStateCloseWingsSpaceship1;
Ogre::AnimationState* animationStateCloseWingsSpaceship2;
Ogre::AnimationState* animationStateCloseWingsSpaceship3;
Ogre::AnimationState* animationStateCloseWingsSpaceship4;

bool open = false;

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
				open = false;
			}
		}

		

		//camara control
		float rotX = _mouse->getMouseState().X.rel * evt.timeSinceLastFrame*-1;
		float rotY = _mouse->getMouseState().Y.rel * evt.timeSinceLastFrame*-1;
		_cam->yaw(Ogre::Radian(rotX));
		_cam->pitch(Ogre::Radian(rotY));
		_cam->moveRelative(tcam*movSpeed*evt.timeSinceLastFrame);
		

		animationStateTurret301->addTime(evt.timeSinceLastFrame);
		animationStateOpenWingsSpaceship1->addTime(evt.timeSinceLastFrame);
		animationStateOpenWingsSpaceship2->addTime(evt.timeSinceLastFrame);
		animationStateOpenWingsSpaceship3->addTime(evt.timeSinceLastFrame);
		animationStateOpenWingsSpaceship4->addTime(evt.timeSinceLastFrame);
		animationStateCloseWingsSpaceship1->addTime(evt.timeSinceLastFrame);
		animationStateCloseWingsSpaceship2->addTime(evt.timeSinceLastFrame);
		animationStateCloseWingsSpaceship3->addTime(evt.timeSinceLastFrame);
		animationStateCloseWingsSpaceship4->addTime(evt.timeSinceLastFrame);
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

	  


	}

};


int main (void)
{

	Example1 app;
	app.go();
	return 0;
}