/*
* DictUtils.h
*
*  Created on: 04/06/2013
*      Author: Ciro
*/

#ifndef DICTUTILS_H_
#define DICTUTILS_H_

#include "cocos2d.h"
#include <string>

USING_NS_CC;

static inline CCString *getStringForCCDictionary(CCDictionary *dict, const char *key, const char *defaultValue) {
  CCObject *obj = dict->objectForKey(key);

  if (obj == NULL) {
    return CCString::create(std::string(defaultValue));
  } else {
    return dynamic_cast<CCString*>(obj);
  }
}

static inline int getIntegerForCCDictionary(CCDictionary *dict, const char *key, int defaultValue) {
  CCObject *obj = dict->objectForKey(key);

  if (obj == NULL) {
    return defaultValue;
  } else {
    return dynamic_cast<CCString*>(obj)->intValue();
  }
}

static inline float getFloatForCCDictionary(CCDictionary *dict, const char *key, float defaultValue) {
  CCObject *obj = dict->objectForKey(key);

  if (obj == NULL) {
    return defaultValue;
  } else {
    return dynamic_cast<CCString*>(obj)->floatValue();
  }
}

static inline double getDoubleForCCDictionary(CCDictionary *dict, const char *key, double defaultValue) {
  CCObject *obj = dict->objectForKey(key);

  if (obj == NULL) {
    return defaultValue;
  } else {
    return dynamic_cast<CCString*>(obj)->doubleValue();
  }
}

static inline double getBoolForCCDictionary(CCDictionary *dict, const char *key, bool defaultValue) {
  CCObject *obj = dict->objectForKey(key);

  if (obj == NULL) {
    return defaultValue;
  } else {
    return dynamic_cast<CCString*>(obj)->boolValue();
  }
}

static inline CCArray *getArrayForCCDictionary(CCDictionary *dict, const char *key) {
  CCObject *obj = dict->objectForKey(key);
  if (!obj) return NULL;
  return dynamic_cast<CCArray *>(obj);
}

#endif /* DICTUTILS_H_ */
