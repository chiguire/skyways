#include "GameScene.h"
#include "PlanetSprite.h"
#include "ShipSprite.h"
#include "StationSprite.h"

USING_NS_CC;

CCScene* GameScene::scene()
{
  // 'scene' is an autorelease object
  CCScene *scene = CCScene::create();

  // 'layer' is an autorelease object
  GameScene *layer = GameScene::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
  //////////////////////////////
  // 1. super init first
  if ( !CCLayer::init() )
  {
    return false;
  }

  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("skywaysgame.plist", "skywaysgame.png");

  gameModel = GameModel::sharedGameModel();
  gameModel->reset();

  CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
  CCPoint middleScreen(visibleSize.width/2.0f, visibleSize.height/2.0f);

  CCSprite *background = CCSprite::create("starBackground.png", CCRectMake(0, 0, visibleSize.width, visibleSize.height));
  ccTexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
  background->getTexture()->setTexParameters(&params);

  background->setPosition(ccp(visibleSize.width*0.5f, visibleSize.height*0.5f));
  addChild(background);

  planetSprites = CCArray::create();
  planetSprites->retain();

  stationSprites = CCArray::create();
  stationSprites->retain();

  shipSprites = CCArray::create();
  shipSprites->retain();

  gameplayLayer = CCLayer::create();
  initGameplayLayer(gameModel);
  addChild(gameplayLayer);

  debugLayer = Box2DDebugLayer::create();
  addChild(debugLayer);

  CCMenuItemLabel *pPauseItem = CCMenuItemLabel::create(CCLabelBMFont::create("Close", 
    "font-text.fnt"),
    this,
    menu_selector(GameScene::pauseHandler));
  pPauseItem->setPosition(ccp(origin.x + visibleSize.width - pPauseItem->getContentSize().width/2- 10.0f,
    origin.y + visibleSize.height - pPauseItem->getContentSize().height/2 - 10.0f));

  // create menu, it's an autorelease object
  CCMenu* pMenu = CCMenu::create(pPauseItem, NULL);
  pMenu->setPosition(CCPointZero);
  this->addChild(pMenu, 1);

  shipStatus = CCLabelBMFont::create("Ships arrived: 0 // Ships lost: 0 // Money: $1000", "font-text.fnt", 1024, kCCTextAlignmentCenter);
  shipStatus->setPosition(ccp(512.0f, 20.0f));
  addChild(shipStatus);

  iconAnimation = CCSequence::create(CCSpawn::create(CCEaseOut::create(CCMoveTo::create(3.0f, middleScreen), 2.0f), 
                                                     CCEaseOut::create(CCFadeOut::create(1.0f), 2.0f), 
                                                     NULL),
                                     CCRemoveSelf::create(),
                                     NULL);
  iconAnimation->retain();

  handPointerAction = CCSequence::create(CCRepeat::create(CCSequence::create(CCEaseSineInOut::create(CCFadeIn::create(0.5)),
                                                                             CCEaseSineInOut::create(CCFadeOut::create(0.5f)),
                                                                             NULL),
                                                          4),
                                         CCRemoveSelf::create(),
                                         NULL);
  handPointerAction->retain();

  setTouchEnabled(true);
  CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
  CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameScene::launchShipHandler), this, 6.0f, false);

  return true;
}

void GameScene::registerWithTouchDispatcher()
{
  CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameScene::initGameplayLayer(GameModel *gm) {
  CCObject *obj;
  CCARRAY_FOREACH(gm->planets, obj) {
    PlanetModel *pm = static_cast<PlanetModel *>(obj);

    PlanetSprite *ps = PlanetSprite::create(pm);
    gameplayLayer->addChild(ps);
    ps->registerPhysics(gm->pWorld);

    planetSprites->addObject(ps);
  }

  CCARRAY_FOREACH(gm->stations, obj) {
    StationModel *sm = static_cast<StationModel *>(obj);

    StationSprite *ss = StationSprite::create(sm);
    gameplayLayer->addChild(ss);
    ss->registerPhysics(gm->pWorld);
    sm->setSprite(ss);

    stationSprites->addObject(ss);
  }

  CCARRAY_FOREACH(gm->ships, obj) {
    ShipModel *sm = static_cast<ShipModel *>(obj);

    ShipSprite *ss = ShipSprite::create(sm);
    gameplayLayer->addChild(ss);
    ss->registerPhysics(gm->pWorld);
    sm->setSprite(ss);

    shipSprites->addObject(ss);
  }

  boundaries = SpaceBoundaries::create();
  gameplayLayer->addChild(boundaries);
  boundaries->registerPhysics(gm->pWorld);
}

void GameScene::update(float dt) {
  gameModel->update(dt);
  shipStatus->setString(CCString::createWithFormat("Ships arrived: %d // Ships lost: %d // Money: $%d", gameModel->getShipsArrived(), gameModel->getShipsLost(), gameModel->getMoney())->getCString());
  CCObject *obj;
  CCARRAY_FOREACH(stationSprites, obj) {
    StationSprite *ss = dynamic_cast<StationSprite *>(obj);
    ss->update(dt);
  }
  CCARRAY_FOREACH_REVERSE(shipSprites, obj) {
    ShipSprite *ss = dynamic_cast<ShipSprite *>(obj);
    ss->update(dt);
    if (ss->getMarkedToRemoveKilled()) {
      ShipModel *sModel = ss->getModel();
      CCPoint shipPosition(ss->getPosition());
      ss->removeFromGame();
      shipSprites->removeObject(ss);
      ss->removeFromParent();

      CCSprite *deathSprite = CCSprite::createWithSpriteFrameName("death.png");
      gameplayLayer->addChild(deathSprite);
      deathSprite->setPosition(shipPosition);
      deathSprite->runAction(iconAnimation);
      
      gameModel->removeShip(sModel, true);
    } else if (ss->getMarkedToRemoveArrived()) {
      ShipModel *sModel = ss->getModel();
      CCPoint shipPosition(ss->getPosition());
      ss->removeFromGame();
      shipSprites->removeObject(ss);
      ss->removeFromParent();

      CCSprite *coinSprite = CCSprite::createWithSpriteFrameName("coin.png");
      gameplayLayer->addChild(coinSprite);
      coinSprite->setPosition(shipPosition);
      coinSprite->runAction(iconAnimation);

      gameModel->removeShip(sModel, false);
    }
  }
}

void GameScene::launchShipHandler(float dt) {
  ShipModel *shipModel = gameModel->launchShip();
  if (shipModel) {
    ShipSprite *shipSprite = ShipSprite::create(shipModel);
    gameplayLayer->addChild(shipSprite);
    shipSprite->registerPhysics(gameModel->pWorld);
    shipModel->setSprite(shipSprite);
    shipSprites->addObject(shipSprite);
  
    b2Vec2 shipCenter(shipSprite->getBody()->GetPosition());
    b2Vec2 stationCenter(shipModel->getDestinationStation()->getSprite()->getBody()->GetPosition());
    b2Vec2 towardsVector(stationCenter - shipCenter);
    towardsVector.Normalize();
    towardsVector *= 0.5f;
    shipSprite->getBody()->ApplyLinearImpulse(towardsVector, b2Vec2_zero);

    CCPoint destinationStationPosition(shipModel->getDestinationStation()->getSprite()->getPosition());
    destinationStationPosition.x -= 30;
    CCSprite *handPointer = CCSprite::createWithSpriteFrameName("handpointer.png");
    gameplayLayer->addChild(handPointer);
    handPointer->setScale(3.0f);
    handPointer->setOpacity(0);
    handPointer->setPosition(destinationStationPosition);
    handPointer->runAction(handPointerAction);
  }
}

void GameScene::pauseHandler(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
  CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
  CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
#endif
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
  gameModel->beginTouchHole(pTouch);
  return true;
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
  gameModel->moveTouchHole(pTouch);
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
  gameModel->endTouchHole(pTouch);
}

void GameScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){ 
  gameModel->endTouchHole(pTouch);
}