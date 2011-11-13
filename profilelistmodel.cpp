/*
 * profilelistmodel.cpp
 *
 *  Created on: 12/03/2010j
 *      Author: christian
 */

#include <QUuid>
#include "profilelistmodel.h"
#include "jsonresource.h"
#include <QMessageBox>
#include "qjson/parser.h"
#include "qjson/serializer.h"

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

    JsonResource jsonResource("http://localhost:8000/proxysettings");
    jsonResource.POST();
    if (jsonResource.error) {
        errorMsg = "Error connecting to Proxy Manager: " +  jsonResource.errorMsg;
        return false;
    }
    qDebug() << "jsonResource.response: " << jsonResource.response;

    QJson::Parser parser;
    bool ok;
    QVariantMap profile = parser.parse(jsonResource.response, &ok).toMap();
    qDebug() << "profile som var: " << profile;
    if (! ok) {
        errorMsg = "Error understanding response from Proxy Manager: " + parser.errorString();
        return false;
    }
    if (profile["name"].toString() == "") {
        profile["name"] = "New profile...";
    }

    createdProfiles << profile["id"].toString();
    append(profile);
    return true;
}

void ProfileListModel::deleteProfile(int row) {
    if (row >= 0 && row < rowCount()) {
        pendingDeletes << item(row, ID)->data(Qt::DisplayRole).toString();
        removeRow(row, QModelIndex());
    }
}



bool ProfileListModel::loadProfiles() {
    JsonResource jsonResource("http://localhost:8000/proxysettings");
    jsonResource.GET();
    if (jsonResource.error) {
        errorMsg = "Error loading profiles from Proxy Manager: " + jsonResource.errorMsg;
        return false;
    }
    qDebug() << "GET, jsonResource response: " << jsonResource.response;

    QJson::Parser parser;
    bool ok;
    QVariantList profiles = parser.parse(jsonResource.response, &ok).toList();
    if (! ok) {
        errorMsg = "Error understanding response from Proxy Manager: " + parser.errorString();
        return false;
    }

    clear();
    QVariantList::iterator it;
    for (it = profiles.begin(); it != profiles.end(); it++) {
        QVariantMap map = (*it).toMap();
        qDebug() << "Appending: " << map;
        append(map);
    }

    createdProfiles.clear();
    return true;
}

bool ProfileListModel::commit() {
    QStringList::iterator it;
    for (it = pendingDeletes.begin(); it != pendingDeletes.end(); it++) {
        JsonResource jsonResource("http://localhost:8000/proxysetting/" + *it);
        jsonResource.DELETE();
    }

    // FIXME only save changed profiles
    for (int row = 0; row < rowCount(); row++) {
        QJson::Serializer serializer;
        QByteArray json = serializer.serialize(row2Map(row));
        JsonResource jsonResource("http://localhost:8000/proxysetting/" + item(row, ID)->data(Qt::DisplayRole).toString());
        qDebug() << "PUT:" << "http://localhost:8000/proxysetting/"  << item(row, ID)->data(Qt::DisplayRole).toString();
        qDebug() << json;
        jsonResource.PUT(json);
    }

    pendingDeletes.clear();
    createdProfiles.clear();

    return true;
}

bool ProfileListModel::rollback() {
    QStringList::iterator it;
    for (it = createdProfiles.begin(); it != createdProfiles.end(); it++) {
        JsonResource jsonResource("http://localhost:8000/proxysetting/" + *it);
        jsonResource.DELETE(); //Try to delete as much as possible, so we silently swallow errors
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
