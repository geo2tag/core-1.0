#ifndef ISONLINEENUM_H
#define ISONLINEENUM_H


#include "StringEnum.h"

#define IS_ONLINE_PARSING_ERROR "isOnline TRACKABLE atribute parsing error!!"

#define IS_ONLINE_ELEMENTS_NUMBER  2
enum IsOnline {ONLINE, OFFLINE};
static char* IS_ONLINE_NAMES[] = {"online", "offline"};

typedef StringEnum<IsOnline, IS_ONLINE_ELEMENTS_NUMBER> IsOnlineEnum;


#endif // ISONLINEENUM_H
