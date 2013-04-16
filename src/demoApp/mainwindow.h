#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMenu>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGraphicsView>
#include "MapScene.h"

#include "LoginQuery.h"
#include "AddUserQuery.h"
#include "AvailableChannelsQuery.h"
#include "SubscribedChannelsQuery.h"
#include "SubscribeChannelQuery.h"
#include "UnsubscribeChannelQuery.h"
#include "LoadTagsQuery.h"
#include "WriteTagQuery.h"
#include "ApplyChannelQuery.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;

    MapScene * m_map;
    Session m_session;

    LoginQuery * m_loginQuery;
    AddUserQuery * m_addUserQuery;
    AvailableChannelsQuery * m_availableChannelsQuery;
    SubscribedChannelsQuery * m_subscribedChannelsQuery;
    SubscribeChannelQuery * m_subscribeChannelQuery;
    UnsubscribeChannelQuery * m_unsubscribeChannelQuery;
    LoadTagsQuery * m_loadTagsQuery;
    WriteTagQuery * m_writeTagQuery;
    ApplyChannelQuery * m_applyChannelQuery;

    void formChannelList();
    void refreshChannelsWidget();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onRegisterCheckboxChanged(int state);
    void onUserActionButtonPressed();
    void onTagActionButtonPressed();
    void onChannelTabActivated(int index);
    void onChannelButtonPressed();
    void onChannelsListChanged(int index);
    void onAddChannelButtonPressed();

    void onLoginSuccess();
    void onLoadTagsSuccess();
    void onSubscribedChannelsSuccess();
    void onAvailableChannelsSuccess();
    void onChannelActionSuccess();
    void onApplyChannelSuccess();



    void onRequestError(int errno);

    
protected:
    void showEvent(QShowEvent * e);

private:
    void initQueries();

    void writeToStatusLog(const QString & text);
};

#endif // MAINWINDOW_H
