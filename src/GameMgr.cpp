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
#include <SoundMgr.h>
#include <UiMgr.h>
#include <iostream>
#include <Types381.h>
#include <Entity381.h>

#include <OgreOverlay.h>
#include <OgreSceneNode.h>


GameMgr::GameMgr(Engine *engine): Mgr(engine) {
	cameraNode = 0;
	baseTime = 0;
	animTime = 0.1;
	winCondition = false;
	turnCounter = 0;
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

	  // A node to attach the camera to so we can move the camera node instead of the camera.
	  cameraNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	  cameraNode->setPosition(0, 0, 0);
	  cameraNode->attachObject(engine->gfxMgr->mCamera);

	  //make camera offset
	  engine->gfxMgr->mCamera->setPosition(1600 ,1600,1600);
	  engine->gfxMgr->mCamera->lookAt(0,0,0);

	  //engine->gfxMgr->MakeSky();
	  MakeEntities();
	  //engine->soundMgr->loadStartBackground();
	  turnCounter = 0;
}


void GameMgr::MakeEntities(){
	Ogre::Vector3 pos = Ogre::Vector3(0, 0, 0);
	engine->entityMgr->CreateEntityOfTypeAtPosition(PlayerType, pos);
	engine->entityMgr->CreateLevel(LevelType, pos, "test.mesh");
	engine->entityMgr->CreateLevel(LevelType, pos, "Room.mesh");
	engine->entityMgr->CreateEntityOfTypeAtPosition(PatrolerType, Ogre::Vector3(600, -100, 0));
	//engine->entityMgr->CreateEntityOfTypeAtPosition(SpeedBoatType, pos);
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

	//play sound
	engine->soundMgr->playSelect();

	//update turn counter
	turnCounter++;
}

void GameMgr::playerWon()
{
	if (!winCondition)
	{
		winCondition = true;
		engine->uiMgr->openTextBox( "Player", "Level 1 completed!");
		engine->uiMgr->openTextBox( "Player", "Press start to restart level");

		if(engine->entityMgr->lvl == 1)
		{
			engine->uiMgr->openTextBox("GAME", "YOU ESCAPED!!");
			engine->uiMgr->openTextBox("Project Arr Credits", "Ronn(With Two Ns) Quijada:\n Sound\n Physics\n Camera\n Master Computer");
			engine->uiMgr->openTextBox("Project Arr Credits", "Brianna Blain-Castelli:\n Artificial Intelligence\n Models\n User Interface\n Debugger");
			engine->uiMgr->openTextBox("Project Arr Credits", "Ryan Fox:\n Artificial Intelligence\n Splash Screen\n User Interface\n Ideas Man");

		}
		else
		{
			engine->entityMgr->lvl++;
			engine->entityMgr->levels[engine->entityMgr->lvl]->Init();
		}
	}
}




