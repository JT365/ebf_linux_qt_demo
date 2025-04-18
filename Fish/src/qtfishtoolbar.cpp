/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : qtplaytoolbar.cpp --- QtPlayToolBar
 作 者    : Niyh  (QQ:393320854)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2019/9/14
*******************************************************************/
#include "qtfishtoolbar.h"
#include "skin.h"

#include <QPainter>
#include <QMouseEvent>
#include <QButtonGroup>

//////////////////////////////////////////////////////////////////////////////////////
FishTitleBarWidget::FishTitleBarWidget(QWidget *parent) : QtToolBar(parent)
{
    m_strText = "";
    m_nBaseWidth = Skin::m_nScreenWidth;
    m_nBaseHeight = 50;

    m_rectBack = QRect(20, 10, 40, 40);
}

FishTitleBarWidget::~FishTitleBarWidget()
{

}

void FishTitleBarWidget::SetText(const QString &text)
{
    m_strText = text;
    this->update();
}

void FishTitleBarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.scale(m_scaleX, m_scaleY);
    painter.fillRect(0, 0, m_nBaseWidth, m_nBaseHeight, QColor("#7f182E3A"));

    painter.drawPixmap(m_rectBack.topLeft(), QPixmap(":/images/photos/toolbar/ic_back.png"));

    painter.setPen("#ffffff");
    painter.setFont(QFont(Skin::m_strAppFontNormal, 18));
    painter.drawText(0, 0, m_nBaseWidth, m_nBaseHeight, Qt::AlignCenter, m_strText);
}

void FishTitleBarWidget::mousePressEvent(QMouseEvent *e)
{
    QRect rect;
    ScaleRect(rect, m_rectBack);
    if (rect.contains(e->pos())) {
        emit signalBack();
    }
    QWidget::mousePressEvent(e);
}

