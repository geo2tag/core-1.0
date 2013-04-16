#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShowEvent>
#include "SettingsStorage.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    SettingsStorage::init();

    ui->setupUi(this);

    m_map = new MapScene(this);
    ui->graphicsView->setScene(m_map);

    m_map->setCenter(60.,30.);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    initQueries();

    connect(ui->registerCheckbox,SIGNAL(stateChanged (int)),this, SLOT(onRegisterCheckboxChanged(int)));
    connect(ui->userActionButton,SIGNAL(pressed()),this,SLOT(onUserActionButtonPressed()));
    connect(ui->tagActionButton,SIGNAL(pressed()),this,SLOT(onTagActionButtonPressed()));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onChannelTabActivated(int)));
    connect(ui->channelActionButton, SIGNAL(pressed()), this, SLOT(onChannelButtonPressed()));
    connect(ui->channelsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onChannelsListChanged(int)));

    connect(ui->addChannelButton, SIGNAL(pressed()), this, SLOT(onAddChannelButtonPressed()));
}

void MainWindow::initQueries()
{
    m_loginQuery = new LoginQuery(this);
    m_addUserQuery = new AddUserQuery();
    m_availableChannelsQuery = new AvailableChannelsQuery();
    m_subscribedChannelsQuery = new SubscribedChannelsQuery();
    m_subscribeChannelQuery = new SubscribeChannelQuery();
    m_unsubscribeChannelQuery = new UnsubscribeChannelQuery();
    m_loadTagsQuery = new LoadTagsQuery();
    m_writeTagQuery = new WriteTagQuery();
    m_applyChannelQuery = new ApplyChannelQuery();


    connect(m_loginQuery,SIGNAL(success()),this, SLOT(onLoginSuccess()));
    connect(m_loginQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));
    connect(m_addUserQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));
    connect(m_loadTagsQuery,SIGNAL(success()),this, SLOT(onLoadTagsSuccess()));

    connect(m_availableChannelsQuery,SIGNAL(success()),this, SLOT(onAvailableChannelsSuccess()));
    connect(m_subscribedChannelsQuery,SIGNAL(success()),this, SLOT(onSubscribedChannelsSuccess()));

    connect(m_subscribeChannelQuery,SIGNAL(success()),this, SLOT(onChannelActionSuccess()));
    connect(m_unsubscribeChannelQuery,SIGNAL(success()),this, SLOT(onChannelActionSuccess()));

    connect(m_applyChannelQuery,SIGNAL(success()),this, SLOT(onApplyChannelSuccess()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showEvent(QShowEvent * e)
{
  if(e->type() == QShowEvent::Show)     // e must not be unused
    this->m_map->update_state();
}

void MainWindow::onRegisterCheckboxChanged(int state)
{
    if (ui->registerCheckbox->isChecked()){
        ui->userActionButton->setText("Register");
        ui->emailEdit->setEnabled(true);
    }else{
        ui->userActionButton->setText("Login");
        ui->emailEdit->setEnabled(false);
    }
}

void MainWindow::onUserActionButtonPressed(){
    QString login = ui->loginEdit->text();
    QString password = ui->passwordEdit->text();
    if (ui->registerCheckbox->isChecked()){

        QString email = ui->emailEdit->text();

        m_addUserQuery->setQuery(login, password, email);
        m_addUserQuery->doRequest();
    }else{
        m_loginQuery->setQuery(login, password);
        m_loginQuery->doRequest();
    }
}

void MainWindow::onTagActionButtonPressed()
{
    if (m_session.isValid())
    {
        double radius = ui->radiusEdit->text().toDouble();
        double latitude = ui->latitudeEdit->text().toDouble();
        double longitude = ui->longitudeEdit->text().toDouble();

        m_loadTagsQuery->setQuery(m_session,latitude, longitude, radius);
        m_loadTagsQuery->doRequest();
    }
}

void MainWindow::refreshChannelsWidget(){
    m_availableChannelsQuery->setQuery(m_session);
    m_availableChannelsQuery->doRequest();
}

void MainWindow::onChannelTabActivated(int index){
    qDebug() << index;
    // If channel tab is visible perform AvailableChannels and SubscribedChannels requests
    if (index == 2 && m_session.isValid()){
        refreshChannelsWidget();

    }
}

void MainWindow::onChannelsListChanged(int index){


    qDebug() << index;

    if (index<0) return ;

    QList<Channel> availableChannels = m_availableChannelsQuery->getChannels();
    QList<Channel> subscribedChannels = m_subscribedChannelsQuery->getChannels();
    bool isSubscribed = subscribedChannels.contains(availableChannels.at(index));
    if (isSubscribed){
        ui->channelActionButton->setText("Unsubscribe");
    }else{
        ui->channelActionButton->setText("Subscribe");
    }


}



void MainWindow::onChannelButtonPressed()
{
    if (m_session.isValid()){
        int index = ui->channelsListWidget->currentRow();
        if (index < 0) return;
        QList<Channel> availableChannels = m_availableChannelsQuery->getChannels();
        QList<Channel> subscribedChannels = m_subscribedChannelsQuery->getChannels();
        bool isSubscribed = subscribedChannels.contains(availableChannels.at(index));
        if (isSubscribed){
            //do unsubscribe
            m_unsubscribeChannelQuery->setQuery(availableChannels.at(index),m_session);
            m_unsubscribeChannelQuery->doRequest();
        }else{
            //do subscribe
            m_subscribeChannelQuery->setQuery(availableChannels.at(index),m_session);
            m_subscribeChannelQuery->doRequest();
        }
    }
}

void MainWindow::onAddChannelButtonPressed()
{
    if (m_session.isValid())
    {
        QString name = ui->channelNameEdit->text();
        QString description = ui->channelDescriptionEdit->text();
        QString url = ui->channelUrlEdit->text();

        m_applyChannelQuery->setQuery(Channel(name,description,url),m_session);
        m_applyChannelQuery->doRequest();
    }
}


void MainWindow::onChannelActionSuccess()
{
    qDebug() << "Channels action success";
    refreshChannelsWidget();
}

void MainWindow::onApplyChannelSuccess()
{
    if (m_session.isValid())
    {
        qDebug() << "Apply channel success";
        refreshChannelsWidget();
    }
}

void MainWindow::onLoginSuccess()
{
    qDebug() << "Recieved session token! " << m_loginQuery->getSession();
    m_session = m_loginQuery->getSession();

}

void MainWindow::onLoadTagsSuccess()
{
    qDebug() << "Recieved tags!";
    // TODO draw tags
    m_map->setMarks(m_loadTagsQuery->getData());

}


void MainWindow::onAvailableChannelsSuccess(){
    qDebug() <<"MainWindow::onAvailableChannelsSuccess()";
    m_subscribedChannelsQuery->setQuery(m_session);
    m_subscribedChannelsQuery->doRequest();
}

void MainWindow::onSubscribedChannelsSuccess(){
    qDebug()<<"MainWindow::onSubscribedChannelsSuccess()";
    formChannelList();
}

void MainWindow::formChannelList(){
    QList<Channel> availableChannels = m_availableChannelsQuery->getChannels();
    QList<Channel> subscribedChannels = m_subscribedChannelsQuery->getChannels();
    QListWidget * listWidget = ui->channelsListWidget;

    while(listWidget->count()>0)
    {
      listWidget->takeItem(0);
    }

    foreach (Channel s, availableChannels){
        QListWidgetItem * item = new QListWidgetItem(s.getName());
        if (subscribedChannels.contains(s)){
            item->setBackgroundColor(Qt::lightGray);
        }

        listWidget->addItem(item);
    }
}

void MainWindow::onRequestError(int errno)
{
    qDebug() << "Error "<< errno << " occured";
}

void MainWindow::writeToStatusLog(const QString & text)
{


}
