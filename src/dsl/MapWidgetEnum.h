#ifndef MAPWIDGETENUM_H
#define MAPWIDGETENUM_H


#include "StringEnum.h"

#define MAP_WIDGET_PARSING_ERROR "MapWidget parsing error!!"

#define MAP_WIDGET_ELEMENTS_NUMBER  3
enum MapWidget {GOOGLE_MOBILE, OSM_MOBILE, NO_MAPWIDGET};
static char* MAP_WIDGET_NAMES[] = {"GOOGLE_MOBILE", "OSM_MOBILE", "NO_MAPWIDGET" };

typedef StringEnum<MapWidget, MAP_WIDGET_ELEMENTS_NUMBER> MapWidgetEnum;


#endif // MAPWIDGETENUM_H
