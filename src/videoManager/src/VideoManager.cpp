#include "VideoManager.h"

VideoFile* VideoManager::cut(VideoFile* f, int from, int to){
	std::string fileName =  std::to_string(from) + std::to_string(to)+f->getName();
	VideoFile *resultFile = new VideoFile(f->getDir(), fileName);
	std::string command = "avconv -i " + f->getAbsoletePath() + " -ss " + std::to_string(from) + " -t "+ 
		std::to_string(to) + " -codec copy " + resultFile->getAbsoletePath();
	
	std::cout << command << std::endl;
	Terminal::getInstance()->execute(command.c_str());
	return resultFile;		
}
