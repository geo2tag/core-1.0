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

    connect(m_map, SIGNAL(mapDoubleClick(QGraphicsSceneMouseEvent *)),
                          this, SLOT(onMapDoubleClick(QGraphicsSceneMouseEvent*)));
    connect(m_map, SIGNAL(mapMiddleButtonPressed(QGraphicsSceneMouseEvent *)),
                          this, SLOT(onMapMiddleButtonPressed(QGraphicsSceneMouseEvent*)));

    ui->graphicsView->setScene(m_map);

    m_map->setCenter(60.,30.);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    initQueries();



    connect(ui->registerCheckbox,SIGNAL(stateChanged (int)),this, SLOT(onRegisterCheckboxChanged(int)));
    connect(ui->userActionButton,SIGNAL(pressed()),this,SLOT(onUserActionButtonPressed()));
    connect(ui->tagActionButton,SIGNAL(pressed()),this,SLOT(onTagActionButtonPressed()));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    connect(ui->channelActionButton, SIGNAL(pressed()), this, SLOT(onChannelButtonPressed()));
    connect(ui->channelsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onChannelsListChanged(int)));

    connect(ui->addChannelButton, SIGNAL(pressed()), this, SLOT(onAddChannelButtonPressed()));

    connect(ui->serverEdit,SIGNAL(textChanged(QString)), this, SLOT(onSettingsChanged()));
    connect(ui->portEdit, SIGNAL(textChanged(QString)), this, SLOT(onSettingsChanged()));

    connect(ui->writeTagButton, SIGNAL(pressed()), this ,SLOT(onWriteTagButtonPressed()));
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
    connect(m_loadTagsQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));


    connect(m_availableChannelsQuery,SIGNAL(success()),this, SLOT(onAvailableChannelsSuccess()));
    connect(m_availableChannelsQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));

    connect(m_subscribedChannelsQuery,SIGNAL(success()),this, SLOT(onSubscribedChannelsSuccess()));
    connect(m_subscribedChannelsQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));

    connect(m_subscribeChannelQuery,SIGNAL(success()),this, SLOT(onChannelActionSuccess()));
    connect(m_subscribeChannelQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));

    connect(m_unsubscribeChannelQuery,SIGNAL(success()),this, SLOT(onChannelActionSuccess()));
    connect(m_unsubscribeChannelQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));

    connect(m_applyChannelQuery,SIGNAL(success()),this, SLOT(onApplyChannelSuccess()));
    connect(m_applyChannelQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));

    connect(m_writeTagQuery, SIGNAL(success()), this, SLOT(onWriteTagSuccess()));
    connect(m_writeTagQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));
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

void MainWindow::onTabChanged(int index){
    qDebug() << index;
    // If channel tab is visible perform AvailableChannels and SubscribedChannels requests
    if (index == 2 && m_session.isValid()){
        refreshChannelsWidget();

    }else if( index == 3 ){
        //Write tag tab
        refreshChannelsWidget();
    }else if( index == 4 ){
        //settings tab
        QString serverAddr = SettingsStorage::getValue("common/server_url", QVariant(DEFAULT_SERVER)).toString();
        QString serverPort = SettingsStorage::getValue("common/server_port", QVariant(DEFAULT_PORT)).toString();

        ui->serverEdit->setText(serverAddr);
        ui->portEdit->setText(serverPort);


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

void MainWindow::onSettingsChanged(){

    SettingsStorage::setValue("server_url",QVariant(ui->serverEdit->text()),"common");
    SettingsStorage::setValue("server_port",QVariant(ui->portEdit->text()),"common");


}

void MainWindow::onWriteTagButtonPressed()
{
    int index = ui->subscribedListWidget->currentRow();
    if (m_session.isValid() && index>=0)
    {
        Channel channel = m_subscribedChannelsQuery->getChannels().at(index);

        qreal lat = ui->writeTagLatEdit->text().toDouble();
        qreal lon = ui->writeTagLonEdit->text().toDouble();
        QString name = ui->writeTagNameEdit->text();

        m_writeTagQuery->setChannel(channel);
        m_writeTagQuery->setSession(m_session);
        m_writeTagQuery->setTag(Tag(0.0, lat, lon, name));
        m_writeTagQuery->doRequest();
    }
}

GeoPoint MainWindow::getCoordinatesByMouseEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF cur_pos = event->scenePos();
    cur_pos.setX(cur_pos.x()/256);
    cur_pos.setY(cur_pos.y()/256);

    GeoPoint geo_coord =
            OSMCoordinatesConverter::TileToGeo(qMakePair(cur_pos, m_map->getZoom()));
    return geo_coord;
}


void MainWindow::onMapMiddleButtonPressed(QGraphicsSceneMouseEvent* event)
{
    if (m_session.isValid()){

        GeoPoint geo_coord = getCoordinatesByMouseEvent(event);

        qDebug() << "Middle button pressed position " <<  geo_coord.first << geo_coord.second;

        ui->latitudeEdit->setText(QString::number(geo_coord.first));
        ui->longitudeEdit->setText(QString::number(geo_coord.second));

        ui->writeTagLatEdit->setText(QString::number(geo_coord.first));
        ui->writeTagLonEdit->setText(QString::number(geo_coord.second));
    }
}


void MainWindow::onMapDoubleClick(QGraphicsSceneMouseEvent* event)
{
    if (m_session.isValid()){

        GeoPoint geo_coord = getCoordinatesByMouseEvent(event);

        qDebug() << "Double clicked position " << geo_coord.first << geo_coord.second;



       // m_map->addMark(geo_coord.second,geo_coord.first,QVariant("kkkkk"));
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
    writeToStatusLog("Login succeed, auth_token = "+m_session.getSessionToken());

}

void MainWindow::onLoadTagsSuccess()
{
    qDebug() << "Recieved tags!";
    writeToStatusLog("LoadTags succeed, returned amount of tags = "+
                    QString::number(m_loadTagsQuery->getData().size()) );
    // TODO draw tags
    m_map->setMarks(m_loadTagsQuery->getData());

}


void MainWindow::onAvailableChannelsSuccess(){
    qDebug() <<"MainWindow::onAvailableChannelsSuccess()";
    writeToStatusLog("AvailableChannels request succeed, returned amount of channels = "+
                     QString::number(m_availableChannelsQuery->getChannels().size()));

    getSubscribedChannels();
}

void MainWindow::onSubscribedChannelsSuccess(){
    qDebug()<<"MainWindow::onSubscribedChannelsSuccess()";
    writeToStatusLog("SubscribedChannels request succeed, returned amount of channels = "+
                     QString::number(m_subscribedChannelsQuery->getChannels().size()));
    formChannelList();
}

void MainWindow::onWriteTagSuccess()
{
    writeToStatusLog("WriteTag request succeed");
}

void MainWindow::formChannelList(){
    QList<Channel> availableChannels = m_availableChannelsQuery->getChannels();
    QList<Channel> subscribedChannels = m_subscribedChannelsQuery->getChannels();
    QListWidget * listWidget = ui->channelsListWidget;
    QListWidget * subscribedListWidget = ui->subscribedListWidget;

    if (availableChannels.size() == 0 ) return;

    // Tab - channels
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

    if (subscribedChannels.size() == 0 ) return;

    //Tab - writeTag
    while(subscribedListWidget->count()>0)
    {
        subscribedListWidget->takeItem(0);
    }

    foreach (Channel s, subscribedChannels){
        subscribedListWidget->addItem(s.getName());
    }

}

void MainWindow::onRequestError(int errno)
{
    writeToStatusLog("Error processing "+
                     QString(this->sender()->metaObject()->className())+", errno = " +
                     QString::number(errno)+", type = " + Errno::initErrnoMap().value(errno));
    qDebug() << "Error "<< errno << " occured";
}

void MainWindow::writeToStatusLog(const QString & text)
{

    ui->statusLog->addItem(QDateTime::currentDateTime().toString()+" : "+text);

}

void MainWindow::getSubscribedChannels()
{
    m_subscribedChannelsQuery->setQuery(m_session);
    m_subscribedChannelsQuery->doRequest();
}
