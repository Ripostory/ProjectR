/*
 * Physics3D.cpp
 *
 *  Created on: Mar 7, 2018
 *      Author: sushil
 *
 *      modified by Ronn Quijada
 */

#include <Physics3D.h>
#include <Entity381.h>
#include <Utils.h>

Physics3D::Physics3D(Entity381 * ent): Aspect(ent) {
	isStatic = false;
	physics = NULL;
	shape = new btSphereShape(1);
	mass = 1.0f;
	friction = 0.3f;
	restitution = 0.2;
	initPhysics();
}

Physics3D::~Physics3D() {
	if (shape != NULL)
	{
		//TODO check
		//delete shape;
		shape = NULL;
	}
	mass = 0.0f;
}


void Physics3D::Tick(float dt){
	  //update physics object
	  if (physics != NULL)
	  {
		  physics->getMotionState()->getWorldTransform(transform);

		  //TODO add more functionality
		  entity->position = btToOgre(transform.getOrigin());
		  std::cout << entity->position << std::endl;
	  }
}

void setCollisionMesh(int)
{
	//TODO implement
}

void Physics3D::setCollisionMesh(int box, Ogre::Vector3 size)
{
	delete shape;
	if (box == PHYS_BOX)
	{
		shape = new btBoxShape(ogreToBt(size));
	}
	else if (box == PHYS_CYLINDER)
	{
		shape = new btCylinderShape(ogreToBt(size));
	}
	else
	{
		std::cout << "INCORRECT PHYS INITIALIZATION" << std::endl;
		std::cout << "Defaulting to sphere" << std::endl;
		shape = new btSphereShape(1.0f);
	}
}

void Physics3D::setCollisionMesh(int sphere, float radius)
{
	delete shape;
	if (sphere == PHYS_SPHERE)
		shape = new btSphereShape(radius);
	else
	{
		std::cout << "INCORRECT PHYS INITIALIZATION" << std::endl;
		std::cout << "Defaulting to sphere" << std::endl;
		shape = new btSphereShape(1.0f);
	}
}

void Physics3D::setCollisionMesh(int capCylCone, float radius, float height)
{
	delete shape;
	if (capCylCone == PHYS_CAPSULE)
	{
		shape = new btCapsuleShape(radius, height);
	}
	else if (capCylCone == PHYS_CONE)
	{
		shape = new btConeShape(radius, height);
	}
	else
	{
		std::cout << "INCORRECT PHYS INITIALIZATION" << std::endl;
		std::cout << "Defaulting to sphere" << std::endl;
		shape = new btSphereShape(1.0f);
	}
}

void Physics3D::initPhysics()
{
	//base initial position on model matrix
	transform.setIdentity();

	//apply glm world position to simulated world
	//transform.setOrigin(glmToBt(glm::vec3(glm::vec4(0, 0, 0, 1) * glm::transpose(model))));
	//glm::quat qt = glm::quat_cast(model);
	//transform.setRotation(btQuaternion(qt.x,qt.y,qt.z,qt.w));

	//determine if static
	if (isStatic)
		mass = 0.0f;
	else
		mass = 1.0f;

	btVector3 inertia(0,0,0);
	shape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* objMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo objCI(0, NULL, NULL, inertia);
	objCI.m_localInertia = inertia;
	objCI.m_collisionShape = shape;
	objCI.m_motionState = objMotionState;
	objCI.m_mass = mass;
	objCI.m_restitution = restitution;
	objCI.m_friction = friction;

	physics = new btRigidBody(objCI);
	entity->engine->physicsMgr->physWorld->addRigidBody(physics);
}


Ogre::Vector3 Physics3D::btToOgre(btVector3 input)
{
	return Ogre::Vector3(input.getX(), input.getY(), input.getZ());
}

Ogre::Matrix4 Physics3D::btToOgre(btTransform matrix)
{
	float *x = NULL;
	matrix.getOpenGLMatrix(x);
	Ogre::Matrix4 final = Ogre::Matrix4(
			*(x+0),*(x+1),*(x+2),*(x+3),
			*(x+4),*(x+5),*(x+6),*(x+7),
			*(x+8),*(x+9),*(x+10),*(x+11),
			*(x+12),*(x+13),*(x+14),*(x+15));
	return final;
}

btVector3 Physics3D::ogreToBt(Ogre::Vector3 input)
{
	return btVector3(input.x, input.y, input.z);
}

void Physics3D::setProperties(float m, float f, float r)
{
	mass = m;
	if (mass == 0)
		isStatic = true;
	friction = f;
	restitution = r;
}

void Physics3D::setDamping(float directional, float rotational)
{
	physics->setDamping(directional, rotational);
}

void Physics3D::applyForce(Ogre::Vector3 force)
{
	physics->activate(false);
	physics->applyCentralImpulse(ogreToBt(force));
}



