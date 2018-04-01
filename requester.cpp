#include "requester.h"

requester::requester() : QObject()
{
}

requester::requester(QString msg) : QObject()
{
	message = msg;
}

requester::~requester()
{

}

QString requester::getUrl(QString root, QString apiTitle)
{
	QString url = QString("http://") + root + QString("/api/") + apiTitle; //+ QString("/");
    /*for (auto iter = data.begin(); iter != data.end(); ++iter)
    {
        url += iter.key() + QString("=") + iter.value().toString() + QString("&");
    }
    return url.remove(url.size() - 1, 1);*/
	return url;
}

QVariantMap requester::getJsonData(const QJsonObject &json)
{
	return json.toVariantMap();
}

QJsonObject requester::setJsonData(const QVariantMap &data)
{
	return QJsonObject::fromVariantMap(data);
}

void requester::sendMessage()
{
	QVariantMap data;
	//data.insert("userId", QVariant(id));
	//data.insert("toWhom", QVariant(id));
	data.insert("sendMessage", QVariant(message));
	QString url = getUrl(LOCALHOST, "sendMessage");
	QJsonObject json = setJsonData(data);
	QJsonDocument jsonDoc = QJsonDocument(json);
}

void requester::getMessageData()
{

}

void requester::getResponseMessageData(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QJsonDocument json = QJsonDocument::fromBinaryData(reply->readAll());
		QVariantMap data = getJsonData(json.object());
		int sum = data.value("sum").toInt();
	}
}
