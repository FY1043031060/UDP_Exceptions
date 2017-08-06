#include "Widget.h"
#include <QTimer>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    m_sendSocket = new QUdpSocket(this);
    m_recvSocket = new QUdpSocket(this);

    m_recvSocket->bind(QHostAddress::LocalHost, 9999);

    connect(m_recvSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));


    connect(ui.lineEditSend,&QLineEdit::textChanged,[&](const QString & strContent){
        this->send(strContent.toLatin1());
    });
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[&](){
        QByteArray temp;
        recv(temp);
        if(!temp.isEmpty())
        ui.lineEditRecv->setText(QString::fromLatin1(temp));
    });
    timer->start(20);
}

int Widget::send(QByteArray &sendData)
{
    m_sendSocket->writeDatagram(sendData,QHostAddress("127.0.0.1"),9999);
    return 0;
}

int Widget::recv(QByteArray &recvData)
{
    if(!m_queue.isEmpty())
        recvData = m_queue.dequeue();
    return 0;
}

void Widget::readPendingDatagrams()
{
    while (m_recvSocket->hasPendingDatagrams()) {
           QByteArray datagram;
           datagram.resize(m_recvSocket->pendingDatagramSize());
           QHostAddress sender;
           quint16 senderPort;

           m_recvSocket->readDatagram(datagram.data(), datagram.size(),
                                   &sender, &senderPort);

           m_queue.enqueue(datagram);
       }
}
