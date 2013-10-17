/*
* ShipSprite.h
*
*      Author: Ciro
*/

#ifndef SHIPSPRITE_H_
#define SHIPSPRITE_H_

#include "cocos2d.h"
#include "TileObject.h"
#include "FingerObject.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class ShipModel;

class ShipSprite: public TileObject {
protected:
  ShipModel *model;

  b2Fixture *bodyFixture;

  CCArray *planetsInVicinity;
  FingerObject *fingerObject;

  bool markedToRemove;

public:
  ShipSprite();
  virtual ~ShipSprite();

  bool init(ShipModel *p);
  static ShipSprite *create(ShipModel *h);
  void registerPhysics(b2World *world);

  virtual void update(float dt);

  virtual void beginContact(TileObject *o);
  virtual void endContact(TileObject *o);

  CC_SYNTHESIZE(int,spriteFrameNumber,SpriteFrameNumber);
};

#endif /* SHIPSPRITE_H_ */
