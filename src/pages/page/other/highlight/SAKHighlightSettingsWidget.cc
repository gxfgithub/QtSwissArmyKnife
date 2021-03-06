﻿/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKHighlightSettings.hh"
#include "SAKHighlightSettingsWidget.hh"

#include "ui_SAKHighlightSettingsWidget.h"

SAKHighlightSettingsWidget::SAKHighlightSettingsWidget(QTextDocument *doc, QWidget* parent)
    :QWidget (parent)
    ,ui (new Ui::SAKHighlightSettingsWidget)
{
    ui->setupUi(this);
    ui->frame->setLayout(&labelLayout);

    highlighter = new SAKHighlightSettings(doc);
    inputLineEdit = ui->lineEdit;
    clearLabelBt = ui->pushButtonClear;
    addLabelBt = ui->pushButtonAdd;

    inputLineEdit->installEventFilter(this);

    connect(clearLabelBt, &QPushButton::clicked, this, &SAKHighlightSettingsWidget::clearLabel);
    connect(addLabelBt, &QPushButton::clicked, this, &SAKHighlightSettingsWidget::addLabelFromInput);
}

SAKHighlightSettingsWidget::~SAKHighlightSettingsWidget()
{
    delete ui;
}

void SAKHighlightSettingsWidget::addLabel(QString str)
{
    if (str.isEmpty()){
        return;
    }

    QStringList keyWords;
    for (int i = 0; i < labelList.length(); i++){
        QString temp = labelList.at(i)->text();
        /// 标签重复不处理
        if (temp.compare(str) == 0){
            return;
        }
        keyWords.append(temp);
    }


    QPushButton* tempLabel = new QPushButton(str);
    tempLabel->installEventFilter(this);
    labelList.append(tempLabel);

    resetLabelViewer();

    keyWords.append(str);
    resetHighlightKeyword(keyWords);
}

void SAKHighlightSettingsWidget::addLabelFromInput()
{
    QString str = inputLineEdit->text();
    addLabel(str);
}

void SAKHighlightSettingsWidget::deleteLabel(QPushButton *bt)
{
    for (int i= 0; i < labelList.length(); i++){
        if (labelList.at(i) == bt){
            QPushButton *BtTemp = labelList.takeAt(i);
            BtTemp->deleteLater();
            break;
        }
    }

    resetLabelViewer();

    QStringList keyWords;
    for (int i = 0; i < labelList.length(); i++){
        keyWords.append(labelList.at(i)->text());
    }    

    resetHighlightKeyword(keyWords);
}

bool SAKHighlightSettingsWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick){
        for (int i = 0; i < labelList.length(); i++){
            if (labelList.at(i) == watched){
                deleteLabel(labelList.at(i));
                return true;
            }
        }
    }

    if (event->type() == QEvent::KeyPress){
        QKeyEvent* e = dynamic_cast<QKeyEvent*>(event);
        if ((e->key() == Qt::Key_Enter) || e->key() == Qt::Key_Return){
            addLabel(inputLineEdit->text());
            inputLineEdit->clear();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void SAKHighlightSettingsWidget::clearLabel()
{
    QPushButton *bt = Q_NULLPTR;
    while (!labelList.isEmpty()) {
        bt = labelList.takeFirst();
        bt->deleteLater();
    }

    resetHighlightKeyword(QStringList());
}

void SAKHighlightSettingsWidget::resetLabelViewer()
{
    for (int index = 0; index < labelList.count(); index++){
        labelLayout.addWidget(labelList.at(index), index/5, index%5);
    }
}

void SAKHighlightSettingsWidget::resetHighlightKeyword(QStringList keyWords)
{
    highlighter->setHighlighterKeyWord(keyWords);
    highlighter->rehighlight();
}
