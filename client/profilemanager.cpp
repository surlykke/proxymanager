#include "profilemanager.h"
#include "profilelistmodel.h"
#include "ui_profilemanager.h"
#include <QtDebug>
#include <QList>
#include <QtCore>

ProfileManager::ProfileManager(QList<Profile> profiles, QWidget * parent) :  QDialog(parent), m_ui(new Ui::ProfileManager), profileListModel(profiles) {
  m_doing_selection_change = false;
  m_ui->setupUi(this);
  m_ui->profileBox->setEnabled(false);
  m_ui->deleteProfileButton->setEnabled(false);
  m_ui->profileList->setEnabled(true);
  m_ui->profileList->setModel(&profileListModel);
  connect(m_ui->profileList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
  connect(m_ui->newProfileButton, SIGNAL(clicked()), this, SLOT(newProfile()));
  connect(m_ui->deleteProfileButton, SIGNAL(clicked()), this, SLOT(deleteProfile()));
  connect(m_ui->profileManagerButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_ui->profileManagerButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_ui->profileNameInput, SIGNAL(textChanged(QString)), this, SLOT(profileDataChanged()));
  connect(m_ui->useProxyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(profileDataChanged()));
  connect(m_ui->proxyHostInput, SIGNAL(textChanged(QString)), this, SLOT(profileDataChanged()));
  connect(m_ui->proxyPortInput, SIGNAL(valueChanged(int)), this, SLOT(profileDataChanged()));
  connect(m_ui->newHostExceptionButton, SIGNAL(clicked()), this, SLOT(newHostException()));
  connect(m_ui->deleteHostExceptionButton, SIGNAL(clicked()), this, SLOT(deleteHostException()));
  connect(m_ui->newDomainExceptionButton, SIGNAL(clicked()), this, SLOT(newDomainException()));
  connect(m_ui->deleteDomainExceptionButton, SIGNAL(clicked()), this, SLOT(deleteDomainException()));
}

ProfileManager::~ProfileManager() {
  delete m_ui;
}

QList<Profile> & ProfileManager::getProfiles() {
  return profileListModel.getProfiles();
}


void ProfileManager::selectionChanged(QItemSelection selected, QItemSelection deselected) {
  Q_UNUSED(deselected);
  Q_UNUSED(selected);
  int row;
  if ((row = currentSelection()) >= 0) {
    qDebug() << "Sætter ny selection" << row;
    m_ui->deleteProfileButton->setEnabled(true);
    m_ui->profileBox->setEnabled(true);
    qDebug() << "Setting name: " << profileListModel[row].name << ", useProxy: " << profileListModel[row].useProxy ;
    m_doing_selection_change = true;
    m_ui->profileNameInput->setText(profileListModel[row].name);
    m_ui->useProxyCheckBox->setChecked(profileListModel[row].useProxy);
    m_ui->proxyData->setEnabled(profileListModel[row].useProxy);
    m_ui->proxyHostInput->setText(profileListModel[row].proxyHost);
    m_ui->proxyPortInput->setValue(profileListModel[row].proxyPort);
    m_ui->hostExceptionsList->clear();
    m_ui->hostExceptionsList->addItems(profileListModel[row].hostExceptions);
    m_ui->domainExceptionsList->clear();
    m_ui->domainExceptionsList->addItems(profileListModel[row].domainExceptions);
    m_doing_selection_change = false;
    qDebug() << "Færdig med at sætte ny selection";
  }
  else {
    m_ui->deleteProfileButton->setEnabled(false);
    m_ui->profileNameInput->clear();
    m_ui->useProxyCheckBox->setChecked(false);
    m_ui->proxyHostInput->clear();
    m_ui->proxyPortInput->clear();
    m_ui->profileBox->setEnabled(false);
    // Disable all
  }
}

void ProfileManager::newProfile() {
  QModelIndex index = profileListModel.addProfile();
  m_ui->profileList->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Clear | QItemSelectionModel::Select);
  m_ui->profileNameInput->setFocus();
  int length = m_ui->profileNameInput->text().length();
  m_ui->profileNameInput->setSelection(0, length);

}

void ProfileManager::deleteProfile() {
  profileListModel.removeRow(currentSelection(), QModelIndex());
}

void ProfileManager::profileDataChanged() {
  if (m_doing_selection_change) return;
  int row = currentSelection();
  if (row < 0) return;
  profileListModel[row].name = m_ui->profileNameInput->text();
  profileListModel[row].useProxy = m_ui->useProxyCheckBox->isChecked();
  m_ui->proxyData->setEnabled(profileListModel[row].useProxy);
  profileListModel[row].proxyHost = m_ui->proxyHostInput->text();
  profileListModel[row].proxyPort = m_ui->proxyPortInput->value();
  profileListModel[row].hostExceptions.clear();
  for (int i = 0; i < m_ui->hostExceptionsList->count(); i++) {
    profileListModel[row].hostExceptions << m_ui->hostExceptionsList->item(i)->text();
  }
  profileListModel[row].domainExceptions.clear();
  for (int i = 0; i < m_ui->domainExceptionsList->count(); i++) {
    profileListModel[row].domainExceptions << m_ui->domainExceptionsList->item(i)->text();
  }
  profileListModel.profileUpdated(row);
}

void ProfileManager::newHostException() {
  bool ok;
  QString exception = QInputDialog::getText(this, "New host exception", "address:", QLineEdit::Normal, "", &ok);
  if (ok && !exception.isEmpty()) {
    int row = m_ui->hostExceptionsList->count();
    m_ui->hostExceptionsList->insertItem(row, exception);
    profileDataChanged();
  }
}


void ProfileManager::deleteHostException() {
  int row = m_ui->hostExceptionsList->currentRow();
  if (row >= 0) {
    delete m_ui->hostExceptionsList->takeItem(row);
    profileDataChanged();
  }
}

void ProfileManager::newDomainException() {
  bool ok;
  QString exception = QInputDialog::getText(this, "New domain exception", "address:", QLineEdit::Normal, "", &ok);
  if (ok && !exception.isEmpty()) {
    int row = m_ui->domainExceptionsList->count();
    m_ui->domainExceptionsList->insertItem(row, exception);
    profileDataChanged();
  }
}

void ProfileManager::deleteDomainException() {
  int row = m_ui->domainExceptionsList->currentRow();
  if (row >= 0) {
    delete m_ui->domainExceptionsList->takeItem(row);
    profileDataChanged();
  }
}


void ProfileManager::changeEvent(QEvent *e) {
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    m_ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

int ProfileManager::currentSelection() {
  QModelIndexList indexes = m_ui->profileList->selectionModel()->selectedRows(0);
  if (! indexes.isEmpty()) {
    return indexes[0].row();
  }
  else {
    return -1;
  }
}

void ProfileManager::showProfiles(QString msg) {
  qDebug() << msg;
  for (int i = 0; i < profileListModel.getProfiles().size(); i++) {
    qDebug() << i << ": name " << profileListModel[i].name << ", useProxy " << profileListModel[i].useProxy;
  }
}



