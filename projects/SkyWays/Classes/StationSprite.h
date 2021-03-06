/*
* StationSprite.h
*
*  Created on: 31/05/2013
*      Author: Ciro
*/

#ifndef STATIONSPRITE_H_
#define STATIONSPRITE_H_

#include "cocos2d.h"
#include "TileObject.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class StationModel;

class StationSprite: public TileObject {
protected:
  b2Fixture *bodyFixture;

  CCSprite *leStation;
  CCSprite *stationDoorBackground;
  CCSprite *stationDoor;

  CCAnimate *doorsOpenAction;

public:
  StationSprite();
  virtual ~StationSprite();

  bool init(StationModel *p);
  static StationSprite *create(StationModel *h);
  void registerPhysics(b2World *world);

  virtual void update(float dt);
  virtual void applyForces();
  void playOpenDoorAnimation();

  CC_SYNTHESIZE(int,spriteFrameNumber,SpriteFrameNumber);
  CC_SYNTHESIZE(StationModel *, model, Model);
};

#endif /* STATIONSPRITE_H_ */
