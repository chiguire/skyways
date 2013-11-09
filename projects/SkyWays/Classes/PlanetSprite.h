/*
* PlanetSprite.h
*
*  Created on: 31/05/2013
*      Author: Ciro
*/

#ifndef PLANETSPRITE_H_
#define PLANETSPRITE_H_

#include "cocos2d.h"
#include "TileObject.h"
#include "PlanetModel.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class PlanetSprite: public TileObject {
protected:
  PlanetModel *model;

  b2Fixture *gravityField;
  b2Fixture *bodyFixture;

  CCDrawNode *gravityFieldSprite;
  CCSprite *planet;

public:
  PlanetSprite();
  virtual ~PlanetSprite();

  bool init(PlanetModel *p);
  static PlanetSprite *create(PlanetModel *h);
  void registerPhysics(b2World *world);
  float getGravity();
  void update();

  CC_SYNTHESIZE(int,spriteFrameNumber,SpriteFrameNumber);

  CC_SYNTHESIZE(PlanetSprite *, sprite, Sprite);
};

#endif /* PLANETSPRITE_H_ */
