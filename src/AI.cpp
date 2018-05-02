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

UnitAI::UnitAI(Entity381* ent, Patroler* pat, Engine* eng):Aspect(ent){
	patroler = pat;
	engine = eng;
}

UnitAI::~UnitAI(){

}


void UnitAI::Tick(float dt){

	Entity381* targetEntity = engine->entityMgr->entities[engine->entityMgr->selectedEntityIndex];
	if(patroler->plane.x == 0)
	{
		if(entity->position.x > 0){
				if (targetEntity->position.x > 550){
					//Intercept

					Ogre::Vector3 difference = Ogre::Vector3(800,
							targetEntity->position.y,
							targetEntity->position.z) - entity->position;


					entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.z, difference.y).valueDegrees());
					entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
				}
		}

		else{
				if (targetEntity->position.x < -550){
					//Intercept
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
				}
		}
	}

	else if(patroler->plane.y == 0)
	{
		if(entity->position.y > 0){
				if (targetEntity->position.y > 550){
					//Intercept
					Ogre::Vector3 difference = Ogre::Vector3(targetEntity->position.x,
							600,
							targetEntity->position.z) - entity->position;
					std::cout << difference.x << " " << difference.z << std::endl;

					entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.x, difference.z).valueDegrees());
					entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
				}
		}

		else{
				if (targetEntity->position.y < -550){
					//Intercept
					std::cout << "On same plane" << std::endl;
										Ogre::Vector3 difference = Ogre::Vector3(targetEntity->position.x,
												-600,
												targetEntity->position.z) - entity->position;
										std::cout << difference.x << " " << difference.z << std::endl;

										entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.x, difference.z).valueDegrees());
										entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
				}
		}
	}

	else if(patroler->plane.z == 0)
		{
		if(entity->position.z > 0){
			 if (targetEntity->position.z > 550){
								//Intercept
						Ogre::Vector3 difference = Ogre::Vector3(targetEntity->position.x,
								targetEntity->position.y,
								600) - entity->position;

					entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.x, difference.z).valueDegrees());
					entity->desiredSpeed = entity->maxSpeed;
				}
				else
				{
					//Pace
				}
		}

		else{
			if (targetEntity->position.y < -550){
								//Intercept
								Ogre::Vector3 difference = Ogre::Vector3(targetEntity->position.x,
															targetEntity->position.z,
															-600) - entity->position;

								entity->desiredHeading = FixAngle(Ogre::Math::ATan2(difference.x, difference.z).valueDegrees());
								entity->desiredSpeed = entity->maxSpeed;

				}
				else
				{
					//Pace
				}
		}
	}

	if(entity->desiredSpeed > entity->speed){
		entity->speed += entity->acceleration * dt;
	} else if (entity->desiredSpeed < entity->speed){
		entity->speed -= entity->acceleration * dt;
	}
	entity->speed = Clamp(entity->minSpeed, entity->maxSpeed, entity->speed);

	std::cout << entity->speed << std::endl;

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
		entity->velocity.y = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * entity->speed;; // just to be safe, we do not want ships in the air.
		entity->velocity.x = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * entity->speed; //adjacent/hyp
		entity->velocity.z = 0; //opposite/hyp
	}

	std::cout << (float)entity->velocity.x << " " << (float)entity->velocity.z << std::endl;

		//This does not change!
	std::cout << "Positionx: " << entity->position.x << " velocity * dt: " << entity->velocity.x * dt << std::endl;
		entity->position = entity->position + entity->velocity * dt;

		std::cout << entity->position.x << std::endl;

}
