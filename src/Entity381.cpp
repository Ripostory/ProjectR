/*
 * Entity381.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include <GfxMgr.h>

#include<Entity381.h>
#include<Physics3D.h>
#include<Physics2D.h>
#include<GameMgr.h>
#include<AI.h>

std::string IntToString(int x){
	char tmp[10000];
	sprintf(tmp, "%i", x);
	return std::string(tmp);
}

Entity381::Entity381(Engine *engine, Ogre::Vector3 pos, int ident, float newMass){

	this->engine = engine;

	entityType = None;
	meshfilename = std::string();
	position = pos;
	velocity = Ogre::Vector3(0, 0, 0);
	identity = ident;
	isSelected = false;

	name = "Entity381";//meshfname + IntToString(identity);
	physType = PHYS_HULL;
/*
	ogreEntity = engine->gfxMgr->mSceneMgr->createEntity(meshfilename);
	sceneNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode(pos);
	sceneNode->attachObject(ogreEntity);*/
	sceneNode = 0;
	ogreEntity = 0;

	Physics3D* phx = new Physics3D(this);
	aspects.push_back((Aspect*) phx);
	Renderable * renderable = new Renderable(this);
	aspects.push_back((Aspect*)renderable);

	this->acceleration = 0;
	this->desiredHeading = this->heading = 0;
	this->turnRate = 0;
	this->desiredSpeed = this->speed = 0;
	this->minSpeed = this->maxSpeed = 0;

	this->playSound = false;
	this->soundFile = "Boat-Sound.wav";
	this->auioId = 0;
	this->didSelectSoundPlay = false;

	//physics data
	mass = newMass;
	friction = 0.3f;
	restitution = 0.2;

}

Entity381::~Entity381(){
	std::cout << "Clearing entity " << name << std::endl;

	//remove from world
	engine->gfxMgr->mSceneMgr->destroySceneNode(sceneNode);
	engine->gfxMgr->mSceneMgr->destroyEntity(ogreEntity);

	//delete all aspects
	std::vector<Aspect*>::iterator it;
	for (it = aspects.begin(); it != aspects.end(); it++)
	{
		delete *it;
	}

	aspects.clear();
}

void Entity381::Init(){

	name = meshfilename + IntToString(identity);
	ogreEntity = engine->gfxMgr->mSceneMgr->createEntity(meshfilename);
	sceneNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
	sceneNode->attachObject(ogreEntity);
	Physics3D *phys = (Physics3D*)aspects[0];
	phys->setCollisionMesh(physType, meshfilename);
	phys->initPhysics();
}

void Entity381::Tick(float dt){
	for(unsigned int i = 0; i < aspects.size(); i++){
		aspects[i]->Tick(dt);
	}
}

void Entity381::onCollision(Entity381* ref)
{
	//a collision has been detected with ref
	//std::cout << this->name << " collided with object " << ref->name << std::endl;

}

void Entity381::onRaycastHit()
{
	//object was hit by a ray
	std::cout << "Oof, " << this->name << " was hit" << std::endl;
}

//-------------------------------------------------------------------------------------------------------------------------------
Player::Player(Engine *engine, Ogre::Vector3 pos, int ident):
		Entity381(engine, pos, ident, 10.0f){

	meshfilename = "SpaceMan.mesh";
	entityType = PlayerType;
	this->minSpeed = 0;
	this->maxSpeed = 16.0f;//meters per second...
	this->acceleration = 5.0f; // fast
	this->turnRate = 20.0f; //4 degrees per second
	Physics3D *ref = (Physics3D*) aspects[0];
	ref->setProperties(60, 0.8, 0.6);
}

void Player::Tick(float dt)
{
	Entity381::Tick(dt);

	//check if win condition satisfied
	if (position.length() > 1300)
	{
		engine->gameMgr->playerWon();
	}

}

Player::~Player(){

}

//-------------------------------------------------------------------------------------------------------------------------------
Level::Level(Engine *engine, Ogre::Vector3 pos, int ident, std::string meshName):
		Entity381(engine, pos, ident, 10.0f){
	meshfilename = meshName;
	entityType = LevelType;
	physType = PHYS_S_MESH;
	Physics3D *ref = (Physics3D*) aspects[0];
	ref->setProperties(0, 0.89, 0.3);
}

Level::~Level(){
}

//-------------------------------------------------------------------------------------------------------------------------------
Patroler::Patroler(Engine *engine, Ogre::Vector3 pos, int ident, Ogre::Vector3 planeInfo):
		Entity381(engine, pos, ident, 0.0f){
	meshfilename = "robot.mesh";
	entityType = PatrolerType;
	this->minSpeed = 0;
	this->maxSpeed = 50.0f;//meters per second...
	this->acceleration = 10.0f; // slow
	this->turnRate = 40.0f; //2 degrees per second
	plane = planeInfo;

	UnitAI* ai = new UnitAI(this, this, engine);
	aspects.push_back((Aspect*) ai);

	if(!planeInfo.x)
	{
		if(this->position.x > 550)
		{
			this->orientation = Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(0, 0, 1));

		}
		else
		{
			this->orientation = Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3(0, 0, 1));
		}
	}
	else if(!planeInfo.y)
	{
		if(this->position.y > 550)
		{
			this->orientation = Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3(1, 0, 0));
		}
		else
		{
			this->orientation = Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3(1, 0, 0));
		}
	}
	else if(!planeInfo.z)
	{
		if(this->position.z > 550)
		{
			this->orientation = Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(1, 0, 0));
		}
		else
		{
			this->orientation = Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3(1, 0, 0));
		}
	}

	//Physics2D* phx = new Physics2D(this);
	//aspects.push_back((Aspect*) phx);

}

Patroler::~Patroler(){

}
