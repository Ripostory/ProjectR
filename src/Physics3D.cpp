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
#include <OgreSubMesh.h>

Physics3D::Physics3D(Entity381 * ent): Aspect(ent) {
	isStatic = false;
	physics = NULL;
	shape = new btSphereShape(50);
	mass = 10.0f;
	friction = 0.3f;
	restitution = 0.2;
}

Physics3D::~Physics3D() {
	//remove self
	if (physics != NULL)
	{
		entity->engine->physicsMgr->physWorld->removeRigidBody(physics);
	}

	if (shape != NULL)
	{
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
		  //get quaternion from transform
		  entity->sceneNode->setOrientation(Ogre::Quaternion(btToOgre(transform.getBasis())));
	  }
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

void Physics3D::setCollisionMesh(int mesh)
{
	if (mesh == PHYS_HULL)
	{
		//load in collision mesh
		Ogre::Mesh *model = entity->ogreEntity->getMesh().getPointer();
		btConvexHullShape *meshShape = new btConvexHullShape();

		//for each submesh
		for (int i = 1; i < model->getNumSubMeshes(); i++)
		{
			Ogre::SubMesh *submesh = model->getSubMesh(i);
			Ogre::VertexData *data = submesh->vertexData;
			const Ogre::VertexElement *vertexPos =
					data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vertexBuffer =
					data->vertexBufferBinding->getBuffer(vertexPos->getSource());
			unsigned char* vertices =
					static_cast<unsigned char*>(vertexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
			float* vertex;
			for (int j = 0; j < data->vertexCount; j++)
			{
				vertexPos->baseVertexPointerToElement(vertices, (void **)&vertex);
				btVector3 v1(vertex[0], vertex[1], vertex[2]);
				meshShape->addPoint(v1, true);
				vertices += vertexBuffer->getVertexSize();
			}
			vertexBuffer->unlock();
		}
		delete shape;
		shape = meshShape;
	}
	else if (mesh == PHYS_S_MESH)
	{
		/*
		glm::vec3 vert;
		btTriangleMesh *mesh = new btTriangleMesh();
		btVector3 *vertex;
		physMesh.clear();
		for (int i = 0; i < final.getIndices().size(); i++)
		{
			//load in points
			vert = final.getVerts()[final.getIndices()[i]].vertex;
			vertex = new btVector3(vert.x, vert.y, vert.z);
			physMesh.push_back(*vertex);
			delete vertex;
		}

		for (int i = 0; i < physMesh.size(); i += 3)
		{
			//send in triangle
			mesh->addTriangle(physMesh[i],physMesh[i+1],physMesh[i+2]);
		}

		delete shape;
		shape = new btBvhTriangleMeshShape(mesh, true);
		isStatic = true;
		 */
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
	transform.setOrigin(ogreToBt(entity->position));
	//Ogre::Quaternion qt = Ogre::Quaternion(entity->sceneNode->getLocalAxes());
	//transform.setRotation(btQuaternion(qt.x,qt.y,qt.z,qt.w));

	//determine if static
	if (isStatic)
		mass = 0.0f;

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
	physics->setActivationState(DISABLE_DEACTIVATION);
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

Ogre::Matrix3 Physics3D::btToOgre(btMatrix3x3 input)
{
	return Ogre::Matrix3(
			input[0][0],input[0][1],input[0][2],
			input[1][0],input[1][1],input[1][2],
			input[2][0],input[2][1],input[2][2]);
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



