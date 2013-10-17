/*
* SpaceBoundaries.cpp
*
*      Author: Ciro
*/

#include "SpaceBoundaries.h"

SpaceBoundaries::SpaceBoundaries()
{
}

SpaceBoundaries::~SpaceBoundaries() {
}

bool SpaceBoundaries::init() {
  TileObject::init();
  return true;
}

SpaceBoundaries *SpaceBoundaries::create() {
  SpaceBoundaries *pRet = new SpaceBoundaries();
  if (pRet && pRet->init())
  {
    pRet->autorelease();
    return pRet;
  }
  else
  {
    CC_SAFE_DELETE(pRet);
    return NULL;
  }
}

void SpaceBoundaries::registerPhysics(b2World *pWorld) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position = b2Vec2(400.0f/PTM_RATIO, 300.0f/PTM_RATIO);

  body = pWorld->CreateBody(&bodyDef);

  b2PolygonShape shape;
  shape.SetAsBox(900.0f/PTM_RATIO, 10.0f/PTM_RATIO, b2Vec2(0.0f, -400.0f/PTM_RATIO), 0);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.isSensor = true;
  fixtureDef.userData = this;

  body->CreateFixture(&fixtureDef);

  shape.SetAsBox(900.0f/PTM_RATIO, 10.0f/PTM_RATIO, b2Vec2(0.0f, 400.0f/PTM_RATIO), 0);
  
  body->CreateFixture(&fixtureDef);

  shape.SetAsBox(10.0f/PTM_RATIO, 800.0f/PTM_RATIO, b2Vec2(-450.0f/PTM_RATIO, 0.0f), 0);

  body->CreateFixture(&fixtureDef);

  shape.SetAsBox(10.0f/PTM_RATIO, 800.0f/PTM_RATIO, b2Vec2(450.0f/PTM_RATIO, 0.0f), 0);

  body->CreateFixture(&fixtureDef);

}