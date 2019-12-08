/******************************************************************
*******************************************************************/
#include "fontpage.h"
#include "skin.h"
#include "appconfig.h"

#include <QApplication>
#include <QProcess>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFontDialog>
#include <QTabWidget> 

FontPage::FontPage(QWidget *parent) : QtWidgetBase(parent)
{
    m_nBaseWidth = Skin::m_nScreenWidth;
    m_nBaseHeight = 400;

    QTabWidget *tabWidget = new QTabWidget;
      
    tabWidget->addTab(new GeneralTab(Skin::m_strAppFontNormal, "FontNormal"), "Font Normal");     
    tabWidget->addTab(new GeneralTab(Skin::m_strAppFontRegular, "FontRegular"), "Font Regular");     
    tabWidget->addTab(new GeneralTab(Skin::m_strAppFontBold, "FontBold"), "Font Bold");     
    tabWidget->addTab(new GeneralTab(Skin::m_strAppFontMedium, "FontMedium"), "Font Medium");             

    tabWidget->setTabShape(QTabWidget::Triangular);  

    tabWidget->setStyleSheet(QString("QTabWidget::pane{border:none; background:white; border-top-left-radius:0px; border-bottom-left-radius:10px; border-bottom-right-radius:10px; border-top-right-radius:10px;} QTabBar::tab{width:110; height:24;}"));    
    QVBoxLayout *mainLayout = new QVBoxLayout; 
    mainLayout->addWidget(tabWidget); 
    mainLayout->setContentsMargins(100, 0, 100, 0); 
    setLayout(mainLayout);    
  
}

FontPage::~FontPage()
{
}

GeneralTab::GeneralTab(QString &str, const QString name, QtWidgetBase *parent) 
  :QtWidgetBase(parent) 
{         
	m_fontstr = &str;
    m_fontname = name;
        
    fontLabel = new QLabel(*m_fontstr); 
    fontLabel->setFont(QFont(*m_fontstr));
    fontLabel->setAlignment(Qt::AlignBottom | Qt::AlignCenter);

    QPushButton *pushButton = new QPushButton(tr("Change"), this);	
    
    connect(pushButton, SIGNAL(clicked()), this, SLOT(getFont()));

    QVBoxLayout *mainLayout = new QVBoxLayout; 
    mainLayout->addWidget(fontLabel); 
    mainLayout->addWidget(pushButton, 0, Qt::AlignCenter); 
    
    //mainLayout->addStretch(1); 
    setLayout(mainLayout);    
}

void GeneralTab::saveFont(QFont &font)
{		

	*m_fontstr = font.family();
	fontLabel->setFont(font);
	fontLabel->setText(*m_fontstr);
	
    AppConfig::SaveSetting("System", m_fontname, font.family());
}    

void GeneralTab::getFont(void)
{
    bool ok = false;    
    QFont font(*m_fontstr); 
    
    font = QFontDialog::getFont(&ok, font); 
    if (ok)
        saveFont(font);
}

GeneralTab::~GeneralTab()
{
}
