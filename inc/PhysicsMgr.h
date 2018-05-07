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

#define PHYS_BOX 		0
#define PHYS_SPHERE 	1
#define PHYS_CAPSULE 	2
#define PHYS_CYLINDER	3
#define PHYS_CONE		4
#define PHYS_HULL		5
#define PHYS_S_MESH		6

#ifndef PHYSICSMGR_H_
#define PHYSICSMGR_H_



class PhysicsMgr : public Mgr
{
public:
	static std::map<btCollisionObject*, Entity381*> colliders;

	PhysicsMgr(Engine *ref);
	~PhysicsMgr();
	virtual void Init();
	virtual void LoadLevel();
	virtual void Tick(float dt);
	virtual void Stop();

	Entity381* btToPhysObject(const btCollisionObject*);
	Ogre::Vector3 getGravity();
	void setGravity(Ogre::Vector3);
	void makePlane(Ogre::Vector3 normal, float distance);
	Ogre::Vector3 baseGravity;
	btDiscreteDynamicsWorld *physWorld;
private:
	Ogre::Vector3 gravity;

    //physics engine
    btDefaultCollisionConfiguration *collConfig;
    btCollisionDispatcher *physDispatcher;
    btBroadphaseInterface *overlapPairCache;
    btSequentialImpulseConstraintSolver *solver;
};


#endif /* PHYSICSMGR_H_ */
