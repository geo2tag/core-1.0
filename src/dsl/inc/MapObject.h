#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <QMap>

#define NAME_PLACEHOLDER "NAME_PLACEHOLDER"
#define BROADCAST_ACTION_PLACEHOLDER "BROADCAST_ACTION_PLACEHOLDER"
#define UPDATE_INTERVAL_PLACEHOLDER "UPDATE_INTERVAL_PLACEHOLDER"


class MapObject{
private:
   QMap<QString, QString> m_map;
public:

   QMap<QString, QString> getReplacementMap() {
       return m_map;
   }

   void addReplacePair(const QString& placeholder, const QString& value){
       m_map.insert(placeholder, value);
   }
};

#endif // MAPOBJECT_H
