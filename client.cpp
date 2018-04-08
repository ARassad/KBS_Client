#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
	manager = new QNetworkAccessManager(this);
}

Client::~Client()
{

}

void Client::post_authorize(QString login, QString password)
{
	QVariantMap data;
	data.insert(QString("login"), login);
	data.insert(QString("password"), password);
	QNetworkReply* reply = manager->post(QNetworkRequest(requester.getUrlPost(LOCALHOST, QString("signIn"))), requester.setJsonData(data));
	connect(reply, SIGNAL(finished()), this, SLOT(isAuthorize()));
}

void Client::post_register(QString login, QString password, int type, QString name, int clientData)
{
	QVariantMap data;
	data.insert(QString("login"), login);
	data.insert(QString("password"), password);
	data.insert(QString("type"), QString().setNum(type));
	data.insert(QString("name"), name);
	data.insert(QString("clientData"), QString().setNum(clientData));
	QNetworkReply* reply = manager->post(QNetworkRequest(requester.getUrlPost(LOCALHOST, QString("signUp"))), requester.setJsonData(data));
	connect(reply, SIGNAL(finished()), this, SLOT(isRegister()));
}

void Client::get_dataClient(int userId, int userType)
{
	QVariantMap data;
	data.insert("userId", userId);
	QUrl url = QUrl(requester.getUrlGet(LOCALHOST, "getDataCompany", data));
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	if (userType == CLIENT_COMPANY)
	{
		connect(reply, SIGNAL(finished()), this, SLOT(getDataCompany()));
	}
	else if (userType == CLIENT_SPECIALIST)
	{
		connect(reply, SIGNAL(finished()), this, SLOT(getDataSpecialist()));
	}
	else 
	{
		connect(reply, SIGNAL(finished()), this, SLOT(getDataDeputy()));
	}
}

void Client::get_createBribeOffer(int from, int to, int sum)
{
	QVariantMap data;
	data.insert("fromUser", from);
	data.insert("toUser", to);
	data.insert("sum", sum);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("createBribeOffer"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getBribeServerResult()));
}

void Client::get_confirmBribe(int from, int sum)
{
	QVariantMap data;
	data.insert("user", from);
	data.insert("sum", sum);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("agreeToSumOfBribe"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getBribeServerResult()));
}

void Client::get_lastBribeSum(int companyId, int otherId)
{
	QVariantMap data;
	data.insert("company", companyId);
	data.insert("user", otherId);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("getLastBribeSum"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getLastBribeSum()));
}

void Client::get_completeCheck(int specialistId, int companyId, int mark, QString characteristic)
{
	QVariantMap data;
	data.insert("specialistId", specialistId);
	data.insert("companyId", companyId);
	data.insert("mark", mark);
	data.insert("characteristic", characteristic);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("completeCheck"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getCheckRequestResult()));
}

void Client::get_createCheck(int companyId, int deputyId, int doctorId, int policeId, int doomId)
{
	QVariantMap data;
	data.insert("companyId", companyId);
	data.insert("deputyId", deputyId);
	data.insert("doctorId", doctorId);
	data.insert("policeId", policeId);
	data.insert("doomId", doomId);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("createCheck"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getCreateCheck()));
}

void Client::get_cancelCheck(int companyId)
{
	QVariantMap data;
	data.insert("companyId", companyId);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("cancelCheck"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getCancelCheck()));
}

void Client::get_confirmCheck(int companyId)
{
	QVariantMap data;
	data.insert("companyId", companyId);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("confirmCheck"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getConfirmCheck()));
}

void Client::get_rejectCheck(int companyId)
{
	QVariantMap data;
	data.insert("companyId", companyId);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("rejectCheck"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getRejectCheck()));
}

void Client::get_allSpecialists(int type)
{
	QVariantMap data;
	data.insert("type", type);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("getAllSpecialists"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getAllSpecialists()));
}

void Client::get_companyCheckData(int companyId)
{
	QVariantMap data;
	data.insert("companyId", companyId);
	QUrl url = requester.getUrlGet(LOCALHOST, QString("getCompanyCheckData"), data);
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(getCompanyCheckData()));
}







void Client::isAuthorize()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		bool loginStatus = (data.value("loginStatus").toString().compare(QString("Ok")) == 0);
		int userId = data.value("userId").toInt();
		int userType = data.value("type").toInt();

		emit authorized(loginStatus, userId, userType);
	}

	reply->deleteLater();
}

void Client::isRegister()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		bool registrationStatus = data.value("registrationStatus").toString().compare(QString("Ok")) == 0;
		emit registered(registrationStatus);
	}

	reply->deleteLater();
}

void Client::getDataCompany()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		QString name = data.value("name").toString();
		bool isFood = data.value("type").toInt();
		emit companyDataRecieved(name, isFood);
	}

	reply->deleteLater();
}

void Client::getDataSpecialist()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		QString name = data.value("name").toString();
		int typeSpecialist = data.value("type").toInt();
		emit specialistDataRecieved(name, typeSpecialist);
	}

	reply->deleteLater();
}

void Client::getDataDeputy()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		QString name = data.value("name").toString();
		QMap<QString, QVariant> companies = data.value("companies").toMap();
		emit deputyDataRecieved(name, companies);
	}

	reply->deleteLater();
}

void Client::getBribeServerResult()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}
	}

	reply->deleteLater();
}

void Client::getLastBribeSum()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		int sum = data.value("sum").toInt();
		QString name = data.value("lastNameBriber").toString();
		emit lastBribeRecieved(sum, name);
	}

	reply->deleteLater();
}

void Client::getCreateCheck()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		bool result = data.value("resultRequest").toString().compare(QString("True")) == 0;
		emit checkCreateResult(result);
	}

	reply->deleteLater();
}

void Client::getCancelCheck()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		bool result = data.value("resultRequest").toString().compare(QString("True")) == 0;
		emit checkCancelResult(result);
	}

	reply->deleteLater();
}

void Client::getConfirmCheck()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		bool result = data.value("resultRequest").toString().compare(QString("True")) == 0;
		emit checkConfirmResult(result);
	}

	reply->deleteLater();
}

void Client::getRejectCheck()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		bool result = data.value("resultRequest").toString().compare(QString("True")) == 0;
		emit checkRejectResult(result);
	}

	reply->deleteLater();
}

void Client::getAllSpecialists()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		QMap<QString, QVariant> people = data.value("people").toMap();
		int type = data.value("type").toInt();
		emit allSpecialistsRecieved(people, type);
	}

	reply->deleteLater();
}

void Client::getCompanyCheckData()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError)
	{
		QVariantMap data = requester.getJsonData(reply->readAll());
		if (data.value("status").toString().compare(QString("Error")) == 0)
		{
			emit errorServerOcurred(QString("Сервер недоступен! Повторите попытку позже!"));
			reply->deleteLater();
			return;
		}

		QMap<QString, QVariant> criteria = data.value("people").toMap();
		QString characteristic = data.value("“characteristic”").toString();
		emit checkDataRecieved(criteria, characteristic);
	}

	reply->deleteLater();
}