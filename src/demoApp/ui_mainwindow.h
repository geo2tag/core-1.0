/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Apr 18 01:47:52 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;
    QTabWidget *tabWidget;
    QWidget *usersTab;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *loginEdit;
    QLabel *label_2;
    QLineEdit *passwordEdit;
    QLabel *label_3;
    QLineEdit *emailEdit;
    QCheckBox *registerCheckbox;
    QPushButton *userActionButton;
    QWidget *tagsTab;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_6;
    QLineEdit *radiusEdit;
    QLabel *label_5;
    QLineEdit *latitudeEdit;
    QLabel *label_4;
    QLineEdit *longitudeEdit;
    QPushButton *tagActionButton;
    QSpacerItem *verticalSpacer_2;
    QWidget *channelsTab;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QListWidget *channelsListWidget;
    QPushButton *channelActionButton;
    QFrame *line;
    QLabel *label_7;
    QLineEdit *channelNameEdit;
    QLabel *label_8;
    QLineEdit *channelDescriptionEdit;
    QLabel *label_9;
    QLineEdit *channelUrlEdit;
    QPushButton *addChannelButton;
    QWidget *writeTagTab;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_15;
    QListWidget *subscribedListWidget;
    QLabel *label_12;
    QLineEdit *writeTagNameEdit;
    QLabel *label_13;
    QLineEdit *writeTagLatEdit;
    QLabel *label_14;
    QLineEdit *writeTagLonEdit;
    QPushButton *writeTagButton;
    QSpacerItem *verticalSpacer_3;
    QWidget *settingsTab;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_10;
    QLineEdit *serverEdit;
    QLabel *label_11;
    QLineEdit *portEdit;
    QSpacerItem *verticalSpacer;
    QListWidget *statusLog;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(-1, -1, 801, 551));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        graphicsView = new QGraphicsView(verticalLayoutWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMaximumSize(QSize(500, 16777215));

        horizontalLayout->addWidget(graphicsView);

        tabWidget = new QTabWidget(verticalLayoutWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setMaximumSize(QSize(300, 16777215));
        usersTab = new QWidget();
        usersTab->setObjectName(QString::fromUtf8("usersTab"));
        verticalLayoutWidget_2 = new QWidget(usersTab);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(0, 0, 291, 211));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        loginEdit = new QLineEdit(verticalLayoutWidget_2);
        loginEdit->setObjectName(QString::fromUtf8("loginEdit"));

        verticalLayout_2->addWidget(loginEdit);

        label_2 = new QLabel(verticalLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        passwordEdit = new QLineEdit(verticalLayoutWidget_2);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));

        verticalLayout_2->addWidget(passwordEdit);

        label_3 = new QLabel(verticalLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_3);

        emailEdit = new QLineEdit(verticalLayoutWidget_2);
        emailEdit->setObjectName(QString::fromUtf8("emailEdit"));
        emailEdit->setEnabled(false);

        verticalLayout_2->addWidget(emailEdit);

        registerCheckbox = new QCheckBox(verticalLayoutWidget_2);
        registerCheckbox->setObjectName(QString::fromUtf8("registerCheckbox"));

        verticalLayout_2->addWidget(registerCheckbox);

        userActionButton = new QPushButton(verticalLayoutWidget_2);
        userActionButton->setObjectName(QString::fromUtf8("userActionButton"));

        verticalLayout_2->addWidget(userActionButton);

        tabWidget->addTab(usersTab, QString());
        tagsTab = new QWidget();
        tagsTab->setObjectName(QString::fromUtf8("tagsTab"));
        tagsTab->setEnabled(true);
        verticalLayoutWidget_4 = new QWidget(tagsTab);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(0, 0, 291, 301));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(verticalLayoutWidget_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_6);

        radiusEdit = new QLineEdit(verticalLayoutWidget_4);
        radiusEdit->setObjectName(QString::fromUtf8("radiusEdit"));

        verticalLayout_4->addWidget(radiusEdit);

        label_5 = new QLabel(verticalLayoutWidget_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_5);

        latitudeEdit = new QLineEdit(verticalLayoutWidget_4);
        latitudeEdit->setObjectName(QString::fromUtf8("latitudeEdit"));

        verticalLayout_4->addWidget(latitudeEdit);

        label_4 = new QLabel(verticalLayoutWidget_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_4);

        longitudeEdit = new QLineEdit(verticalLayoutWidget_4);
        longitudeEdit->setObjectName(QString::fromUtf8("longitudeEdit"));

        verticalLayout_4->addWidget(longitudeEdit);

        tagActionButton = new QPushButton(verticalLayoutWidget_4);
        tagActionButton->setObjectName(QString::fromUtf8("tagActionButton"));

        verticalLayout_4->addWidget(tagActionButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        tabWidget->addTab(tagsTab, QString());
        channelsTab = new QWidget();
        channelsTab->setObjectName(QString::fromUtf8("channelsTab"));
        channelsTab->setEnabled(true);
        verticalLayoutWidget_3 = new QWidget(channelsTab);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(0, 0, 291, 411));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        channelsListWidget = new QListWidget(verticalLayoutWidget_3);
        channelsListWidget->setObjectName(QString::fromUtf8("channelsListWidget"));

        verticalLayout_3->addWidget(channelsListWidget);

        channelActionButton = new QPushButton(verticalLayoutWidget_3);
        channelActionButton->setObjectName(QString::fromUtf8("channelActionButton"));
        channelActionButton->setEnabled(true);

        verticalLayout_3->addWidget(channelActionButton);

        line = new QFrame(verticalLayoutWidget_3);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        label_7 = new QLabel(verticalLayoutWidget_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_7);

        channelNameEdit = new QLineEdit(verticalLayoutWidget_3);
        channelNameEdit->setObjectName(QString::fromUtf8("channelNameEdit"));

        verticalLayout_3->addWidget(channelNameEdit);

        label_8 = new QLabel(verticalLayoutWidget_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_8);

        channelDescriptionEdit = new QLineEdit(verticalLayoutWidget_3);
        channelDescriptionEdit->setObjectName(QString::fromUtf8("channelDescriptionEdit"));

        verticalLayout_3->addWidget(channelDescriptionEdit);

        label_9 = new QLabel(verticalLayoutWidget_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_9);

        channelUrlEdit = new QLineEdit(verticalLayoutWidget_3);
        channelUrlEdit->setObjectName(QString::fromUtf8("channelUrlEdit"));

        verticalLayout_3->addWidget(channelUrlEdit);

        addChannelButton = new QPushButton(verticalLayoutWidget_3);
        addChannelButton->setObjectName(QString::fromUtf8("addChannelButton"));

        verticalLayout_3->addWidget(addChannelButton);

        tabWidget->addTab(channelsTab, QString());
        writeTagTab = new QWidget();
        writeTagTab->setObjectName(QString::fromUtf8("writeTagTab"));
        verticalLayoutWidget_6 = new QWidget(writeTagTab);
        verticalLayoutWidget_6->setObjectName(QString::fromUtf8("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(0, 0, 291, 411));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_15 = new QLabel(verticalLayoutWidget_6);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_15);

        subscribedListWidget = new QListWidget(verticalLayoutWidget_6);
        subscribedListWidget->setObjectName(QString::fromUtf8("subscribedListWidget"));

        verticalLayout_6->addWidget(subscribedListWidget);

        label_12 = new QLabel(verticalLayoutWidget_6);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_12);

        writeTagNameEdit = new QLineEdit(verticalLayoutWidget_6);
        writeTagNameEdit->setObjectName(QString::fromUtf8("writeTagNameEdit"));

        verticalLayout_6->addWidget(writeTagNameEdit);

        label_13 = new QLabel(verticalLayoutWidget_6);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_13);

        writeTagLatEdit = new QLineEdit(verticalLayoutWidget_6);
        writeTagLatEdit->setObjectName(QString::fromUtf8("writeTagLatEdit"));

        verticalLayout_6->addWidget(writeTagLatEdit);

        label_14 = new QLabel(verticalLayoutWidget_6);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_14);

        writeTagLonEdit = new QLineEdit(verticalLayoutWidget_6);
        writeTagLonEdit->setObjectName(QString::fromUtf8("writeTagLonEdit"));

        verticalLayout_6->addWidget(writeTagLonEdit);

        writeTagButton = new QPushButton(verticalLayoutWidget_6);
        writeTagButton->setObjectName(QString::fromUtf8("writeTagButton"));

        verticalLayout_6->addWidget(writeTagButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_3);

        tabWidget->addTab(writeTagTab, QString());
        settingsTab = new QWidget();
        settingsTab->setObjectName(QString::fromUtf8("settingsTab"));
        verticalLayoutWidget_5 = new QWidget(settingsTab);
        verticalLayoutWidget_5->setObjectName(QString::fromUtf8("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(0, 0, 291, 261));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_5);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(verticalLayoutWidget_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_10);

        serverEdit = new QLineEdit(verticalLayoutWidget_5);
        serverEdit->setObjectName(QString::fromUtf8("serverEdit"));

        verticalLayout_5->addWidget(serverEdit);

        label_11 = new QLabel(verticalLayoutWidget_5);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_11);

        portEdit = new QLineEdit(verticalLayoutWidget_5);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));

        verticalLayout_5->addWidget(portEdit);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        tabWidget->addTab(settingsTab, QString());

        horizontalLayout->addWidget(tabWidget);


        verticalLayout->addLayout(horizontalLayout);

        statusLog = new QListWidget(verticalLayoutWidget);
        statusLog->setObjectName(QString::fromUtf8("statusLog"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(statusLog->sizePolicy().hasHeightForWidth());
        statusLog->setSizePolicy(sizePolicy1);
        statusLog->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(statusLog);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Login", 0, QApplication::UnicodeUTF8));
        loginEdit->setText(QApplication::translate("MainWindow", "Paul", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Pasword", 0, QApplication::UnicodeUTF8));
        passwordEdit->setText(QApplication::translate("MainWindow", "test", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Email", 0, QApplication::UnicodeUTF8));
        registerCheckbox->setText(QApplication::translate("MainWindow", "Register new user", 0, QApplication::UnicodeUTF8));
        userActionButton->setText(QApplication::translate("MainWindow", "Login", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(usersTab), QApplication::translate("MainWindow", "Users", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Radius", 0, QApplication::UnicodeUTF8));
        radiusEdit->setText(QApplication::translate("MainWindow", "99999999", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Latitude (press mouse wheel on map)", 0, QApplication::UnicodeUTF8));
        latitudeEdit->setText(QApplication::translate("MainWindow", "30.0", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Longitude (press mouse wheel on map)", 0, QApplication::UnicodeUTF8));
        longitudeEdit->setText(QApplication::translate("MainWindow", "60.0", 0, QApplication::UnicodeUTF8));
        tagActionButton->setText(QApplication::translate("MainWindow", "Filter!", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tagsTab), QApplication::translate("MainWindow", "Tags", 0, QApplication::UnicodeUTF8));
        channelActionButton->setText(QApplication::translate("MainWindow", "Subscribe/Unsubscribe", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Channel name", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Description", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Url", 0, QApplication::UnicodeUTF8));
        addChannelButton->setText(QApplication::translate("MainWindow", "Add channel", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(channelsTab), QApplication::translate("MainWindow", "Channels", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "Select channel below:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "Latatiude (press mouse wheel on map)", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Longitude (press mouse wheel on map)", 0, QApplication::UnicodeUTF8));
        writeTagButton->setText(QApplication::translate("MainWindow", "WriteTag", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(writeTagTab), QApplication::translate("MainWindow", "WriteTag", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "Server", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Port", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(settingsTab), QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
