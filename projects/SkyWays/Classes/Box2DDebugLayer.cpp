/*
 * Box2DDebugLayer.cpp
 *
 *  Created on: 13/06/2013
 *      Author: Ciro
 */

#include "Box2DDebugLayer.h"
#include "TileObject.h"
#include "GLES-Render.h"

Box2DDebugLayer::Box2DDebugLayer()
{
}

Box2DDebugLayer::~Box2DDebugLayer() {
}

bool Box2DDebugLayer::init() {
	GLESDebugDraw *debugDraw = new GLESDebugDraw(PTM_RATIO);

	gameModel = GameModel::sharedGameModel();
	gameModel->pWorld->SetDebugDraw(debugDraw);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	//        flags += b2Draw::e_aabbBit;
	//        flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugDraw->SetFlags(flags);

	return true;
}

void Box2DDebugLayer::draw() {
	CCLayer::draw();
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
	kmGLPushMatrix();
	gameModel->pWorld->DrawDebugData();
	kmGLPopMatrix();
}
