/*
 * Command.h
 *
 *  Created on: Apr 3, 2018
 *      Author: Ronn Quijada
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include "Entity381.h"

class Command
{
public:
	Command(Entity381* ent);
	virtual ~Command();

	virtual void init();
	virtual void tick(float dt);
	virtual bool done();

	Entity381* entity;
};

class MoveTo: public Command
{
public:
	MoveTo(Entity381* ent, Ogre::Vector3 location);
	~MoveTo();

	void init();
	void tick(float dt);
	bool done();

	Ogre::Vector3 targetLocation;
	float MOVE_DISTANCE_THRESHOLD;
};

class Intercept: public Command
{
public:
	Intercept(Entity381* ent, Entity381* target);
	~Intercept();

	void init();
	void tick(float dt);
	bool done();

	Entity381 *target;
	float MOVE_DISTANCE_THRESHOLD;
};



#endif /* COMMAND_H_ */
