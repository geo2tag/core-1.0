#ifndef SERVICEGENERATOR_H
#define SERVICEGENERATOR_H

#include <QString>
#include "ServiceDescription.h"

// Commands
#define COPY_STUB_PROJECT QString("cp -R stubproject/ %1/")
#define SED_REPLACE_COMMAND QString("sed -i \'s|%1|%2|g\' %3")

// Directories
#define TEMPLATES_DIR QString("templates/")
#define SOURCES_SUBDIR QString("src/org/geo2tag/stubproject/")

// Template files
#define SETTINGS_FILE QString("Settings.java")
#define MONITOR_FILE QString("CustomMonitor.java")
#define TRACKABLE_FILE QString("CustomTrackable.java")
#define MANIFEST_FILE QString("AndroidManifest.xml")
#define CUSTOM_MONITOR_FILE QString("CustomMonitor.java")
#define CUSTOM_TRACKABLE_FILE QString("CustomTrackable.java")

// Placeholders
#define SERVICES_PLACEHOLDER "SERVICES_PLACEHOLDER"
#define SERVER_URL_PLACEHOLDER "SERVER_URL_PLACEHOLDER"
#define DB_NAME_PLACEHOLDER "DB_NAME_PLACEHOLDER"


#define SERVICE_XML_STRING QString("<service android:enabled=\"true\" android:name=\"org.geo2tag.stubproject.%1\" />")


class ServiceGenerator
{
private:

    QString m_outputDir;
    QString m_serviceName;
    ServiceDescription m_serviceDescription;

    QString m_servicesList;

    bool isDirectoryWritable(const QString &outputDir) const;

    void createStubProject();
    void initSettings();
    void initMonitors();
    void initTrackables();
    void initManifestFile();

    void addServiceToList(const QString& serviceName);

    QString getServicePath();

    bool copyFile(const QString& sourceFile, const QString& destinationDir, const QString& destinationName="");

    void replacePlaceholders(const QString& file, const QString& placeholder,
                                               const QString& value);
public:

    ServiceGenerator(const QString &serviceName, const QString &outputDir, const ServiceDescription &serviceDescription);

    void generateService();
    QString getServiceSourcesPath();
    void replacePlaceholderForMapObject(const QString &file, MapObject *object);
};

#endif // SERVICEGENERATOR_H