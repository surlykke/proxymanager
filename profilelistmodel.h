/*
  Copyright 2010, 2011 Christian Surlykke

  This file is part of ProxyManager.

  PitchTrainer is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3

  PitchTrainer is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with PitchTrainer.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PROFILELISTMODEL_H_
#define PROFILELISTMODEL_H_
#include <QtCore>
#include <QList>
#include <QPair>
#include <QString>
#include <QStandardItemModel>
#include <QStandardItem>

namespace PLM {

}

class ProfileListModel : public QStandardItemModel {
    Q_OBJECT

public:

    enum column{
        NAME,
        ID,
        USE_HTTP,
        HTTP_HOST,
        HTTP_PORT,
        USE_HTTPS,
        HTTPS_HOST,
        HTTPS_PORT,
        USE_FTP,
        FTP_HOST,
        FTP_PORT,
        EXCEPTIONS
    };

    static const int COLUMNS = 10;

    ProfileListModel(QObject *parent = 0);
    virtual ~ProfileListModel();

    void sameProxy(int row);

    void loadProfiles();
    void saveProfiles();

    QString id(int row);
    QString name(int row);

public slots:
    void newProfile();
};

#endif /* TESTMODEL_H_ */
