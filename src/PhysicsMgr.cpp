/*
 * PhysicsMgr.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: rquijada
 */

#include "PhysicsMgr.h"

std::map<btCollisionObject*, Entity381*> PhysicsMgr::colliders;

PhysicsMgr::PhysicsMgr(Engine *ref) : Mgr(ref)
{
	baseGravity = Ogre::Vector3(0, -9.8*15, 0);
	gravity = baseGravity;
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
	  physWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

}

void PhysicsMgr::LoadLevel()
{

}

void PhysicsMgr::Tick(float dt)
{
    //Update the physics
    physWorld->stepSimulation(dt, 15);
		physWorld->performDiscreteCollisionDetection();

    int numManifolds = physWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold =  physWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        //check if any collisions have occurred, and send message
		Entity381* physObA = btToPhysObject(obA);
		Entity381* physObB = btToPhysObject(obB);
    	contactManifold->refreshContactPoints(obA->getWorldTransform(), obB->getWorldTransform());

			if (physObA != NULL && physObB != NULL)
				physObA->onCollision(physObB);
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
	physWorld->setGravity(btVector3(gravity.x,gravity.y,gravity.z));
}

void PhysicsMgr::makePlane(Ogre::Vector3 n, float distance)
{
	btRigidBody *planeCollider;
	btCollisionShape* groundShape =
			new btStaticPlaneShape(btVector3(n.x, n.y, n.z), distance);
	btDefaultMotionState* groundMotionState =
	        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
	        groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_restitution = 0.5f;
	groundRigidBodyCI.m_friction = 0.5f;

	//add collider
	planeCollider = new btRigidBody(groundRigidBodyCI);
	physWorld->addRigidBody(planeCollider);
}

Entity381* PhysicsMgr::btToPhysObject(const btCollisionObject* obj)
{
	std::map<btCollisionObject*,Entity381*>::iterator it;

	it = colliders.find((btCollisionObject*)obj);
	if (it != colliders.end())
		return it -> second;

	return NULL;
}





