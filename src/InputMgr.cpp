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
	float move = 400.0f;
	float rotate = 0.1f;

	 Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;
	 Ogre::Vector3 cameraPosition = engine->gameMgr->cameraNode->getPosition();
	 Ogre::Vector3 desiredPosition = engine->gameMgr->desiredCameraPosition;
	 Ogre::Quaternion cameraOrientation = engine->gameMgr->cameraNode->getOrientation();
	 Ogre::Quaternion desiredOrientation = engine->gameMgr->desiredOrientation;

	 //if(cameraPosition == desiredPosition && cameraOrientation == desiredOrientation)
	// {

	    if (mKeyboard->isKeyDown(OIS::KC_W))
	    {
	    	std::cout << "test " << engine->physicsMgr->getGravity().y << std::endl;
		  if(engine->physicsMgr->getGravity().y == Ogre::Real(9.8)){
			  engine->physicsMgr->setGravity(Ogre::Vector3(0, 0, 9.8));
			  engine->gameMgr->cameraNode->setPosition(0, 900, 0);
			  engine->gameMgr->cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_LOCAL);
		  }
		  else if(engine->physicsMgr->getGravity().z == Ogre::Real(9.8)){
			  engine->physicsMgr->setGravity(Ogre::Vector3(0, -9.8, 0));
			  engine->gameMgr->cameraNode->setPosition(0, 0, 900);
			  engine->gameMgr->cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_LOCAL);
	      }
		  if(engine->physicsMgr->getGravity().y == Ogre::Real(-9.8)){
		    	std::cout << "test2" << std::endl;
			  engine->physicsMgr->setGravity(Ogre::Vector3(0,0, -9.8));
			  engine->gameMgr->cameraNode->setPosition(0, -900, 0);
			  engine->gameMgr->cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_LOCAL);
	      }
		  else if(engine->physicsMgr->getGravity().z == Ogre::Real(-9.8)){
			  engine->physicsMgr->setGravity(Ogre::Vector3(0,9.8,0));
			  engine->gameMgr->cameraNode->setPosition(0, 0, -900);
			  engine->gameMgr->cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_LOCAL);
	      }
	    }
	// }


	 /*else
	 {
		 if(cameraPosition != desiredPosition)
		 {
			 if (cameraPosition.x > desiredPosition.x){
				 engine->gameMgr->cameraNode->setPosition(cameraPosition.x + engine->gameMgr->cameraSpeed * dt, cameraPosition.y, cameraPosition.z);
				 engine->gameMgr->cameraNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);
			 }
			 else if (cameraPosition.x < desiredPosition.x){
				 engine->gameMgr->cameraNode->setPosition(cameraPosition.x + -1 * engine->gameMgr->cameraSpeed * dt, cameraPosition.y, cameraPosition.z);
		 	 }
			 if (cameraPosition.y > desiredPosition.y){
				engine->gameMgr->cameraNode->setPosition(cameraPosition.x, cameraPosition.y + engine->gameMgr->cameraSpeed * dt, cameraPosition.z);
			 }
			 else if (cameraPosition.y < desiredPosition.y){
					engine->gameMgr->cameraNode->setPosition(cameraPosition.x, cameraPosition.y + -1 * engine->gameMgr->cameraSpeed * dt, cameraPosition.z);
			 }
			 if (cameraPosition.z > desiredPosition.z){
					engine->gameMgr->cameraNode->setPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z + engine->gameMgr->cameraSpeed * dt);
			 }
			 else if (cameraPosition.z < desiredPosition.z){
					engine->gameMgr->cameraNode->setPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z + -1 * engine->gameMgr->cameraSpeed * dt);
			 }
		 }
		 if(cameraOrientation != desiredOrientation)
		 {
			 if (cameraOrientation.x > desiredOrientation.x){
				 engine->gameMgr->cameraNode->setOrientation(1 ,cameraOrientation.x + engine->gameMgr->cameraTurnRate * dt, cameraOrientation.y, cameraOrientation.z);
			 }
			 else if (cameraOrientation.x < desiredOrientation.x){
				 engine->gameMgr->cameraNode->setOrientation(1, cameraOrientation.x + -1 * engine->gameMgr->cameraTurnRate * dt, cameraOrientation.y, cameraOrientation.z);
		 	 }
			 if (cameraOrientation.y > desiredOrientation.y){
				engine->gameMgr->cameraNode->setOrientation(1, cameraOrientation.x, cameraOrientation.y + engine->gameMgr->cameraTurnRate * dt, cameraOrientation.z);
			 }
			 else if (cameraOrientation.y < desiredOrientation.y){
					engine->gameMgr->cameraNode->setOrientation(1, cameraOrientation.x, cameraOrientation.y + -1 * engine->gameMgr->cameraTurnRate * dt, cameraOrientation.z);
			 }
			 if (cameraOrientation.z > desiredOrientation.z){
					engine->gameMgr->cameraNode->setOrientation(1, cameraOrientation.x, cameraOrientation.y, cameraOrientation.z + engine->gameMgr->cameraTurnRate * dt);
			 }
			 else if (cameraOrientation.z < desiredOrientation.z){
					engine->gameMgr->cameraNode->setOrientation(1, cameraOrientation.x, cameraOrientation.y, cameraOrientation.z + -1 * engine->gameMgr->cameraTurnRate * dt);
			 }
		 }*/

	 // engine->gameMgr->cameraNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);
}

void InputMgr::UpdateVelocityAndSelection(float dt){
	keyboardTimer -= dt;

	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD8)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredSpeed += deltaDesiredSpeed;
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD2)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredSpeed -= deltaDesiredSpeed;
	}


	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD9)){
		keyboardTimer = keyTime;
		if(engine->entityMgr->selectedFlyingEntity != 0){
			engine->entityMgr->selectedFlyingEntity->desiredAltitude += deltaDesiredAltitude;
		}
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD3)){
		keyboardTimer = keyTime;
		if(engine->entityMgr->selectedFlyingEntity != 0)
			engine->entityMgr->selectedFlyingEntity->desiredAltitude -= deltaDesiredAltitude;
	}


	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD4)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredHeading -= deltaDesiredHeading;
	//turn left is decreasing degrees, turn right is increasing degrees because increasing degrees gives us the +ive Z axis
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD6)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredHeading += deltaDesiredHeading;
	}
	engine->entityMgr->selectedEntity->desiredHeading = FixAngle(engine->entityMgr->selectedEntity->desiredHeading);




	//Set velocity to zero....
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_SPACE)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->velocity = Ogre::Vector3::ZERO;
		engine->entityMgr->selectedEntity->desiredSpeed = engine->entityMgr->selectedEntity->speed = 0;
		engine->entityMgr->selectedEntity->desiredHeading = engine->entityMgr->selectedEntity->heading;
	}

	//tab handling
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_TAB)){
		keyboardTimer = keyTime;
		engine->entityMgr->SelectNextEntity();
	}
}

void InputMgr::LoadLevel(){

}

bool InputMgr::keyPressed(const OIS::KeyEvent& ke){
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


