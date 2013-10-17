/*
* PlanetModel.cpp
*
*  Created on: 29/05/2013
*      Author: Ciro
*/

#include "DictUtils.h"
#include "Common.h"
#include "PlanetModel.h"

PlanetModel::PlanetModel()
  : position()
  , gravity(1.0f)
  , planetRadius(1.0f)
  , gravityFieldRadius(2.0f)
  , isBlackHole(false)
  , spriteFrameName(NULL)
{
}

PlanetModel::~PlanetModel() {
}

bool PlanetModel::init(CCDictionary *root) {
  position.x = getFloatForCCDictionary(root, PLANETMODEL_POSITION_X, 0.0f);
  position.y = getFloatForCCDictionary(root, PLANETMODEL_POSITION_Y, 0.0f);
  gravity = getFloatForCCDictionary(root, PLANETMODEL_GRAVITY, 1.0f);
  planetRadius = getFloatForCCDictionary(root, PLANETMODEL_PLANET_RADIUS, 1.0f);
  gravityFieldRadius = getFloatForCCDictionary(root, PLANETMODEL_GRAVITY_FIELD_RADIUS, 2.0f);
  isBlackHole = getBoolForCCDictionary(root, PLANETMODEL_BLACKHOLE, false);
  spriteFrameName = getStringForCCDictionary(root, PLANETMODEL_SPRITEFRAMENAME, "planet-1.png");
  spriteFrameName->retain();

  return true;
}

PlanetModel *PlanetModel::create(CCDictionary *dict) {
  PlanetModel *pRet = new PlanetModel();
  if (pRet && pRet->init(dict)) {
    pRet->autorelease();
  }
  else
  {
    CC_SAFE_DELETE(pRet);
  }
  return pRet;
}
