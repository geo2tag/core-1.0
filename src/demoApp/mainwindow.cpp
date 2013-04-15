#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShowEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_map = new MapScene(this);
    ui->graphicsView->setScene(m_map);

    m_map->setCenter(60.,30.);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->registerCheckbox,SIGNAL(stateChanged (int)),this, SLOT(onRegisterCheckboxChanged(int)));
    connect(ui->userActionButton,SIGNAL(pressed()),this,SLOT(onUserActionButtonPressed()));

    initQueries();
}

void MainWindow::initQueries()
{
    m_loginQuery = new LoginQuery(parent());
//    m_addUserQuery = new AddUserQuery();

//    connect(m_loginQuery,SIGNAL(success()),this, SLOT(onLoginSuccess()));
//    connect(m_loginQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));
//    connect(m_addUserQuery,SIGNAL(errorOccured(int)),this, SLOT(onRequestError(int)));

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
    }else{
        m_loginQuery->setQuery(login, password);
    }
}


void MainWindow::onLoginSuccess()
{
    qDebug() << "Recieved session token! " << m_loginQuery->getSession();

}

void MainWindow::onRequestError(int errno)
{
    qDebug() << "Error "<< errno << " occured";
}

void MainWindow::writeToStatusLog(const QString & text)
{


}
