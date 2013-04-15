#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMenu>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGraphicsView>
#include "MapScene.h"

#include "DefaultQuery.h"
#include "LoginQuery.h"
#include "AddUserQuery.h"
#include "AvailableChannelsQuery.h"
#include "SubscribedChannelsQuery.h"
#include "SubscribeChannelQuery.h"
#include "UnsubscribeChannelQuery.h"
#include "LoadTagsQuery.h"
#include "WriteTagQuery.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;

    MapScene * m_map;
    QString login;

    LoginQuery * m_loginQuery;
    AddUserQuery * m_addUserQuery;
    AvailableChannelsQuery * m_availableChannelsQuery;
    SubscribedChannelsQuery * m_subscribedChannelsQuery;
    SubscribeChannelQuery * m_subscribeChannelQuery;
    UnsubscribeChannelQuery * m_unsubscribeChannelQuery;
    LoadTagsQuery * m_loadTagsQuery;
    WriteTagQuery * m_writeTagQuery;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onRegisterCheckboxChanged(int state);
    void onUserActionButtonPressed();

    void onLoginSuccess();
    void onRequestError(int errno);

    
protected:
    void showEvent(QShowEvent * e);

private:
    void initQueries();

    void writeToStatusLog(const QString & text);
};

#endif // MAINWINDOW_H
