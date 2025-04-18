/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : videoplayer.h ---
 作 者    : Niyh  (QQ:393320854)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2019/9/7
*******************************************************************/
#ifndef FISH_H
#define FISH_H

#include <QWidget>
#include <QTapAndHoldGesture>
#include <QGestureEvent>

#include "qtwidgetbase.h"
#include "qtfiledialog.h"
#include "magic_pool.h"
#include "qtfishtoolbar.h"

class Fish : public QtAnimationWidget
{
    Q_OBJECT

public:
    Fish(QWidget *parent = 0);
    ~Fish();

    typedef enum {BtnHome, BtnBack, BtnAdd, BtnSetting} ToolBtn;

private:
    QString             m_strDirPath;

    FishTitleBarWidget  *m_titleBar;

    // 停留自动关闭
    QTimer     *m_timerShow;
    bool        m_bToolBarShow;

    MagicPool           *m_magicPool;
    QtFileDialog        *m_dirDialog;
    QtPixmapButton      *m_btnAdd;
    QtPixmapButton      *m_btnBack;
    QtPixmapButton      *m_btnSetting;

private:
    void InitWidget();
    void ShowToolBar();

private slots:
    void SltAutoCloseToolBar();
    void SltBackClicked();

protected:
    void resizeEvent(QResizeEvent *e);
    bool event(QEvent *event) override;
    bool gestureEvent(QGestureEvent *event);
};

#endif // FISH_H
