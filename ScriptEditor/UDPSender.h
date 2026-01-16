#pragma execution_character_set("utf-8")
#pragma once
#ifndef UDPSENDER_H
#define UDPSENDER_H
//-----------------------------------------------------------------
#include <QObject>
#include <QUdpSocket>
#include <QString>
//-----------------------------------------------------------------
class UDPSender : public QObject
{
    Q_OBJECT
public:
    explicit    UDPSender(QObject* parent = nullptr);
    bool        sendScript(const QString& script, const QString& address, quint16 port);

signals:
    // Сигналы для связи с UI
    void        errorOccurred(const QString& error);
    void        scriptSent();
    void        logMessage(const QString& message);

private:
    QUdpSocket* socket;

};

#endif // UDPSENDER_H
//-----------------------------------------------------------------
