/*
* Box2DDebugLayer.h
*
*  Created on: 13/06/2013
*      Author: Ciro
*/

#ifndef BOX2DDEBUGLAYER_H_
#define BOX2DDEBUGLAYER_H_

#include "cocos2d.h"
#include "Common.h"
#include "Box2D/Box2D.h"
#include "GameModel.h"

USING_NS_CC;

class Box2DDebugLayer : public CCLayer {
public:
  Box2DDebugLayer();
  virtual ~Box2DDebugLayer();

  bool init();
  virtual void draw();

  GameModel *gameModel;

  CREATE_FUNC(Box2DDebugLayer);
};

#endif /* BOX2DDEBUGLAYER_H_ */
