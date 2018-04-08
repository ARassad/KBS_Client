#include "requester.h"

Requester::Requester() : QObject()
{
}

Requester::Requester(QString msg) : QObject()
{
	message = msg;
}

Requester::~Requester()
{

}

QString Requester::getUrlPost(QString root, QString apiTitle)
{
	return QString("http://") + root + QString("/api/") + apiTitle;
}

QString Requester::getUrlGet(QString root, QString apiTitle, QVariantMap params)
{
	QString url = QString("http://") + root + QString("api?method=") + apiTitle;
    for (auto iter = params.begin(); iter != params.end(); ++iter)
    {
        url += QString("&") + iter.key() + QString("=") + iter.value().toString();
    }
    return url;//url.remove(url.size() - 1, 1);
}

QVariantMap Requester::getJsonData(const QByteArray &data)
{
	QJsonDocument jsonDocument = QJsonDocument::fromBinaryData(data);
	return jsonDocument.object().toVariantMap();
}

QByteArray Requester::setJsonData(const QVariantMap &data)
{
	QJsonObject jsonObject = QJsonObject::fromVariantMap(data);
	return QJsonDocument(jsonObject).toBinaryData();
}

void Requester::sendMessage()
{
	QVariantMap data;
	//data.insert("userId", QVariant(id));
	//data.insert("toWhom", QVariant(id));
	data.insert("sendMessage", QVariant(message));
	QString url = getUrlPost(LOCALHOST, "sendMessage");
	
	//QJsonDocument jsonDoc = QJsonDocument(json);
}

void Requester::getMessageData()
{

}

void Requester::getResponseMessageData(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = getJsonData(reply->readAll());
		int sum = data.value("sum").toInt();
	}
}
