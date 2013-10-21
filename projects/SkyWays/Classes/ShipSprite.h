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
  b2Fixture *bodyFixture;

  CCArray *planetsInVicinity;
  FingerObject *fingerObject;

  void unregisterPhysics();

public:
  ShipSprite();
  virtual ~ShipSprite();

  bool init(ShipModel *p);
  static ShipSprite *create(ShipModel *h);
  void registerPhysics(b2World *world);
  void removeFromGame();

  virtual void update(float dt);

  virtual void beginContact(TileObject *o);
  virtual void endContact(TileObject *o);

  CC_SYNTHESIZE(int,spriteFrameNumber,SpriteFrameNumber);
  CC_SYNTHESIZE(bool,markedToRemoveKilled,MarkedToRemoveKilled);
  CC_SYNTHESIZE(bool,markedToRemoveArrived,MarkedToRemoveArrived);
  CC_SYNTHESIZE(ShipModel *,model,Model);
};

#endif /* SHIPSPRITE_H_ */
