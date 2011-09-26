#include "profilemanager.h"
#include "profilelistmodel.h"
#include "ui_profilemanager.h"
#include <QtDebug>
#include <QList>
#include <QtCore>
#include <QModelIndex>
#include <QStringList>

ProfileManager::ProfileManager(ProfileListModel* profileListModel, QWidget * parent) :  QDialog(parent), m_ui(new Ui::ProfileManager) {
    this->profileListModel = profileListModel;
    updating_ui = false;
    m_ui->setupUi(this);
    m_ui->profileBox->setEnabled(false);
    m_ui->deleteProfileButton->setEnabled(false);
    m_ui->profileList->setEnabled(true);
    m_ui->profileList->setModel(profileListModel);
    connect(m_ui->profileList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            profileListModel, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    connect(profileListModel, SIGNAL(selectedProfileChanged(Profile*)), this, SLOT(showProfile(Profile*)));
    connect(m_ui->newProfileButton, SIGNAL(clicked()), this, SLOT(newProfile()));
    connect(m_ui->deleteProfileButton, SIGNAL(clicked()), this, SLOT(deleteProfile()));
    connect(m_ui->profileManagerButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_ui->profileManagerButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(m_ui->profileNameInput, SIGNAL(textChanged(QString)), profileListModel, SLOT(nameChanged(QString)));
    connect(m_ui->useProxyCheckBox, SIGNAL(stateChanged(int)), profileListModel, SLOT(useProxyChanged(int)));
    connect(m_ui->proxyHostInput, SIGNAL(editingFinished()), profileListModel, SLOT(proxyHostChanged(QString)));
    connect(m_ui->proxyPortInput, SIGNAL(editingFinished()), profileListModel, SLOT(proxyPortChanged(int)));
    connect(m_ui->newHostExceptionButton, SIGNAL(clicked()), this, SLOT(newHostException()));
    connect(m_ui->deleteHostExceptionButton, SIGNAL(clicked()), this, SLOT(deleteHostException()));
    connect(m_ui->newDomainExceptionButton, SIGNAL(clicked()), this, SLOT(newDomainException()));
    connect(m_ui->deleteDomainExceptionButton, SIGNAL(clicked()), this, SLOT(deleteDomainException()));

    if (profileListModel->selectedProfile >= 0) {
        m_ui->profileList->selectionModel()->select(profileListModel->currentIndex(), QItemSelectionModel::Clear | QItemSelectionModel::Select);
    }
}

ProfileManager::~ProfileManager() {
    delete m_ui;
}


void ProfileManager::showProfile(Profile* profile) {
    updating_ui = true;
    if (profile != 0) {
        qDebug() << "Showing profile: " << *profile;
        m_ui->deleteProfileButton->setEnabled(true);
        m_ui->profileBox->setEnabled(true);
        m_ui->profileNameInput->setText(profile->name);
        m_ui->useProxyCheckBox->setChecked(profile->useProxy);
        m_ui->proxyData->setEnabled(profile->useProxy);
        m_ui->proxyHostInput->setText(profile->proxyHost);
        m_ui->proxyPortInput->setValue(profile->proxyPort);
        m_ui->hostExceptionsList->clear();
        m_ui->hostExceptionsList->addItems(profile->hostExceptions);
        m_ui->domainExceptionsList->clear();
        m_ui->domainExceptionsList->addItems(profile->domainExceptions);
    }
    else {
        // Disable all
        m_ui->deleteProfileButton->setEnabled(false);
        m_ui->profileNameInput->clear();
        m_ui->useProxyCheckBox->setChecked(false);
        m_ui->proxyHostInput->clear();
        m_ui->proxyPortInput->clear();
        m_ui->profileBox->setEnabled(false);
    }
    updating_ui = false;
}

void ProfileManager::newProfile() {
    profileListModel->newProfile();
    m_ui->profileList->selectionModel()->setCurrentIndex(profileListModel->currentIndex(), QItemSelectionModel::Clear | QItemSelectionModel::Select);
    m_ui->profileNameInput->setFocus();
    int length = m_ui->profileNameInput->text().length();
    m_ui->profileNameInput->setSelection(0, length);

}

void ProfileManager::deleteProfile() {
    profileListModel->removeRow(currentSelection(), QModelIndex());
}



void ProfileManager::newHostException() {
    bool ok;
    QString exception = QInputDialog::getText(this, "New host exception", "address:", QLineEdit::Normal, "", &ok);
    if (ok && !exception.isEmpty()) {
        int row = m_ui->hostExceptionsList->count();
        m_ui->hostExceptionsList->insertItem(row, exception);
        profileListModel->hostExceptionListChanged(widgetListData(m_ui->hostExceptionsList));
   }
}

void ProfileManager::deleteHostException() {
    int row = m_ui->hostExceptionsList->currentRow();
    if (row >= 0) {
        delete m_ui->hostExceptionsList->takeItem(row);
         profileListModel->hostExceptionListChanged(widgetListData(m_ui->hostExceptionsList));
   }
}

void ProfileManager::newDomainException() {
    bool ok;
    QString exception = QInputDialog::getText(this, "New domain exception", "address:", QLineEdit::Normal, "", &ok);
    if (ok && !exception.isEmpty()) {
        int row = m_ui->domainExceptionsList->count();
        m_ui->domainExceptionsList->insertItem(row, exception);
        profileListModel->domainExceptionListChanged(widgetListData(m_ui->domainExceptionsList));
   }
}

void ProfileManager::deleteDomainException() {
    int row = m_ui->domainExceptionsList->currentRow();
    if (row >= 0) {
        delete m_ui->domainExceptionsList->takeItem(row);
        profileListModel->domainExceptionListChanged(widgetListData(m_ui->domainExceptionsList));
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
    for (int i = 0; i < profileListModel->profiles.size(); i++) {
        qDebug() << i << ": name " << profileListModel->profiles[i].name << ", useProxy " << profileListModel->profiles[i].useProxy;
    }
}

void ProfileManager::accept() {
    profileListModel->saveProfiles();
    QDialog::accept();
}

void ProfileManager::reject() {
    profileListModel->loadProfiles();
    QDialog::reject();
}

QStringList ProfileManager::widgetListData(QListWidget *list) {
    QStringList result;
    for (int i = 0; i < list->count(); i++) {
        result << list->item(i)->text();
    }
    return result;
}
