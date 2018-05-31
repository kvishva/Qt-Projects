#include "winclient.h"


WinClient::WinClient(QObject *parent) : QObject(parent)
{
    m_portNo = 0;
    m_serverName = "dev";
    m_schoolName = "Dev";
    m_domainName = "https://api-test.learnsafe.com/api//token";

    connect(this,SIGNAL(doRegistration(QString)),this,SLOT(getRegistration(QString)));
    connect(this,SIGNAL(readLibrary(QJsonArray)),this,SLOT(readLibraryFromJson(QJsonArray)));
    connect(this,SIGNAL(readMonitorPolicy(QJsonObject)),this,SLOT(readMonitorPolicyFromJson(QJsonObject)));

}

void WinClient::setUserName()
{

}

void WinClient::setMachineName()
{

}

void WinClient::setServerName(const QString &serverName)
{
    if(m_serverName.compare(serverName)==0)
        return;

    m_serverName = serverName;
    emit serverNameChanged();
    qDebug()<< "server name changed : "<<m_serverName;
}

void WinClient::setSchoolName(const QString &schoolName)
{
    if(m_schoolName.compare(schoolName) ==0 )
        return;
    m_schoolName = schoolName;
    emit schoolNameChanged();
    qDebug()<< "school name changed : "<< m_schoolName;
}


void WinClient::setDomainName(const QString &domainName)
{
    if(m_domainName.compare(domainName)==0)
        return;

    m_domainName = domainName;
    emit domainNameChanged();
    qDebug() << "domain name changed : "<<m_domainName;
}

void WinClient::setPortNumber(const quint32 &portNumber)
{
    if(m_portNo == portNumber)
        return;

    m_portNo = portNumber;
    emit portNumberChanged();
    qDebug() << "port number changed : "<<m_portNo;
}

QString WinClient::serverName()
{
    return m_serverName;
}

QString WinClient::schoolName()
{
    return m_schoolName;
}

QString WinClient::domainName()
{
    return m_domainName;
}

quint32 WinClient::portNumber()
{
    return m_portNo;
}

void WinClient::sendRegistration(QString msg)
{
    qDebug()<<"Start sending registration request from " << msg;

    QByteArray data;
    QJsonObject jobject;

    jobject["serverName"]=m_serverName;
    qint64 etime = QDateTime::currentMSecsSinceEpoch();
    QString key ="75d25e25-0519-47a1-9104-792d9ef35c9b-40404e79-af88-4653-a66c-f37ac3b13236";
    QByteArray k1;
    k1.append(key);
    k1.append(QString::number(etime));
    jobject["epochdate"]=QString::number(etime);
    jobject["hash"]= QString(QCryptographicHash::hash(k1,QCryptographicHash::Md5).toHex()).toUpper();
    QJsonDocument doc(jobject);

    data = doc.toJson();
    qDebug()<<"Registration Request"<< data.data();

    m_NetReq.setUrl(QUrl(m_domainName));
    m_NetReq.setRawHeader("Content-Type","application/json");
    m_NetReq.setRawHeader("Accept","application/json");

    qDebug()<<m_NetReq.url().url();
    QNetworkReply *reply =  m_NetAccMgr.post(m_NetReq,data);

    connect(reply,&QNetworkReply::finished,[=] (){

       if(reply->error())
            qDebug()<<"Status : " <<reply->errorString();

       QByteArray data = reply->readAll();

       qDebug()<<data.toStdString().c_str();

       QJsonDocument doc = QJsonDocument::fromJson(data);
       if(!doc.isNull()){
           if(doc.isObject())
               qDebug()<<"\nJson is object\n";
           else if(doc.isArray())
               qDebug()<<"\nJson is array\n";
       }
           else
               qDebug()<<"issue to convert back to json";


       QJsonObject obj;
       obj = doc.object();
       QString str;
       if(obj.contains("token") && obj["token"].isString())
           str = obj["token"].toString();

       qDebug()<<"token : "<<str;

       emit doRegistration(str);

       reply->deleteLater();

    }

    );

   /* connect( reply,&QNetworkReply::readyRead, [=] (){
        reply->deleteLater();}
    );*/


}

void WinClient::getRegistration(QString token)
{

    QByteArray data;
    QJsonObject jobject;

    jobject["serverName"]=m_serverName;
    jobject["schoolName"]=m_schoolName;
    jobject["hostname"]="vishva-pc";
    jobject["userName"]="vishvanathan kuppusamy";

    QJsonDocument doc(jobject);

    data = doc.toJson();
    qDebug()<<"Registration Request"<< data.data();

    QString auth("Bearer ");
    auth += token;

    qDebug()<<auth;

    m_NetReq.setUrl(QUrl("https://api-test.learnsafe.com/api/Registration"));
    m_NetReq.setRawHeader("Content-Type","application/json");
    m_NetReq.setRawHeader("Authorization",auth.toUtf8());

    qDebug()<<m_NetReq.url().url();



    QNetworkReply *reply =  m_NetAccMgr.post(m_NetReq,data);

    connect(reply,&QNetworkReply::finished,[=] (){

       if(reply->error())
            qDebug()<<"Status : " <<reply->errorString();

       QByteArray data = reply->readAll();

      // qDebug()<<data.toStdString().c_str();

       QJsonDocument doc = QJsonDocument::fromJson(data);
       if(!doc.isNull()){
           if(doc.isObject())
               qDebug()<<"\nJson is object\n";
           else if(doc.isArray())
               qDebug()<<"\nJson is array\n";
       }
           else
               qDebug()<<"issue to convert back to json";

       QJsonObject obj;
       obj = doc.object();
       QString str;
       QJsonArray arrLib;
       if(obj.contains("library") && obj["library"].isArray())
           arrLib = obj["library"].toArray();
       emit readLibrary(arrLib);

       if(obj.contains("monitoringPolicy") && obj["monitoringPolicy"].isObject())
           emit readMonitorPolicy(obj["monitoringPolicy"].toObject());

       reply->deleteLater();

    }

    );

   /* connect( reply,&QNetworkReply::readyRead, [=] (){
        reply->deleteLater();}
    );*/


}

void WinClient::readLibraryFromJson(QJsonArray arrLib)
{

    qDebug()<<"yes library";

}

void WinClient::readMonitorPolicyFromJson(QJsonObject obj)
{
    qDebug()<<"Monitoring policy";
}
