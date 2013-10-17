/*
* SpaceBoundaries.h
*
*      Author: Ciro
*/

#ifndef SPACEBOUNDARIES_H_
#define SPACEBOUNDARIES_H_

#include "TileObject.h"

class SpaceBoundaries : public TileObject {
public:
  SpaceBoundaries();
  virtual ~SpaceBoundaries();

  bool init();
  static SpaceBoundaries *create();
  void registerPhysics(b2World *world);

};

#endif /* SPACEBOUNDARIES_H_ */