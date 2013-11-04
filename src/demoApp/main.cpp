#include <QApplication>
#include "mainwindow.h"
#include "SettingsStorage.h"

int main(int argc, char** argv){
	SettingsStorage::init();

	QApplication a(argc, argv);
  	MainWindow window;
  	window.show();
	return a.exec();

}
