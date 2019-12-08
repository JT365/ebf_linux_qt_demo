/******************************************************************
*******************************************************************/
#ifndef FONTPAGE_H
#define FONTPAGE_H

#include "qtwidgetbase.h"
#include "qtlistwidget.h"

#include <QFont>
#include <QLabel>

class FontPage : public QtWidgetBase
{
    Q_OBJECT
public:
    explicit FontPage(QWidget *parent = 0);
    ~FontPage();

private:
    QtPixmapButton   *m_btnOk;

                 
protected:

};

class GeneralTab : public QtWidgetBase
{
    Q_OBJECT
public:
    explicit GeneralTab(QString &str, const QString name, QtWidgetBase *parent=0);
    ~GeneralTab();

private:
    QString          *m_fontstr;
    QString          m_fontname;
    QLabel *fontLabel;
            
    void saveFont( QFont &font);
  
private slots:
    void getFont(void); 
    
protected:

};
#endif // FONTPAGE_H
