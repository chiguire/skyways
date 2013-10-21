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
  bodyDef.position = b2Vec2(0.0f, 0.0f);
  bodyDef.userData = this;

  body = pWorld->CreateBody(&bodyDef);

  b2PolygonShape shape;
  shape.SetAsBox(TO_B2D_COORDS(1024.0f+SPACE_BORDER*2.0f), TO_B2D_COORDS(10.0f), 
                 b2Vec2(TO_B2D_COORDS(1024.0f/2.0f), TO_B2D_COORDS(-SPACE_BORDER)), 0);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.isSensor = true;

  body->CreateFixture(&fixtureDef);

  shape.SetAsBox(TO_B2D_COORDS(1024.0f+SPACE_BORDER*2.0f), TO_B2D_COORDS(10.0f),
                 b2Vec2(TO_B2D_COORDS(600.0f/2.0f), TO_B2D_COORDS(600.0f+SPACE_BORDER)), 0);
  
  body->CreateFixture(&fixtureDef);

  shape.SetAsBox(TO_B2D_COORDS(10.0f), TO_B2D_COORDS(600.0f+SPACE_BORDER*2.0f),
                 b2Vec2(TO_B2D_COORDS(-SPACE_BORDER), TO_B2D_COORDS(600.0f/2.0f)), 0);

  body->CreateFixture(&fixtureDef);

  shape.SetAsBox(TO_B2D_COORDS(10.0f), TO_B2D_COORDS(600.0f+SPACE_BORDER*2.0f),
                 b2Vec2(TO_B2D_COORDS(1024.0f+SPACE_BORDER), TO_B2D_COORDS(600.0f/2.0f)), 0);

  body->CreateFixture(&fixtureDef);

}