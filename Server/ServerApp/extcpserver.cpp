#include "extcpserver.h"
#include "ui_extcpserver.h"


ExTcpServer::ExTcpServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExTcpServer)
{
    ui->setupUi(this);

    m_labListen = new QLabel("Монитор состояния:  ");
    m_labSocket = new QLabel("Статус подключения: ");
    m_labListen->setMidLineWidth(200);
    m_labSocket->setMinimumWidth(200);
    ui->statusBar->addWidget(m_labListen);
    ui->statusBar->addWidget(m_labSocket);

    QString localeIp = getLocalIp();
    setWindowTitle(windowTitle() + "--- IP address:" + localeIp);
    ui->comboBox->addItem(localeIp);

    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

}

ExTcpServer::~ExTcpServer()
{
    delete ui;
}


QString ExTcpServer::getLocalIp()
{
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    ui->plainTextEdit->appendPlainText("Computer name:" + hostName);
    QString locaIp;

    QList<QHostAddress> list = hostInfo.addresses();

    if (list.empty())
        return "null QString";

    foreach (QHostAddress addr, list) {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol) {
            locaIp = addr.toString();
            break;
        }
    }

    return locaIp;
}

void ExTcpServer::closeEvent(QCloseEvent *event)   // Закрыть окно остановки прослушивания
{
    if (m_tcpServer->isListening())
        m_tcpServer->close();

    event->accept();
}

void ExTcpServer::on_actStart_triggered()
{
    QString Ip = ui->comboBox->currentText();
    quint16 port = ui->spinBox->value();

    QHostAddress addr(Ip);
    m_tcpServer->listen(addr, port);  // Монитор указанный IP и указанный порт
    ui->plainTextEdit->appendPlainText("Адрес сервера:" + m_tcpServer->serverAddress().toString() +  " Порт сервера: " + QString::number(m_tcpServer->serverPort()));
    ui->plainTextEdit->appendPlainText("Начало прослушивания ...");

    ui->actStart->setEnabled(false);
    ui->actStop->setEnabled(true);
    m_labListen->setText("Монитор состояния: прослушивание ...");
}


void ExTcpServer::on_actStop_triggered()
{
    if (!m_tcpServer->isListening())
        return;

    m_tcpServer->close();  // Остановить прослушивание

    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);
    m_labListen->setText("Монитор состояния: прослушивание остановлено");
}

void ExTcpServer::on_actClear_triggered()
{
    ui->plainTextEdit->clear();
}

void ExTcpServer::on_actQuit_triggered()
{
    close();
}

void ExTcpServer::on_btnSend_clicked()
{
    QString msg = ui->lineEdit->text();
    ui->plainTextEdit->appendPlainText("[Сервер:]" + msg);
    ui->lineEdit->clear();
    ui->plainTextEdit->hasFocus();

    QByteArray str = msg.toUtf8();
    str.append('\n');
    m_tcpSocket->write(str);
}

void ExTcpServer::onSocketReadyRead()     // Читаем текстовый буфер строки
{
    while (m_tcpSocket->canReadLine()) {
        ui->plainTextEdit->appendPlainText("[Клиент:]" + m_tcpSocket->readLine());
    }
}

void ExTcpServer::onClientConnected()    // Когда клиент подключен
{
    ui->plainTextEdit->appendPlainText("Соединился с Клиентом. Адрес: " + m_tcpSocket->peerAddress().toString()
                                       + " Порт: " +  QString::number(m_tcpSocket->peerPort()));

}


void ExTcpServer::onClientDisonnected()  // Когда клиент отключается
{
    ui->plainTextEdit->appendPlainText("Клиент отключен");
    m_tcpSocket->deleteLater();
}

void ExTcpServer::onNewConnection()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection();   // Создание сокета

    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(onClientConnected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onClientDisonnected()));
    connect(m_tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(m_tcpSocket->state());
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void ExTcpServer::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch (socketState) {
    case QAbstractSocket::UnconnectedState:
        m_labSocket->setText("Статус подключения: UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        m_labSocket->setText("Статус подключения: HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        m_labSocket->setText("Статус подключения: ConnectingState");
        break;
    case QAbstractSocket::ConnectedState:
        m_labSocket->setText("Статус подключения: ConnectedState");
        break;
    case QAbstractSocket::BoundState:
        m_labSocket->setText("Статус подключения: BoundState");
        break;
    case QAbstractSocket::ClosingState:
        m_labSocket->setText("Статус подключения: ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        m_labSocket->setText("Статус подключения: ListeningState");
        break;
    default:
        m_labSocket->setText("Статус подключения: Другое Неизвестное состояние ...");
        break;
    }
}
