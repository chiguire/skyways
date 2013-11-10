#include "AppDelegate.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
  // initialize director
  CCDirector* pDirector = CCDirector::sharedDirector();
  CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

  pDirector->setOpenGLView(pEGLView);

  pEGLView->setDesignResolutionSize(1024, 600, kResolutionShowAll);

  // turn on display FPS
  pDirector->setDisplayStats(false);

  // set FPS. the default value is 1.0/60 if you don't call this
  pDirector->setAnimationInterval(1.0 / 60);

  // create a scene. it's an autorelease object
  CCScene *pScene = GameScene::scene();

  CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("background.wav");
  CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("snd/planethit.wav");
  CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("snd/shipcollected.wav");
  CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("snd/shipejected.wav");
  CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("snd/shipexplosion.wav");
  CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("snd/shiplost.wav");
  CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
  // run
  pDirector->runWithScene(pScene);

  return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
  CCDirector::sharedDirector()->stopAnimation();

  // if you use SimpleAudioEngine, it must be pause
  // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
  CCDirector::sharedDirector()->startAnimation();

  // if you use SimpleAudioEngine, it must resume here
  // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
