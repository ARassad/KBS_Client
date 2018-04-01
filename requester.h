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

class requester : public QObject
{
	Q_OBJECT

public:
	requester();
	requester(QString msg);
	~requester();

	QString getUrl(QString root, QString apiTitle);
	QVariantMap getJsonData(const QJsonObject &json);
	QJsonObject setJsonData(const QVariantMap &data);

	void getMessageData();

private:
	QString message;
	
private slots:
	void sendMessage();
	void getResponseMessageData(QNetworkReply* reply);

};

#endif // REQUESTER_H
