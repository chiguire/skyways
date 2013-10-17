/*
* FingerObject.cpp
*
*      Author: Ciro
*/

#include "FingerObject.h"

FingerObject::FingerObject()
  : touchId(SCREEN_NOTOUCH)
{
}

FingerObject::~FingerObject() {
}

bool FingerObject::init() {
  TileObject::init();
  return true;
}

FingerObject *FingerObject::create() {
  FingerObject *pRet = new FingerObject();
  if (pRet && pRet->init())
  {
    pRet->autorelease();
    return pRet;
  }
  else
  {
    CC_SAFE_DELETE(pRet);
    return NULL;
  }
}