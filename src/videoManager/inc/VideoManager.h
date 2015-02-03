#ifndef H_VIDEOMANAGER
#define H_VIDEOMANAGER
#include <QString>
#include"VideoFile.h"
#include"Terminal.h" 

class VideoManager
{
public:
	VideoFile* cut(VideoFile* f, int from, int to);

	VideoManager(){};
	~VideoManager(){};
};
#endif

