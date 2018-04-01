#include "systemofbribe.h"

systemOfBribe::systemOfBribe(QWidget *parent) : QMainWindow(parent)
{
	uiMain.setupUi(this);

	authorizationWidget = new QWidget();
	uiAutho.setupUi(authorizationWidget);
	authorizationWidget->show();

	connect(uiAutho.authorizeBtn, SIGNAL(clicked()), this, SLOT(clientAuthorization()));
	connect(uiAutho.goToRegisterBtn, SIGNAL(clicked()), this, SLOT(goToRegistration()));

	registrationWidget = new QWidget();
	uiReg.setupUi(registrationWidget);
	connect(uiReg.registerBtn, SIGNAL(clicked()), this, SLOT(newClientRegistration()));
}

systemOfBribe::~systemOfBribe()
{

}

void systemOfBribe::clientAuthorization()
{
	authorizationWidget->close();
	client.post_authorize(uiAutho.login->text(), uiAutho.password->text());
	show();
}

void systemOfBribe::newClientRegistration()
{
	registrationWidget->close();
	authorizationWidget->show();
}

void systemOfBribe::goToRegistration()
{
	authorizationWidget->close();
	registrationWidget->show();
}
