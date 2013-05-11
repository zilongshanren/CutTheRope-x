LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
	../../Classes/src/AbstractModel.cpp \
	../../Classes/src/AppDelegate.cpp \
	../../Classes/src/Constants.cpp \
	../../Classes/src/CoordinateHelper.cpp \
	../../Classes/src/GLES-Render.cpp \
	../../Classes/src/GameManager.cpp \
	../../Classes/src/HelloWorldScene.cpp \
	../../Classes/src/LevelEditor.cpp \
	../../Classes/src/LevelFileHandler.cpp \
	../../Classes/src/LevelSelectionScene.cpp \
	../../Classes/src/MyContactListener.cpp \
	../../Classes/src/PineappleModel.cpp \
	../../Classes/src/PopupMenu.cpp \
	../../Classes/src/RopeModel.cpp \
	../../Classes/src/RopeSprite.cpp \
	../../Classes/src/vpoint.cpp \
	../../Classes/src/vrope.cpp \
	../../Classes/src/vstick.cpp \
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes/include \
			$(LOCAL_PATH)/../../../external/Box2D \
			$(LOCAL_PATH)/../../../cocos2dx/support/tinyxml2

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) \
$(call import-module,external/Box2D)
