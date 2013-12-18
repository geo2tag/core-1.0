#include "ServiceGenerator.h"
#include <QFileInfo>
#include <QDir>

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
    for (int i=0; i<monitors.size(); i++){
        Monitor monitor = monitors.at(i);
        copyFile(TEMPLATES_DIR+MONITOR_FILE, getServiceSourcesPath(),
                 monitor.name+".java");
        QString newMonitorFile = getServiceSourcesPath() + "/" + monitor.name + ".java";

        replacePlaceholderForMapObject(newMonitorFile, &monitor);
        addAndroidServiceToList(monitor.name);
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
    for (int i=0; i<trackables.size(); i++){
        Trackable trackable = trackables.at(i);
        copyFile(TEMPLATES_DIR+TRACKABLE_FILE, getServiceSourcesPath(),
                 trackable.name+".java");
        QString newTrackableFile = getServiceSourcesPath() + "/" + trackable.name +
                ".java";

        replacePlaceholderForMapObject(newTrackableFile, &trackable);
        addAndroidServiceToList(trackable.name);
    }
}

void ServiceGenerator::initMapActivity()
{
    if (m_serviceDescription.mapWidget.value == NO_MAPWIDGET ){
        qDebug() << "No map widget!!";
        return;
    }

    m_mapActivity = MAP_ACTIVITY_XML_STRING;
    m_mapActivityTransfer = MAP_ACTIVITY_TRANSFER_STRING;

    QString sourceMapActivity = TEMPLATES_DIR + MAP_ACTIVITY_FILE;

    copyFile(sourceMapActivity, getServiceSourcesPath(),
             MAP_ACTIVITY_FILE);
    QString mapActivityFile = getServiceSourcesPath() + "/" + MAP_ACTIVITY_FILE;
    QString mapActivityBaseClass;

    if (m_serviceDescription.mapWidget.value == GOOGLE_MOBILE )
        mapActivityBaseClass = GOOGLE_MAP_ACTIVITY_CLASS;
    else if (m_serviceDescription.mapWidget.value == OSM_MOBILE )
        mapActivityBaseClass = OSM_MAP_ACTIVITY_CLASS;

    replacePlaceholders(mapActivityFile, BASE_ACTIVITY_PLACEHOLDER, mapActivityBaseClass );
//    if (m_serviceDescription.mapWidget.value == GOOGLE_MOBILE )
//        sourceMapActivity = TEMPLATES_DIR+GOOGLE_MAP_ACTIVITY_FILE;
//    else if (m_serviceDescription.mapWidget.value == OSM_MOBILE )
//        sourceMapActivity = TEMPLATES_DIR+OSM_MAP_ACTIVITY_FILE;


//    copyFile(sourceMapActivity, getServiceSourcesPath(),
//             MAP_ACTIVITY_NAME);

    initMapActivityTransition();
}

void ServiceGenerator::initMapActivityTransition()
{
	QString mainActivityFile = getServiceSourcesPath() + "/" + MAIN_ACTIVITY_FILE;

	replacePlaceholders(mainActivityFile, MAP_ACTIVITY_TRANSFER_PLACEHOLDER, MAP_ACTIVITY_TRANSFER_STRING );
	
}

void ServiceGenerator::initManifestFile()
{
    copyFile(TEMPLATES_DIR+MANIFEST_FILE, getServicePath());
    QString newManifestFilePath = getServicePath() + "/" + MANIFEST_FILE;

    qDebug() << m_servicesList;

    //system(QString("cat %1").arg(newManifestFilePath).toStdString().c_str());
    replacePlaceholders(newManifestFilePath, ANDROID_SERVICES_PLACEHOLDER, m_servicesList);
    replacePlaceholders(newManifestFilePath, MAP_ACTIVITY_PLACEHOLDER, m_mapActivity);



}

void ServiceGenerator::addAndroidServiceToList(const QString &serviceName)
{
    m_servicesList += ANDROID_SERVICE_XML_STRING.arg(serviceName);
}

QString ServiceGenerator::getServiceSourcesPath() const{
    return getServicePath()+"/"+SOURCES_SUBDIR;
}

QString ServiceGenerator::getServiceTestPath() const{
    return getServicePath()+"/"+TEST_SUBDIR;
}

QString ServiceGenerator::getServicePath() const{
    return m_outputDir+"/"+m_serviceName;
}

void ServiceGenerator::generateService()
{
    createStubProject();
    initSettings();
    initMonitors();
    initTrackables();
    initMapActivity();
    initManifestFile();
    initTests();
}

void ServiceGenerator::replacePlaceholders(const QString& file, const QString& placeholder,
                                           const QString& value)
{
    qDebug() << "ServiceGenerator::replacePlaceholders:" << file << placeholder << value;
    system(SED_REPLACE_COMMAND.arg(placeholder).arg(value).arg(file).toStdString().c_str());
}

bool ServiceGenerator::copyFile(const QString& sourceFile, const QString& destinationDir,
                                const QString& destinationName)
{
    qDebug() << sourceFile << destinationDir<< destinationName;


    QFileInfo fileInfo(sourceFile);
    QString fileName;

    if (!destinationName.isEmpty())
        fileName = destinationName;
    else
        fileName = fileInfo.fileName();

    QString destinationFile = destinationDir + QDir::separator() + fileName;

    qDebug() << "Copying " << sourceFile << destinationFile;

    bool result = QFile::copy(sourceFile, destinationFile);
    return result;
}


void ServiceGenerator::initTests(){

	system(QString("mkdir -p %1").arg(getServiceTestPath()).toStdString().c_str());

	for (int i=0; i<m_serviceDescription.trackables.size(); i++)
		createTestForAndroidService(m_serviceDescription.trackables.at(i).name);

	for (int i=0; i<m_serviceDescription.monitors.size(); i++)
		createTestForAndroidService(m_serviceDescription.monitors.at(i).name);
}


QString ServiceGenerator::getAndroidServiceTestFileName(const QString& serviceName) const{
	return serviceName + ANDROID_SERVICE_TEST_TEMPLATE; 
}

void ServiceGenerator::createTestForAndroidService(const QString& serviceName){
	QString templateFile = TEMPLATES_DIR + ANDROID_SERVICE_TEST_FILE;
	QString destinationFile = getServiceTestPath() + QDir::separator() + 
		getAndroidServiceTestFileName(serviceName);
	// Create file 
	copyFile(templateFile, getServiceTestPath(), getAndroidServiceTestFileName(serviceName));

	// Replace placeholders
	replacePlaceholders(destinationFile, ANDROID_SERVICE_NAME_PLACEHOLDER, serviceName);
}
