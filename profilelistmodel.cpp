/*
 * profilelistmodel.cpp
 *
 *  Created on: 12/03/2010j
 *      Author: christian
 */

#include <QUuid>
#include "profilelistmodel.h"
#include <QMessageBox>
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include <QFile>
#include <QSettings>


ProfileListModel::ProfileListModel(QObject *parent) : QStandardItemModel(parent) {
    loadProfiles();
}

ProfileListModel::~ProfileListModel() {
    setColumnCount(COLUMNS);
}

QString ProfileListModel::id(int row) {
    return item(row, ID)->data(Qt::DisplayRole).toString();
}

QString ProfileListModel::name(int row) {
    return item(row, NAME)->data(Qt::DisplayRole).toString();
}

bool ProfileListModel::exists(QString id) {
    for (int row = 0; row < rowCount(); row++) {
        if (id == item(row, ID)->data(Qt::DisplayRole).toString()) {
            return true;
        }
    }
    return false;
}

void ProfileListModel::sameProxy(int row) {
    if (row >= 0 && row < rowCount()) {
        item(row, USE_HTTPS)->setData(item(row, USE_HTTP)->data(Qt::DisplayRole), Qt::DisplayRole);
        item(row, HTTPS_HOST)->setData(item(row, HTTP_HOST)->data(Qt::DisplayRole), Qt::DisplayRole);
        item(row, HTTPS_PORT)->setData(item(row, HTTP_PORT)->data(Qt::DisplayRole), Qt::DisplayRole);
        item(row, USE_FTP)->setData(item(row, USE_HTTP)->data(Qt::DisplayRole), Qt::DisplayRole);
        item(row, FTP_HOST)->setData(item(row, HTTP_HOST)->data(Qt::DisplayRole), Qt::DisplayRole);
        item(row, FTP_PORT)->setData(item(row, HTTP_PORT)->data(Qt::DisplayRole), Qt::DisplayRole);
    }
}

bool ProfileListModel::newProfile() {
    QVariantMap profile;
    QString id = QUuid::createUuid().toString();
    // Strip leading '{' and trailing '}'
    id = id.right(id.size() - 1);
    id = id.left(id.size() - 1);
    qDebug() << "id: " << id;
    profile["id"] = id;
    profile["name"] = "New profile...";
    QFile settingsFile(profilesDir.absolutePath() + "/" + id);
    settingsFile.open(QIODevice::WriteOnly);
    settingsFile.write(QJson::Serializer().serialize(profile));
    settingsFile.close();
    createdProfiles << id;
    append(profile);
    return true;
}

void ProfileListModel::deleteProfile(int row) {
    if (row >= 0 && row < rowCount()) {
        pendingDeletes << item(row, ID)->data(Qt::DisplayRole).toString();
        removeRow(row, QModelIndex());
    }
}

QVariantMap ProfileListModel::id2map(QString id) {
    for (int row = 0; row < rowCount(); row++) {
        if (id == item(row, ID)->data(Qt::DisplayRole).toString()) {
            return row2Map(row);
        }
    }
    return QVariantMap();
}

bool ProfileListModel::loadProfiles() {
    QStringList settingsFiles = profilesDir.entryList(QDir::Files);
    QStringList::Iterator it;
    QJson::Parser parser;
    for (it = settingsFiles.begin(); it != settingsFiles.end(); it++) {
        qDebug() << "Læser: " << profilesDir.absoluteFilePath(*it);
        QFile settingsFile(profilesDir.absoluteFilePath(*it));
        QVariantMap settings = QJson::Parser().parse(&settingsFile).toMap();
        qDebug() << "Appender: " << settings;
        append(settings);
    }

    pendingDeletes.clear();
    createdProfiles.clear();
    return true;
}

bool ProfileListModel::commit() {
    QStringList::iterator it;
    for (it = pendingDeletes.begin(); it != pendingDeletes.end(); it++) {
        QFile(profilesDir.absolutePath() + "/" + *it).remove();
    }

    // FIXME only save changed profiles
    for (int row = 0; row < rowCount(); row++) {
        QFile settingsFile(profilesDir.absolutePath() + "/" + item(row, ID)->data(Qt::DisplayRole).toString());
        settingsFile.open(QIODevice::WriteOnly);
        settingsFile.write(QJson::Serializer().serialize(row2Map(row)));
        settingsFile.close();
    }

    pendingDeletes.clear();
    createdProfiles.clear();

    return true;
}

bool ProfileListModel::rollback() {
    QStringList::iterator it;
    for (it = createdProfiles.begin(); it != createdProfiles.end(); it++) {
        QFile(profilesDir.absolutePath() + "/" + *it).remove();
    }

    loadProfiles();
    pendingDeletes.clear();
    createdProfiles.clear();

    return true;
}


void ProfileListModel::append(QVariantMap &profile) {
    int row = rowCount();
    setItem(row, NAME, new QStandardItem(profile["name"].toString()));
    setItem(row, ID, new QStandardItem(profile["id"].toString()));
    setItem(row, USE_HTTP, new QStandardItem(profile["httpProxy"].toMap()["useProxy"].toString()));
    setItem(row, HTTP_HOST, new QStandardItem(profile["httpProxy"].toMap()["host"].toString()));
    setItem(row, HTTP_PORT, new QStandardItem(profile["httpProxy"].toMap()["port"].toString()));
    setItem(row, USE_HTTPS, new QStandardItem(profile["httpProxy"].toMap()["useProxy"].toString()));
    setItem(row, HTTPS_HOST, new QStandardItem(profile["httpProxy"].toMap()["host"].toString()));
    setItem(row, HTTPS_PORT, new QStandardItem(profile["httpProxy"].toMap()["port"].toString()));
    setItem(row, USE_FTP, new QStandardItem(profile["httpProxy"].toMap()["useProxy"].toString()));
    setItem(row, FTP_HOST, new QStandardItem(profile["httpProxy"].toMap()["host"].toString()));
    setItem(row, FTP_PORT, new QStandardItem(profile["httpProxy"].toMap()["port"].toString()));
    setItem(row, EXCEPTIONS, new QStandardItem(profile["exceptions"].toString()));
}

QVariantMap ProfileListModel::row2Map(int row) {
    QVariantMap map;
    map["name"] = item(row, NAME)->data(Qt::DisplayRole).toString();
    map["id"] = item(row, ID)->data(Qt::DisplayRole).toString();
    QVariantMap httpProxy;
    httpProxy["useProxy"] = item(row, USE_HTTP)->data(Qt::DisplayRole).toBool();
    httpProxy["host"] = item(row, HTTP_HOST)->data(Qt::DisplayRole).toString();
    httpProxy["port"] = item(row, HTTP_PORT)->data(Qt::DisplayRole).toInt();
    map["httpProxy"] = httpProxy;
    QVariantMap httpsProxy;
    httpsProxy["useProxy"] = item(row, USE_HTTPS)->data(Qt::DisplayRole).toBool();
    httpsProxy["host"] = item(row, HTTPS_HOST)->data(Qt::DisplayRole).toString();
    httpsProxy["port"] = item(row, HTTPS_PORT)->data(Qt::DisplayRole).toInt();
    map["httpsProxy"] = httpsProxy;
    QVariantMap ftpProxy;
    ftpProxy["useProxy"] = item(row, USE_FTP)->data(Qt::DisplayRole).toBool();
    ftpProxy["host"] = item(row, FTP_HOST)->data(Qt::DisplayRole).toString();
    ftpProxy["port"] = item(row, FTP_PORT)->data(Qt::DisplayRole).toInt();
    map["ftpProxy"] = ftpProxy;
    map["exceptions"] = item(row, EXCEPTIONS)->data(Qt::DisplayRole).toString();
    return map;
}

QDir ProfileListModel::profilesDir;
