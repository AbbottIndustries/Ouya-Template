LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_CFLAGS += -DOUYA=1 -DCOCOS2D_DEBUG=1 -std=gnu++11

include $(LOCAL_PATH)/../../Classes/Android.mk

LOCAL_SRC_FILES += hellocpp/main.cpp \
					../../Classes/Platform/OUYA/Controls/GameControls.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../cocos2dx/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../external

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx/platform/ouya)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions/ouya)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
