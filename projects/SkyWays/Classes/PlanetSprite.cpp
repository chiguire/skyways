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
	model = p;
	model->retain();
	this->initWithSpriteFrameName(p->getSpriteFrameName()->getCString());
	setAnchorPoint(ccp(0.5f, 0.5f));
  setPosition(p->getPosition());
  setScale(model->getSize()*0.38f);

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
	bodyDef.fixedRotation = true;
	body = world->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = model->getSize();

  bodyFixture = body->CreateFixture(&shape, 1.0f);

  b2FixtureDef fixtureDef;
  shape.m_radius *= 3.0f;
  fixtureDef.shape = &shape;
  fixtureDef.isSensor = true;

  gravityField = body->CreateFixture(&fixtureDef);
	setUserData((void *)body);
}

void PlanetSprite::update(float dt) {
	if (body) {
		
	}
}

void PlanetSprite::applyForces() {
	
}
