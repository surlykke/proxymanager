/*
 * profilelistmodel.cpp
 *
 *  Created on: 12/03/2010j
 *      Author: christian
 */

#include <QUuid>
#include "profilelistmodel.h"

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

void ProfileListModel::newProfile() {

    int row = rowCount();
    QString uuid = QUuid::createUuid().toString();
    setItem(row, NAME, new QStandardItem("new profile..."));
    setItem(row, ID, new QStandardItem(uuid));
    setItem(row, USE_HTTP, new QStandardItem());
    setItem(row, HTTP_HOST, new QStandardItem());
    setItem(row, HTTP_PORT, new QStandardItem());
    setItem(row, USE_HTTPS, new QStandardItem());
    setItem(row, HTTPS_HOST, new QStandardItem());
    setItem(row, HTTPS_PORT, new QStandardItem());
    setItem(row, USE_FTP, new QStandardItem());
    setItem(row, FTP_HOST, new QStandardItem());
    setItem(row, FTP_PORT, new QStandardItem());
    setItem(row, EXCEPTIONS, new QStandardItem());
}


void ProfileListModel::loadProfiles() {
    QSettings settings;
    settings.beginGroup("profiles");
    QStringList ids = settings.childGroups();
    for (int i = 0; i < ids.size(); i++) {
        settings.beginGroup(ids.at(i));
        setItem(i, NAME, new QStandardItem(settings.value("name").toString()));
        setItem(i, ID, new QStandardItem(ids.at(i)));
        setItem(i, USE_HTTP, new QStandardItem(settings.value("httpProxy/useProxy").toString()));
        setItem(i, HTTP_HOST, new QStandardItem(settings.value("httpProxy/host").toString()));
        setItem(i, HTTP_PORT, new QStandardItem(settings.value("httpProxy/port").toString()));
        setItem(i, USE_HTTPS, new QStandardItem(settings.value("httpsProxy/useProxy").toString()));
        setItem(i, HTTPS_HOST, new QStandardItem(settings.value("httpsProxy/host").toString()));
        setItem(i, HTTPS_PORT, new QStandardItem(settings.value("httpsProxy/port").toString()));
        setItem(i, USE_FTP, new QStandardItem(settings.value("ftpProxy/useProxy").toString()));
        setItem(i, FTP_HOST, new QStandardItem(settings.value("ftpProxy/host").toString()));
        setItem(i, FTP_PORT, new QStandardItem(settings.value("ftpProxy/port").toString()));
        setItem(i, EXCEPTIONS, new QStandardItem(settings.value("exceptions").toString()));
        settings.endGroup();
    }
    settings.endGroup();
}

void ProfileListModel::saveProfiles() {
    // FIXME only save changed profiles
    QSettings settings;
    settings.remove("profiles");
    settings.beginGroup("profiles");
    for (int row = 0; row < rowCount(); row++) {
        settings.beginGroup(item(row, ID)->data(Qt::DisplayRole).toString());
        settings.setValue("name", item(row, NAME)->data(Qt::DisplayRole).toString());
        settings.setValue("httpProxy/useProxy", item(row, USE_HTTP)->data(Qt::DisplayRole).toBool());
        settings.setValue("httpProxy/host", item(row, HTTP_HOST)->data(Qt::DisplayRole).toString());
        settings.setValue("httpProxy/port", item(row, HTTP_PORT)->data(Qt::DisplayRole).toInt());
        settings.setValue("httpsProxy/useProxy", item(row, USE_HTTPS)->data(Qt::DisplayRole).toBool());
        settings.setValue("httpsProxy/host", item(row, HTTPS_HOST)->data(Qt::DisplayRole).toString());
        settings.setValue("httpsProxy/port", item(row, HTTPS_PORT)->data(Qt::DisplayRole).toInt());
        settings.setValue("ftpProxy/useProxy", item(row, USE_FTP)->data(Qt::DisplayRole).toBool());
        settings.setValue("ftpProxy/host", item(row, FTP_HOST)->data(Qt::DisplayRole).toString());
        settings.setValue("ftpProxy/port", item(row, FTP_PORT)->data(Qt::DisplayRole).toInt());
        settings.setValue("exceptions", item(row, EXCEPTIONS)->data(Qt::DisplayRole).toString());
        settings.endGroup();
    }
    settings.endGroup();
}


