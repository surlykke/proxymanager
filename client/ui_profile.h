/********************************************************************************
** Form generated from reading UI file 'profile.ui'
**
** Created: Wed Feb 17 21:21:28 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROFILE_H
#define UI_PROFILE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Profile
{
public:
    QListView *listView_2;
    QLabel *label_4;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QSpinBox *spinBox;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QCheckBox *checkBox;

    void setupUi(QDialog *Profile)
    {
        if (Profile->objectName().isEmpty())
            Profile->setObjectName(QString::fromUtf8("Profile"));
        Profile->resize(306, 285);
        listView_2 = new QListView(Profile);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setGeometry(QRect(15, 119, 201, 140));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listView_2->sizePolicy().hasHeightForWidth());
        listView_2->setSizePolicy(sizePolicy);
        label_4 = new QLabel(Profile);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(166, 67, 16, 16));
        layoutWidget = new QWidget(Profile);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 67, 271, 26));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        lineEdit_2 = new QLineEdit(layoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout->addWidget(lineEdit_2);

        spinBox = new QSpinBox(layoutWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        horizontalLayout->addWidget(spinBox);

        layoutWidget_2 = new QWidget(Profile);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(215, 119, 70, 50));
        verticalLayout = new QVBoxLayout(layoutWidget_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_3 = new QPushButton(layoutWidget_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        verticalLayout->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(layoutWidget_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        label_2 = new QLabel(Profile);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(16, 99, 61, 20));
        lineEdit = new QLineEdit(Profile);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(10, 12, 141, 24));
        checkBox = new QCheckBox(Profile);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(10, 40, 96, 23));

        retranslateUi(Profile);

        QMetaObject::connectSlotsByName(Profile);
    } // setupUi

    void retranslateUi(QDialog *Profile)
    {
        Profile->setWindowTitle(QApplication::translate("Profile", "Dialog", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Profile", ":", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Profile", "http://", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Profile", "New", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Profile", "Delete", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Profile", "Exceptions", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("Profile", "Use a proxy", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Profile: public Ui_Profile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILE_H
