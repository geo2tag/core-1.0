#ifndef MAPWIDGETENUM_H
#define MAPWIDGETENUM_H


#include "StringEnum.h"

#define MAP_WIDGET_PARSING_ERROR "MapWidget parsing error!!"

#define MAP_WIDGET_ELEMENTS_NUMBER  3
enum MapWidget {OSM_DESKTOP, OSM_MOBILE, OFF};
static char* MAP_WIDGET_NAMES[] = {"OSM_DESKTOP", "OSM_MOBILE", "OFF" };

typedef StringEnum<MapWidget, MAP_WIDGET_ELEMENTS_NUMBER> MapWidgetEnum;


#endif // MAPWIDGETENUM_H
