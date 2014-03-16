#include "Test_RiakInteractor.h"
#include <QTest>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QDebug>
#include <QByteArray>
#include <iostream>


void Test::Test_RiakInteractor::putData()
{
    RiakInteractor riakObject("127.0.0.1", "8087");
    QFile TestDocument(":TestBlobs/Test_Document");
    QFile TestImage(":TestBlobs/Test_Image.jpg");
    QCOMPARE(TestDocument.exists(), true);
    QCOMPARE(TestImage.exists(), true);
    TestDocument.open(QIODevice::ReadOnly);
    TestImage.open(QIODevice::ReadOnly);
    QDataStream inTestDocument(&TestDocument);
    QDataStream inTestImage(&TestImage);
    char * dataTestDocument;
    char * dataTestImage;
    dataTestDocument = (char*)malloc(TestDocument.size());
    dataTestImage = (char*)malloc(TestImage.size());
    inTestDocument.readRawData(dataTestDocument, TestDocument.size());
    inTestImage.readRawData(dataTestImage, TestImage.size());
    QString dataTestDocumentString = QString::fromStdString(std::string(dataTestDocument, TestDocument.size()));
    QString dataTestImageString = QString::fromStdString(std::string(dataTestImage, TestImage.size()));

    qDebug() << dataTestImageString.size();
    qDebug() << dataTestDocumentString.size();

    riakObject.putData("Test_Bucket", "Test_KeyDocument", dataTestDocumentString, "text/plain");
    riakObject.putData("Test_Bucket", "Test_KeyImage", dataTestImageString, "image/jpeg");
    free(dataTestDocument);
    free(dataTestImage);
}

void Test::Test_RiakInteractor::getData()
{
    RiakInteractor riakObject("127.0.0.1", "8087");

    QString dataTestImageString;
    QString dataTestDocumentString;
    dataTestImageString = riakObject.getData("Test_Bucket", "Test_KeyImage");
    dataTestDocumentString = riakObject.getData("Test_Bucket", "Test_KeyDocument");

    qDebug() << dataTestImageString.size();
    qDebug() << dataTestDocumentString.size();

    QDir current = QDir::current();
    current.cdUp();

    QFile TestDocument(current.path() + "/tst/Test_DocumentReceived");
    QFile TestImage(current.path() + "/tst/Test_ImageReceived");
    TestDocument.open(QIODevice::WriteOnly);
    TestImage.open(QIODevice::WriteOnly);
    QDataStream outTestDocument(&TestDocument);
    QDataStream outTestImage(&TestImage);
    outTestImage.writeRawData(dataTestImageString.toStdString().c_str(), dataTestImageString.size());
    outTestDocument.writeRawData(dataTestDocumentString.toStdString().c_str(), dataTestDocumentString.size());

}
