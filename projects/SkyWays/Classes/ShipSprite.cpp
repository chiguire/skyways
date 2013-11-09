/*
* ShipSprite.cpp
*
*      Author: Ciro
*/

#include "GameModel.h"
#include "ShipModel.h"
#include "ShipSprite.h"
#include "StationSprite.h"
#include "PlanetSprite.h"
#include "SpaceBoundaries.h"
#include "SimpleAudioEngine.h"

ShipSprite::ShipSprite()
  : model(NULL)
{
}

ShipSprite::~ShipSprite() {
  if (model != NULL) {
    model->release();
  }
}

bool ShipSprite::init(ShipModel *p) {
  CCSprite::init();
  model = p;
  model->retain();
  leShip = CCSprite::createWithSpriteFrameName(p->getSpriteFrameName()->getCString());
  leShip->setAnchorPoint(ccp(0.5f, 0.5f));
  setPosition(p->getPosition());
  leShip->setColor(model->getColor());
  addChild(leShip);

  planetsInVicinity = CCArray::create();
  planetsInVicinity->retain();

  fingerObject = NULL;
  
  markedToRemoveKilled = false;
  markedToRemoveArrived = false;

  return true;
}

ShipSprite *ShipSprite::create(ShipModel *h) {
  ShipSprite *pRet = new ShipSprite();
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

void ShipSprite::registerPhysics(b2World *world) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;

  bodyDef.position.Set(model->getPosition().x/PTM_RATIO, model->getPosition().y/PTM_RATIO);
  bodyDef.userData = this;
  bodyDef.fixedRotation = true;
  body = world->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = 0.5f;

  bodyFixture = body->CreateFixture(&shape, 3.0f);

  setUserData((void *)body);
}

void ShipSprite::removeFromGame() {
  unregisterPhysics();
  CC_SAFE_RELEASE_NULL(model);
  planetsInVicinity->removeAllObjects();
  planetsInVicinity->release();
  fingerObject = NULL;
}

void ShipSprite::unregisterPhysics() {
  setUserData(NULL);
  body->GetWorld()->DestroyBody(body);
  body = NULL;
}

void ShipSprite::update(float dt) {
  if (!body) {
    return;
}
  setPosition(ccp(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO));
  if (body->GetLinearVelocity().Length() > 0.05f) {
    setRotation(CC_RADIANS_TO_DEGREES(atan2f(body->GetLinearVelocity().y, -body->GetLinearVelocity().x))-90);
  }

  if (planetsInVicinity->data->num > 0) {
    b2Vec2 shipCenter(body->GetPosition());
    body->SetLinearDamping(0.1f);

    CCObject *obj;

    CCARRAY_FOREACH(planetsInVicinity, obj) {
      PlanetSprite *ps = dynamic_cast<PlanetSprite *>(obj);
      b2Vec2 planetCenter(ps->getBody()->GetPosition());
      b2Vec2 towardsVector(planetCenter - shipCenter);
      towardsVector.Normalize();
      towardsVector *= ps->getGravity();
      body->ApplyForceToCenter(towardsVector);
    }
  } else {
    body->SetLinearDamping(0.0f);
  }

  if (fingerObject && fingerObject->touchId != SCREEN_NOTOUCH) {
    b2Vec2 touchPosition(fingerObject->getBody()->GetPosition());
    b2Vec2 towardsVector(touchPosition - body->GetPosition());
    towardsVector.Normalize();
    towardsVector *= FINGER_GRAVITY;
    body->ApplyForceToCenter(towardsVector);
  } else {
    fingerObject = NULL;
  }
}

void ShipSprite::beginContact(TileObject *obj) {
  if (dynamic_cast<PlanetSprite *>(obj)) {
    planetsInVicinity->addObject(obj);
  }
  if (dynamic_cast<FingerObject *>(obj)) {
    fingerObject = (FingerObject *)obj;
  }
  if (dynamic_cast<SpaceBoundaries *>(obj)) {
    markedToRemoveKilled = true;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("shiplost.wav");
  }
  if (dynamic_cast<StationSprite *>(obj)) {
    StationSprite *spr = dynamic_cast<StationSprite *>(obj);
    if (model->getDestinationStation() == spr->getModel()) {
      markedToRemoveArrived = true;
      spr->playOpenDoorAnimation();
    }
  }
  if (dynamic_cast<ShipSprite *>(obj)) {
    markedToRemoveKilled = true;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("shipexplosion.wav");
  }
}

void ShipSprite::endContact(TileObject *obj) {
  if (dynamic_cast<PlanetSprite *>(obj)) {
    planetsInVicinity->removeObject(obj);
  }
  if (dynamic_cast<FingerObject *>(obj)) {
    fingerObject = NULL;
  }
}

bool ShipSprite::draggedByFinger() {
  return fingerObject;
}