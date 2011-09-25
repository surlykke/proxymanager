#ifndef PROFILE_H
#define PROFILE_H
#include <QtCore>
#include <QDebug>
#include <QVariant>
#include <QMap>
#include <QString>
#include <QStringListModel>

struct Profile
{

    QString id; // Don't touch
    QString name; 
    bool useProxy;
    QString proxyHost;
    int proxyPort;
    QStringList hostExceptions;
    QStringList domainExceptions;

    void save();
    void load();
    void remove();


};

QDebug operator<<(QDebug dbg, const Profile &c);

#endif // PROFILE_H
