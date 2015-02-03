#ifndef H_VIDEOFILE
#define H_VIDEOFILE
#include <QString>
#include"Terminal.h"
class VideoFile
{
public:
	VideoFile(QString fileDir, QString fileName);
	~VideoFile(){};
	void rename(QString newName);
	void replace(QString newDir);
	void remove();
	QString getAbsoletePath();
	QString getName();
	QString getPath();
	QString getDir() ;
private:
	//static const QString videoFilesDir;
	QString m_fileDir;
	QString m_fileName;
};
//VideoFile::videoFilesDir = ".";
#endif
