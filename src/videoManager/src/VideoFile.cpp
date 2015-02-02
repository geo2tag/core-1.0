#include "VideoFile.h"


VideoFile::VideoFile(std::string fileDir, std::string fileName){
	m_fileDir = fileDir;
	m_fileName = fileName;
}

void VideoFile::rename(std::string newName){
	std::string fromPath = getAbsoletePath();
	m_fileName = newName;
	std::string toPath = getAbsoletePath();
	std::string command = "mv " + fromPath + " " + toPath;
	Terminal::getInstance()->execute(command.c_str());
}

void VideoFile::replace(std::string newDir){
	std::string fromPath = getAbsoletePath();
	m_fileDir = newDir;
	std::string toPath = getAbsoletePath();
	std::string command = "mv " + fromPath + " " + toPath;
	Terminal::getInstance()->execute(command.c_str());
}

void VideoFile::remove(){
	std::string command = "rm " + getAbsoletePath();
	Terminal::getInstance()->execute(command.c_str());
}

std::string VideoFile::getAbsoletePath(){
	return m_fileDir + m_fileName;
}

std::string VideoFile::getName(){ return m_fileName; }
std::string VideoFile::getPath(){ return m_fileDir + m_fileName; }
std::string VideoFile::getDir() { return m_fileDir; }
