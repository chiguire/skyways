/*
* StationSprite.cpp
*
*  Created on: 31/05/2013
*      Author: Ciro
*/

#include "StationSprite.h"
#include "StationModel.h"

StationSprite::StationSprite()
  : model(NULL)
{
}

StationSprite::~StationSprite() {
  if (model != NULL) {
    model->release();
  }
}

bool StationSprite::init(StationModel *p) {
  CCSprite::init();
  
  model = p;
  model->retain();
  
  setPosition(p->getPosition());

  leStation = CCSprite::createWithSpriteFrameName(p->getSpriteFrameName()->getCString());
  leStation->setAnchorPoint(ccp(0.5f, 0.5f));
  leStation->setColor(model->getColor());
  addChild(leStation);

  stationDoorBackground = CCSprite::createWithSpriteFrameName("door-bg.png");
  stationDoorBackground->setAnchorPoint(ccp(0.5f, 0.5f));
  addChild(stationDoorBackground);
  
  stationDoor = CCSprite::createWithSpriteFrameName("door00.png");
  stationDoor->setAnchorPoint(ccp(0.5f, 0.5f));
  addChild(stationDoor);

  CCArray *animArray = CCArray::create();
  for (int i = 0; i != 13; i++) {
    animArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
      CCString::createWithFormat("door%02d.png", i)->getCString()
    ));
  }
  for (int i = 12; i != -1; i--) {
    animArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
      CCString::createWithFormat("door%02d.png", i)->getCString()
    ));
  }

  CCAnimation *doorsOpenAnimation = CCAnimation::createWithSpriteFrames(animArray, 0.05f);
  doorsOpenAnimation->setRestoreOriginalFrame(true);

  doorsOpenAction = CCAnimate::create(doorsOpenAnimation);
  doorsOpenAction->retain();

  return true;
}

StationSprite *StationSprite::create(StationModel *h) {
  StationSprite *pRet = new StationSprite();
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

void StationSprite::registerPhysics(b2World *world) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_kinematicBody;

  bodyDef.position.Set(model->getPosition().x/PTM_RATIO, model->getPosition().y/PTM_RATIO);
  bodyDef.userData = this;
  bodyDef.fixedRotation = true;
  body = world->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = 0.3f;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.isSensor = true;

  bodyFixture = body->CreateFixture(&fixtureDef);
}

void StationSprite::update(float dt) {
  if (body) {
    setPosition(ccp(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO));
    //if (body->GetLinearVelocity().Length() > 0.05f) {
    //  setRotation(CC_RADIANS_TO_DEGREES(atan2f(body->GetLinearVelocity().y, -body->GetLinearVelocity().x))-90);
    //}
  }
}

void StationSprite::applyForces() {

}

void StationSprite::playOpenDoorAnimation() {
  stationDoor->runAction(doorsOpenAction); 
}
