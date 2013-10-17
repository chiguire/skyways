/*
* PlanetModel.h
*
*  Created on: 29/05/2013
*      Author: Ciro
*/

#ifndef PLANETMODEL_H_
#define PLANETMODEL_H_

#include "cocos2d.h"

USING_NS_CC;

class PlanetModel : public CCObject {
public:
  PlanetModel();
  virtual ~PlanetModel();

  virtual bool init(CCDictionary *dict);
  static PlanetModel *create(CCDictionary *dict);

  CC_SYNTHESIZE(CCPoint, position, Position);
  CC_SYNTHESIZE(float, gravity, Gravity);
  CC_SYNTHESIZE(float, planetRadius, PlanetRadius);
  CC_SYNTHESIZE(float, gravityFieldRadius, GravityFieldRadius);
  CC_SYNTHESIZE(bool, isBlackHole, IsBlackHole);
  CC_SYNTHESIZE(CCString *, spriteFrameName, SpriteFrameName);
};

#endif /* PLANETMODEL_H_ */
