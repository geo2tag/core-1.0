#ifndef H_VIDEOMANAGER
#define H_VIDEOMANAGER
#include"VideoFile.h"
#include"Terminal.h"
#include <iostream>
#include <string> 

class VideoManager
{
public:
	VideoFile* cut(VideoFile* f, int from, int to);

	VideoManager(){};
	~VideoManager(){};
};
#endif

