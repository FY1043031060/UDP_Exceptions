#ifndef WIDGET_H
#define WIDGET_H

#include "ui_Widget.h"
#include <QUdpSocket>
#include <QQueue>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    int send(QByteArray& sendData);
    int recv(QByteArray& recvData);
public Q_SLOTS:
    void readPendingDatagrams();

private:
    Ui::Widget ui;
    QUdpSocket* m_sendSocket;
    QUdpSocket* m_recvSocket;
    QQueue<QByteArray> m_queue;
};

#endif // WIDGET_H
