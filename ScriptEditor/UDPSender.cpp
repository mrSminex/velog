#pragma execution_character_set("utf-8")
#include "UDPSender.h"
#include <QNetworkDatagram>
#include <QHostAddress>
//-----------------------------------------------------------------
UDPSender::UDPSender(QObject* parent) : QObject(parent), socket(new QUdpSocket(this))
{

}

bool UDPSender::sendScript(const QString& script, const QString& address, quint16 port)
{
    // проверка входных данных
    if (script.isEmpty()) {
        emit errorOccurred("Скрипт пуст!");
        return false;
    }

    QByteArray data = script.toUtf8();  // текст преобразуем в байты
    emit logMessage(QString("DataSize: %1 byte").arg(data.size()));

    QHostAddress hostAddress;
    if (address.isEmpty() || address == "localhost") {
        hostAddress = QHostAddress::LocalHost;
        emit logMessage("ip = localhost(127.0.0.1)");
    }
    else {
        hostAddress = QHostAddress(address);
        if (hostAddress.isNull()) {
            QString error = QString("Неверный IP адрес: %1").arg(address);
            emit errorOccurred(error);
            emit logMessage("ОШИБКА: " + error);
            return false;
        }
    }
    // если всё ок
    emit logMessage(QString("Отправка на %1:%2").arg(hostAddress.toString()).arg(port));

    qint64 bytesSent = socket->writeDatagram(data, hostAddress, port);

    if (bytesSent == -1) {
        QString error = QString("Ошибка отправки: %1").arg(socket->errorString());
        emit errorOccurred(error);
        emit logMessage("ERROR: " + error);
        return false;
    }
    else if (bytesSent != data.size()) {
        QString error = QString("Отправлено %1 из %2 байт").arg(bytesSent).arg(data.size());
        emit errorOccurred(error);
        emit logMessage("ERROR: " + error);
        return false;
    }
    emit scriptSent();
    emit logMessage(QString("Успешно отправлено %1 байт").arg(bytesSent));

    return true;
}
//-----------------------------------------------------------------
