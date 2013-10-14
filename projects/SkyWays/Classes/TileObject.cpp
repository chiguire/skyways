/*
 * TileObject.cpp
 *
 *  Created on: 04/06/2013
 *      Author: Ciro
 */

#include "TileObject.h"

TileObject::TileObject()
: body(NULL)
, defaultZ(1)
, isStatic(true)
, tileName("")
, tileType("") {

}

TileObject::~TileObject() {
}

void TileObject::slewX(float x) {
	slew(x, getPositionY());
}

void TileObject::slewY(float y) {
	slew(getPositionX(), y);
}

void TileObject::slew(CCPoint pos) {
	slew(pos.x, pos.y);
}

void TileObject::slew(float x, float y) {
	if (body) {
		b2Transform transform = body->GetTransform();
		transform.p.x = x;
		transform.p.y = y;
		body->SetTransform(transform.p, transform.q.GetAngle());
	}

	setPosition(ccp(x*PTM_RATIO, y*PTM_RATIO));
}
