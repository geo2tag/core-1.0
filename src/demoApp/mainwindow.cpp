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

}

void MainWindow::initQueries()
{
    m_loginQuery = new LoginQuery();
    m_addUserQuery = new AddUserQuery();
    m_availableChannelsQuery = new AvailableChannelsQuery();
    m_subscribedChannelsQuery = new SubscribedChannelsQuery();
    m_subscribeChannelQuery = new SubscribeChannelQuery();
    m_unsubscribeChannelQuery = new UnsubscribeChannelQuery();
    m_loadTagsQuery = new LoadTagsQuery();
    m_writeTagQuery = new WriteTagQuery();


    connect(m_loginQuery,SIGNAL(success()),this, SLOT(onLoginSuccess()));
    connect(m_loginQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));
    connect(m_addUserQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));
    connect(m_loadTagsQuery,SIGNAL(success()),this, SLOT(onLoadTagsSuccess()));

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


void MainWindow::onRequestError(int errno)
{
    qDebug() << "Error "<< errno << " occured";
}

void MainWindow::writeToStatusLog(const QString & text)
{


}
