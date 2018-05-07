#ifndef __Entity381_h_
#define __Entity381_h_

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <Aspect.h>
#include <Types381.h>

#include <Engine.h>

#define PHYS_BOX 		0
#define PHYS_SPHERE 	1
#define PHYS_CAPSULE 	2
#define PHYS_CYLINDER	3
#define PHYS_CONE		4
#define PHYS_HULL		5
#define PHYS_S_MESH		6

class Patroler;

class Entity381
{
public:
  Entity381(Engine *engine, Ogre::Vector3 pos, int identity, float newMass);
  Entity381(Engine *engine, Ogre::Vector3 pos, int identity, float newMass, Patroler* pat);
  virtual ~Entity381();

  Engine *engine;

  //static data
  int identity;
  int physType;
  std::string name;
  std::string meshfilename;
  Ogre::SceneNode* sceneNode;
  Ogre::Entity*    ogreEntity;
  float acceleration, turnRate;
  float minSpeed, maxSpeed;

  //dynamic data
  Ogre::Vector3 position;
  Ogre::Vector3 velocity;
  bool isSelected;
  float desiredHeading, desiredSpeed;
  float heading, speed;

  //physics data
  Ogre::Quaternion orientation;
  float mass;
  float friction;
  float restitution;
  bool didSelectSoundPlay;

  //physics collision function
  virtual void onCollision(Entity381*);
  virtual void onRaycastHit();

  //sound
  std::string soundFile;
  bool playSound;
  unsigned int auioId;

  EntityTypes entityType;

  std::vector<Aspect* > aspects;

  void Init();

  void Tick(float dt);

protected:


};



class Level: public Entity381 {
public:
	Level(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~Level();

};

class DDG51: public Entity381 {
public:
	DDG51(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~DDG51();

};

class SpeedBoat: public Entity381 {
public:
	SpeedBoat(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~SpeedBoat();

};

class Frigate: public Entity381 {
public:
	Frigate(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~Frigate();

};

class Alien: public Entity381 {
public:
	Alien(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~Alien();

};

class Patroler: public Entity381 {
public:
	Patroler(Engine *engine, Ogre::Vector3 pos, int identity, Ogre::Vector3 planeInfo);
	virtual ~Patroler();

	Ogre::Vector3 plane;
};


#endif // #ifndef __Entity381_h_
