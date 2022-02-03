#ifndef EXTCPSERVER_H
#define EXTCPSERVER_H

#include <QStatusBar>

//#include <QDialog>
#include <QMainWindow>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>

namespace Ui {
class ExTcpServer;
}

class ExTcpServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExTcpServer(QWidget *parent = nullptr);
    ~ExTcpServer();

private:
    QString getLocalIp();      // Получить родной IP

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    // Пользовательский интерфейс слот функции
    void on_actStart_triggered();      // Начало прослушивания
    void on_actStop_triggered();       // Остановить прослушивание
    void on_actClear_triggered();      // Очистить содержимое текстового поля
    void on_actQuit_triggered();       // выйти из программы
    void on_btnSend_clicked();         // Отправляется

    // Функция паза Индивидуальной
    void onSocketReadyRead();          // Читаем данные в случае сокет
    void onClientConnected();          //client socket conneted
    void onClientDisonnected();        //client socket disconneted
    void onNewConnection();            // NewConnect () сигнал для QTcpServer
    void onSocketStateChange(QAbstractSocket::SocketState socketState);

private:
    Ui::ExTcpServer *ui;

    QLabel* m_labListen;
    QLabel* m_labSocket;
    QTcpServer* m_tcpServer;
    QTcpSocket* m_tcpSocket;
};

#endif // EXTCPSERVER_H
