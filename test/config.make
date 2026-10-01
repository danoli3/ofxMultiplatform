################################################################################
# ofxMultiPlatform — unit test app
# Builds against parent Manager sources + ofxUnitTests (OF core addon).
################################################################################

# OF root: CI sets this. From apps/myApps/ofxMultiPlatform/test the tree root is four levels up.
ifndef OF_ROOT
	OF_ROOT=$(realpath ../../../..)
endif

################################################################################
# Compile Manager core from the parent project (not platform shells / demos)
################################################################################

MP_ROOT = $(realpath ..)

# Only Manager/ — scene registration for the main app lives in ../src/ofxRegisterScenes.cpp
# (excluded here); tests provide ofxRegisterProjectScenes() themselves.
PROJECT_EXTERNAL_SOURCE_PATHS = $(MP_ROOT)/src/Manager

# Headers for Manager (+ base app path for includes used by headers)
PROJECT_CFLAGS = -I$(MP_ROOT)/src/Manager -I$(MP_ROOT)/src/Apps -I$(MP_ROOT)/src
PROJECT_CFLAGS += -DOF_X_MULTIPLATFORM_TEST=1

################################################################################
# Addons
################################################################################
PROJECT_ADDONS = ofxUnitTests
