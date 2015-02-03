#include "Terminal.h"

Terminal* Terminal::m_instance;

Terminal* Terminal::getInstance(){
	if (!Terminal::m_instance){
		Terminal::m_instance = new Terminal();
	}
	return Terminal::m_instance;
}

void Terminal::execute(const char* command){
	system(command);
}
