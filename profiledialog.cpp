/*
  Copyright 2011, 2012 Christian Surlykke

  This file is part of ProxyManager

  ProxyManager is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3

  ProxyManager is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ProxyManager.  If not, see <http://www.gnu.org/licenses/>.
*/
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
#include <QMessageBox>

ProfileDialog::ProfileDialog(QString selectedProfileId, ProfileListModel* profileListModel, QWidget * parent) :  QDialog(parent), m_ui(new Ui::ProfileManager) {
    this->profileListModel = profileListModel;
    m_ui->setupUi(this);
    m_ui->profileBox->setEnabled(false);

    m_ui->deleteProfileButton->setEnabled(false);
    m_ui->profileList->setModel(profileListModel);
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(profileListModel);
    mapper->addMapping(m_ui->useProxyCheckBox, ProfileListModel::USE_PROXY);
    mapper->addMapping(m_ui->hostInput, ProfileListModel::HOST);
    mapper->addMapping(m_ui->portInput, ProfileListModel::PORT);
    mapper->addMapping(m_ui->exceptionsInput, ProfileListModel::EXCEPTIONS);
    mapper->addMapping(m_ui->useAuthenticationCheckBox, ProfileListModel::USE_AUTHENTICATION);
    mapper->addMapping(m_ui->domainInput, ProfileListModel::NTDOMAIN);
    mapper->addMapping(m_ui->userIdInput, ProfileListModel::USERID);
    mapper->addMapping(m_ui->passwordInput, ProfileListModel::PASSWORD);
    connect(m_ui->profileList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    connect(m_ui->newProfileButton, SIGNAL(clicked()), this, SLOT(newProfile()));
    connect(m_ui->deleteProfileButton, SIGNAL(clicked()), this, SLOT(deleteProfile()));
    connect(m_ui->profileManagerButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_ui->profileManagerButtonBox, SIGNAL(rejected()), this, SLOT(reject()));


    connect(m_ui->useProxyCheckBox, SIGNAL(toggled(bool)), m_ui->hostInput, SLOT(setEnabled(bool)));
    connect(m_ui->useProxyCheckBox, SIGNAL(toggled(bool)), m_ui->portInput, SLOT(setEnabled(bool)));
    connect(m_ui->useProxyCheckBox, SIGNAL(toggled(bool)), m_ui->exceptionsInput, SLOT(setEnabled(bool)));

    connect(m_ui->useAuthenticationCheckBox, SIGNAL(toggled(bool)), m_ui->domainInput, SLOT(setEnabled(bool)));
    connect(m_ui->useAuthenticationCheckBox, SIGNAL(toggled(bool)), m_ui->userIdInput, SLOT(setEnabled(bool)));
    connect(m_ui->useAuthenticationCheckBox, SIGNAL(toggled(bool)), m_ui->passwordInput, SLOT(setEnabled(bool)));

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
   if (profileListModel->newProfile()) {
        select(profileListModel->rowCount() - 1, true);
   }
   else {
       QMessageBox(QMessageBox::Critical, "Error creating new profile...", profileListModel->errorMsg, QMessageBox::NoButton, this).exec();
   }

}

void ProfileDialog::deleteProfile() {
    profileListModel->deleteProfile(currentSelection());
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
    profileListModel->commit();
    QDialog::accept();
}

void ProfileDialog::reject() {
    profileListModel->rollback();
    QDialog::reject();
}


void ProfileDialog::select(int row, bool edit) {
    m_ui->profileList->selectionModel()->clearSelection();
    QModelIndex index = profileListModel->index(row, ProfileListModel::NAME);
    m_ui->profileList->selectionModel()->select(index, QItemSelectionModel::Select);
    if (edit) {
        m_ui->profileList->edit(index);
    }
}
