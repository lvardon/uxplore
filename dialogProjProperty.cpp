#include "dialogProjProperty.h"

#include "projectData.h"

#include <QDebug>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>


dialogProjProperty::dialogProjProperty() : QDialog(0)
{
    QGridLayout *layout = new QGridLayout();

    QPushButton *button;
    QDialogButtonBox *ButtonBox = new QDialogButtonBox( Qt::Horizontal );

    button = new QPushButton( "Ok" );
    connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::AcceptRole );

    button = new QPushButton( "Annuler" );
    connect( button, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::RejectRole );

    QLabel *projNbWptLabel = new QLabel("Nombre de balises", this);
    projNbWptLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    projNbWpt = new QLineEdit(this);
    projNbWpt->setFixedWidth(15*4);
    layout->addWidget(projNbWptLabel, 1 , 0) ;
    layout->addWidget(projNbWpt, 1 , 1);

    QLabel *m_projUpdatedLabel = new QLabel( trUtf8("Projet Sauvegardé"), this);
    m_projUpdatedLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_projUpdated  = new QLineEdit(this) ;
    layout->addWidget(m_projUpdatedLabel, 2 , 0) ;
    layout->addWidget(m_projUpdated, 2 , 1);

    QLabel *m_nameLabel = new QLabel("Nom", this);
    m_nameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_name  = new QLineEdit(this) ;
    layout->addWidget(m_nameLabel, 3 , 0) ;
    layout->addWidget(m_name, 3 , 1);

    QLabel *m_descLabel = new QLabel("Description", this);
    m_descLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_desc  = new QTextEdit(this);
    layout->addWidget(m_descLabel, 4 , 0) ;
    layout->addWidget(m_desc, 4 , 1);

    QLabel *m_author_nameLabel = new QLabel("Auteur", this);
    m_author_nameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_author_name  = new QLineEdit(this);
    layout->addWidget(m_author_nameLabel, 5 , 0) ;
    layout->addWidget(m_author_name, 5 , 1);

    QLabel *m_author_mailLabel = new QLabel("Mail", this);
    m_author_mailLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_author_mail  = new QLineEdit(this);
    layout->addWidget(m_author_mailLabel, 6 , 0) ;
    layout->addWidget(m_author_mail, 6 , 1);

    QLabel *m_author_linkLabel = new QLabel("Lien", this);
    m_author_linkLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_author_link  = new QLineEdit(this);
    layout->addWidget(m_author_linkLabel, 7 , 0) ;
    layout->addWidget(m_author_link, 7 , 1);

    QLabel *m_copyrightLabel = new QLabel("Copyright", this);
    m_copyrightLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_copyright  = new QLineEdit(this);
    layout->addWidget(m_copyrightLabel, 8 , 0) ;
    layout->addWidget(m_copyright, 8 , 1);

    QLabel *m_timeLabel = new QLabel("Date", this);
    m_timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_time  = new QLineEdit(this);
    layout->addWidget(m_timeLabel, 9 , 0) ;
    layout->addWidget(m_time, 9 , 1);

    QLabel *m_keywordsLabel = new QLabel( QString::fromUtf8("Mots clés"), this);
    m_keywordsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_keywords  = new QLineEdit(this);
    layout->addWidget(m_keywordsLabel, 10 , 0) ;
    layout->addWidget(m_keywords, 10, 1);

    QLabel *m_latminLabel = new QLabel("Latitude mini", this);
    m_latminLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_latmin = new QLineEdit(this);
    layout->addWidget(m_latminLabel, 11 , 0) ;
    layout->addWidget(m_latmin, 11 , 1);

    QLabel *m_lonminLabel = new QLabel("Longitude mini", this);
    m_lonminLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lonmin  = new QLineEdit(this);
    layout->addWidget(m_lonminLabel, 12 , 0) ;
    layout->addWidget(m_lonmin, 12 , 1);

    QLabel *m_latmaxLabel = new QLabel("Latitude maxi", this);
    m_latmaxLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_latmax  = new QLineEdit(this);
    layout->addWidget(m_latmaxLabel, 13 , 0) ;
    layout->addWidget(m_latmax, 13 , 1);

    QLabel *m_lonmaxLabel = new QLabel("Longitude maxi", this);
    m_lonmaxLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lonmax  = new QLineEdit(this);
    layout->addWidget(m_lonmaxLabel, 14 , 0) ;
    layout->addWidget(m_lonmax, 14 , 1);

    QLabel *m_extensionsLabel = new QLabel("Extentions", this);
    m_extensionsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_extensions  = new QLineEdit(this);
    layout->addWidget(m_extensionsLabel, 15 , 0) ;
    layout->addWidget(m_extensions, 15 , 1);

    layout->addWidget( ButtonBox , 99 , 1);

    setLayout(layout);

    setWindowTitle( QString::fromUtf8("Propriétés du projet"));
}

void dialogProjProperty::okClicked()
{

    mainParentForm->projectData.m_metadata.m_name = m_name->text();

    mainParentForm->projectData.m_metadata.m_desc = m_desc->toPlainText();

    mainParentForm->projectData.m_metadata.m_author_name = m_author_name->text();
    mainParentForm->projectData.m_metadata.m_author_mail = m_author_mail->text();
    mainParentForm->projectData.m_metadata.m_author_link = m_author_link->text();
    mainParentForm->projectData.m_metadata.m_copyright = m_copyright->text();
    mainParentForm->projectData.m_metadata.m_time = m_time->text();
    mainParentForm->projectData.m_metadata.m_keywords = m_keywords->text();
    mainParentForm->projectData.m_metadata.m_latmin = m_latmin->text();
    mainParentForm->projectData.m_metadata.m_lonmin = m_lonmin->text();
    mainParentForm->projectData.m_metadata.m_latmax = m_latmax->text();
    mainParentForm->projectData.m_metadata.m_lonmax = m_lonmax->text();
    mainParentForm->projectData.m_metadata.m_extensions = m_extensions->text();

    QDialog::accept();

}

void dialogProjProperty::cancelClicked()
{
    QDialog::accept();
}

void dialogProjProperty::showEvent( QShowEvent* event )
{

    QWidget::showEvent( event );

    if (mainParentForm->projectData.dataSaved == true)
    {
        m_projUpdated->setText( "Oui" );
    }
    else
    {
        m_projUpdated->setText( "Non" );
    }

    projNbWpt->setText( QString::number(mainParentForm->projectData.m_wptList.size()) );
    m_name->setText(mainParentForm->projectData.m_metadata.m_name );
    m_desc->setText(mainParentForm->projectData.m_metadata.m_desc );
    m_author_name->setText(mainParentForm->projectData.m_metadata.m_author_name );
    m_author_mail->setText(mainParentForm->projectData.m_metadata.m_author_mail );
    m_author_link->setText(mainParentForm->projectData.m_metadata.m_author_link );
    m_copyright->setText(mainParentForm->projectData.m_metadata.m_copyright );
    m_time->setText(mainParentForm->projectData.m_metadata.m_time );
    m_keywords->setText(mainParentForm->projectData.m_metadata.m_keywords );
    m_latmin->setText(mainParentForm->projectData.m_metadata.m_latmin );
    m_lonmin->setText(mainParentForm->projectData.m_metadata.m_lonmin );
    m_latmax->setText(mainParentForm->projectData.m_metadata.m_latmax );
    m_lonmax->setText(mainParentForm->projectData.m_metadata.m_lonmax );
    m_extensions->setText(mainParentForm->projectData.m_metadata.m_extensions );

}

#include "dialogProjProperty.moc"

