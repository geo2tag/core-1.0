#include "ServiceGenerator.h"
#include <QFileInfo>

ServiceGenerator::ServiceGenerator(const QString &serviceName,
                                   const QString &outputDir,
                                   const ServiceDescription &serviceDescription):
    m_serviceName(serviceName),m_outputDir(outputDir),
    m_serviceDescription(serviceDescription)
{

    if (!isDirectoryWritable(m_outputDir))
        qFatal(QString("Could not write to %1").
               arg(m_outputDir).toStdString().c_str());
}

bool ServiceGenerator::isDirectoryWritable(const QString &outputDir) const
{
    QFileInfo fileInfo(outputDir);
    return (fileInfo.isDir() && fileInfo.isWritable());
}

void ServiceGenerator::createStubProject()
{
    system(COPY_STUB_PROJECT.arg(getServicePath()).toStdString().c_str());
}


void ServiceGenerator::initSettings()
{
    // Cp settings.java to new project
    copyFile(TEMPLATES_DIR+SETTINGS_FILE, getServiceSourcesPath());
    // Replace all placeholders by its values
    QString newSettingsFilePath = getServiceSourcesPath() + "/" + SETTINGS_FILE;
    replacePlaceholders(newSettingsFilePath,
                        SERVER_URL_PLACEHOLDER, m_serviceDescription.server);
    replacePlaceholders(newSettingsFilePath,
                        DB_NAME_PLACEHOLDER, m_serviceDescription.dbName);
}

void ServiceGenerator::initMonitors()
{
    QList<Monitor> monitors = m_serviceDescription.monitors;
    for (int i=0; i<monitors; i++){
        Monitor monitor = monitors.at(i);
        copyFile(TEMPLATES_DIR+MONITOR_FILE, getServiceSourcesPath(),
                 monitor.name+".java");
        QString newMonitorFile = getServiceSourcesPath() + "/" + MONITOR_FILE;

        replacePlaceholderForMapObject(newMonitorFile, &monitor);

    }
}

void ServiceGenerator::replacePlaceholderForMapObject(const QString& file,
                                                      MapObject* object)
{
    QMap<QString, QString> replaceMap = object->getReplacementMap();

    QMap<QString, QString>::iterator i;
     for (i = replaceMap.begin(); i != replaceMap.end(); ++i)
         replacePlaceholders(file, i.key(), i.value());

}

void ServiceGenerator::initTrackables()
{
    QList<Trackable> trackables = m_serviceDescription.trackables;
    for (int i=0; i<trackables; i++){
        Trackable trackable = trackable.at(i);
        copyFile(TEMPLATES_DIR+TRACKABLE_FILE, getServiceSourcesPath(),
                 trackable.name+".java");
        QString newTrackableFile = getServiceSourcesPath() + "/" + TRACKABLE_FILE;

        replacePlaceholderForMapObject(newTrackableFile, &trackable);

    }
}

void ServiceGenerator::initManifestFile()
{
    copyFile(TEMPLATES_DIR+MANIFEST_FILE, getServicePath());
    QString newManifestFilePath = getServicePath() + "/" + MANIFEST_FILE;

    replacePlaceholders(newManifestFilePath, SERVICES_PLACEHOLDER, m_servicesList);
}

void ServiceGenerator::addServiceToList(const QString &serviceName)
{
    m_servicesList += SERVICE_XML_STRING.arg(serviceName);
}

QString ServiceGenerator::getServiceSourcesPath(){
    return getServicePath()+"/"+SOURCES_SUBDIR;
}


QString ServiceGenerator::getServicePath(){
    return m_outputDir+"/"+m_serviceName;
}

void ServiceGenerator::generateService()
{
    createStubProject();
    initSettings();
    initMonitors();
    initTrackables();
    initManifestFile();

}

void ServiceGenerator::replacePlaceholders(const QString& file, const QString& placeholder,
                                           const QString& value)
{
    system(SED_REPLACE_COMMAND.arg(placeholder).arg(value).arg(file));
}

bool ServiceGenerator::copyFile(const QString& sourceFile, const QString& destinationDir,
                                const QString& destinationName)
{
    QFileInfo fileInfo(sourceFile);
    QString fileName;

    if (!destinationName.isEmpty())
        fileName = destinationName;
    else
        fileName = fileInfo.fileName();

    QString destinationFile = destinationDir + QDir::separator() + fileName;
    bool result = QFile::copy(sourceFile, destinationFile);
    return result;
}

