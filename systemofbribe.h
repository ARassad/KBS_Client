#ifndef SYSTEMOFBRIBE_H
#define SYSTEMOFBRIBE_H

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QTextCodec>
#include "ui_systemofbribe.h"
#include "ui_authorization.h"
#include "ui_registration.h"
#include "ui_createCheck.h"
#include "client.h"

class systemOfBribe : public QMainWindow
{
	Q_OBJECT

public:
	systemOfBribe(QWidget *parent = 0);
	~systemOfBribe();

private:
	int id;
	int currentUser;
	int currentPage;
	QTextCodec* c;
	Client client;
	Ui::systemOfBribeClass uiMain;

	QWidget* authorizationWidget;
	Ui::authorizationClass uiAutho;

	QWidget* registrationWidget;
	Ui::registrationClass uiReg;

	QMainWindow* createCheckWidget;
	Ui::createCheckClass uiCreateCheck;

private slots:
	void clientAuthorization();
	void goToRegistration();

	void newClientRegistration();
	void registrationSuccessfull(bool isOk);
	void authorizationSuccessfull(bool isOk, int userId, int userType);

	void setRegistrationData();

	void setCompanyData(QString name, bool isFood);
	void setSpecialistData(QString name, int type);
	void setDeputyData(QString name, QMap<QString, QVariant> companies);


	void createNewBribeCompany();
	void createNewBribeSpecialist();
	void createNewBribeDeputy();
	void confirmBribeCompany();
	void confirmBribeSpecialist();
	void confirmBribeDeputy();

	void getBribesForCompany(QListWidgetItem* interlocutor);
	void getBribesForOther(QListWidgetItem* interlocutor);
	void getLastBribe(int sum, QString name);

	void endCheck();
	void isCheckCreateCompleted(bool isOk);
	void isCheckCancelCompleted(bool isOk);
	void isCheckConfirmCompleted(bool isOk);
	void isCheckRejectCompleted(bool isOk);

	void createCheck();
	void confirmCheckCreate();
	void cancelCheck();
	void confirmCheck();
	void rejectCheck();

	void setSpecialists(QMap<QString, QVariant> specialists, int type);
	void addDoctor();
	void addPolice();
	void addDoom();
	void deleteSpecialist();


	void updateDataCompany();
	void updateDataCompany(QListWidgetItem* company);
	void setCriteriaCompany(QMap<QString, QVariant> criteria, QString characteristic);
};

#endif // SYSTEMOFBRIBE_H
