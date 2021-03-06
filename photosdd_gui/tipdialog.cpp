#include "tipdialog.h"
#include "ui_tipdialog.h"
#include <QSettings>

TipDialog::TipDialog(QWidget *parent) :
	QDialog(parent), ui(new Ui::TipDialog) {
	ui->setupUi(this);
	tipNumber = 0;
	loadConfig();
	loadTips();
	setTip();
}

TipDialog::~TipDialog() {
	this->nextTip();
	saveConfig();
	delete ui;
}

void TipDialog::startUp() {
	if (ui->checkBox->isChecked())
		this->exec();
}

void TipDialog::setTip() {
	if (tipNumber > tips.size() - 1)
		tipNumber = 0;
	if (tipNumber < 0)
		tipNumber = tips.size() - 1;
	ui->textField->setText(tips.at(tipNumber));
}

void TipDialog::nextTip() {
	tipNumber++;
	setTip();
}

void TipDialog::previousTip() {
	tipNumber--;
	setTip();
}

/*
 Loads a list of tips to show them in the Dialog.
 */
void TipDialog::loadTips() {
	tips.append(
			QString("Functions of this program are organized in plugins.<br/>"
				"You have to config and activate plugins first, to use them."));
	tips.append(
			QString("Activated plugins can be configured through their widget."));
}

/*
 Loads configuration
 */
void TipDialog::loadConfig() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	qDebug("[TipDialog] settings path: %s / %s",
			qPrintable(QCoreApplication::organizationName()),
			qPrintable(QCoreApplication::applicationName()));

	ui->checkBox->setChecked(settings.value("showTips", true).toBool());
	tipNumber = settings.value("tipNumber", QVariant(0)).toInt();
	qDebug("[TipDialog] loaded config for TipDialog");
}

/*
 Saves configuration
 Windows saves to regestry.
 Unix to config file in home folder
 */
void TipDialog::saveConfig() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	qDebug("[TipDialog] settings path: %s / %s",
			qPrintable(QCoreApplication::organizationName()),
			qPrintable(QCoreApplication::applicationName()));
	settings.setValue("showTips", ui->checkBox->isChecked());
	settings.setValue("tipNumber", QVariant(tipNumber));
	qDebug("[TipDialog] saved config for TipDialog");
}

void TipDialog::on_previousButton_clicked() {
	this->previousTip();
}

void TipDialog::on_nextButton_clicked() {
	this->nextTip();
}
