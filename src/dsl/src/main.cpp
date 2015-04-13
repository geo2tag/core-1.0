#include <QDebug>

#include "ServiceDescription.h"
#include "ServiceFileParser.h"
#include "ServiceGenerator.h"

#include "InitStringEnums.h"

int main(int argc, char** argv)
{
    initStringEnums();

    if (argc != 3)
	{
		qDebug() << "Not enough arguments, usage:\n ./dsl service_description output_dir";
		return 1;
	}

    QString file = argv[1];
    QString outputDir = argv[2];
    QString serviceName = file.left(file.indexOf(".")-1);

    system("pwd");
    qDebug() << serviceName;

    ServiceDescription serviceDescription = ServiceFileParser().parse(file);

    ServiceGenerator serviceGenerator(serviceName, outputDir, serviceDescription);
    serviceGenerator.generateService();

	return 0;
}
