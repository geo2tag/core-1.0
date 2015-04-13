#ifndef INIT_STRING_ENUMS_H
#define INIT_STRING_ENUMS_H

#include "TargetEnum.h"
#include "IsOnlineEnum.h"
#include "MapWidgetEnum.h"


// HACK
// TODO - find how initialize concrete template classes static variables in the header

void initStringEnums(){

    TargetEnum::STRING_NAMES = TARGET_NAMES;
    TargetEnum::ERROR_MESSAGE = TARGET_PARSING_ERROR;

    IsOnlineEnum::STRING_NAMES = IS_ONLINE_NAMES;
    IsOnlineEnum::ERROR_MESSAGE = IS_ONLINE_PARSING_ERROR;

    MapWidgetEnum::STRING_NAMES = MAP_WIDGET_NAMES;
    MapWidgetEnum::ERROR_MESSAGE = MAP_WIDGET_PARSING_ERROR;
}

#endif
