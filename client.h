#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "requester.h"

class client : public QObject
{
	Q_OBJECT

public:
	client(QObject *parent = nullptr);
	~client();
	void post_authorize(QString login, QString password);
	void get_dataCompany(QString userId);

private:
	requester requester;
	QTcpSocket* socket;
	QNetworkAccessManager* manager;

private slots:
	void isAuthorize(QNetworkReply* reply);
	void getDataCompany(QNetworkReply* reply);
	
};

#endif // CLIENT_H
