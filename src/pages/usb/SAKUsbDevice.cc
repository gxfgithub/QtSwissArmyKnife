﻿/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDebug>
#include <QEventLoop>

#include <QApplication>
#include "SAKUsbDevice.hh"
#include "SAKUsbDebugPage.hh"
#include "SAKUsbDeviceController.hh"

SAKUsbDevice::SAKUsbDevice(SAKUsbDebugPage *debugPage, QObject *parent)
    :SAKDevice(parent)
    ,debugPage (debugPage)
    ,hidDevice (nullptr)
{

}

SAKUsbDevice::~SAKUsbDevice()
{

}

void SAKUsbDevice::run()
{
    QEventLoop eventLoop;
    SAKUsbDeviceController *deviceController = debugPage->controllerInstance();
    path = deviceController->devicePath();
    hidDevice = hid_open_path(path.toLatin1().constData());
    if (!hidDevice){
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法打开设备")+QString::fromWCharArray(hid_error(hidDevice)), false);
        return;
    }

    emit deviceStateChanged(true);
    hid_set_nonblocking(hidDevice, 1);

    while (true) {
        /// @brief 响应中中断
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 读取数据
        QByteArray data;
        data.resize(1024);
        int ret = hid_read(hidDevice, reinterpret_cast<unsigned char*>(data.data()), static_cast<size_t>(data.length()));
        if(ret > 0){
            data.resize(ret);
            emit bytesRead(data);
        }

        /// @brief 发送数据
        while (true) {
            QByteArray bytes = takeWaitingForWrittingBytes();
            if (bytes.length()){
                quint8 endpoint = deviceController->endpoint();
                quint8 cmd = deviceController->cmd();
                bytes.prepend(reinterpret_cast<char*>(&cmd), 1);
                bytes.prepend(reinterpret_cast<char*>(&endpoint), 1);
                int ret = hid_write(hidDevice, reinterpret_cast<const unsigned char*>(bytes.constData()), static_cast<size_t>(bytes.length()));
                if (ret == -1){
                    emit messageChanged(tr("发送数据失败：")+QString::fromWCharArray(hid_error(hidDevice)), false);
                }else{
                    emit bytesWritten(bytes);
                }
            }else{
                break;
            }
        }

        /// @brief 线程睡眠
        threadMutex.lock();
        threadWaitCondition.wait(&threadMutex, debugPage->readWriteParameters().runIntervalTime);
        threadMutex.unlock();
    }

    hid_exit();
    emit deviceStateChanged(false);
}
