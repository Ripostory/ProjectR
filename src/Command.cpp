/*
 * Command.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: Ronn Quiajda
 */

#include "Command.h"

//base command
//empty command, does nothing
Command::Command(Entity381* ent) : entity(ent) {}
Command::~Command() {}
void Command::init() {}
void Command::tick(float dt) {}
bool Command::done() {return true;}

//go to command

MoveTo::MoveTo(Entity381* ent, Ogre::Vector3 location) : Command(ent)
{
	MOVE_DISTANCE_THRESHOLD = 10;
	targetLocation = location;
}

MoveTo::~MoveTo()
{
}

void MoveTo::init()
{
}

void MoveTo::tick(float dt)
{
	//always point to destination and move forward
	Ogre::Vector3 diff = targetLocation - entity->position;
	entity->desiredHeading = atan2(diff.z, diff.x) * (180.0/3.141592653589793238463);
	entity->desiredSpeed = entity->maxSpeed;
}

bool MoveTo::done()
{
	float distance = (entity->position).distance(targetLocation);
	if (distance < MOVE_DISTANCE_THRESHOLD)
	{
		//reset
		entity->desiredSpeed = 0;
		return true;
	}
	else
		return false;
}

//intercept command

Intercept::Intercept(Entity381* ent, Entity381* tar) : Command(ent)
{
	MOVE_DISTANCE_THRESHOLD = 30;
	target = tar;
}

Intercept::~Intercept()
{
}

void Intercept::init()
{
}

void Intercept::tick(float dt)
{
	//always point to destination and move forward
	Ogre::Vector3 diff = target->position - entity->position;
	Ogre::Vector3 relVel = target->velocity - entity->velocity;
	float t = diff.length()/relVel.length();

	Ogre::Vector3 finalPos = target->position + target->velocity * t;

	entity->desiredHeading = atan2(finalPos.z, finalPos.x) * (180.0/3.141592653589793238463);
	entity->desiredSpeed = entity->maxSpeed;
}

bool Intercept::done()
{
	float distance = (entity->position).distance(target->position);
	if (distance < MOVE_DISTANCE_THRESHOLD)
	{
		//reset
		entity->desiredSpeed = 0;
		return true;
	}
	else
		return false;
}





