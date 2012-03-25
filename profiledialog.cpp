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
#include "ui_profilemanager.h"
#include <QtDebug>
#include <QList>
#include <QtCore>
#include <QModelIndex>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QListWidgetItem>

ProfileDialog::ProfileDialog(QString profileName, QWidget * parent) :  QDialog(parent), m_ui(new Ui::ProfileManager), selectedProfileName(profileName) {
    settings.beginGroup("profiles");
    foreach (QString key, settings.allKeys()) {
        rollbackPoint.insert(key, settings.value(key));
    }

    m_ui->setupUi(this);
    m_ui->profileBox->setEnabled(false);
    m_ui->deleteProfileButton->setEnabled(false);

    connect(m_ui->profileList, SIGNAL(itemSelectionChanged()), this, SLOT(loadProfile()));
    connect(m_ui->newProfileButton, SIGNAL(clicked()), this, SLOT(newProfile()));
    connect(m_ui->deleteProfileButton, SIGNAL(clicked()), this, SLOT(deleteProfile()));


    connect(m_ui->useProxyCheckBox, SIGNAL(toggled(bool)), m_ui->hostInput, SLOT(setEnabled(bool)));
    connect(m_ui->useProxyCheckBox, SIGNAL(toggled(bool)), m_ui->portInput, SLOT(setEnabled(bool)));
    connect(m_ui->useProxyCheckBox, SIGNAL(toggled(bool)), m_ui->exceptionsInput, SLOT(setEnabled(bool)));

    connect(m_ui->useAuthenticationCheckBox, SIGNAL(toggled(bool)), m_ui->domainInput, SLOT(setEnabled(bool)));
    connect(m_ui->useAuthenticationCheckBox, SIGNAL(toggled(bool)), m_ui->userIdInput, SLOT(setEnabled(bool)));
    connect(m_ui->useAuthenticationCheckBox, SIGNAL(toggled(bool)), m_ui->passwordInput, SLOT(setEnabled(bool)));

    connect(m_ui->useProxyCheckBox, SIGNAL(toggled(bool)), this, SLOT(saveProfile()));
    connect(m_ui->hostInput, SIGNAL(textChanged(QString)), this, SLOT(saveProfile()));
    connect(m_ui->portInput, SIGNAL(valueChanged(int)), this, SLOT(saveProfile()));
    connect(m_ui->exceptionsInput, SIGNAL(textChanged(QString)), this, SLOT(saveProfile()));
    connect(m_ui->useAuthenticationCheckBox, SIGNAL(toggled(bool)), this, SLOT(saveProfile()));
    connect(m_ui->domainInput, SIGNAL(textChanged(QString)), this, SLOT(saveProfile()));
    connect(m_ui->userIdInput, SIGNAL(textChanged(QString)), this, SLOT(saveProfile()));
    connect(m_ui->passwordInput, SIGNAL(textChanged(QString)), this, SLOT(saveProfile()));

    connect(m_ui->profileList, SIGNAL(currentItemChanged(QListWidgetItem* ,QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(m_ui->profileList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));

    connect(m_ui->profileManagerButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(closeOrResetButtonClicked(QAbstractButton*)));

    loadProfiles();
    setSelection(selectedProfileName);
}

void ProfileDialog::setSelection(QString profileName) {
    m_ui->profileList->clearSelection();
    for (int row = 0; row < m_ui->profileList->count(); row++) {
        qDebug() << "Sammenligner: " << profileName << " med " << m_ui->profileList->item(row)->text();
        if (profileName == m_ui->profileList->item(row)->text()) {
            m_ui->profileList->setCurrentRow(row);
            break;
        }
    }
}

ProfileDialog::~ProfileDialog() {
    delete m_ui;
}

void ProfileDialog::loadProfiles() {
    qDebug() << "loadProfiles, selectedProfileName: " << selectedProfileName;
    currentProfileName = "";
    m_ui->profileList->clear();
    foreach (QString profileName, settings.childGroups()) {
        addItem(profileName);
    }
    setSelection(selectedProfileName);
}


QListWidgetItem* ProfileDialog::addItem(QString profileName) {
    m_ui->profileList->addItem(profileName);
    QListWidgetItem *item = m_ui->profileList->item(m_ui->profileList->count() - 1);
    item->setFlags(Qt::ItemIsEditable | item->flags());
    return item;
}

void ProfileDialog::loadProfile() {
    if (lock.tryLock()) {
    settings.beginGroup(currentProfileName);
    m_ui->useProxyCheckBox->setChecked(settings.value(USEPROXY).toBool());
    m_ui->hostInput->setText(settings.value(HOST).toString());
    m_ui->portInput->setValue(settings.value(PORT).toInt());
    m_ui->exceptionsInput->setText(settings.value(EXCEPTIONS).toString());
    m_ui->useAuthenticationCheckBox->setChecked(settings.value(USEAUTHENTICATION).toBool());
    m_ui->domainInput->setText(settings.value(NTDOMAIN).toString());
    m_ui->userIdInput->setText(settings.value(USERID).toString());
    m_ui->passwordInput->setText(settings.value(PASSWORD).toString());
    settings.endGroup();
    lock.unlock();
    }
}

void ProfileDialog::saveProfile()
{
    if (lock.tryLock()) {
    qDebug() << "Ind i saveProfile - currentProfileName: " << currentProfileName;
    settings.beginGroup(currentProfileName);
    settings.setValue(USEPROXY, m_ui->useProxyCheckBox->isChecked());
    settings.setValue(HOST, m_ui->hostInput->text());
    settings.setValue(PORT, m_ui->portInput->value());
    settings.setValue(EXCEPTIONS, m_ui->exceptionsInput->text());
    settings.setValue(USEAUTHENTICATION, m_ui->useAuthenticationCheckBox->isChecked());
    settings.setValue(NTDOMAIN, m_ui->domainInput->text());
    settings.setValue(USERID, m_ui->userIdInput->text());
    settings.setValue(PASSWORD, m_ui->passwordInput->text());
    settings.endGroup();
    lock.unlock();
    }
}



void ProfileDialog::itemChanged(QListWidgetItem *item, QListWidgetItem *oldItem) {
    qDebug() << "Ind i itemChanged";
        if (item == 0) {
            qDebug() << "Deselection..";
            m_ui->deleteProfileButton->setEnabled(false);
            m_ui->profileBox->setEnabled(false);
        }
        else {
            m_ui->deleteProfileButton->setEnabled(true);
            m_ui->profileBox->setEnabled(true);
            if (oldItem != item) {
                qDebug() << "Row selected: " << item->text();
                currentProfileName = item->text();
                loadProfile();
            }
        }
        qDebug() << "Ud af itemChanged";
}


void ProfileDialog::itemChanged(QListWidgetItem *item) {
    if (currentProfileName != "" && currentProfileName != item->text()) {
        qDebug() << "Ind i den anden itemChanged";
        qDebug() << "Name edited: " << currentProfileName << " --> " << item->text();
        settings.remove(currentProfileName);
        item->setText(newName(item->text())); // Ensure uniqueness
        currentProfileName = item->text();
        saveProfile();
    }
}

void ProfileDialog::newProfile() {
    qDebug() << "Ind i newProfile";
    currentProfileName = "";
    QListWidgetItem *item = addItem(newName());
    m_ui->profileList->setCurrentItem(item);
    currentProfileName = item->text();
    saveProfile();
    m_ui->profileList->editItem(item);
    qDebug() << "Ud af newProfile";
}


QString ProfileDialog::newName(QString hint) {
    qDebug() << "C: " << settings.group() << ", childgroups: " << settings.childGroups();
    hint = hint.trimmed();
    if (hint == "") {
        hint = "Profile";
    }
    if (! settings.childGroups().contains(hint)) {
        return hint;
    }
    else {
        for (int i = 2; ; i++) {
            QString newSuggestion = hint + " (" + QString::number(i) + ")";
            if (! settings.childGroups().contains(newSuggestion)) {
                return newSuggestion;
            }
        }
    }
}


void ProfileDialog::deleteProfile() {
    if (m_ui->profileList->currentItem() != 0) {
        settings.remove(currentProfileName);
        delete m_ui->profileList->currentItem();
    }
}


void ProfileDialog::closeOrResetButtonClicked(QAbstractButton *button) {
    QDialogButtonBox::ButtonRole role = m_ui->profileManagerButtonBox->buttonRole(button);
    if (role == QDialogButtonBox::ResetRole) {
        qDebug() << "Reset";
        settings.remove("");
        foreach (QString key, rollbackPoint.keys()) {
            settings.setValue(key, rollbackPoint.value(key));
        }
        loadProfiles();
    }
    else {
        qDebug() << "Close";
        close();
    }
}
