/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : photosview.h ---
 作 者    : Niyh  (QQ:393320854)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2019/9/8
*******************************************************************/
#ifndef PHOTOSVIEW_H
#define PHOTOSVIEW_H

#include <QWidget>
#include <QPushButton>

#include "qtwidgetbase.h"
#include "photolistview.h"
#include "qtfiledialog.h"

class PhotosView : public QtAnimationWidget
{
    Q_OBJECT

public:
    PhotosView(QWidget *parent = 0);
    ~PhotosView();

    typedef enum {BtnHome, BtnBack, BtnAdd, BtnSetting} ToolBtn;

private:
    QString m_strDirPath;

    PhotoListView *m_photoListView;
    QMap<int, QtPageListWidgetItem*> m_listItems;
    QtFileDialog            *m_dirDialog;
    QtPixmapButton          *m_btnAdd;
    QtPixmapButton          *m_btnBack;
    QtPixmapButton          *m_btnSetting;

private:
    void InitWidget();

private slots:
    void SltLoadPhotos();
    void SltCurrentItemClicked(QtPageListWidgetItem *item);
    void SltDirSelected(const QString &fileName);
    void SltDirDialogClose();
    void SltToolBtnClicked(int index);

protected:
    void resizeEvent(QResizeEvent *e);
};

#endif // PHOTOSVIEW_H
