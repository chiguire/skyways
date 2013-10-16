/*
 * ShipSprite.cpp
 *
 *  Created on: 31/05/2013
 *      Author: Ciro
 */

#include "GameModel.h"
#include "ShipSprite.h"
#include "PlanetSprite.h"

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
	model = p;
	model->retain();
	this->initWithSpriteFrameName(p->getSpriteFrameName()->getCString());
	setAnchorPoint(ccp(0.5f, 0.5f));
  setPosition(p->getPosition());
  //setScale(model->getSize()*0.38f);

  planetsInVicinity = CCArray::create();
  planetsInVicinity->retain();

  touchGameModel = NULL;

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

  bodyFixture = body->CreateFixture(&shape, 1.0f);

	body->ApplyLinearImpulse(b2Vec2(1, 3), b2Vec2_zero);

	setUserData((void *)body);
}

void ShipSprite::update(float dt) {
	if (body) {
		setPosition(ccp(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO));
    setRotation(CC_RADIANS_TO_DEGREES(atan2f(body->GetLinearVelocity().y, -body->GetLinearVelocity().x))-90);

    if (planetsInVicinity->data->num > 0) {
      b2Vec2 shipCenter(body->GetPosition());
    
      CCObject *obj;

      CCARRAY_FOREACH(planetsInVicinity, obj) {
        PlanetSprite *ps = dynamic_cast<PlanetSprite *>(obj);
        b2Vec2 planetCenter(ps->getBody()->GetPosition());
        b2Vec2 towardsVector(planetCenter - shipCenter);
        towardsVector.Normalize();
        towardsVector *= 2.0f;
        body->ApplyForceToCenter(towardsVector);
        }
      }

    if (touchGameModel) {
      b2Vec2 touchPosition(touchGameModel->getFingerPosition().x/PTM_RATIO, touchGameModel->getFingerPosition().y/PTM_RATIO);
      b2Vec2 towardsVector(touchPosition - body->GetPosition());
      towardsVector.Normalize();
      towardsVector *= 2.0f;
      body->ApplyForceToCenter(towardsVector);
      }
	}
}

void ShipSprite::beginContact(TileObject *obj) {
  if (dynamic_cast<PlanetSprite *>(obj)) {
    planetsInVicinity->addObject(obj);
    }
  if (dynamic_cast<GameModel *>(obj)) {
    touchGameModel = (GameModel *)obj;
    }
}

void ShipSprite::endContact(TileObject *obj) {
  if (dynamic_cast<PlanetSprite *>(obj)) {
    planetsInVicinity->removeObject(obj);
    }
  if (dynamic_cast<GameModel *>(obj)) {
    touchGameModel = NULL;
    }
}
