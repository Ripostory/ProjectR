/*
 * EntityMgr.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include <EntityMgr.h>
#include <Engine.h>

EntityMgr::EntityMgr(Engine *eng): Mgr(eng){
	selectedEntity = 0;
	count = 0;
	lvl = 0;
	selectedEntityIndex = -1;
}

EntityMgr::~EntityMgr(){

}


void EntityMgr::SelectNextEntity(){
	if(selectedEntityIndex >= count - 1) {
		selectedEntityIndex = 0;
	} else {
		selectedEntityIndex++;
	}
	Select(selectedEntityIndex);
/*	if(selectedEntity != 0)
		selectedEntity->isSelected = false;
	selectedEntity = entities[selectedEntityIndex];
	selectedEntity->isSelected = true;
	SetSelectedFlyingEntity();*/
}

void EntityMgr::Select(int i){
	if(i >= 0 && i < count){
		if(selectedEntity != 0)
			selectedEntity->isSelected = false;
		selectedEntityIndex = i;
		selectedEntity = entities[i];
		selectedEntity->isSelected = true;
	}
}




void EntityMgr::CreateEntityOfTypeAtPosition(EntityTypes entType, Ogre::Vector3 pos){

	Entity381 * ent;
	switch(entType)
	{
	case PlayerType:
		//CreateDDG51(pos);
		ent = (Entity381 *) ( new Player(engine, pos, count++));
		break;
	case PatrolerType:
		ent = (Entity381 *) (new Patroler(engine, pos, count++, {0,1,1}));
		break;
	default:
		ent = (Entity381*) (new Player(engine, pos, count++));//CreateEntity("robot.mesh", pos);
		break;
	}
	ent->Init();
	entities.push_back(ent);

}

void EntityMgr::Tick(float dt){
	for(int i = 0; i < count; i++){
		entities[i]->Tick(dt);
	}
}

void EntityMgr::RemoveEntity(Entity381 *ref)
{
	//search for entity
	std::vector<Entity381*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++)
	{
		if (*it == ref)
		{
			delete ref;
			entities.erase(it);
			break;
		}
	}
}

void EntityMgr::ClearEntities()
{
	std::vector<Entity381*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++)
	{
		delete *it;
		*it = NULL;
	}

	entities.clear();
	count = 0;
	selectedEntity = NULL;
	selectedEntityIndex = 0;
}

void EntityMgr::CreateLevel(std::string meshName)
{
	Entity381 *ent = (Entity381 *) (new Level(engine, Ogre::Vector3(0,0,0), count++, meshName));
	std::cout << ent->position << std::endl;
	ent->Init();
	entities.push_back(ent);
}

