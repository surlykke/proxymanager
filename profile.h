#ifndef PROFILE_H
#define PROFILE_H
#include <QString>
#include <QList>
#include <QDebug>

struct Profile
{
    QString id;
    QString name;
    bool useProxy;
    QString host;
    int port;
    QString exceptions;
    bool useAuthentication;
    QString ntDomain;
    QString userId;
    QString password;

    void save();
    static Profile loadProfile(QString id);
    static QList<Profile> loadAll();
    static Profile newProfile();
    static void deleteProfile(QString id);

private:
    void load();

};

QDebug operator<<(QDebug debug, Profile& profile);


#endif // PROFILE_H
