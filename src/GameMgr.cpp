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
	zoom = 8.0f;
	zoomMin = 8.0f;
	zoomMax = 45.0f;
	currentZoom = 8.0f;
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

	  //move camera to follow point
	  Entity381 *ref = engine->entityMgr->selectedEntity;


	  //update camera
	  if (ref != NULL)
	  {
		  //move camera
		  Ogre::Vector3 focus = ref->position;

		  //transform focus position to camera space
		  Ogre::Quaternion rotate = cameraNode->getOrientation();
		  Ogre::Matrix3 ref;
		  rotate.normalise();
		  rotate = rotate.Inverse();
		  rotate.ToRotationMatrix(ref);

		  focus = ref * focus;

		  Ogre::Vector3 transformNormal(Ogre::Vector3(-1,-1,-1));
		  transformNormal.normalise();
		  Ogre::Vector3 origin(0,0,0);
		  Ogre::Vector3 finalPosition = focus - origin;
		  finalPosition.normalise();
		  float dist = finalPosition.dotProduct(transformNormal);
		  finalPosition = focus - dist*transformNormal;

		  //move camera to new position
		  engine->gfxMgr->mCamera->setPosition(finalPosition+1600);
	  }

	  //set camera zoom
	  currentZoom += (zoom-currentZoom)/30;
	  engine->gfxMgr->mCamera->setFOVy(Ogre::Degree(currentZoom));
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

void GameMgr::addZoom(float dt)
{
	zoom += dt;
	if (zoom > zoomMax)
	{
		zoom = zoomMax;
	}
	else if (zoom < zoomMin)
	{
		zoom = zoomMin;
	}
}

void GameMgr::resetOrientation()
{
	//reset gravity
	Ogre::Vector3 base = engine->physicsMgr->baseGravity;
	Ogre::Quaternion ref(Ogre::Degree(90), Ogre::Vector3(1,0,0));

	//reset all orientations
	baseOrientation = ref;
	desiredOrientation = ref;
	baseTime = 1.0f;

	//reset gravity orientation
	Ogre::Vector3 nDir = ref * base;
	engine->physicsMgr->setGravity(nDir);
}

void GameMgr::loadLevel1()
{
	resetOrientation();
	Ogre::Vector3 pos = Ogre::Vector3(0, 0, 0);
	engine->entityMgr->CreateEntityOfTypeAtPosition(PlayerType, pos);
	engine->entityMgr->SelectNextEntity();
	engine->entityMgr->CreateLevel("Level1.mesh");
}

void GameMgr::loadLevel2()
{
	resetOrientation();
	engine->entityMgr->lvl++;
	engine->entityMgr->ClearEntities();
	engine->entityMgr->CreateEntityOfTypeAtPosition(PlayerType, Ogre::Vector3(0,0,0));
	engine->entityMgr->SelectNextEntity();
	engine->entityMgr->CreateLevel("Level2.mesh");
	engine->entityMgr->CreateEntityOfTypeAtPosition(PatrolerType, Ogre::Vector3(13, 490, -40));
}

void GameMgr::loadLevel3()
{
	engine->entityMgr->lvl++;
	engine->entityMgr->ClearEntities();
	engine->entityMgr->CreateEntityOfTypeAtPosition(PlayerType, Ogre::Vector3(0,0,0));
	engine->entityMgr->SelectNextEntity();
	engine->entityMgr->CreateLevel("Level3.mesh");
	resetOrientation();
	engine->entityMgr->CreateEntityOfTypeAtPosition(PatrolerType, Ogre::Vector3(1163, -456, 0));
}

void GameMgr::loadLevel4()
{
	engine->entityMgr->lvl++;
	engine->entityMgr->ClearEntities();
	engine->entityMgr->CreateEntityOfTypeAtPosition(PlayerType, Ogre::Vector3(0,0,0));
	engine->entityMgr->SelectNextEntity();
	engine->entityMgr->CreateLevel("Level4.mesh");
	resetOrientation();
	engine->entityMgr->CreateEntityOfTypeAtPosition(PatrolerType, Ogre::Vector3(578, -267, -881));
}

void GameMgr::resetGame()
{
	winCondition = false;
	engine->entityMgr->lvl = 0;
	engine->entityMgr->ClearEntities();
	loadLevel1();
}

void GameMgr::playerWon()
{
	int currentLvl = engine->entityMgr->lvl;
	int finalLvl = 3;

	if (!winCondition)
	{
		winCondition = true;
		engine->soundMgr->playClear();

		if(currentLvl == finalLvl)
		{
			engine->entityMgr->ClearEntities();
			engine->uiMgr->openTextBox("GAME", "YOU ESCAPED!!");
			engine->uiMgr->openTextBox("Project Arr Credits", "Ronn(With Two Ns) Quijada - Master Computer:\n Sound\n Physics\n Camera\n ");
			engine->uiMgr->openTextBox("Project Arr Credits", "Brianna Blain-Castelli - Debugger:\n Artificial Intelligence\n Models\n User Interface\n ");
			engine->uiMgr->openTextBox("Project Arr Credits", "Ryan Fox - Ideas Man:\n Artificial Intelligence\n Splash Screen\n User Interface\n");
			engine->uiMgr->mTrayMgr->createLabel(OgreBites::TL_CENTER,"MenuLabel","Thanks For Playing!",250);
			engine->uiMgr->mTrayMgr->createButton(OgreBites::TL_CENTER, "MainMenu4", "Play Again", 250);
			engine->uiMgr->mTrayMgr->createButton(OgreBites::TL_CENTER, "MainMenu3", "Quit Game", 250);
		}
		else
		{
			if (currentLvl == 0)
			{
				engine->uiMgr->openTextBox( "Lt.Astolfo", "Well, at least I'm free from my cell...");
				engine->uiMgr->openTextBox( "Lt.Astolfo", "Uh oh, a guard is coming!");
				engine->uiMgr->openTextBox( "Lt.Astolfo", "I just need to get to that door around the corner...");
				loadLevel2();
				winCondition = false;
			}
			else if (currentLvl == 1)
			{
				engine->uiMgr->openTextBox( "Lt.Astolfo", "Got out of that mess without any trouble.");
				engine->uiMgr->openTextBox( "Lt.Astolfo", "Look's like one of the loading bays are open; maybe I can slip out through there");
				loadLevel3();
				winCondition = false;
			}
			else if (currentLvl == 2)
			{
				engine->uiMgr->openTextBox( "Lt.Astolfo", "Almost there, I can almost feel it!");
				engine->uiMgr->openTextBox( "Lt.Astolfo", "Out that escape pod entrance and I'll be free!");
				loadLevel4();
				winCondition = false;
			}
		}
	}
}




