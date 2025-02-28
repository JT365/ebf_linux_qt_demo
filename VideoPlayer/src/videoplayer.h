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
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>

#include "qtvideowidget.h"
#include "qtwidgetbase.h"
#include "videolistviewer.h"
#include "qtfiledialog.h"

class VideoPlayer : public QtAnimationWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    typedef enum {BtnHome, BtnBack, BtnAdd, BtnSetting} ToolBtn;

private:
    QString             m_strDirPath;

    VideoListViewer     *m_videosList;
    QtVideoWidget       *m_videoWidget;
    QtFileDialog        *m_dirDialog;
    QtPixmapButton      *m_btnAdd;
    QtPixmapButton      *m_btnBack;
    QtPixmapButton      *m_btnSetting;

private:
    void InitWidget();

private slots:
    void LoadMedias();
    void SltItemClicked(QtPageListWidgetItem *item);
    void SltDirSelected(const QString &fileName);
    void SltDirDialogClose();
    void SltToolBtnClicked(int index);

protected:
    void resizeEvent(QResizeEvent *e);
};

#endif // VIDEOPLAYER_H
