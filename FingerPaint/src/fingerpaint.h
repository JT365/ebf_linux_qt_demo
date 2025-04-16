/******************************************************************
 Copyright (C) 2025 - All Rights Reserved by
 文 件 名 : fingerpaint.h ---
 作 者    : wdz
 编写日期 : 2025
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2025/4/15
*******************************************************************/
#ifndef FINGERPAINT_H
#define FINGERPAINT_H

#include <QWidget>
#include <QPushButton>

#include "qtwidgetbase.h"
#include "scribblearea.h"

class FingerPaint : public QtAnimationWidget
{
    Q_OBJECT

public:
    FingerPaint(QWidget *parent = 0);
    ~FingerPaint();

    typedef enum {BtnHome, BtnBack, BtnClear, BtnSetting} ToolBtn;

private:
    QString m_strDirPath;

    FingerPaint *m_fingerpaint;
    QtPixmapButton          *m_btnClear;
    QtPixmapButton          *m_btnBack;
    QtPixmapButton          *m_btnSetting;
    ScribbleArea            *m_scribble;

private:
    void InitWidget();

private slots:
 //   void SltLoadPhotos();
//    void SltCurrentItemClicked(QtPageListWidgetItem *item);
//    void SltDirSelected(const QString &fileName);
//    void SltDirDialogClose();
    void SltToolBtnClicked(int index);

protected:
    void resizeEvent(QResizeEvent *e);
};

#endif // FINGERPAINT_H
