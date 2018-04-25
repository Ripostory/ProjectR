/*
 * PhysicsMgr.h
 *
 *  Created on: Apr 25, 2018
 *      Author: rquijada
 *
 *      handles all bullet physics
 */

#include "Mgr.h"
#include "Entity381.h"
#include <bullet/btBulletDynamicsCommon.h>

#ifndef PHYSICSMGR_H_
#define PHYSICSMGR_H_

class PhysicsMgr : public Mgr
{
public:
	PhysicsMgr(Engine *ref);
	~PhysicsMgr();
	virtual void Init();
	virtual void LoadLevel();
	virtual void Tick(float dt);
	virtual void Stop();

	Ogre::Vector3 getGravity();
	void setGravity(Ogre::Vector3);
private:
	Ogre::Vector3 gravity;

    //physics engine
    btDefaultCollisionConfiguration *collConfig;
    btCollisionDispatcher *physDispatcher;
    btBroadphaseInterface *overlapPairCache;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *physWorld;
};


#endif /* PHYSICSMGR_H_ */
