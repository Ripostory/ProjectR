/*
 * GameMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */

#include <Engine.h>

#include <EntityMgr.h>
#include <GameMgr.h>
#include <GfxMgr.h>
#include <PhysicsMgr.h>

#include <iostream>
#include <Types381.h>

#include <OgreOverlay.h>
#include <OgreSceneNode.h>


GameMgr::GameMgr(Engine *engine): Mgr(engine) {
	cameraNode = 0;
	baseTime = 0;
	animTime = 0.1;
}

GameMgr::~GameMgr() {
	// TODO Auto-generated destructor stub
}

void GameMgr::Init(){

}

void GameMgr::Tick(float dt)
{
	//update animation timer
	 if (baseTime <= 1.0f)
		 baseTime += dt/animTime;
}

void GameMgr::LoadLevel(){

	  engine->gfxMgr->mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	  engine->gfxMgr->mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	  Ogre::Light* light = engine->gfxMgr->mSceneMgr->createLight("MainLight");
	  light->setPosition(20.0, 80.0, 50.0);

	  //room
	    Ogre::Entity* ogreEntity3 = engine->gfxMgr->mSceneMgr->createEntity("Room.mesh");
	    Ogre::SceneNode* ogreNode3 = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	    ogreNode3->setPosition(0, 0, 0);
	    ogreNode3->attachObject(ogreEntity3);

	  // A node to attach the camera to so we can move the camera node instead of the camera.
	  cameraNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	  cameraNode->setPosition(0, 0, 0);
	  cameraNode->attachObject(engine->gfxMgr->mCamera);

	  //make camera offset
	  engine->gfxMgr->mCamera->setPosition(800 ,800,800);
	  engine->gfxMgr->mCamera->lookAt(0,0,0);

	  //engine->gfxMgr->MakeSky();
	  engine->physicsMgr->makePlane();
	  MakeEntities();
}


void GameMgr::MakeEntities(){
	Ogre::Vector3 pos = Ogre::Vector3(0, 0, 0);
	engine->entityMgr->CreateEntityOfTypeAtPosition(DDG51Type, pos);
	engine->entityMgr->SelectNextEntity(); //sets selection
}


void GameMgr::turnCamX(bool isPositive)
{
	Ogre::Vector3 axis(1,0,0);
	if (isPositive)
		rotateCam(90, axis);
	else
		rotateCam(-90, axis);
}

void GameMgr::turnCamY(bool isPositive)
{
	Ogre::Vector3 axis(0,1,0);
	if (isPositive)
		rotateCam(90, axis);
	else
		rotateCam(-90, axis);
}

void GameMgr::turnCamZ(bool isPositive)
{
	Ogre::Vector3 axis(0,0,1);
	if (isPositive)
		rotateCam(90, axis);
	else
		rotateCam(-90, axis);
}


void GameMgr::rotateCam(float degrees, Ogre::Vector3 axis)
{
	Ogre::Vector3 dir = engine->physicsMgr->baseGravity;
	Ogre::Quaternion base(cameraNode->getOrientation());
	Ogre::Quaternion offset(Ogre::Degree(degrees), axis);
	//set base orientation only if last animation finished
	if (baseTime < 1.0f)
	{
		baseOrientation = desiredOrientation;
		desiredOrientation = baseOrientation * offset;
	}
	else
	{
		desiredOrientation = base * offset;
		baseOrientation = base;
	}
	baseTime = 0;
	Ogre::Vector3 nDir = desiredOrientation * dir;
	engine->physicsMgr->setGravity(nDir);
}






