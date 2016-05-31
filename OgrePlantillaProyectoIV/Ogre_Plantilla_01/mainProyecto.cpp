#include "Ogre\ExampleApplication.h"


class Example1 : public ExampleApplication
{

public:

	void createCamera() {

		mCamera = mSceneMgr->createCamera("MyCamera1");
		mCamera->setPosition(0,10,50);
		mCamera->lookAt(0,0,-50);
		mCamera->setNearClipDistance(5);

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

	}

};


int main (void)
{

	Example1 app;
	app.go();
	return 0;
}