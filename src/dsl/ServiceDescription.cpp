#include "ServiceDescription.h"
#include "ServiceDescriptionValidityException.h"
#include <QDebug>

ServiceDescription::ServiceDescription()
{
}

void ServiceDescription::checkDescriptionValidity() {

    for (int i=0; i<trackables.size(); i++){
        const Channel* channel = getChannelForTrackable(trackables.at(i));
        if (channel == NULL)
            throw ServiceDescriptionValidityException(
                    QString("Description is invalid: no channel for trackable \"%1\"").
                    arg(trackables.at(i).name));

        (trackables[i])
                .setChannel(channel->name);

    }

}

const Channel* ServiceDescription::getChannelForTrackable(const Trackable& trackable) const
{
    const Channel* channel = NULL;
    for (int i=0; i<channels.size(); i++){
        QString channelTrackable = channels.at(i).trackable;
        if (channelTrackable.compare(trackable.name, Qt::CaseInsensitive) == 0){
            channel = &channels.at(i);
            return channel;
        }
    }

    return channel;
}

void ServiceDescription::addDescriptionEntry(const QString& prefix,
                                             const QString& mainPart)
{

    if (prefix.compare(CHANNEL, Qt::CaseInsensitive) == 0){
        channels.append(Channel(mainPart));
    }else if (prefix.compare(TRACKABLE, Qt::CaseInsensitive) == 0){
        trackables.append(Trackable(mainPart));
    }else if (prefix.compare(MONITOR, Qt::CaseInsensitive) == 0){
        monitors.append(Monitor(mainPart));
    }else if (prefix.compare(SERVER, Qt::CaseInsensitive) == 0){
        server = mainPart;
    }else if (prefix.compare(DB_NAME, Qt::CaseInsensitive) == 0){
        dbName = mainPart;
    }else if (prefix.compare(TARGET, Qt::CaseInsensitive) == 0){
        target = TargetEnum(mainPart);
    }else if (prefix.compare(MAP_WIDGET, Qt::CaseInsensitive) == 0){
        mapWidget = MapWidgetEnum(mainPart);
    }else{
        throw ServiceFileParserException("Unknown entry type!");
    }

}
