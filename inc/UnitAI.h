/*
 * UnitAI.h
 *
 *  Created on: Apr 3, 2018
 *      Author: Ronn Quijada
 */

#ifndef UNITAI_H_
#define UNITAI_H_

#include "Aspect.h"
#include "Entity381.h"
#include "Command.h"
#include <deque>

class UnitAI : public Aspect
{
public:
	UnitAI(Entity381* ent);
	virtual ~UnitAI();

	void Tick(float dt);
	void SetCommand(Command*);
	void AddCommand(Command*);
	void MoveTo(Ogre::Vector3 pos);
	void AddMoveTo(Ogre::Vector3 pos);
	void Intercept(Entity381 *target);
	void AddIntercept(Entity381 *target);
private:
	std::deque<Command*> commands;
};

#endif /* UNITAI_H_ */
