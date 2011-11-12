#include "profiledialog.h"
#include "profilelistmodel.h"
#include "ui_profilemanager.h"
#include <QtDebug>
#include <QList>
#include <QtCore>
#include <QModelIndex>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>

ProfileDialog::ProfileDialog(QString selectedProfileId, ProfileListModel* profileListModel, QWidget * parent) :  QDialog(parent), m_ui(new Ui::ProfileManager) {
    this->profileListModel = profileListModel;
    m_ui->setupUi(this);
    m_ui->profileBox->setEnabled(false);

    m_ui->deleteProfileButton->setEnabled(false);
    m_ui->profileList->setModel(profileListModel);
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(profileListModel);
    mapper->addMapping(m_ui->useHttpProxyCheckBox, ProfileListModel::USE_HTTP);
    mapper->addMapping(m_ui->httpProxyHostInput, ProfileListModel::HTTP_HOST);
    mapper->addMapping(m_ui->httpProxyPortInput, ProfileListModel::HTTP_PORT);
    mapper->addMapping(m_ui->useHttpsProxyCheckBox, ProfileListModel::USE_HTTPS);
    mapper->addMapping(m_ui->httpsProxyHostInput, ProfileListModel::HTTPS_HOST);
    mapper->addMapping(m_ui->httpsProxyPortInput, ProfileListModel::HTTPS_PORT);
    mapper->addMapping(m_ui->useFtpProxyCheckBox, ProfileListModel::USE_FTP);
    mapper->addMapping(m_ui->ftpProxyHostInput, ProfileListModel::FTP_HOST);
    mapper->addMapping(m_ui->ftpProxyPortInput, ProfileListModel::FTP_PORT);
    mapper->addMapping(m_ui->exceptionsInput, ProfileListModel::EXCEPTIONS);
    connect(m_ui->profileList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    connect(m_ui->newProfileButton, SIGNAL(clicked()), this, SLOT(newProfile()));
    connect(m_ui->deleteProfileButton, SIGNAL(clicked()), this, SLOT(deleteProfile()));
    connect(m_ui->sameProxyButton, SIGNAL(clicked()), this, SLOT(sameProxy()));
    connect(m_ui->profileManagerButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_ui->profileManagerButtonBox, SIGNAL(rejected()), this, SLOT(reject()));


    connect(m_ui->useHttpProxyCheckBox, SIGNAL(toggled(bool)), m_ui->httpProxyHostInput, SLOT(setEnabled(bool)));
    connect(m_ui->useHttpProxyCheckBox, SIGNAL(toggled(bool)), m_ui->httpProxyPortInput, SLOT(setEnabled(bool)));

    connect(m_ui->useHttpsProxyCheckBox, SIGNAL(toggled(bool)), m_ui->httpsProxyHostInput, SLOT(setEnabled(bool)));
    connect(m_ui->useHttpsProxyCheckBox, SIGNAL(toggled(bool)), m_ui->httpsProxyPortInput, SLOT(setEnabled(bool)));

    connect(m_ui->useFtpProxyCheckBox, SIGNAL(toggled(bool)), m_ui->ftpProxyHostInput, SLOT(setEnabled(bool)));
    connect(m_ui->useFtpProxyCheckBox, SIGNAL(toggled(bool)), m_ui->ftpProxyPortInput, SLOT(setEnabled(bool)));

    for (int row = 0; row < profileListModel->rowCount(); row++) {
        if (profileListModel->id(row) == selectedProfileId) {
            select(row);
            break;
        }
    }
}

ProfileDialog::~ProfileDialog() {
    delete m_ui;
}

void ProfileDialog::selectionChanged(QItemSelection selected, QItemSelection deselected) {
    Q_UNUSED(deselected);
    QModelIndexList indexes = selected.indexes();
    if (indexes.size() != 1) {
        m_ui->profileBox->setEnabled(false);
        m_ui->deleteProfileButton->setEnabled(false);
        return;
    }
    else {
        m_ui->profileBox->setEnabled(true);
        m_ui->deleteProfileButton->setEnabled(true);
        mapper->setCurrentIndex(indexes.at(0).row());
    }
}

void ProfileDialog::newProfile() {
    profileListModel->newProfile();
    select(profileListModel->rowCount() - 1, true);
}

void ProfileDialog::deleteProfile() {
    profileListModel->removeRow(currentSelection(), QModelIndex());
}

void ProfileDialog::sameProxy() {
    m_ui->sameProxyButton->setFocus();
    profileListModel->sameProxy(mapper->currentIndex());
}


int ProfileDialog::currentSelection() {
    QModelIndexList indexes = m_ui->profileList->selectionModel()->selectedIndexes();
    if (! indexes.isEmpty()) {
        return indexes[0].row();
    }
    else {
        return -1;
    }
}


void ProfileDialog::accept() {
    profileListModel->saveProfiles();
    QDialog::accept();
}

void ProfileDialog::reject() {
    profileListModel->loadProfiles();
    QDialog::reject();
}


void ProfileDialog::select(int row, bool edit) {
    QModelIndex index = profileListModel->index(row, ProfileListModel::NAME);
    m_ui->profileList->selectionModel()->select(index, QItemSelectionModel::Select);
    if (edit) {
        m_ui->profileList->edit(index);
    }
}
