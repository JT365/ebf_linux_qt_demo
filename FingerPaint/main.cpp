/******************************************************************
 Copyright (C) 2025 - All Rights Reserved by
 文 件 名 : main.cpp ---
 作 者    : wdz  
 编写日期 : 2025
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2025/4/15
*******************************************************************/
#include "fingerpaint.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FingerPaint w;
#ifdef __arm__
    w.showFullScreen();
#else
    w.resize(800, 480);
    w.show();
#endif

    return a.exec();
}
