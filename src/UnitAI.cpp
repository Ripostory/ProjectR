/*
 * UnitAI.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: Ronn Quijada
 */

#include "UnitAI.h"

UnitAI::UnitAI(Entity381* ent) : Aspect(ent)
{
}

UnitAI::~UnitAI()
{
}

void UnitAI::Tick(float dt)
{
	if (commands.size() != 0)
	{
		//tick front
		commands.front()->tick(dt);

		//delete front once finished
		if (commands.front()->done())
		{
			delete commands.front();
			commands.pop_front();

			//init next command
			if (commands.size() != 0)
				commands.front()->init();
		}
	}
}

void UnitAI::SetCommand(Command *cmd)
{
	//clear list and add command
	commands.clear();
	commands.push_back(cmd);

	//init new command
	commands.front()->init();
}

void UnitAI::AddCommand(Command *cmd)
{
	//push back command
	commands.push_back(cmd);

	//init if the only command
	if (commands.size() == 1)
		commands.front()->init();
}

void UnitAI::MoveTo(Ogre::Vector3 pos)
{
	//clean command list and do moveto event
	SetCommand(new ::MoveTo(entity, pos));
}

void UnitAI::AddMoveTo(Ogre::Vector3 pos)
{
	//clean command list and do moveto event
	AddCommand(new ::MoveTo(entity, pos));
}

void UnitAI::Intercept(Entity381 *target)
{
	//clean command list and do intercept event
	SetCommand(new ::Intercept(entity, target));
}

void UnitAI::AddIntercept(Entity381 *target)
{
	//clean command list and do intercept event
	AddCommand(new ::Intercept(entity, target));
}




