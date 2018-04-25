/*
 * Physics3D.h
 *
 *  Created on: Mar 7, 2018
 *      Author: sushil
 */

#ifndef PHYSICS3D_H_
#define PHYSICS3D_H_

#include <FlyingEntity381.h>
#include <Physics2D.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <PhysicsMgr.h>

class Physics3D: public Aspect {
public:
	Physics3D(Entity381* ent);
	virtual ~Physics3D();

	void Tick(float dt);

	virtual void initPhysics();
	void applyForce(Ogre::Vector3);

    void setCollisionMesh(int base);
    void setCollisionMesh(int box, Ogre::Vector3 size);
    void setCollisionMesh(int sphere, float radius);
    void setCollisionMesh(int capCylCone, float, float);
    void setProperties(float mass, float friction, float restitution);
    void setDamping(float, float);

protected:
    Ogre::Vector3 btToOgre(btVector3);
    Ogre::Matrix4 btToOgre(btTransform);
    Ogre::Matrix3 btToOgre(btMatrix3x3);
    btVector3 ogreToBt(Ogre::Vector3);

    btRigidBody *physics;
    btCollisionShape *shape;
    btTransform transform;
    float mass;
    float friction;
    float restitution;
    bool isStatic;
};

#endif /* PHYSICS3D_H_ */
