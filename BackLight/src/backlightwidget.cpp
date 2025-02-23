/******************************************************************
 Copyright (C) 2019 - All Rights Reserved by
 文 件 名 : backlightwidget.cpp ---
 作 者    : Niyh(lynnhua)
 论 坛    : http://www.firebbs.cn
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
  Niyh	   2019    	1.0.0 1     文件创建
*******************************************************************/
#include "backlightwidget.h"
#include "skin.h"
#include "HAL.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QDebug>

BackLightWidget::BackLightWidget(QWidget *parent) : QtAnimationWidget(parent),m_nLevel(0)
{
    this->SetBackground(QPixmap(":/images/backlight/ic_background.png"));
    InitWidget();
    ReadBacklight();
}

BackLightWidget::~BackLightWidget()
{

}

void BackLightWidget::InitWidget()
{
    QtWidgetTitleBar *widgetTitle= new QtWidgetTitleBar(this);
    widgetTitle->SetScalSize(Skin::m_nScreenWidth, 80);
    widgetTitle->SetBackground(Qt::transparent);
    widgetTitle->setFont(QFont(Skin::m_strAppFontBold));
    widgetTitle->SetTitle(tr("Brightness Adjustment"), "#ffffff", 24);
    widgetTitle->SetBtnHomePixmap(QPixmap(":/images/backlight/menu_icon.png"), QPixmap(":/images/backlight/menu_icon_pressed.png"));
    connect(widgetTitle, SIGNAL(signalBackHome()), this, SIGNAL(signalBackHome()));

    m_knobSwitch = new QtKnobSwitch(this);
    m_knobSwitch->setFont(QFont(Skin::m_strAppFontBold));
    connect(m_knobSwitch, SIGNAL(valueChanged(int)), this, SLOT(SltValueChanged(int)));

    QVBoxLayout *verLayout = new QVBoxLayout(this);
    verLayout->setContentsMargins(0, 0, 0, 0);
    verLayout->setSpacing(0);
    verLayout->addWidget(widgetTitle, 1);
    verLayout->addStretch(5);
}

// 读取配置
void BackLightWidget::ReadBacklight()
{
#ifdef __arm__
    unsigned char max, current;

    getBrightness(&max, &current);
    if (current > 5 && current < 100) {
        m_knobSwitch->setValue(current);
    }
#endif
}

void BackLightWidget::SltValueChanged(int value)
{
    if (m_nLevel == value)  return;

    m_nLevel = (value < 5 ? 5 : value);
#ifdef __arm__
    setBrightness(m_nLevel);
#endif
}

void BackLightWidget::resizeEvent(QResizeEvent *e)
{
    SetScaleValue();
    m_knobSwitch->setGeometry(242 * m_scaleX, 100 * m_scaleY, 316 * m_scaleY, 316 * m_scaleY);
    QWidget::resizeEvent(e);
}
