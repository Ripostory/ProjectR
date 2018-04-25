/*
 * PhysicsMgr.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: rquijada
 */

#include "PhysicsMgr.h"

PhysicsMgr::PhysicsMgr(Engine *ref) : Mgr(ref)
{
}

PhysicsMgr::~PhysicsMgr()
{
}

void PhysicsMgr::Init()
{
	  //initialize physics engine
	  overlapPairCache = new btDbvtBroadphase();

	  collConfig = new btDefaultCollisionConfiguration();
	  physDispatcher = new btCollisionDispatcher(collConfig);
	  solver = new btSequentialImpulseConstraintSolver;

	  physWorld = new btDiscreteDynamicsWorld(physDispatcher,
			  overlapPairCache, solver, collConfig);
	  physWorld->setGravity(btVector3(0, -9.8, 0));
}

void PhysicsMgr::LoadLevel()
{

}

void PhysicsMgr::Tick(float dt)
{
    //Update the physics
    physWorld->stepSimulation((float) dt/1000.0f, 30);
		physWorld->performDiscreteCollisionDetection();


    int numManifolds = physWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold =  physWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        //TODO update for Entity381 Physics aspect
		//PhysObject* physObA = PhysObject::btToPhysObject (obA);
		//PhysObject* physObB = PhysObject::btToPhysObject (obB);
    	//contactManifold->refreshContactPoints(obA->getWorldTransform(), obB->getWorldTransform());

			//if (physObA != NULL && physObB != NULL)
				//physObA -> OnCollisionDetected (physObB);
    }
}

void PhysicsMgr::Stop()
{
	  delete physWorld;
	  delete solver;
	  delete overlapPairCache;
	  delete physDispatcher;
	  delete collConfig;
}

Ogre::Vector3 PhysicsMgr::getGravity()
{
	return gravity;
}

void PhysicsMgr::setGravity(Ogre::Vector3 ref)
{
	gravity = ref;
}


