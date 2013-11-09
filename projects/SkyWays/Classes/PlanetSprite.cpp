/*
* PlanetSprite.cpp
*
*  Created on: 31/05/2013
*      Author: Ciro
*/

#include "PlanetSprite.h"

PlanetSprite::PlanetSprite()
  : model(NULL)
{
}

PlanetSprite::~PlanetSprite() {
  if (model != NULL) {
    model->release();
  }
}

bool PlanetSprite::init(PlanetModel *p) {
  CCSprite::init();

  model = p;
  model->retain();

  setPosition(p->getPosition());

  gravityFieldSprite = CCDrawNode::create();
  const int SEGMENTS = 180;
  float radius = model->getGravityFieldRadius()*PTM_RATIO;
  ccColor4F colorBorder = { 0.7f, 0.7f, 1.0f, 0.3f };
  ccColor4F colorBackground = { 0.7f, 0.7f, 1.0f, 0.1f };
  float previousAngle = 0;
  CCPoint pVertices[SEGMENTS];
  for (int i = 0; i != SEGMENTS; i++) {
    float angle = (float(i+1)/float(SEGMENTS))*(2.0f*M_PI);
    CCPoint vertices[2] = {CCPoint(radius * cos(previousAngle), radius * sin(previousAngle)),
                           CCPoint(radius * cos(angle), radius * sin(angle))};
    gravityFieldSprite->drawSegment(vertices[0], vertices[1], 1.0f, colorBorder);
    pVertices[i] = vertices[0];
    previousAngle = angle;
  }
  gravityFieldSprite->drawDot(CCPointZero, 5.0f, colorBorder);
  gravityFieldSprite->drawPolygon(pVertices, SEGMENTS, colorBackground, 0.0f, colorBorder);

  addChild(gravityFieldSprite);

  planet = CCSprite::createWithSpriteFrameName(p->getSpriteFrameName()->getCString());
  planet->setAnchorPoint(ccp(0.5f, 0.5f));
  planet->setScale(model->getPlanetRadius()*0.38f);
  addChild(planet);

  return true;
}

PlanetSprite *PlanetSprite::create(PlanetModel *h) {
  PlanetSprite *pRet = new PlanetSprite();
  if (pRet && pRet->init(h))
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

void PlanetSprite::registerPhysics(b2World *world) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;

  bodyDef.position.Set(model->getPosition().x/PTM_RATIO, model->getPosition().y/PTM_RATIO);
  bodyDef.userData = this;
  bodyDef.fixedRotation = false;
  body = world->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = model->getPlanetRadius();

  bodyFixture = body->CreateFixture(&shape, 1.0f);

  b2FixtureDef fixtureDef;
  shape.m_radius = model->getGravityFieldRadius();
  fixtureDef.shape = &shape;
  fixtureDef.isSensor = true;

  gravityField = body->CreateFixture(&fixtureDef);
  setUserData((void *)body);
}

float PlanetSprite::getGravity() {
  return model->getGravity();
}

void PlanetSprite::update() {
  if (body) {
    setPosition(ccp(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO));
  }
}