﻿/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTCPSERVERDEVICECONTROLLER_HH
#define SAKTCPSERVERDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QTcpSocket>

namespace Ui {
    class SAKTcpServerDeviceController;
}

class SAKTcpServerDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKTcpServerDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKTcpServerDeviceController();

    QString serverHost();
    quint16 serverPort();

    QString currentClientHost();
    quint16 currentClientPort();

    void refresh();
    void setUiEnable(bool enable);

    void addClient(QString host, quint16 port, QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);
private:
    Ui::SAKTcpServerDeviceController *ui;

    QComboBox *serverHostComboBox;
    QLineEdit *serverPortLineEdit;
    QComboBox *clientHostComboBox;
    QMutex uiMutex;
};

#endif
