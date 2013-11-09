/*
* GameModel.cpp
*
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

  fingerObject = FingerObject::create();
  fingerObject->retain();
}

void GameModel::reset() {
  planets = CCArray::create();
  planets->retain();

  planets->addObject(createPlanet(ccp(250.0f, 120.0f), 5.0f, 1.0f, 4.0f, false, "planet-1.png"));
  planets->addObject(createPlanet(ccp(500.0f, 350.0f), 5.0f, 1.0f, 5.0f, false, "planet-5.png"));
  planets->addObject(createPlanet(ccp(750.0f, 450.0f), 5.0f, 1.5f, 4.0f, false, "planet-6.png"));

  stations = CCArray::create();
  stations->retain();

  stations->addObject(createStation(ccp(200.0f, 540.0f), "enemyShip.png", ccBLUE));
  stations->addObject(createStation(ccp(700.0f, 60.0f), "enemyShip.png", ccYELLOW));
  stations->addObject(createStation(ccp(900.0f, 300.0f), "enemyShip.png", ccGREEN));

  ships = CCArray::create();
  ships->retain();

  fingerObject->touchId = SCREEN_NOTOUCH;

  shipsArrived = 0;
  shipsLost = 0;
  money = 1000;
  gameOver = false;
}

void GameModel::update(float time) {
  if (gameOver) return;

  pWorld->Step(time, 8, 3);

  if (money < 100) {
     if (ships->data->num == 0) {
       gameOver = true;
     }
  }
}

ShipModel *GameModel::launchShip() {
  if (ships->data->num >= 6) {
    CCLOG("Too many ships");
    return NULL;
  }
  
  if (money < 100) {
    CCLOG("No money left to launch");
    return NULL;
  }

  ccColor3B chosenColor;
  
  float p = CCRANDOM_0_1();
  
  if (p < 1.0f/3.0f) {
    chosenColor = ccBLUE;
  } else if (p < 2.0f/3.0f) {
    chosenColor = ccYELLOW;
  } else {
    chosenColor = ccGREEN;
  }

  StationModel *sourceStation;
  ccColor3B sourceColor;
  while (true) {
    sourceStation = static_cast<StationModel *>(stations->randomObject());
    sourceColor = sourceStation->getColor();
    if (sourceColor.r != chosenColor.r ||
        sourceColor.g != chosenColor.g ||
        sourceColor.b != chosenColor.b) break;
  }
  

  StationModel *destinationStation;
  ccColor3B destinationColor;
  while (true) {
    destinationStation = static_cast<StationModel *>(stations->randomObject()); 
    destinationColor = destinationStation->getColor();
    if (destinationStation != sourceStation &&
        destinationColor.r == chosenColor.r &&
        destinationColor.g == chosenColor.g &&
        destinationColor.b == chosenColor.b) break;
  }
  
  ShipModel *ship = createShip(sourceStation->getPosition(), "ship.png", chosenColor);
  ship->setDestinationStation(destinationStation);
  ships->addObject(ship);

  money -= 100;

  return ship;
}

void GameModel::removeShip(ShipModel *ship, bool killed) {
  ships->removeObject(ship);
  if (killed) {
    shipsLost++;
  } else {
    shipsArrived++;
    money += 200;
  }
}

void GameModel::beginTouchHole(CCTouch *pTouch) {
  if (fingerObject->touchId == SCREEN_NOTOUCH) {
    fingerObject->touchId = pTouch->getID();
    fingerObject->setPosition(ccp(pTouch->getLocation().x, pTouch->getLocation().y));
    createTouchHole(pTouch);
  }
}

void GameModel::moveTouchHole(CCTouch *pTouch) {
  if (fingerObject->touchId != SCREEN_NOTOUCH) {
    fingerObject->setPosition(ccp(pTouch->getLocation().x, pTouch->getLocation().y));
    fingerObject->getBody()->SetTransform(b2Vec2(fingerObject->getPositionX()/PTM_RATIO, fingerObject->getPositionY()/PTM_RATIO), 0);
  }
}

void GameModel::endTouchHole(CCTouch *pTouch) {
  if (fingerObject->touchId != SCREEN_NOTOUCH) {
    fingerObject->touchId = SCREEN_NOTOUCH;
    destroyTouchHole(pTouch);
    fingerObject->removeFromParent();
  }
}

void GameModel::createTouchHole(CCTouch *touch) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;

  bodyDef.position.Set(fingerObject->getPositionX()/PTM_RATIO, fingerObject->getPositionY()/PTM_RATIO);
  bodyDef.allowSleep = false;

  b2Body *fingerBody = pWorld->CreateBody(&bodyDef);
  fingerBody->SetUserData(fingerObject);
  fingerObject->setBody(fingerBody);

  b2CircleShape shape;
  shape.m_radius = FINGER_RADIUS;

  b2FixtureDef leDef;
  leDef.isSensor = true;
  leDef.shape = &shape;

  fingerBody->CreateFixture(&leDef);
}

void GameModel::destroyTouchHole(CCTouch *touch) {
  fingerObject->getBody()->SetUserData(NULL);
  pWorld->DestroyBody(fingerObject->getBody());
  fingerObject->setBody(NULL);
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

PlanetModel *GameModel::createPlanet(CCPoint position, float gravity, float planetRadius, float gravityFieldRadius, bool isBlackhole, char *spriteFrameName) {
  CCDictionary *planetDict = CCDictionary::create();
  planetDict->setObject(CCString::createWithFormat("%f", position.x), std::string(PLANETMODEL_POSITION_X));
  planetDict->setObject(CCString::createWithFormat("%f", position.y), std::string(PLANETMODEL_POSITION_Y));
  planetDict->setObject(CCString::createWithFormat("%f", gravity), std::string(PLANETMODEL_GRAVITY));
  planetDict->setObject(CCString::createWithFormat("%f", planetRadius), std::string(PLANETMODEL_PLANET_RADIUS));
  planetDict->setObject(CCString::createWithFormat("%f", gravityFieldRadius), std::string(PLANETMODEL_GRAVITY_FIELD_RADIUS));
  planetDict->setObject(CCString::createWithFormat("%d", isBlackhole), std::string(PLANETMODEL_BLACKHOLE));
  planetDict->setObject(CCString::create(spriteFrameName), std::string(PLANETMODEL_SPRITEFRAMENAME));
  return PlanetModel::create(planetDict);
}

StationModel *GameModel::createStation(CCPoint position, char *spriteFrameName, const ccColor3B &color) {
  CCDictionary *stationDict = CCDictionary::create();
  stationDict->setObject(CCString::createWithFormat("%f", position.x), std::string(PLANETMODEL_POSITION_X));
  stationDict->setObject(CCString::createWithFormat("%f", position.y), std::string(PLANETMODEL_POSITION_Y));
  stationDict->setObject(CCString::create(spriteFrameName), std::string(PLANETMODEL_SPRITEFRAMENAME));

  CCArray *colorArray = CCArray::create();
  colorArray->addObject(CCFloat::create(color.r));
  colorArray->addObject(CCFloat::create(color.g));
  colorArray->addObject(CCFloat::create(color.b));
  stationDict->setObject(colorArray, STATIONMODEL_COLOR);
  return StationModel::create(stationDict);
}

ShipModel *GameModel::createShip(CCPoint position, char *spriteFrameName, const ccColor3B &color) {
  CCDictionary *shipDict = CCDictionary::create();
  shipDict->setObject(CCString::createWithFormat("%f", position.x), std::string(PLANETMODEL_POSITION_X));
  shipDict->setObject(CCString::createWithFormat("%f", position.y), std::string(PLANETMODEL_POSITION_Y));
  shipDict->setObject(CCString::create(spriteFrameName), std::string(PLANETMODEL_SPRITEFRAMENAME));

  CCArray *colorArray = CCArray::create();
  colorArray->addObject(CCFloat::create(color.r));
  colorArray->addObject(CCFloat::create(color.g));
  colorArray->addObject(CCFloat::create(color.b));
  shipDict->setObject(colorArray, STATIONMODEL_COLOR);
  return ShipModel::create(shipDict);
}
