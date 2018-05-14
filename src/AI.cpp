/*
 * UnitAI.cpp
 *
 *  Created on: Mar 1, 2018
 *      Author: sushil
 */


#include <AI.h>
#include <Entity381.h>
#include <Utils.h>
#include <Engine.h>
#include <EntityMgr.h>
#include <UiMgr.h>
#include <GameMgr.h>

UnitAI::UnitAI(Entity381* ent, Patroler* pat, Engine* eng):Aspect(ent){
	patroler = pat;
	engine = eng;
	isAware = false;
}

UnitAI::~UnitAI(){

}

bool UnitAI::Done(Entity381* player, Entity381* Patroler)
{
	if((player->position - Patroler->position) < Ogre::Vector3(30, 30, 30) && (player->position - Patroler->position) > Ogre::Vector3(-30, -30, -30))
	{
		return true;
	}
	return false;
}

void UnitAI::onSpot()
{

}

void UnitAI::Tick(float dt){

	Entity381* targetEntity = engine->entityMgr->entities[engine->entityMgr->selectedEntityIndex];

	std::cout << "Player Position: " << targetEntity->position << std::endl;

	if(patroler->plane.x == 0)
	{
		if(entity->position.x > 0){
				if ((targetEntity->position - patroler->position) < Ogre::Vector3(50, 500, 500) && (targetEntity->position - patroler->position) > Ogre::Vector3(-50, -500, -500)){
					//Intercept
					if(!isAware)
					{
						onSpot();
					}
					isAware = true;



					Ogre::Vector3 difference = Ogre::Vector3(800,
							targetEntity->position.y,
							targetEntity->position.z) - entity->position;


					entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.z, difference.y).valueDegrees());
					entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
					isAware = false;
				}
		}

		else{
				if ((targetEntity->position - patroler->position) < Ogre::Vector3(50, 500, 500) && (targetEntity->position - patroler->position) > Ogre::Vector3(-50, -500, -500)){
					//Intercept
					if(!isAware)
					{
						onSpot();
					}
					isAware = true;

std::cout << "On same x plane" << std::endl;
					Ogre::Vector3 difference = Ogre::Vector3(-800,
							targetEntity->position.y,
							targetEntity->position.z) - entity->position;


					entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.z, difference.y).valueDegrees());
					entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
					isAware = false;

				}
		}
	}

	else if(patroler->plane.y == 0)
	{
		if(entity->position.y > 0){
				if ((targetEntity->position - patroler->position) < Ogre::Vector3(500, 50, 500) && (targetEntity->position - patroler->position) > Ogre::Vector3(-500, -50, -500)){
					//Intercept
					if(!isAware)
					{
						onSpot();
					}
					isAware = true;

					Ogre::Vector3 difference = Ogre::Vector3(targetEntity->position.x,
							600,
							targetEntity->position.z) - entity->position;
					//std::cout << difference.x << " " << difference.z << std::endl;

					entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.x, difference.z).valueDegrees());
					entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
					isAware = false;

				}
		}

		else{
				if ((targetEntity->position - patroler->position) < Ogre::Vector3(500, 50, 500) && (targetEntity->position - patroler->position) > Ogre::Vector3(-500, -50, -500)){
					//Intercept
					if(!isAware)
					{
						onSpot();
					}
					isAware = true;

					std::cout << "On same plane" << std::endl;
					Ogre::Vector3 difference = Ogre::Vector3(targetEntity->position.x,
							-600,
							targetEntity->position.z) - entity->position;
					//std::cout << difference.x << " " << difference.z << std::endl;

					entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.x, difference.z).valueDegrees());
					entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
					isAware = false;

				}
		}
	}

	else if(patroler->plane.z == 0)
		{
		if(entity->position.z > 0){
			 if ((targetEntity->position - patroler->position) < Ogre::Vector3(500, 500, 50) && (targetEntity->position - patroler->position) > Ogre::Vector3(-500, -500, -50)){
								//Intercept
					if(!isAware)
					{
						onSpot();
					}
					isAware = true;

						Ogre::Vector3 difference = Ogre::Vector3(targetEntity->position.x,
								targetEntity->position.y,
								0) - entity->position;

					entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.x, difference.y).valueDegrees());
					entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
					isAware = false;

				}
		}

		else{
			if ((targetEntity->position - patroler->position) < Ogre::Vector3(500, 500, 50) && (targetEntity->position - patroler->position) > Ogre::Vector3(-500, -500, -50)){
								//Intercept
				if(!isAware)
				{
					onSpot();
				}
				isAware = true;

								Ogre::Vector3 difference = targetEntity->position - entity->position;

								entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.x, difference.y).valueDegrees());
								entity->desiredSpeed = entity->maxSpeed;

				}
				else
				{
					//Pace
					isAware = false;
				}
		}
	}

	if(entity->desiredSpeed > entity->speed){
		entity->speed += entity->acceleration * dt;
	} else if (entity->desiredSpeed < entity->speed){
		entity->speed -= entity->acceleration * dt;
	}
	entity->speed = Clamp(entity->minSpeed, entity->maxSpeed, entity->speed);

	//When I try to go from a heading of 350 to a heading of 10,
	//I should turn to right/starboard not keep decreasing heading till I get to 10 because it is 20 degrees from -10 (350) to +10 by turning to port/right and
	//340 degrees from 350 (-10) to 10 by turning left/port

	//now, get new heading from desired heading
	if(entity->desiredHeading > entity->heading){
		if(entity->desiredHeading - entity->heading > 180)
			entity->heading -= entity->turnRate * dt;
		else
			entity->heading += entity->turnRate * dt;
	} else if (entity->desiredHeading < entity->heading){
		if(entity->desiredHeading - entity->heading < -180)
			entity->heading += entity->turnRate * dt;
		else
			entity->heading -= entity->turnRate * dt;;
	}
	entity->heading = FixAngle(entity->heading);

	if(patroler->plane.x == 0){
		entity->velocity.y = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * entity->speed; // just to be safe, we do not want ships in the air.
		entity->velocity.x = 0; //adjacent/hyp
		entity->velocity.z = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * entity->speed; //opposite/hyp
	}

	else if(patroler->plane.y == 0){
		entity->velocity.y = 0; // just to be safe, we do not want ships in the air.
		entity->velocity.x = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * entity->speed; //adjacent/hyp
		entity->velocity.z = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * entity->speed; //opposite/hyp
	}
	else if(patroler->plane.z == 0){
		entity->velocity.y = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * entity->speed;; // just to be safe, we do not want ships in the air.
		entity->velocity.x = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * entity->speed; //adjacent/hyp
		entity->velocity.z = 0; //opposite/hyp
	}

	//std::cout << (float)entity->velocity.x << " " << (float)entity->velocity.z << std::endl;

		//This does not change!
	//std::cout << "Positionx: " << entity->position.x << " velocity * dt: " << entity->velocity.x * dt << std::endl;
		entity->position = entity->position + entity->velocity * dt;

		//std::cout << entity->position.x << std::endl;

	if(this->Done(targetEntity, entity))
	{
		engine->uiMgr->openTextBox("GAME", "YOU'VE BEEN CAPTURED!");
		engine->uiMgr->openTextBox("GAME", "AGAIN!");
		engine->gameMgr->resetGame();
	}

}
