#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "proxy_manager_daemon.h"
#include <QStringList>
#include <QCryptographicHash>
#include <iostream>

ProxyManagerDaemon::ProxyManagerDaemon(int argc, char* argv[])
    : QCoreApplication(argc, argv), resolvConfWatcher(QStringList("/etc/resolv.conf")) {
    connect(&resolvConfWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}

ProxyManagerDaemon::~ProxyManagerDaemon() {
}

void ProxyManagerDaemon::configureProxy(bool use_proxy, const QString &host, ushort port, const QStringList &host_exceptions, const QStringList &domain_exceptions) {
    qDebug() << "configureProxy(" << use_proxy << ", " << host << ", " << port << ", " << host_exceptions << ", " << domain_exceptions << ");";
    QFile file("/var/lib/ProxyManager/ProxyManagerSquid.conf");
    if (file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        if (use_proxy) {
            stream << "cache_peer " << host << " parent " << port << " 0 default\n";

            if (domain_exceptions.size() > 0) {
                stream << "acl NO_PROXY_FOR_DOMAINS dstdomain ";
                for (int i = 0; i < domain_exceptions.size(); i++)
                    stream << domain_exceptions.at(i) << " ";
                stream << "\n";
                stream << "always_direct allow NO_PROXY_FOR_DOMAINS\n";
            }

            if (host_exceptions.size() > 0) {
                stream << "acl NO_PROXY_FOR_HOSTS dst ";
                for (int i = 0; i < host_exceptions.size(); i++)
                    stream << host_exceptions.at(i) << " ";
                stream << "\n";
                stream << "always_direct allow NO_PROXY_FOR_HOSTS\n";
            }

            stream << "never_direct allow ALL\n";
        }
        else {
            qDebug() << "writing no proxy...";
            stream << "# No proxy\n";
        }
        file.close();
        qDebug() << "Reconfigure squid";
        system("squid -k reconfigure");
    }
}


void ProxyManagerDaemon::fileChanged(QString path) {
    qDebug() << "FileChanged(" << path << ")";
    QString signature = networkSignature();
    qDebug() << "Trying to emit: " << signature;
    emit NetworkChanged(signature);
    if (!resolvConfWatcher.files().contains("/etc/resolv.conf")) {
        resolvConfWatcher.addPath("/etc/resolv.conf");
    }
}

QString ProxyManagerDaemon::networkSignature() {
    QString result = "";
    QFile resolvConf("/etc/resolv.conf");
    if (resolvConf.exists() && resolvConf.open(QIODevice::ReadOnly)) {
        QTextStream stream(&resolvConf);
        QCryptographicHash hash(QCryptographicHash::Md5);
        bool foundSome = false;
        while (! stream.atEnd()) {
            QString line = stream.readLine();
            qDebug() << "Reading " << line;
            if (notWhiteSpace(line)) {
                qDebug() << "Adding";
                hash.addData(line.toAscii());
                foundSome = true;
            }
        }
        if (foundSome)  result  = hash.result().toHex();
    }
    return result;
}


bool ProxyManagerDaemon::notWhiteSpace(QString& string){
    static QRegExp reg("^\\s*#.*$|^\\s*$");
    return ! reg.exactMatch(string);
}

