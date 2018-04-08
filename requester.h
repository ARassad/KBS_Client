#ifndef REQUESTER_H
#define REQUESTER_H

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#define LOCALHOST QString("127.0.0.1:8000")

class Requester : public QObject
{
	Q_OBJECT

public:
	Requester();
	Requester(QString msg);
	~Requester();

	QString getUrlPost(QString root, QString apiTitle);
	QString getUrlGet(QString root, QString apiTitle, QVariantMap params);
	QVariantMap getJsonData(const QByteArray &data);
	QByteArray setJsonData(const QVariantMap &data);

	void getMessageData();

private:
	QString message;
	
private slots:
	void sendMessage();
	void getResponseMessageData(QNetworkReply* reply);

};

#endif // REQUESTER_H
