/*
* FingerObject.h
*
*      Author: Ciro
*/

#ifndef FINGEROBJECT_H_
#define FINGEROBJECT_H_

#include "TileObject.h"

#define SCREEN_NOTOUCH (-1)
#define FINGER_GRAVITY (4.0f)
#define FINGER_RADIUS (3.0f)

class FingerObject : public TileObject {
public:
  FingerObject();
  virtual ~FingerObject();

  bool init();
  static FingerObject *create();

  int touchId;

};

#endif /* FINGEROBJECT_H_ */