#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "GameModel.h"
#include "SpaceBoundaries.h"
#include "Box2DDebugLayer.h"
class GameScene : public cocos2d::CCLayer
{
  GameModel *gameModel;

  CCMenu *gameUI;
  CCLayer *gameplayLayer;
  Box2DDebugLayer *debugLayer;

  CCArray *planetSprites;
  CCArray *stationSprites;
  CCArray *shipSprites;

  SpaceBoundaries *boundaries;

public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();  

  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  static cocos2d::CCScene* scene();

  void initGameplayLayer(GameModel *gm);

  void update(float dt);
  void launchShipHandler(float dt);

  virtual void registerWithTouchDispatcher();
  virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
  virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
  virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
  virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);

  // a selector callback
  void pauseHandler(CCObject* pSender);

  // implement the "static node()" method manually
  CREATE_FUNC(GameScene);
};

#endif // __GAMESCENE_H__
