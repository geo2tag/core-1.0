#ifndef H_TERMINAL
#define H_TERMINAL
#include<stdlib.h>
class Terminal
{
	static Terminal* m_instance;
	Terminal() {}
	Terminal(const Terminal&){};
public:
	static Terminal* getInstance();
	void execute(const char* command);
};

#endif

