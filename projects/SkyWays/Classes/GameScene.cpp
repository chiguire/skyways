#include "GameScene.h"
#include "PlanetSprite.h"
#include "ShipSprite.h"
#include "StationSprite.h"
#include "SimpleAudioEngine.h"

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
  debugLayer->setVisible(false);

  pathDisplayLayer = CCDrawNode::create();
  addChild(pathDisplayLayer);

  ccColor4B gameoverColor = {0, 0, 0, 127};
  gameOverLayer = CCLayerColor::create(gameoverColor);
  gameOverLayer->setVisible(false);
  addChild(gameOverLayer);
  
  gameOverTitle = CCSprite::createWithSpriteFrameName("gameover.gif");
  gameOverLayer->addChild(gameOverTitle);
  gameOverTitle->setAnchorPoint(ccp(0.5f, 0.5f));
  gameOverTitle->setPosition(ccp(visibleSize.width/2.0f, visibleSize.height/2.0f));

  CCMenuItemLabel *pPauseItem = CCMenuItemLabel::create(CCLabelBMFont::create("Close", 
    "font-text.fnt"),
    this,
    menu_selector(GameScene::pauseHandler));
  pPauseItem->setPosition(ccp(origin.x + visibleSize.width - pPauseItem->getContentSize().width/2- 10.0f,
    origin.y + visibleSize.height - pPauseItem->getContentSize().height/2 - 10.0f));

  CCMenuItemLabel *pDebugItem = CCMenuItemLabel::create(CCLabelBMFont::create("Debug", "font-text.fnt"),
    this,
    menu_selector(GameScene::debugHandler));
  pDebugItem->setPosition(ccp(origin.x + visibleSize.width - pPauseItem->getContentSize().width - pDebugItem->getContentSize().width/2 - 40.0f,
    origin.y + visibleSize.height - pPauseItem->getContentSize().height/2 - 10.0f));

  // create menu, it's an autorelease object
  CCMenu* pMenu = CCMenu::create(pPauseItem, pDebugItem, NULL);
  pMenu->setPosition(CCPointZero);
  this->addChild(pMenu, 1);

  shipStatus = CCLabelBMFont::create("Ships arrived: 0 // Ships lost: 0 // Money: $1000", "font-text.fnt", 1024, kCCTextAlignmentCenter);
  shipStatus->setPosition(ccp(512.0f, 20.0f));
  addChild(shipStatus);

  iconAnimation = CCSequence::create(CCSpawn::create(CCEaseOut::create(CCMoveTo::create(3.0f, middleScreen), 2.0f), 
                                                     CCEaseOut::create(CCFadeOut::create(1.0f), 2.0f), 
                                                     NULL),
                                     CCRemoveSelf::create(true),
                                     NULL);
  iconAnimation->retain();

  handPointerAction = CCSequence::create(CCRepeat::create(CCSequence::create(CCEaseSineInOut::create(CCFadeIn::create(0.5)),
                                                                             CCEaseSineInOut::create(CCFadeOut::create(0.5f)),
                                                                             NULL),
                                                          4),
                                         CCRemoveSelf::create(true),
                                         NULL);
  handPointerAction->retain();

  CCArray *explosionArr = CCArray::create();
  for (int i = 0; i != 7; i++) {
    explosionArr->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
      CCString::createWithFormat("explosion%d.png", i)->getCString()
    ));
  }

  CCAnimation *explosionAnim = CCAnimation::createWithSpriteFrames(explosionArr, 0.05f);
  explosionAnim->setRestoreOriginalFrame(true);

  explosionAnimation = CCSequence::create(CCAnimate::create(explosionAnim),
                                          CCRemoveSelf::create(),
                                          NULL);
  explosionAnimation->retain();

  shipDeathAnimation = CCSequence::create(CCDelayTime::create(2.0f),
                                          CCFadeOut::create(1.0f),
                                          CCRemoveSelf::create(),
                                          NULL);
  shipDeathAnimation->retain();

  CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.wav", true);
  
  setTouchEnabled(true);
  CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
  CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameScene::launchShipHandler), this, 6.0f, false);

  return true;
}

void GameScene::onExit() {
  CCLayer::onExit();
  CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void GameScene::registerWithTouchDispatcher()
{
  CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameScene::initGameplayLayer(GameModel *gm) {
  CCObject *obj;
  planetSprites->removeAllObjects();
  CCARRAY_FOREACH(gm->planets, obj) {
    PlanetModel *pm = static_cast<PlanetModel *>(obj);

    PlanetSprite *ps = PlanetSprite::create(pm);
    gameplayLayer->addChild(ps);
    ps->registerPhysics(gm->pWorld);

    planetSprites->addObject(ps);
  }

  stationSprites->removeAllObjects();
  CCARRAY_FOREACH(gm->stations, obj) {
    StationModel *sm = static_cast<StationModel *>(obj);

    StationSprite *ss = StationSprite::create(sm);
    gameplayLayer->addChild(ss);
    ss->registerPhysics(gm->pWorld);
    sm->setSprite(ss);

    stationSprites->addObject(ss);
  }

  shipSprites->removeAllObjects();
  CCARRAY_FOREACH(gm->ships, obj) {
    ShipModel *sm = static_cast<ShipModel *>(obj);

    ShipSprite *ss = ShipSprite::create(sm);
    gameplayLayer->addChild(ss);
    ss->registerPhysics(gm->pWorld);
    sm->setSprite(ss);

    shipSprites->addObject(ss);
  }

  /* UGLY HACK TO MAKE JOINTS IN TWO PLANETS */
  // Should build in the next iteration a way to specify a joint between
  // two planets so they rotate around a point
  b2Body *b1 = ((PlanetSprite *)planetSprites->objectAtIndex(1))->getBody();
  b1->SetType(b2_dynamicBody);
  b2Body *b2 = ((PlanetSprite *)planetSprites->objectAtIndex(2))->getBody();
  b2->SetType(b2_dynamicBody);

  //Invisible point around which the two planets will rotate.
  b2Vec2 distanceVector(b2->GetPosition()-b1->GetPosition());
  float distanceVectorLength = distanceVector.Length();
  distanceVector.Normalize();

  b2BodyDef bDef;
  bDef.type = b2_staticBody;
  bDef.position = (distanceVectorLength/2.0f)*distanceVector+b1->GetPosition();
  b2Body *b3 = gm->pWorld->CreateBody(&bDef);

  b2DistanceJointDef jDef;
  jDef.Initialize(b1, b2, b1->GetWorldCenter(), b2->GetWorldCenter());

  b2RevoluteJointDef rDef;
  rDef.Initialize(b1, b3, b3->GetWorldCenter());
  rDef.maxMotorTorque = 50.0f;
  rDef.motorSpeed = 10 * M_PI / 180.0f;
  rDef.enableMotor = true;

  gm->pWorld->CreateJoint(&jDef);

  gm->pWorld->CreateJoint(&rDef);

  rDef.bodyA = b2;
  gm->pWorld->CreateJoint(&rDef);


  boundaries = SpaceBoundaries::create();
  gameplayLayer->addChild(boundaries);
  boundaries->registerPhysics(gm->pWorld);
}

void GameScene::update(float dt) {
  gameModel->update(dt);

  if (gameModel->getGameOver()) {
    if (!gameOverLayer->isVisible()) {
      gameOverLayer->setVisible(true);

      runAction(CCSequence::create(CCDelayTime::create(5.0f),
                                   CCCallFunc::create(this, callfunc_selector(GameScene::resetGameHandler)),
                                   NULL));
    }
  } else {
    shipStatus->setString(CCString::createWithFormat("Ships arrived: %d // Ships lost: %d // Money: $%d", gameModel->getShipsArrived(), gameModel->getShipsLost(), gameModel->getMoney())->getCString());
    CCObject *obj;
    CCARRAY_FOREACH(planetSprites, obj) {
      PlanetSprite *ss = dynamic_cast<PlanetSprite *>(obj);
      ss->update();
    }
    CCARRAY_FOREACH(stationSprites, obj) {
      StationSprite *ss = dynamic_cast<StationSprite *>(obj);
      ss->update(dt);
    }
    pathDisplayLayer->clear();
    CCARRAY_FOREACH_REVERSE(shipSprites, obj) {
      ShipSprite *ss = dynamic_cast<ShipSprite *>(obj);
      ss->update(dt);

      if (ss->draggedByFinger()) {
        ccColor4F c = { 0.6f, 0.0f, 0.0f, 0.15f };
        //CCPoint ctr1 = CCPoint(ss->getPosition()) + CCPoint(ss->getBody()->GetLinearVelocity().x, ss->getBody()->GetLinearVelocity().y)*PTM_RATIO;
        //CCPoint ctr2 = CCPoint(ss->getPosition() - ss->getModel()->getDestinationStation()->getSprite()->getPosition()).normalize()*PTM_RATIO;
        pathDisplayLayer->drawSegment(ss->getPosition(), ss->getModel()->getDestinationStation()->getSprite()->getPosition(), 2.0f, c);
      }
      if (ss->getMarkedToRemoveKilled()) {
        ShipModel *sModel = ss->getModel();
        CCPoint shipPosition(ss->getPosition());
        float shipAngle = ss->getRotation();
        ss->removeFromGame();
        shipSprites->removeObject(ss);
        ss->removeFromParent();

        CCSprite *deathSprite = CCSprite::createWithSpriteFrameName("death.png");
        gameplayLayer->addChild(deathSprite);
        deathSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        deathSprite->setPosition(shipPosition);
        deathSprite->runAction(((CCAction *)((CCAction *)iconAnimation->copy())->autorelease()));

        CCSprite *shipDeathSprite = CCSprite::createWithSpriteFrameName("ship-damaged.png");
        shipDeathSprite->addChild(shipDeathSprite);
        shipDeathSprite->setAnchorPoint(ccp(0.5f, 0.f));
        shipDeathSprite->setRotation(shipAngle);
        shipDeathSprite->setPosition(shipPosition);
        shipDeathSprite->runAction(((CCAction *)((CCAction *)shipDeathAnimation->copy())->autorelease()));

        CCSprite *explosionSprite = CCSprite::create();
        gameplayLayer->addChild(explosionSprite);
        explosionSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        explosionSprite->setPosition(shipPosition);
        explosionSprite->runAction(((CCAction *)((CCAction *)explosionAnimation->copy())->autorelease()));

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
        coinSprite->runAction(((CCAction *)((CCAction *)iconAnimation->copy())->autorelease()));

        gameModel->removeShip(sModel, false);

        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shipcollected.wav");
      }
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
    handPointer->runAction(((CCAction *)((CCAction *)handPointerAction->copy())->autorelease()));

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shipejected.wav");
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
  addChild(gameModel->getFingerObject());
  return true;
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
  gameModel->moveTouchHole(pTouch);
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
  gameModel->endTouchHole(pTouch);
  removeChild(gameModel->getFingerObject());
}

void GameScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){ 
  gameModel->endTouchHole(pTouch);
}

void GameScene::resetGameHandler() {

}

void GameScene::debugHandler(CCObject *pSender) {
  debugLayer->setVisible(!debugLayer->isVisible());
}