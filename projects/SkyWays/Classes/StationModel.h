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

class StationModel : public CCObject {
public:
	StationModel();
	virtual ~StationModel();

	virtual bool init(CCDictionary *dict);
	static StationModel *create(CCDictionary *dict);

  CC_SYNTHESIZE(CCPoint, position, Position);
  CC_SYNTHESIZE(CCString *, spriteFrameName, SpriteFrameName);
};

#endif /* STATIONMODEL_H_ */
