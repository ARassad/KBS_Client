#include "systemofbribe.h"

/*
**************************************************************************************
**************************************************************************************
**************************************************************************************
**************************************************************************************


			1) ПОЛУЧЕНИЕ СПИСКА СОБЕСЕДНИКОВ
			2) ОТКРЫТИЕ ОКНА ПРОВЕДЕНИЯ ПРОВЕРКИ У СПЕЦИАЛИСТА


**************************************************************************************
**************************************************************************************
**************************************************************************************
**************************************************************************************
*/
systemOfBribe::systemOfBribe(QWidget *parent) : QMainWindow(parent)
{
	uiMain.setupUi(this);
	c = QTextCodec::codecForLocale();

	createCheckWidget = new QMainWindow(this, Qt::Dialog);
	uiCreateCheck.setupUi(createCheckWidget);

	authorizationWidget = new QWidget();
	uiAutho.setupUi(authorizationWidget);
	connect(uiAutho.authorizeBtn, SIGNAL(clicked()), this, SLOT(clientAuthorization()));
	connect(uiAutho.goToRegisterBtn, SIGNAL(clicked()), this, SLOT(goToRegistration()));
	connect(&client, SIGNAL(authorized(bool,int,int)), this, SLOT(authorizationSuccessfull(bool,int,int)));

	registrationWidget = new QWidget();
	uiReg.setupUi(registrationWidget);
	setRegistrationData();
	uiMain.idCompanyCheck->setVisible(false);
	connect(uiReg.registerBtn, SIGNAL(clicked()), this, SLOT(newClientRegistration()));
	connect(&client, SIGNAL(registered(bool)), this, SLOT(registrationSuccessfull(bool)));
	connect(uiReg.isCompany, SIGNAL(clicked()), this, SLOT(setRegistrationData()));
	connect(uiReg.isDeputy, SIGNAL(clicked()), this, SLOT(setRegistrationData()));
	connect(uiReg.isSpecialist, SIGNAL(clicked()), this, SLOT(setRegistrationData()));

	connect(&client, SIGNAL(companyDataRecieved(QString, bool)), this, SLOT(setCompanyData(QString, bool)));
	connect(&client, SIGNAL(specialistDataRecieved(QString, int)), this, SLOT(setSpecialistData(QString, int)));
	connect(&client, SIGNAL(deputyDataRecieved(QString, QMap<int, QString>)), this, SLOT(setDeputyData(QString, QMap<QString, QVariant>)));


	connect(uiMain.companyChangeBribeBtn, SIGNAL(clicked()), this, SLOT(createNewBribeCompany()));
	connect(uiMain.specialistChangeBribeBtn, SIGNAL(clicked()), this, SLOT(createNewBribeSpecialist()));
	connect(uiMain.deputyChangeBribeBtn, SIGNAL(clicked()), this, SLOT(createNewBribeDeputy()));

	connect(uiMain.companyConfirmBribeBtn, SIGNAL(clicked()), this, SLOT(confirmBribeCompany()));
	connect(uiMain.specialistConfirmBribeBtn, SIGNAL(clicked()), this, SLOT(confirmBribe()));
	connect(uiMain.deputyConfirmBribeBtn, SIGNAL(clicked()), this, SLOT(confirmBribe()));

	connect(uiMain.companyListInterlocutors, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(getBribesForCompany(QListWidgetItem*)));
	connect(uiMain.specialistListInterlocutors, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(getBribesForOther(QListWidgetItem*)));
	connect(uiMain.deputyListInterlocutors, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(getBribesForOther(QListWidgetItem*)));
	connect(&client, SIGNAL(lastBribeRecieved(int, QString)), this, SLOT(getLastBribe(int, QString)));



	connect(&client, SIGNAL(checkCreateResult(bool)), this, SLOT(isCheckCreateCompleted(bool)));
	connect(&client, SIGNAL(checkCancelResult(bool)), this, SLOT(isCheckCancelCompleted(bool)));
	connect(&client, SIGNAL(checkConfirmResult(bool)), this, SLOT(isCheckConfirmCompleted(bool)));
	connect(&client, SIGNAL(checkRejectResult(bool)), this, SLOT(isCheckRejectCompleted(bool)));

	connect(uiMain.endCheckBtn, SIGNAL(clicked()), this, SLOT(endCheck()));

	connect(uiMain.createCheckBtn, SIGNAL(clicked()), this, SLOT(createCheck()));
	connect(uiCreateCheck.confirmCheckCreate, SIGNAL(clicked()), this, SLOT(confirmCheckCreate()));

	connect(uiMain.cancelCheckBtn, SIGNAL(clicked()), this, SLOT(cancelCheck()));
	connect(uiMain.confirmCheckBtn, SIGNAL(clicked()), this, SLOT(confirmCheck()));
	connect(uiMain.rejectCheckBtn, SIGNAL(clicked()), this, SLOT(rejectCheck()));

	connect(&client, SIGNAL(allSpecialistsRecieved(QMap<QString, QVariant>, int)), this, SLOT(setSpecialists(QMap<QString, QVariant>, int)));
	connect(uiCreateCheck.addDoctor, SIGNAL(clicked()), this, SLOT(addDoctor()));
	connect(uiCreateCheck.addPolice, SIGNAL(clicked()), this, SLOT(addPolice()));
	connect(uiCreateCheck.addDoom, SIGNAL(clicked()), this, SLOT(addDoom()));
	connect(uiCreateCheck.deleteSpecialist, SIGNAL(clicked()), this, SLOT(deleteSpecialist()));


	connect(uiMain.tabCompany, SIGNAL(currentChanged(int)), this, SLOT(updateDataCompany()));
	connect(uiMain.deputyListCompany, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(updateDataCompany(QListWidgetItem*)));
	connect(uiMain.listCheckedCompany, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(updateDataCompany(QListWidgetItem*)));
	connect(&client, SIGNAL(checkDataRecieved(QMap<QString, QVariant>, QString)), this, SLOT(setCriteriaCompany(QMap<QString, QVariant>, QString)));


	authorizationWidget->show();
}

systemOfBribe::~systemOfBribe()
{

}

void systemOfBribe::setRegistrationData()
{
	if (uiReg.isCompany->isChecked())
	{
		uiReg.labelName->setText(c->toUnicode("Название предприятия"));
		uiReg.dataLabel->setText(c->toUnicode("Тип предоставляемого товара"));
		uiReg.clientData->clear();
		uiReg.clientData->addItem(c->toUnicode("Непродовольственный товар"));
		uiReg.clientData->addItem(c->toUnicode("Продовольственный товар"));
		uiReg.dataLabel->setVisible(true);
		uiReg.clientData->setVisible(true);
	}
	else if (uiReg.isSpecialist->isChecked())
	{
		uiReg.labelName->setText(c->toUnicode("Ваше имя"));
		uiReg.dataLabel->setText(c->toUnicode("Ваша профессия"));
		uiReg.clientData->clear();
		uiReg.clientData->addItem(c->toUnicode("Врач"));
		uiReg.clientData->addItem(c->toUnicode("Полицейский"));
		uiReg.clientData->addItem(c->toUnicode("Специалист от думы"));
		uiReg.dataLabel->setVisible(true);
		uiReg.clientData->setVisible(true);
	}
	else
	{
		uiReg.labelName->setText(c->toUnicode("Ваше имя"));
		uiReg.dataLabel->setVisible(false);
		uiReg.clientData->setVisible(false);
	}
}

void systemOfBribe::clientAuthorization()
{
	authorizationWidget->close();
	client.post_authorize(uiAutho.login->text(), uiAutho.password->text());
	show();
}

void systemOfBribe::newClientRegistration()
{
	if (uiReg.password->text().compare(uiReg.passwordConfirm->text()) == 0)
	{
		registrationWidget->close();

		QString login = uiReg.login->text();
		QString password = uiReg.password->text();
		int type;
		if (uiReg.isCompany->isChecked())
			type = 2;
		else if (uiReg.isSpecialist->isChecked())
			type = 0;
		else
			type = 1;
		QString name = uiReg.nameRegistration->text();
		int clientData = uiReg.clientData->currentIndex();

		client.post_register(login, password, type, name, clientData);
	}
	else
		QMessageBox::warning(this, c->toUnicode("Ошибка регистрации"), c->toUnicode("Пароли не совпадают!"));
}

void systemOfBribe::goToRegistration()
{
	authorizationWidget->close();
	registrationWidget->show();
}

void systemOfBribe::registrationSuccessfull(bool isOk)
{
	if (isOk)
	{
		QMessageBox::information(this, c->toUnicode("Регистрация нового клиента"), c->toUnicode("Регистрация прошла успешно!"));
		authorizationWidget->show();
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Регистрация нового клиента"), c->toUnicode("Ошибка регистрации! Проверьте данные!"));
	}
}

void systemOfBribe::authorizationSuccessfull(bool isOk, int userId, int userType)
{
	if (isOk)
	{
		show();
		if (userType == CLIENT_SPECIALIST)
		{
			uiMain.stackedWidget->setCurrentWidget(uiMain.specialist);
		}
		else if (userType == CLIENT_DEPUTY)
		{
			uiMain.stackedWidget->setCurrentWidget(uiMain.deputy);
		}
		else
		{
			uiMain.stackedWidget->setCurrentWidget(uiMain.company);
			client.get_companyCheckData(userId);
		}
		id = userId;
		currentUser = userType;
		client.get_dataClient(userId, userType);
		QMessageBox::information(this, c->toUnicode("Авторизация"), c->toUnicode("Авторизация прошла успешно!"));
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Авторизация"), c->toUnicode("Ошибка авторизации! Неправильное имя пользователя или пароль!"));
	}
}

void systemOfBribe::setCompanyData(QString name, bool isFood)
{
	uiMain.companyName->setText(name);
	isFood ? uiMain.isFood->setChecked(isFood) : uiMain.isNotFood->setChecked(!isFood);
}

void systemOfBribe::setSpecialistData(QString name, int type)
{
	uiMain.specialistName->setText(name);
	QString profession;
	switch (type)
	{
	case 0:
		{
			profession = c->toUnicode("Врач");
			break;
		}
	case 1:
		{
			profession = c->toUnicode("Полицейский");
			break;
		}
	case 2:
		{
			profession = c->toUnicode("Специалист от думы");
			break;
		}
	}
	uiMain.specialistType->setText(profession);
}

void systemOfBribe::setDeputyData(QString name, QMap<QString, QVariant> companies)
{
	uiMain.deputyName->setText(name);
	for (auto iter = companies.cbegin(); iter != companies.cend(); ++iter)
	{
		QListWidgetItem* company = new QListWidgetItem((*iter).toString(), uiMain.deputyListCompany);
		company->setData(Qt::UserRole, iter.key().toInt());
	}
}

void systemOfBribe::createNewBribeCompany()
{
	QListWidgetItem* interlocutor = uiMain.companyListInterlocutors->currentItem();
	if (interlocutor != NULL)
	{
		int from = id;
		int to = interlocutor->data(Qt::UserRole).toInt();
		int sum = uiMain.companyBribeSum->value();
		client.get_createBribeOffer(from, to, sum);
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Дача взятки"), c->toUnicode("Выберите пользователя из предложенного списка собеседников!"));
	}
}

void systemOfBribe::createNewBribeSpecialist()
{
	QListWidgetItem* interlocutor = uiMain.specialistListInterlocutors->currentItem();
	if (interlocutor != NULL)
	{
		int from = id;
		int to = interlocutor->data(Qt::UserRole).toInt();
		int sum = uiMain.specialistBribeSum->value();
		client.get_createBribeOffer(from, to, sum);
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Дача взятки"), c->toUnicode("Выберите пользователя из предложенного списка собеседников!"));
	}
}

void systemOfBribe::createNewBribeDeputy()
{
	QListWidgetItem* interlocutor = uiMain.deputyListInterlocutors->currentItem();
	if (interlocutor != NULL)
	{
		int from = id;
		int to = interlocutor->data(Qt::UserRole).toInt();
		int sum = uiMain.deputyBribeSum->value();
		client.get_createBribeOffer(from, to, sum);
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Дача взятки"), c->toUnicode("Выберите пользователя из предложенного списка собеседников!"));
	}
}

void systemOfBribe::confirmBribeCompany()
{
	int from = id;
	int sum = uiMain.companyBribeSum->value();
	client.get_confirmBribe(from, sum);
}

void systemOfBribe::confirmBribeSpecialist()
{
	int from = id;
	int sum = uiMain.specialistBribeSum->value();
	client.get_confirmBribe(from, sum);
}

void systemOfBribe::confirmBribeDeputy()
{
	int from = id;
	int sum = uiMain.deputyBribeSum->value();
	client.get_confirmBribe(from, sum);
}

void systemOfBribe::getLastBribe(int sum, QString name)
{
	QListWidgetItem* item;
	if (currentUser == CLIENT_COMPANY)
	{
		item = new QListWidgetItem(uiMain.companyHistoryBribes);
	}
	else if (currentUser == CLIENT_SPECIALIST)
	{
		item = new QListWidgetItem(uiMain.specialistHistoryBribes);
	}
	else
	{
		item = new QListWidgetItem(uiMain.deputyHistoryBribes);
	}
	item->setText(name + c->toUnicode(" предоставил ") + QString().setNum(sum) + c->toUnicode(" у.е."));
}

void systemOfBribe::getBribesForCompany(QListWidgetItem* interlocutor)
{
	client.get_lastBribeSum(id, interlocutor->data(Qt::UserRole).toInt());
}

void systemOfBribe::getBribesForOther(QListWidgetItem* interlocutor)
{
	client.get_lastBribeSum(interlocutor->data(Qt::UserRole).toInt(), id);
}

void systemOfBribe::endCheck()
{
	int specialistId = id;
	int companyId = uiMain.idCompanyCheck->value();
	int mark = uiMain.criteriaMark->value();
	QString characteristic = uiMain.specialistCharacterCheck->toPlainText();

	client.get_completeCheck(specialistId, companyId, mark, characteristic);
}

void systemOfBribe::isCheckCreateCompleted(bool isOk)
{
	if (isOk)
	{
		QListWidgetItem* company = uiMain.deputyListCompany->currentItem()->clone();
		uiMain.listCurrentCheckCompany->addItem(company);
		QMessageBox::information(this, c->toUnicode("Создание новой проверки"), c->toUnicode("Новая проверка создана успешно!"));
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Создание новой проверки"), c->toUnicode("Произошла ошибка при создании новой проверки!"));
	}
}

void systemOfBribe::isCheckCancelCompleted(bool isOk)
{
	if (isOk)
	{
		QListWidgetItem* company = uiMain.listCurrentCheckCompany->takeItem(uiMain.listCurrentCheckCompany->currentRow());
		delete company;
		QMessageBox::information(this, c->toUnicode("Отмена проверки"), c->toUnicode("Проверка успешно отменена!"));
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Отмена проверки"), c->toUnicode("Произошла ошибка при отмене проверки!"));
	}
}

void systemOfBribe::isCheckConfirmCompleted(bool isOk)
{
	if (isOk)
	{
		QListWidgetItem* company = uiMain.listCheckedCompany->takeItem(uiMain.listCurrentCheckCompany->currentRow());
		delete company;
		QMessageBox::information(this, c->toUnicode("Подтверждение проверки"), c->toUnicode("Проверка успешно подтверждена!"));
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Подтверждение проверки"), c->toUnicode("Произошла ошибка при подтверждении заявки!"));
	}
}

void systemOfBribe::isCheckRejectCompleted(bool isOk)
{
	if (isOk)
	{
		QListWidgetItem* company = uiMain.listCheckedCompany->takeItem(uiMain.listCurrentCheckCompany->currentRow());
		uiMain.listCurrentCheckCompany->addItem(company);
		QMessageBox::information(this, c->toUnicode("Отклонение проверки"), c->toUnicode("Проверка успешно отклонена!"));
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Отклонение проверки"), c->toUnicode("Произошла ошибка при отклонении заявки!"));
	}
}



void systemOfBribe::createCheck()
{
	if (uiMain.deputyListCompany->currentItem() != NULL)
	{
		client.get_allSpecialists(0);
		client.get_allSpecialists(1);
		client.get_allSpecialists(2);
		createCheckWidget->show();
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Создание проверки"), c->toUnicode("Предприятие не выбрано!"));
	}
}

void systemOfBribe::confirmCheckCreate()
{
	int companyId = uiMain.deputyListCompany->currentItem()->data(Qt::UserRole).toInt();
	int deputyId = id;
	int doctorId = uiCreateCheck.endSpecList->findItems(c->toUnicode("Врач"), Qt::MatchContains).at(0)->data(Qt::UserRole).toInt();
	int policeId = uiCreateCheck.endSpecList->findItems(c->toUnicode("Полицейский"), Qt::MatchContains).at(0)->data(Qt::UserRole).toInt();
	int doomId = uiCreateCheck.endSpecList->findItems(c->toUnicode("Специалист от думы"), Qt::MatchContains).at(0)->data(Qt::UserRole).toInt();
	
	client.get_createCheck(companyId, deputyId, doctorId, policeId, doomId);

	createCheckWidget->close();
}

void systemOfBribe::cancelCheck()
{
	QListWidgetItem* company = uiMain.listCurrentCheckCompany->currentItem();
	if (company != NULL)
	{
		int companyId = company->data(Qt::UserRole).toInt();
		client.get_cancelCheck(companyId);
	}
}

void systemOfBribe::confirmCheck()
{
	QListWidgetItem* company = uiMain.listCheckedCompany->currentItem();
	if (company != NULL)
	{
		int companyId = company->data(Qt::UserRole).toInt();
		client.get_confirmCheck(companyId);
	}
}

void systemOfBribe::rejectCheck()
{
	QListWidgetItem* company = uiMain.listCheckedCompany->currentItem();
	if (company != NULL)
	{
		int companyId = company->data(Qt::UserRole).toInt();
		client.get_rejectCheck(companyId);
	}
}

void systemOfBribe::setSpecialists(QMap<QString, QVariant> specialists, int type)
{
	QListWidget* view;
	switch (type)
	{
	case 0:
		{
			view = uiCreateCheck.doctorList;
			break;
		}
	case 1:
		{
			view = uiCreateCheck.policeList;
			break;
		}
	case 2:
		{
			view = uiCreateCheck.doomList;
			break;
		}
	default:
		break;
	}

	for (auto iter = specialists.cbegin(); iter != specialists.cend(); ++iter)
	{
		QListWidgetItem* specialist = new QListWidgetItem(iter->toString(), view);
		specialist->setData(Qt::UserRole, iter.key().toInt());
	}
}

void systemOfBribe::addDoctor()
{
	if (uiCreateCheck.endSpecList->findItems(c->toUnicode("Врач"), Qt::MatchContains).isEmpty())
	{
		if (uiCreateCheck.doctorList->currentItem() != NULL)
		{
			QListWidgetItem* doctor = uiCreateCheck.doctorList->takeItem(uiCreateCheck.doctorList->currentRow());
			doctor->setText(c->toUnicode("Врач: ") + doctor->text());
			uiCreateCheck.endSpecList->addItem(doctor);
		}
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Назначение врача на проверку"), c->toUnicode("На данную проверку врач уже назначен!"));
	}
}

void systemOfBribe::addPolice()
{
	if (uiCreateCheck.endSpecList->findItems(c->toUnicode("Полицейский"), Qt::MatchContains).isEmpty())
	{
		if (uiCreateCheck.policeList->currentItem() != NULL)
		{
			QListWidgetItem* police = uiCreateCheck.policeList->takeItem(uiCreateCheck.policeList->currentRow());
			police->setText(c->toUnicode("Полицейский: ") + police->text());
			uiCreateCheck.endSpecList->addItem(police);
		}
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Назначение полицейского на проверку"), c->toUnicode("На данную проверку полицейский уже назначен!"));
	}
}

void systemOfBribe::addDoom()
{
	if (uiCreateCheck.endSpecList->findItems(c->toUnicode("Специалист от думы"), Qt::MatchContains).isEmpty())
	{
		if (uiCreateCheck.doomList->currentItem() != NULL)
		{
			QListWidgetItem* doom = uiCreateCheck.doomList->takeItem(uiCreateCheck.doomList->currentRow());
			doom->setText(c->toUnicode("Специалист от думы: ") + doom->text());
			uiCreateCheck.endSpecList->addItem(doom);
		}
	}
	else
	{
		QMessageBox::warning(this, c->toUnicode("Назначение специалиста от думы на проверку"), c->toUnicode("На данную проверку специалист от думы уже назначен!"));
	}
}

void systemOfBribe::deleteSpecialist()
{
	if (uiCreateCheck.endSpecList->currentItem() != NULL)
	{
		QListWidgetItem* spec = uiCreateCheck.endSpecList->takeItem(uiCreateCheck.endSpecList->currentRow());
		QString text = spec->text();
		if (text.contains(c->toUnicode("Врач: ")))
		{
			text.remove(c->toUnicode("Врач: "));
			uiCreateCheck.doctorList->addItem(spec);
		}
		else if (text.contains(c->toUnicode("Полицейский: ")))
		{
			text.remove(c->toUnicode("Полицейский: "));
			uiCreateCheck.policeList->addItem(spec);
		}
		else
		{
			text.remove(c->toUnicode("Специалист от думы: "));
			uiCreateCheck.doomList->addItem(spec);
		}
		spec->setText(text);
	}
}

void systemOfBribe::updateDataCompany()
{
	client.get_companyCheckData(id);
}

void systemOfBribe::updateDataCompany(QListWidgetItem* company)
{
	client.get_companyCheckData(company->data(Qt::UserRole).toInt());
}

void systemOfBribe::setCriteriaCompany(QMap<QString, QVariant> criteria, QString characteristic)
{
	if (currentUser == CLIENT_COMPANY)
	{
		uiMain.progressBarGoods->setValue(criteria.value("goods").toInt());
		uiMain.progressBarSanitation->setValue(criteria.value("sanitation").toInt());
		uiMain.progressBarSecurity->setValue(criteria.value("security").toInt());
		uiMain.progressBarConsumerRights->setValue(criteria.value("consumerRights").toInt());
		
		uiMain.characterCheck->setText(characteristic);
	}
	else
	{
		if (uiMain.tabDeputy->currentIndex() == 0)
		{
			uiMain.goodsMark->setValue(criteria.value("goods").toInt());
			uiMain.sanitaryMark->setValue(criteria.value("sanitation").toInt());
			uiMain.securityMark->setValue(criteria.value("security").toInt());
			uiMain.consumerRightsMark->setValue(criteria.value("consumerRights").toInt());
		}
		else
		{
			uiMain.infoGoods->setValue(criteria.value("goods").toInt());
			uiMain.infoSanitary->setValue(criteria.value("sanitation").toInt());
			uiMain.infoSecurity->setValue(criteria.value("security").toInt());
			uiMain.infoConsumerRights->setValue(criteria.value("consumerRights").toInt());

			uiMain.infoCharacteristic->setText(characteristic);
		}
	}
}