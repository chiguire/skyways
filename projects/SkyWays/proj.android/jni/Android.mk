LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/AppDelegate.h \
../../Classes/Box2DDebugLayer.cpp \
../../Classes/Box2DDebugLayer.h \
../../Classes/Common.h \
../../Classes/DictUtils.h \
../../Classes/FingerObject.cpp \
../../Classes/FingerObject.h \
../../Classes/GLES-Render.cpp \
../../Classes/GLES-Render.h \
../../Classes/GameModel.cpp \
../../Classes/GameModel.h \
../../Classes/GameScene.cpp \
../../Classes/GameScene.h \
../../Classes/PlanetModel.cpp \
../../Classes/PlanetModel.h \
../../Classes/PlanetSprite.cpp \
../../Classes/PlanetSprite.h \
../../Classes/ShipModel.cpp \
../../Classes/ShipModel.h \
../../Classes/ShipSprite.cpp \
../../Classes/ShipSprite.h \
../../Classes/SpaceBoundaries.cpp \
../../Classes/SpaceBoundaries.h \
../../Classes/StationModel.cpp \
../../Classes/StationModel.h \
../../Classes/StationSprite.cpp \
../../Classes/StationSprite.h \
../../Classes/TileObject.cpp \
../../Classes/TileObject.h

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
