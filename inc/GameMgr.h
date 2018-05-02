/*
 * GameMgr.h
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */

#ifndef GAMEMGR_H_
#define GAMEMGR_H_

#include <Mgr.h>

class GameMgr: public Mgr {
public:
	GameMgr(Engine *engine);
	virtual ~GameMgr();

	void Init();
	void LoadLevel();
	void Tick(float dt);

	void MakeGround();
	void MakeSky();
	void MakeEntities();

	Ogre::SceneNode *cameraNode;
	Ogre::Vector3 desiredCameraPosition;
	Ogre::Quaternion desiredOrientation;
	Ogre::Quaternion baseOrientation;
	float baseTime;
	float animTime;

	void turnCamX(bool isPositive);
	void turnCamY(bool isPositive);
	void turnCamZ(bool isPositive);
	void playerWon();
private:
	void rotateCam(float degrees, Ogre::Vector3 dir);
	bool winCondition;
};

#endif /* GAMEMGR_H_ */
