/*
* StationModel.cpp
*
*  Created on: 29/05/2013
*      Author: Ciro
*/

#include "DictUtils.h"
#include "Common.h"
#include "StationModel.h"
#include "StationSprite.h"

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

  CCArray *cArray = getArrayForCCDictionary(root, STATIONMODEL_COLOR);

  color.r = static_cast<CCFloat *>(cArray->objectAtIndex(0))->getValue();
  color.g = static_cast<CCFloat *>(cArray->objectAtIndex(1))->getValue();
  color.b = static_cast<CCFloat *>(cArray->objectAtIndex(2))->getValue();

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
