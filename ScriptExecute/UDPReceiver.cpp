#pragma execution_character_set("utf-8")
#include "UDPReceiver.h"
#include <QNetworkDatagram>
//----------------------------------------------------------
UDPReceiver::UDPReceiver(QObject* parent)
    : QObject(parent)
    , socket(new QUdpSocket(this))
    , currentPort(0)
{
    //при получении данных -> слот onReadyRead
    connect(socket, &QUdpSocket::readyRead, this, &UDPReceiver::onReadyRead);
}
//----------------------------------------------------------
bool UDPReceiver::startListening(quint16 port)
{
    // Если уже слушаем, останавливаем
    if (isListening()) {
        socket->close();
        emit listeningStopped();
        emit logMessage("Остановлено предыдущее прослушивание");
    }

    // Пытаемся привязать сокет к порту
    if (!socket->bind(QHostAddress::Any, port)) {  // "0.0.0.0"
        QString error = QString("ошибка привязки порта %1: %2")
            .arg(port)
            .arg(socket->errorString());
        emit errorOccurred(error);
        emit logMessage("ERROR: " + error);
        return false;
    }
    currentPort = port;
    //emit logMessage(QString("Начато прослушивание порта %1").arg(port));
    emit listeningStarted(port);
    return true;
}
//----------------------------------------------------------
void UDPReceiver::stopListening()
{
    if (isListening()) {
        socket->close();
        emit logMessage(QString("Прослушивание порта %1 остановлено").arg(currentPort));
        emit listeningStopped();
        currentPort = 0;
    }
    else {
        emit logMessage("Сокет не был активен");
    }
}
//----------------------------------------------------------
void UDPReceiver::onReadyRead()
{
    // чтение всех доступных дейтаграмм
    while (socket->hasPendingDatagrams()) {
        // version QT > 5.8 - это совеременно
        QNetworkDatagram datagram = socket->receiveDatagram();

        if (datagram.isValid()) {
            // байты в строку
            QString script = QString::fromUtf8(datagram.data());

            // получаем информацию об отправителе
            QString senderAddress = datagram.senderAddress().toString();
            quint16 senderPort = datagram.senderPort();

            QString logMsg = QString("Получен скрипт от %1:%2 (%3 байт)")
                .arg(senderAddress)
                .arg(senderPort)
                .arg(datagram.data().size());

            emit logMessage(logMsg);
            emit scriptReceived(script);
        }
    }
}
//----------------------------------------------------------