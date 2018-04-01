#ifndef SYSTEMOFBRIBE_H
#define SYSTEMOFBRIBE_H

#include <QtWidgets/QMainWindow>
#include "ui_systemofbribe.h"
#include "ui_authorization.h"
#include "ui_registration.h"
#include "client.h"

class systemOfBribe : public QMainWindow
{
	Q_OBJECT

public:
	systemOfBribe(QWidget *parent = 0);
	~systemOfBribe();

private:
	client client;
	Ui::systemOfBribeClass uiMain;

	QWidget* authorizationWidget;
	Ui::authorizationClass uiAutho;

	QWidget* registrationWidget;
	Ui::registrationClass uiReg;

private slots:
	void clientAuthorization();
	void goToRegistration();

	void newClientRegistration();

};

#endif // SYSTEMOFBRIBE_H
