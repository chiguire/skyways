/*
 * ShipSprite.h
 *
 *  Created on: 31/05/2013
 *      Author: Ciro
 */

#ifndef SHIPSPRITE_H_
#define SHIPSPRITE_H_

#include "cocos2d.h"
#include "TileObject.h"
#include "ShipModel.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class ShipSprite: public TileObject {
protected:
  ShipModel *model;

	b2Fixture *bodyFixture;

public:
	ShipSprite();
	virtual ~ShipSprite();

	bool init(ShipModel *p);
	static ShipSprite *create(ShipModel *h);
	void registerPhysics(b2World *world);

	virtual void update(float dt);
	virtual void applyForces();

	CC_SYNTHESIZE(int,spriteFrameNumber,SpriteFrameNumber);
};

#endif /* SHIPSPRITE_H_ */
