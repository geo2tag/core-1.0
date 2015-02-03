#include "VideoManager.h"

VideoFile* VideoManager::cut(VideoFile* f, int from, int to){
	QString fileName =  QString::number(from) + QString::number(to)+f->getName();
	VideoFile *resultFile = new VideoFile(f->getDir(), fileName);
	QString command = "avconv -i " + f->getAbsoletePath() + " -ss " + QString::number(from) + " -t "+ 
		QString::number(to) + " -codec copy " + resultFile->getAbsoletePath();

	Terminal::getInstance()->execute(command.toStdString().c_str());
	return resultFile;		
}
