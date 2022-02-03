#ifndef EXTCPCLIENT_H
#define EXTCPCLIENT_H

//#include <QDialog>

#include <QMainWindow>
#include <QLabel>
#include <QTcpSocket>
#include <QHostInfo>

namespace Ui {
class ExTcpClient;
}

class ExTcpClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExTcpClient(QWidget *parent = nullptr);
    ~ExTcpClient();

private:
    QString getLocalIp();                  // Получить выше IP машины

    protected:
        void closeEvent(QCloseEvent *event);

private slots:
    // щ определенная функция канавки
    void on_actConnect_triggered();     // Запрос на подключение к серверу
    void on_actDisconnect_triggered();  // Отключение с сервером
    void on_actClear_triggered();       // содержимое Очистить
    void on_actQuit_triggered();        // выйти из программы
    void on_btnSend_clicked();          // Отправить текстовое сообщение

    // Функция паза Индивидуальной
    void onConnected();
    void onDisconnected();
    void onSocketReadyRead();           // Прочитать входящие данные из сокета
    void onSocketStateChange(QAbstractSocket::SocketState socketState);


private:
    Ui::ExTcpClient *ui;

    QLabel* m_labSocket;
    QTcpSocket* m_tcpSocket;
};

#endif // EXTCPCLIENT_H
