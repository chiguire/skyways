/*
 * GameModel.cpp
 *
 *  Created on: 09/10/2013
 *      Author: Ciro
 */

#include "GameModel.h"
#include "PlanetModel.h"

static GameModel *_sharedGameModel;

GameModel *GameModel::sharedGameModel() {
	static bool firstUseGameModel = true;

	if (firstUseGameModel) {
		firstUseGameModel = false;
		_sharedGameModel = new GameModel();
	}

	return _sharedGameModel;
}

GameModel::GameModel() {
	user = CCUserDefault::sharedUserDefault();
	playerName = user->getStringForKey(USERDEFAULT_PLAYERNAME, USERDEFAULT_PLAYERNAME_DEFAULT);
	highScore = user->getIntegerForKey(USERDEFAULT_HIGHSCORE, USERDEFAULT_HIGHSCORE_DEFAULT);

	pWorld = new b2World(b2Vec2(0.0f, 0.0f));
	pWorld->SetAllowSleeping(true);
	pWorld->SetContactListener(this);

	b2BodyDef def;
	groundBody = pWorld->CreateBody(&def);
	
	touchId = SCREEN_NOTOUCH;
}

void GameModel::reset() {
  planets = CCArray::create();
  planets->retain();

  planets->addObject(createPlanet(ccp(200.0f, 300.0f), 3.0f, 1.0f, false, "planet-1.png"));
  planets->addObject(createPlanet(ccp(600.0f, 500.0f), 4.0f, 1.0f, false, "planet-5.png"));

  stations = CCArray::create();
  stations->retain();

  stations->addObject(createStation(ccp(400.0f, 400.0f), "enemyShip.png"));

  ships = CCArray::create();
  ships->retain();

  ships->addObject(createShip(ccp(500.0f, 100.0f), "ship.png"));
  
  touchId = SCREEN_NOTOUCH;
}

void GameModel::update(float time) {
  pWorld->Step(time, 8, 3);
}

void GameModel::beginTouchHole(CCTouch *pTouch) {
  if (touchId == SCREEN_NOTOUCH) {
    touchId = pTouch->getID();
	createTouchHole(pTouch);
    
  }
}

void GameModel::moveTouchHole(CCTouch *pTouch) {
  if (touchId != SCREEN_NOTOUCH) {
    fingerPosition.setPoint(pTouch->getLocation().x/PTM_RATIO, pTouch->getLocation().y/PTM_RATIO);
  	fingerBody->SetTransform(b2Vec2(fingerPosition.x, fingerPosition.y), 0);
  }
}

void GameModel::endTouchHole(CCTouch *pTouch) {
  if (touchId != SCREEN_NOTOUCH) {
    touchId = SCREEN_NOTOUCH;
	destroyTouchHole(pTouch);
  }
}

void GameModel::createTouchHole(CCTouch *touch) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  fingerPosition.setPoint(touch->getLocation().x/PTM_RATIO, touch->getLocation().y/PTM_RATIO);

  bodyDef.position.Set(fingerPosition.x, fingerPosition.y);
  bodyDef.allowSleep = false;

  fingerBody = pWorld->CreateBody(&bodyDef);
  fingerBody->SetUserData(this);

  b2CircleShape shape;
  shape.m_radius = 3.0f;

  b2FixtureDef leDef;
  leDef.isSensor = true;
  leDef.shape = &shape;

  fingerBody->CreateFixture(&leDef);
}

void GameModel::destroyTouchHole(CCTouch *touch) {
  fingerBody->SetUserData(NULL);
	pWorld->DestroyBody(fingerBody);
	fingerBody = NULL;
}

void GameModel::BeginContact(b2Contact* contact){
	TileObject* objA=(TileObject*) contact->GetFixtureA()->GetBody()->GetUserData();
	TileObject* objB=(TileObject*) contact->GetFixtureB()->GetBody()->GetUserData();
	if(objA)
		objA->beginContact(objB);
	if(objB)
		objB->beginContact(objA);

}
void GameModel::EndContact(b2Contact* contact){
	TileObject* objA=(TileObject*) contact->GetFixtureA()->GetBody()->GetUserData();
	TileObject* objB=(TileObject*) contact->GetFixtureB()->GetBody()->GetUserData();
	if(objA)
		objA->endContact(objB);
	if(objB)
		objB->endContact(objA);
}
void GameModel::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){

}

void GameModel::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){

}

bool GameModel::ReportFixture(b2Fixture* fixture) {
	return true;
}

PlanetModel *GameModel::createPlanet(CCPoint position, float gravity, float bodyRadius, bool isBlackhole, char *spriteFrameName) {
  CCDictionary *planetDict = CCDictionary::create();
  planetDict->setObject(CCString::createWithFormat("%f", position.x), std::string(PLANETMODEL_POSITION_X));
  planetDict->setObject(CCString::createWithFormat("%f", position.y), std::string(PLANETMODEL_POSITION_Y));
  planetDict->setObject(CCString::createWithFormat("%f", gravity), std::string(PLANETMODEL_GRAVITY));
  planetDict->setObject(CCString::createWithFormat("%f", bodyRadius), std::string(PLANETMODEL_SIZE));
  planetDict->setObject(CCString::createWithFormat("%d", isBlackhole), std::string(PLANETMODEL_BLACKHOLE));
  planetDict->setObject(CCString::create(spriteFrameName), std::string(PLANETMODEL_SPRITEFRAMENAME));
  return PlanetModel::create(planetDict);
}

StationModel *GameModel::createStation(CCPoint position, char *spriteFrameName) {
  CCDictionary *stationDict = CCDictionary::create();
  stationDict->setObject(CCString::createWithFormat("%f", position.x), std::string(PLANETMODEL_POSITION_X));
  stationDict->setObject(CCString::createWithFormat("%f", position.y), std::string(PLANETMODEL_POSITION_Y));
  stationDict->setObject(CCString::create(spriteFrameName), std::string(PLANETMODEL_SPRITEFRAMENAME));
  return StationModel::create(stationDict);
}

ShipModel *GameModel::createShip(CCPoint position, char *spriteFrameName) {
  CCDictionary *shipDict = CCDictionary::create();
  shipDict->setObject(CCString::createWithFormat("%f", position.x), std::string(PLANETMODEL_POSITION_X));
  shipDict->setObject(CCString::createWithFormat("%f", position.y), std::string(PLANETMODEL_POSITION_Y));
  shipDict->setObject(CCString::create(spriteFrameName), std::string(PLANETMODEL_SPRITEFRAMENAME));
  return ShipModel::create(shipDict);
}