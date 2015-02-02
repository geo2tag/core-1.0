#ifndef H_VIDEOFILE
#define H_VIDEOFILE
#include<string>
#include"Terminal.h"
class VideoFile
{
public:
	VideoFile(std::string fileDir, std::string fileName);
	~VideoFile(){};
	void rename(std::string newName);
	void replace(std::string newDir);
	void remove();
	std::string getAbsoletePath();
	std::string getName();
	std::string getPath();
	std::string getDir() ;
private:
	//static const std::string videoFilesDir;
	std::string m_fileDir;
	std::string m_fileName;
};
//VideoFile::videoFilesDir = ".";
#endif
