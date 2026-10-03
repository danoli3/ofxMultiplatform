#include "TestScenes.h"

int TestSceneA::setupCount = 0;
int TestSceneA::enterCount = 0;
int TestSceneA::exitCount = 0;
int TestSceneA::pauseCount = 0;
int TestSceneA::resumeCount = 0;
int TestSceneA::backCount = 0;
bool TestSceneA::handleBack = false;

int TestSceneB::setupCount = 0;
int TestSceneB::enterCount = 0;
int TestSceneB::exitCount = 0;

int TestSceneEvent::destroyedDuringUpdate = 0;
