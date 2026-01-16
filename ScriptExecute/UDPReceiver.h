#pragma execution_character_set("utf-8")
#pragma once
#ifndef UDPRECEIVE_H
#define UDPRECEIVE_H
//----------------------------------------------------------
#include <QObject>
#include <QUdpSocket>
#include <QString>
//----------------------------------------------------------
class UDPReceiver : public QObject
{
    Q_OBJECT
public:
    explicit    UDPReceiver(QObject* parent = nullptr);

    bool        startListening(quint16 port);
    void        stopListening();

    bool        isListening()    const { return socket->state() == QUdpSocket::BoundState; }

    quint16     getCurrentport() const { return currentPort; }

signals:
    // cигнал о получении нового скрипта
    void        scriptReceived(const QString& script);

    // cигналы статуса
    void        listeningStarted(quint16 port);
    void        listeningStopped();
    void        errorOccurred(const QString& error);
    void        logMessage(const QString& message);

private slots:
    // cлот для обработки входящих данных
    void        onReadyRead();

private:
    QUdpSocket* socket;
    quint16     currentPort;
};

#endif // UDPRECEIVE_H
//----------------------------------------------------------