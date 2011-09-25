#include "profile.h"

void Profile::load() {
    QSettings settings;
    settings.beginGroup("profiles");
    settings.beginGroup(id);
    name = settings.value("name").toString();
    useProxy = settings.value("useProxy").toBool();
    proxyHost = settings.value("proxyHost").toString();
    proxyPort = settings.value("proxyPort").toInt();
    hostExceptions = settings.value("hostExceptions").toStringList();
    domainExceptions = settings.value("domainExceptions").toStringList();
    settings.endGroup();
    settings.endGroup();
}

void Profile::save() {
    qDebug() << "Saving " << *this;
    QSettings settings;
    settings.beginGroup("profiles");
    settings.beginGroup(id);
    settings.setValue("name", name);
    settings.setValue("useProxy", useProxy);
    settings.setValue("proxyHost", proxyHost);
    settings.setValue("proxyPort", proxyPort);
    settings.setValue("hostExceptions", QVariant(hostExceptions));
    settings.setValue("domainExceptions", QVariant(domainExceptions));
    settings.endGroup();
    settings.endGroup();
}

void Profile::remove() {
    QSettings settings;
    settings.beginGroup("profiles");
    settings.remove(id);
    settings.endGroup();
    id = "";
}

QDebug operator<<(QDebug dbg, const Profile& profile) {
    dbg.nospace()  << "{id: " << profile.id
                   << ", name: " << profile.name
                   << ", useProxy: " << profile.useProxy
                   << ", proxyHost: " << profile.proxyHost
                   << ", proxyPort: " << profile.proxyPort
                   << ", hostExceptions: " << profile.hostExceptions
                   << ", domainExceptions: " << profile.domainExceptions << "}";

    return dbg.space();
}
