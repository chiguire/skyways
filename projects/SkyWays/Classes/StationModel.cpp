/*
 * StationModel.cpp
 *
 *  Created on: 29/05/2013
 *      Author: Ciro
 */

#include "DictUtils.h"
#include "Common.h"
#include "StationModel.h"

StationModel::StationModel()
: position()
, spriteFrameName(NULL)
{
}

StationModel::~StationModel() {
}

bool StationModel::init(CCDictionary *root) {
  position.x = getFloatForCCDictionary(root, PLANETMODEL_POSITION_X, 0.0f);
  position.y = getFloatForCCDictionary(root, PLANETMODEL_POSITION_Y, 0.0f);
  spriteFrameName = getStringForCCDictionary(root, PLANETMODEL_SPRITEFRAMENAME, "enemyShip.png");
  spriteFrameName->retain();

	return true;
}

StationModel *StationModel::create(CCDictionary *dict) {
	StationModel *pRet = new StationModel();
	if (pRet && pRet->init(dict)) {
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}
