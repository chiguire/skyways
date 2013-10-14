/*
 * ShipSprite.cpp
 *
 *  Created on: 31/05/2013
 *      Author: Ciro
 */

#include "ShipSprite.h"

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

	setUserData((void *)body);
}

void ShipSprite::update(float dt) {
	if (body) {
		
	}
}

void ShipSprite::applyForces() {
	
}
