﻿/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qter188/QtSwissArmyKnife
 *     https://gitee.com/qter188/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is qter188@outlook.com.
 */
#include "SAKDebugPage.hh"
#include "SAKBaseTransmissionItemWidget.hh"

SAKBaseTransmissionItemWidget::SAKBaseTransmissionItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,debugPage (debugPage)
{
    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKBaseTransmissionItemWidget::write);
    connect(this, &SAKBaseTransmissionItemWidget::bytesRead, debugPage, &SAKDebugPage::write);
}

void SAKBaseTransmissionItemWidget::write(QByteArray data)
{
    Q_UNUSED(data);
}
