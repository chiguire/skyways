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
  gravityField = CCDrawNode::create();
  const int SEGMENTS = 30;
  float radius = FINGER_RADIUS*PTM_RATIO;
  ccColor4F colorBorder = { 0.6f, 0.6f, 1.0f, 0.6f };
  ccColor4F colorBackground = { 0.6f, 0.6f, 1.0f, 0.2f };
  float previousAngle = 0;
  CCPoint pVertices[SEGMENTS];
  for (int i = 0; i != SEGMENTS; i++) {
    float angle = (float(i+1)/float(SEGMENTS))*(2.0f*M_PI);
    CCPoint vertices[2] = {CCPoint(radius * cos(previousAngle), radius * sin(previousAngle)),
                           CCPoint(radius * cos(angle), radius * sin(angle))};
    gravityField->drawSegment(vertices[0], vertices[1], 1.0f, colorBorder);
    pVertices[i] = vertices[0];
    previousAngle = angle;
  }
  gravityField->drawPolygon(pVertices, SEGMENTS, colorBackground, 0.0f, colorBorder);

  addChild(gravityField);

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

void FingerObject::draw() {
}