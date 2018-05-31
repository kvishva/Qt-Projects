#ifndef WINCLIENT_H
#define WINCLIENT_H

#include <QObject>
#include <QtDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class WinClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverName READ serverName WRITE setServerName NOTIFY serverNameChanged)
    Q_PROPERTY(QString schoolName READ schoolName WRITE setSchoolName NOTIFY schoolNameChanged)
    Q_PROPERTY(QString domainName READ domainName WRITE setDomainName NOTIFY domainNameChanged)
    Q_PROPERTY(quint32 portNumber READ portNumber WRITE setPortNumber NOTIFY portNumberChanged)
public:
    explicit WinClient(QObject *parent = nullptr);

    QString serverName();
    QString schoolName();
    QString domainName();
    quint32 portNumber();
    void setUserName();
    void setMachineName();


signals:
    void serverNameChanged();
    void schoolNameChanged();
    void domainNameChanged();
    void portNumberChanged();
    void doRegistration(QString token);
    void readLibrary(QJsonArray);
    void readMonitorPolicy(QJsonObject);

public slots:
    void setServerName(const QString &serverName);
    void setSchoolName(const QString &schoolName);
    void setDomainName(const QString &domainName);
    void setPortNumber(const quint32 &portNumber);
    void sendRegistration(QString msg);
    void getRegistration(QString token);
    void readLibraryFromJson(QJsonArray arrLib);
    void readMonitorPolicyFromJson(QJsonObject obj);

private:

    QString m_userName;
    QString m_machineName;
    QString m_serverName;
    QString m_schoolName;
    QString m_domainName;
    quint32 m_portNo;

    QNetworkAccessManager m_NetAccMgr;
    QNetworkRequest m_NetReq;


};

#endif // WINCLIENT_H
