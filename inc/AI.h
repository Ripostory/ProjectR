/*
 * Physics2D.h
 *
 *  Created on: Mar 1, 2018
 *      Author: sushil
 */

#ifndef AI_H_
#define AI_H_

#include <Aspect.h>

class UnitAI : Aspect
{
public:
	UnitAI(Entity381 *ent, Patroler* pat, Engine* eng);
	~UnitAI();

	void Tick(float dt);

	Patroler* patroler;
	Engine* engine;
};



#endif /* AI_H_ */
