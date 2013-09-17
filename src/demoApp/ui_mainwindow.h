/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Aug 26 23:50:37 2013
**      by: Qt User Interface Compiler version 4.7.4
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
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;
    QTabWidget *tabWidget;
    QWidget *usersTab;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *loginEdit;
    QLabel *label_2;
    QLineEdit *passwordEdit;
    QLabel *label_3;
    QLineEdit *emailEdit;
    QCheckBox *registerCheckbox;
    QPushButton *userActionButton;
    QSpacerItem *verticalSpacer_4;
    QWidget *tagsTab;
    QVBoxLayout *verticalLayout_9;
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
    QVBoxLayout *verticalLayout_10;
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
    QVBoxLayout *verticalLayout_11;
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
    QVBoxLayout *verticalLayout_12;
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
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_7 = new QVBoxLayout(centralwidget);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy1);
        graphicsView->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(graphicsView);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setMinimumSize(QSize(350, 0));
        tabWidget->setMaximumSize(QSize(350, 16777215));
        usersTab = new QWidget();
        usersTab->setObjectName(QString::fromUtf8("usersTab"));
        sizePolicy1.setHeightForWidth(usersTab->sizePolicy().hasHeightForWidth());
        usersTab->setSizePolicy(sizePolicy1);
        verticalLayout_8 = new QVBoxLayout(usersTab);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        label = new QLabel(usersTab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        loginEdit = new QLineEdit(usersTab);
        loginEdit->setObjectName(QString::fromUtf8("loginEdit"));

        verticalLayout_2->addWidget(loginEdit);

        label_2 = new QLabel(usersTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        passwordEdit = new QLineEdit(usersTab);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));

        verticalLayout_2->addWidget(passwordEdit);

        label_3 = new QLabel(usersTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_3);

        emailEdit = new QLineEdit(usersTab);
        emailEdit->setObjectName(QString::fromUtf8("emailEdit"));
        emailEdit->setEnabled(false);

        verticalLayout_2->addWidget(emailEdit);

        registerCheckbox = new QCheckBox(usersTab);
        registerCheckbox->setObjectName(QString::fromUtf8("registerCheckbox"));

        verticalLayout_2->addWidget(registerCheckbox);

        userActionButton = new QPushButton(usersTab);
        userActionButton->setObjectName(QString::fromUtf8("userActionButton"));

        verticalLayout_2->addWidget(userActionButton);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);


        verticalLayout_8->addLayout(verticalLayout_2);

        tabWidget->addTab(usersTab, QString());
        tagsTab = new QWidget();
        tagsTab->setObjectName(QString::fromUtf8("tagsTab"));
        tagsTab->setEnabled(true);
        verticalLayout_9 = new QVBoxLayout(tagsTab);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_6 = new QLabel(tagsTab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_6);

        radiusEdit = new QLineEdit(tagsTab);
        radiusEdit->setObjectName(QString::fromUtf8("radiusEdit"));

        verticalLayout_4->addWidget(radiusEdit);

        label_5 = new QLabel(tagsTab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_5);

        latitudeEdit = new QLineEdit(tagsTab);
        latitudeEdit->setObjectName(QString::fromUtf8("latitudeEdit"));

        verticalLayout_4->addWidget(latitudeEdit);

        label_4 = new QLabel(tagsTab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_4);

        longitudeEdit = new QLineEdit(tagsTab);
        longitudeEdit->setObjectName(QString::fromUtf8("longitudeEdit"));

        verticalLayout_4->addWidget(longitudeEdit);

        tagActionButton = new QPushButton(tagsTab);
        tagActionButton->setObjectName(QString::fromUtf8("tagActionButton"));

        verticalLayout_4->addWidget(tagActionButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);


        verticalLayout_9->addLayout(verticalLayout_4);

        tabWidget->addTab(tagsTab, QString());
        channelsTab = new QWidget();
        channelsTab->setObjectName(QString::fromUtf8("channelsTab"));
        channelsTab->setEnabled(true);
        verticalLayout_10 = new QVBoxLayout(channelsTab);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        channelsListWidget = new QListWidget(channelsTab);
        channelsListWidget->setObjectName(QString::fromUtf8("channelsListWidget"));

        verticalLayout_3->addWidget(channelsListWidget);

        channelActionButton = new QPushButton(channelsTab);
        channelActionButton->setObjectName(QString::fromUtf8("channelActionButton"));
        channelActionButton->setEnabled(true);

        verticalLayout_3->addWidget(channelActionButton);

        line = new QFrame(channelsTab);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        label_7 = new QLabel(channelsTab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_7);

        channelNameEdit = new QLineEdit(channelsTab);
        channelNameEdit->setObjectName(QString::fromUtf8("channelNameEdit"));

        verticalLayout_3->addWidget(channelNameEdit);

        label_8 = new QLabel(channelsTab);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_8);

        channelDescriptionEdit = new QLineEdit(channelsTab);
        channelDescriptionEdit->setObjectName(QString::fromUtf8("channelDescriptionEdit"));

        verticalLayout_3->addWidget(channelDescriptionEdit);

        label_9 = new QLabel(channelsTab);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_9);

        channelUrlEdit = new QLineEdit(channelsTab);
        channelUrlEdit->setObjectName(QString::fromUtf8("channelUrlEdit"));

        verticalLayout_3->addWidget(channelUrlEdit);

        addChannelButton = new QPushButton(channelsTab);
        addChannelButton->setObjectName(QString::fromUtf8("addChannelButton"));

        verticalLayout_3->addWidget(addChannelButton);


        verticalLayout_10->addLayout(verticalLayout_3);

        tabWidget->addTab(channelsTab, QString());
        writeTagTab = new QWidget();
        writeTagTab->setObjectName(QString::fromUtf8("writeTagTab"));
        verticalLayout_11 = new QVBoxLayout(writeTagTab);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_15 = new QLabel(writeTagTab);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_15);

        subscribedListWidget = new QListWidget(writeTagTab);
        subscribedListWidget->setObjectName(QString::fromUtf8("subscribedListWidget"));

        verticalLayout_6->addWidget(subscribedListWidget);

        label_12 = new QLabel(writeTagTab);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_12);

        writeTagNameEdit = new QLineEdit(writeTagTab);
        writeTagNameEdit->setObjectName(QString::fromUtf8("writeTagNameEdit"));

        verticalLayout_6->addWidget(writeTagNameEdit);

        label_13 = new QLabel(writeTagTab);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_13);

        writeTagLatEdit = new QLineEdit(writeTagTab);
        writeTagLatEdit->setObjectName(QString::fromUtf8("writeTagLatEdit"));

        verticalLayout_6->addWidget(writeTagLatEdit);

        label_14 = new QLabel(writeTagTab);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_14);

        writeTagLonEdit = new QLineEdit(writeTagTab);
        writeTagLonEdit->setObjectName(QString::fromUtf8("writeTagLonEdit"));

        verticalLayout_6->addWidget(writeTagLonEdit);

        writeTagButton = new QPushButton(writeTagTab);
        writeTagButton->setObjectName(QString::fromUtf8("writeTagButton"));

        verticalLayout_6->addWidget(writeTagButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_3);


        verticalLayout_11->addLayout(verticalLayout_6);

        tabWidget->addTab(writeTagTab, QString());
        settingsTab = new QWidget();
        settingsTab->setObjectName(QString::fromUtf8("settingsTab"));
        verticalLayout_12 = new QVBoxLayout(settingsTab);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_10 = new QLabel(settingsTab);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_10);

        serverEdit = new QLineEdit(settingsTab);
        serverEdit->setObjectName(QString::fromUtf8("serverEdit"));

        verticalLayout_5->addWidget(serverEdit);

        label_11 = new QLabel(settingsTab);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_11);

        portEdit = new QLineEdit(settingsTab);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));

        verticalLayout_5->addWidget(portEdit);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        verticalLayout_12->addLayout(verticalLayout_5);

        tabWidget->addTab(settingsTab, QString());

        horizontalLayout->addWidget(tabWidget);


        verticalLayout->addLayout(horizontalLayout);

        statusLog = new QListWidget(centralwidget);
        statusLog->setObjectName(QString::fromUtf8("statusLog"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(statusLog->sizePolicy().hasHeightForWidth());
        statusLog->setSizePolicy(sizePolicy2);
        statusLog->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(statusLog);


        verticalLayout_7->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


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
        radiusEdit->setText(QApplication::translate("MainWindow", "9999", 0, QApplication::UnicodeUTF8));
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
