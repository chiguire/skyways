/*
 * TileObject.h
 *
 *  Created on: 04/06/2013
 *      Author: Ciro
 */

#ifndef TILEOBJECT_H_
#define TILEOBJECT_H_

#include "cocos2d.h"
#include "Common.h" 
#include "Box2D/Box2D.h"
#include <string>

USING_NS_CC;
using namespace std;

class TileObject: public cocos2d::CCSprite {
public:
	TileObject();
	virtual ~TileObject();

	CC_SYNTHESIZE(b2Body *, body, Body);

	CC_SYNTHESIZE(int, defaultZ, DefaultZ);
	CC_SYNTHESIZE(bool, isStatic, IsStatic);
	CC_SYNTHESIZE(string, tileName, TileName);
	CC_SYNTHESIZE(string, tileType, TileType);

	// slew as in "Slew Mode" from Flight Simulator. Position an object without regard to physics simulation.
	void slewX(float x);
	void slewY(float y);
	void slew(float x, float y);
	void slew(CCPoint pos);

	virtual void registerBodyDef(b2BodyDef &bodyDef) { CC_UNUSED_PARAM(bodyDef); }
	virtual void registerFixtureDef(b2FixtureDef &fixtureDef) { CC_UNUSED_PARAM(fixtureDef); }
	virtual void registerWorld(b2World *world) { CC_UNUSED_PARAM(world); }

	virtual void beginContact(TileObject *o) { CC_UNUSED_PARAM(o); }
	virtual void endContact(TileObject *o) { CC_UNUSED_PARAM(o); }
};

#endif /* TILEOBJECT_H_ */
