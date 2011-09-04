#ifndef PROFILE_H
#define PROFILE_H
#include <QtCore>
#include <QVariant>
#include <QMap>
#include <QString>
#include <QStringListModel>

class Profile
{
public:
    Profile();
    Profile(QMap<QString, QVariant> serialized);
    ~Profile();

    QMap<QString, QVariant> serialize();

    void copy(Profile & otherProfile);

    QString networkSignature;
    QString name; 
    bool useProxy;
    QString proxyHost;
    int proxyPort;
    QStringList hostExceptions;
    QStringList domainExceptions;

};

#endif // PROFILE_H
