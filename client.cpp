#include "client.h"

client::client(QObject *parent) : QObject(parent)
{
	/*socket = new QTcpSocket();
	socket->connectToHost(nameHost, numPort);*/

	manager = new QNetworkAccessManager(this);
	//connect(socket, SIGNAL(connected()), SLOT(isConnect()));
}

client::~client()
{

}

void client::post_authorize(QString login, QString password)
{
	QByteArray data;
	data.append(QString("login=") + login);
	data.append(QString("&password=") + password);
	connect(manager, SIGNAL(finshed(QNetworkReply*)), this, SLOT(isAuthorize(QNetworkReply*)));
	manager->post(QNetworkRequest(requester.getUrl(LOCALHOST, QString("signIn"))), data);
}

void client::get_dataCompany(QString userId)
{
	QString data = QString("?userId=") + userId;
	QUrl url = QUrl(requester.getUrl(LOCALHOST, "getDataCompany") + data);
	connect(manager, SIGNAL(finshed(QNetworkReply*)), this, SLOT(getDataCompany(QNetworkReply*)));
	manager->get(QNetworkRequest(url));
}

void client::isAuthorize(QNetworkReply* reply)
{
	QString strId;
	if (reply->error() == QNetworkReply::NoError)
	{
		strId = QString::fromUtf8(reply->readAll());
	}

	reply->deleteLater();

	get_dataCompany(strId);
}

void client::getDataCompany(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QString str = QString::fromUtf8(reply->readAll());

		QByteArray data;
	}

	reply->deleteLater();
}