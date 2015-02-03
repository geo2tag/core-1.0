#include "VideoFile.h"


VideoFile::VideoFile(QString fileDir, QString fileName){
	m_fileDir = fileDir;
	m_fileName = fileName;
}

void VideoFile::rename(QString newName){
	QString fromPath = getAbsoletePath();
	m_fileName = newName;
	QString toPath = getAbsoletePath();
	QString command = "mv " + fromPath + " " + toPath;
	Terminal::getInstance()->execute(command.toStdString().c_str());
}

void VideoFile::replace(QString newDir){
	QString fromPath = getAbsoletePath();
	m_fileDir = newDir;
	QString toPath = getAbsoletePath();
	QString command = "mv " + fromPath + " " + toPath;
	Terminal::getInstance()->execute(command.toStdString().c_str());
}

void VideoFile::remove(){
	QString command = "rm " + getAbsoletePath();
	Terminal::getInstance()->execute(command.toStdString().c_str());
}

QString VideoFile::getAbsoletePath(){
	return m_fileDir + m_fileName;
}

QString VideoFile::getName(){ return m_fileName; }
QString VideoFile::getPath(){ return m_fileDir + m_fileName; }
QString VideoFile::getDir() { return m_fileDir; }
