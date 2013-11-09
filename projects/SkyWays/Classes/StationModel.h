/*
* StationModel.h
*
*  Created on: 29/05/2013
*      Author: Ciro
*/

#ifndef STATIONMODEL_H_
#define STATIONMODEL_H_

#include "cocos2d.h"

USING_NS_CC;

class StationSprite;

class StationModel : public CCObject {
public:
  StationModel();
  virtual ~StationModel();

  virtual bool init(CCDictionary *dict);
  static StationModel *create(CCDictionary *dict);

  CC_SYNTHESIZE(CCPoint, position, Position);
  CC_SYNTHESIZE(CCString *, spriteFrameName, SpriteFrameName);

  CC_SYNTHESIZE(StationSprite *, sprite, Sprite);
  CC_SYNTHESIZE(ccColor3B, color, Color);
};

#endif /* STATIONMODEL_H_ */
