/*
* GameModel.h
*
*      Author: Ciro
*/

#ifndef GAMEMODEL_H_
#define GAMEMODEL_H_

#include <string>
#include "Common.h"
#include "PlanetModel.h"
#include "StationModel.h"
#include "ShipModel.h"
#include "TileObject.h"
#include "FingerObject.h"
#include "cocos2d.h"
#include "Box2D/Box2D.h"

using namespace std;
USING_NS_CC;

class GameModel : public b2QueryCallback, public b2ContactListener {
protected:
  CCUserDefault *user;

  string playerName;
  int highScore;

  PlanetModel *createPlanet(CCPoint position, float gravity, float planetRadius, float gravityFieldRadius, bool isBlackhole, char *spriteFrameName);

  StationModel *createStation(CCPoint position, char *spriteFrameName, const ccColor3B &color);

  ShipModel *createShip(CCPoint position, char *spriteFrameName, const ccColor3B &color);

  void createTouchHole(CCTouch *touch);
  void destroyTouchHole(CCTouch *touch);

public:
  static GameModel *sharedGameModel();
  GameModel();
  void reset();
  void update(float time);
  ShipModel *launchShip();

  void beginTouchHole(CCTouch *pTouch);
  void moveTouchHole(CCTouch *pTouch);
  void endTouchHole(CCTouch *pTouch);

  void removeShip(ShipModel *ship, bool killed);

  b2Body *groundBody;
  b2World *pWorld;
  CCArray *planets;
  CCArray *stations;
  CCArray *ships;

  void BeginContact(b2Contact *contact);
  void EndContact(b2Contact *contact);
  void PreSolve(b2Contact *contact, const b2Manifold *manifold);
  void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
  bool ReportFixture(b2Fixture *fixture);

  CC_SYNTHESIZE(FingerObject *, fingerObject, FingerObject);
  CC_SYNTHESIZE(int, shipsArrived, ShipsArrived);
  CC_SYNTHESIZE(int, shipsLost, ShipsLost);
  CC_SYNTHESIZE(int, money, Money);
};

#endif /* GAMEMODEL_H_ */
