/*
 * StationSprite.cpp
 *
 *  Created on: 31/05/2013
 *      Author: Ciro
 */

#include "StationSprite.h"

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
	model = p;
	model->retain();
	this->initWithSpriteFrameName(p->getSpriteFrameName()->getCString());
	setAnchorPoint(ccp(0.5f, 0.5f));
  setPosition(p->getPosition());

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
	bodyDef.type = b2_dynamicBody;

	bodyDef.position.Set(model->getPosition().x/PTM_RATIO, model->getPosition().y/PTM_RATIO);
	bodyDef.userData = this;
	bodyDef.fixedRotation = true;
	body = world->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = 0.3f;

  bodyFixture = body->CreateFixture(&shape, 1.0f);
}

void StationSprite::update(float dt) {
	if (body) {
		
	}
}

void StationSprite::applyForces() {
	
}
