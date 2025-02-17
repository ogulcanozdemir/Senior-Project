LOCAL_PATH := $(call my-dir)

include $(call all-subdir-makefiles)
include $(CLEAR_VARS)
LOCAL_MODULE := opencv-prebuilt
LOCAL_SRC_FILES = ../../../../OpenCVAndroid/sdk/native/libs/$(TARGET_ARCH_ABI)/libopencv_java.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../OpenCVAndroid/sdk/native/jni/include/
LOCAL_STATIC_LIBRARIES :=  opencv_features2d opencv_imgproc opencv_core opencv_highgui

LOCAL_MODULE    := augmentedreality
LOCAL_SHARED_LIBRARIES := opencv-prebuilt
LOCAL_SRC_FILES := AugmentedReality.cpp ARMarker.cpp ARUtils.cpp ARCameraCalibration.cpp ARCube.cpp ARMatrix.cpp
LOCAL_LDLIBS 	+= -llog -ldl -landroid -lGLESv2 -lGLESv1_CM

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

include $(BUILD_SHARED_LIBRARY)