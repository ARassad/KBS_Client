#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "requester.h"

#define CLIENT_SPECIALIST 0
#define CLIENT_DEPUTY 1
#define CLIENT_COMPANY 2

class Client : public QObject
{
	Q_OBJECT

public:
	Client(QObject *parent = nullptr);
	~Client();
	void post_authorize(QString login, QString password);
	void post_register(QString login, QString password, int type, QString name, int clientData);
	void get_dataClient(int userId, int userType);
	void get_createBribeOffer(int from, int to, int sum);
	void get_confirmBribe(int from, int sum);
	void get_lastBribeSum(int companyId, int otherId);
	void get_completeCheck(int specialistId, int companyId, int mark, QString characteristic);
	void get_createCheck(int companyId, int deputyId, int doctorId, int policeId, int doomId);
	void get_cancelCheck(int companyId);
	void get_confirmCheck(int companyId);
	void get_rejectCheck(int companyId);
	void get_allSpecialists(int type);
	void get_companyCheckData(int companyId);

private:
	Requester requester;
	QTcpSocket* socket;
	QNetworkAccessManager* manager;

private slots:
	void isAuthorize();
	void isRegister();
	void getDataCompany();
	void getDataSpecialist();
	void getDataDeputy();
	void getBribeServerResult();
	void getLastBribeSum();
	void getCreateCheck();
	void getCancelCheck();
	void getConfirmCheck();
	void getRejectCheck();
	void getAllSpecialists();
	void getCompanyCheckData();

signals:
	void errorServerOcurred(QString errorMsg);
	void authorized(bool loginStatus, int userId, int usertype);
	void registered(bool registrationStatus);
	void companyDataRecieved(QString name, bool isFood);
	void specialistDataRecieved(QString name, int type);
	void deputyDataRecieved(QString name, QMap<QString, QVariant> companies);
	void lastBribeRecieved(int sum, QString name);
	void checkCreateResult(bool result);
	void checkCancelResult(bool result);
	void checkConfirmResult(bool result);
	void checkRejectResult(bool result);
	void allSpecialistsRecieved(QMap<QString, QVariant> people, int type);
	void checkDataRecieved(QMap<QString, QVariant> criteria, QString characteristic);
	
};

#endif // CLIENT_H
