#ifndef TARGETENUM_H
#define TARGETENUM_H

#include "StringEnum.h"

#define TARGET_PARSING_ERROR "Target parsing error!!"

#define TARGET_ELEMENTS_NUMBER  2
enum Target {ANDROID, J2ME};
static char* TARGET_NAMES[] = {"Android", "J2ME"};

typedef StringEnum<Target, TARGET_ELEMENTS_NUMBER> TargetEnum;

//template<> char** TargetEnum::STRING_NAMES = TARGET_NAMES;
//template<> char* TargetEnum::ERROR_MESSAGE = TARGET_PARSING_ERROR;

#endif // TARGETENUM_H
