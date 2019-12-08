/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : skin.cpp --- Skin
 作 者    : Niyh  (QQ:393320854)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2019/8/31
*******************************************************************/
#include "skin.h"
#include <QDebug>


QString Skin::m_strAppFontNormal = "Arial Unicode MS Normal";
QString Skin::m_strAppFontRegular = "Arial Unicode MS Regular";
QString Skin::m_strAppFontBold = "Arial Unicode MS Bold";
QString Skin::m_strAppFontMedium = "Arial Unicode MS Medium";

int     Skin::m_nScreenWidth = 800;
int     Skin::m_nScreenHeight = 480;

Skin::Skin()
{

}

void Skin::InitSkin()
{
    qDebug() << "Skin InitOk: build at " << __DATE__ << __TIME__;
}
