#ifndef __COMMON_H__
#define __COMMON_H__

#define PTM_RATIO (32.0f)
#define TO_B2D_COORDS(__x__) ((__x__)/PTM_RATIO)
#define FROM_B2D_COORDS(__x__) ((__x__)*PTM_RATIO)
#define SPACE_BORDER (40.0f)

#define USERDEFAULT_PLAYERNAME "playerName"
#define USERDEFAULT_PLAYERNAME_DEFAULT "Player"

#define USERDEFAULT_HIGHSCORE "highScore"
#define USERDEFAULT_HIGHSCORE_DEFAULT 100

#define PLANETMODEL_POSITION_X "positionX"
#define PLANETMODEL_POSITION_Y "positionY"
#define PLANETMODEL_GRAVITY "gravity"
#define PLANETMODEL_PLANET_RADIUS "planetRadius"
#define PLANETMODEL_GRAVITY_FIELD_RADIUS "gravityFieldRadius"
#define PLANETMODEL_BLACKHOLE "isBlackHole"
#define PLANETMODEL_SPRITEFRAMENAME "spriteFrameName"
#define STATIONMODEL_COLOR "color"

#endif
