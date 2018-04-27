/*
 * InputMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */
#include<cfloat>

#include <Engine.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <EntityMgr.h>
#include <GameMgr.h>
#include <UiMgr.h>
#include <PhysicsMgr.h>

#include <Utils.h>

InputMgr::InputMgr(Engine *engine) : Mgr(engine), OIS::KeyListener(), OIS::MouseListener() {

	this->mInputMgr = 0;
	this->mKeyboard = 0;
	this->mMouse = 0;
	this->keyboardTimer = keyTime;
	deltaDesiredSpeed = 10.0f;
	deltaDesiredHeading = 10.0f;
	deltaDesiredAltitude = 20;
	this->selectionDistanceSquaredThreshold = 10000;
}

InputMgr::~InputMgr() {

}

void InputMgr::Init(){

	  OIS::ParamList pl;
	  size_t windowHandle = 0;
	  std::ostringstream windowHandleStr;

	  engine->gfxMgr->mWindow->getCustomAttribute("WINDOW", &windowHandle);
	  windowHandleStr << windowHandle;
	  pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));
	  // insert the following lines right before calling mInputSystem = OIS::InputManager::createInputSystem( paramList );
		#if defined OIS_WIN32_PLATFORM
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
		#elif defined OIS_LINUX_PLATFORM
		pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
		//pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
		#endif

	  mInputMgr = OIS::InputManager::createInputSystem(pl);

	  mKeyboard = static_cast<OIS::Keyboard*>(
	    mInputMgr->createInputObject(OIS::OISKeyboard, true));
	  mMouse = static_cast<OIS::Mouse*>(
	    mInputMgr->createInputObject(OIS::OISMouse, true));

	  int left, top;
	  unsigned int width, height, depth;

	  engine->gfxMgr->mWindow->getMetrics(width, height, depth, left, top);
	  const OIS::MouseState &ms = mMouse->getMouseState();
	  ms.width = width;
	  ms.height = height;
	  mMouse->setEventCallback(this);
	  mKeyboard->setEventCallback(this);

}



void InputMgr::Stop(){
	if(mInputMgr){
		mInputMgr->destroyInputObject(mMouse);
		mInputMgr->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputMgr);
		mInputMgr = 0;
   	}
}

void InputMgr::Tick(float dt){

	mKeyboard->capture();
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)){
		engine->keepRunning = false;
	}
	mMouse->capture();
//	mTrayMgr->frameRenderingQueued(fe);

	UpdateCamera(dt);

}

void InputMgr::UpdateCamera(float dt){
	 //Ogre::Vector3 cameraPosition = engine->gameMgr->cameraNode->getPosition();
	 //Ogre::Vector3 desiredPosition = engine->gameMgr->desiredCameraPosition;
	 Ogre::Quaternion cameraOrientation = engine->gameMgr->baseOrientation;
	 Ogre::Quaternion desiredOrientation = engine->gameMgr->desiredOrientation;

	 float animTime = engine->gameMgr->baseTime;

	 //get interpolated rotation
	 Ogre::Quaternion current =
			 Ogre::Quaternion::Slerp(animTime,cameraOrientation,desiredOrientation,false);

	 //rotate cameranode
	 if (animTime < 1.0f)
		 engine->gameMgr->cameraNode->setOrientation(current);
	 else
		 engine->gameMgr->cameraNode->setOrientation(desiredOrientation);
}

void InputMgr::UpdateVelocityAndSelection(float dt){
	keyboardTimer -= dt;

	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD8)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredSpeed += deltaDesiredSpeed;
	}
}

void InputMgr::LoadLevel(){

}

bool InputMgr::keyPressed(const OIS::KeyEvent& ke){
    if (mKeyboard->isKeyDown(OIS::KC_W))
    {
    	engine->gameMgr->turnCamX(true);
    }

    if (mKeyboard->isKeyDown(OIS::KC_S))
    {
    	engine->gameMgr->turnCamX(false);
    }

    if (mKeyboard->isKeyDown(OIS::KC_A))
    {
    	engine->gameMgr->turnCamY(true);
    }

    if (mKeyboard->isKeyDown(OIS::KC_D))
    {
    	engine->gameMgr->turnCamY(false);
    }

    if (mKeyboard->isKeyDown(OIS::KC_E))
    {
    	engine->gameMgr->turnCamZ(true);
    }

    if (mKeyboard->isKeyDown(OIS::KC_Q))
    {
    	engine->gameMgr->turnCamZ(false);
    }

    if (mKeyboard->isKeyDown(OIS::KC_F))
    {
    	Ogre::Vector3 pos = Ogre::Vector3(0, 0, 0);
    	engine->entityMgr->CreateEntityOfTypeAtPosition(DDG51Type, pos);
    }


	return true;
}

bool InputMgr::keyReleased(const OIS::KeyEvent& ke){
	return true;
}

bool InputMgr::mouseMoved(const OIS::MouseEvent& me){
    if (engine->uiMgr->mTrayMgr->injectMouseMove(me)) return true;
	return true;
}

bool InputMgr::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID mid){
	std::cout << "Mouse pressed" << std::endl;
    if (engine->uiMgr->mTrayMgr->injectMouseDown(me, mid)) return true;
	if(OIS::MB_Left == mid){
		std::cout << "Left mouse press" << std::endl;
		HandleMouseSelection(me);
	}

	return true;
}

bool InputMgr::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID mid){
    if (engine->uiMgr->mTrayMgr->injectMouseUp(me, mid)) return true;
	return true;
}

//check if ms.width and ms.height need to be adjusted when things change
void InputMgr::HandleMouseSelection(const OIS::MouseEvent &me){
	const OIS::MouseState &ms = mMouse->getMouseState();
	int index = -1;
	Ogre::Ray mouseRay = engine->gfxMgr->mCamera->getCameraToViewportRay(ms.X.abs/(float) ms.width, ms.Y.abs/(float)ms.height);
	std::pair<bool, float> result = mouseRay.intersects(engine->gfxMgr->oceanSurface);
	if(result.first){
		Ogre::Vector3 posUnderMouse = mouseRay.getPoint(result.second);
		float minDistanceSquared = FLT_MAX;
		float distanceSquared; //because squareroot is expensive
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			distanceSquared = posUnderMouse.squaredDistance(engine->entityMgr->entities[i]->position);
			if (distanceSquared < selectionDistanceSquaredThreshold){
				if (distanceSquared < minDistanceSquared){
					index = i;
					minDistanceSquared = distanceSquared;
				}
			}
		}
		engine->entityMgr->Select(index);
	}
}


