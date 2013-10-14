/*
 * ShipModel.h
 *
 *  Created on: 29/05/2013
 *      Author: Ciro
 */

#ifndef SHIPMODEL_H_
#define SHIPMODEL_H_

#include "cocos2d.h"

USING_NS_CC;

class ShipModel : public CCObject {
public:
	ShipModel();
	virtual ~ShipModel();

	virtual bool init(CCDictionary *dict);
	static ShipModel *create(CCDictionary *dict);

  CC_SYNTHESIZE(CCPoint, position, Position);
  CC_SYNTHESIZE(CCString *, spriteFrameName, SpriteFrameName);
};

#endif /* SHIPMODEL_H_ */
