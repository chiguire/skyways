/*
* TileObject.h
*
*      Author: Ciro
*/

#ifndef TILEOBJECT_H_
#define TILEOBJECT_H_

#include "cocos2d.h"
#include "Common.h" 
#include "Box2D/Box2D.h"
#include <string>

USING_NS_CC;
using namespace std;

class TileObject: public cocos2d::CCSprite {
public:
  TileObject();
  virtual ~TileObject();

  CC_SYNTHESIZE(b2Body *, body, Body);

  virtual void beginContact(TileObject *o) { CC_UNUSED_PARAM(o); }
  virtual void endContact(TileObject *o) { CC_UNUSED_PARAM(o); }
};

#endif /* TILEOBJECT_H_ */
