#ifndef DIALOGPROJPROPERTY_H
#define DIALOGPROJPROPERTY_H

#include "global.h"
#include "mainWindow.h"

#include <QDialog>
#include <QCheckBox>


class dialogProjProperty : public QDialog
{
    Q_OBJECT

public:
    dialogProjProperty();
    mainWindow *mainParentForm;


public slots:
    void okClicked();
    void cancelClicked();

private :

    QLineEdit *m_projUpdated ;

    QLineEdit *projNbWpt ;

    QLineEdit *m_name ; // Nom
    QTextEdit *m_desc; // Description
    QLineEdit *m_author_name; // l'auteur du fichier comprenant son nom, une adresse mail et un lien vers son site web.
    QLineEdit *m_author_mail;
    QLineEdit *m_author_link;
    QLineEdit *m_copyright ; // Licence et année
    //QList<QString> m_links;  // un ou plusieurs liens vers des informations additionnelles (page web, photo, vidéo, etc.)
    QLineEdit *m_time; // la date et l'heure de création du fichier
    QLineEdit *m_keywords;     //un ou plusieurs mots-clés
    QLineEdit *m_latmin; //les frontières couvertes par le fichiers GPX (latitudes et longitudes minimum et maximum)
    QLineEdit *m_lonmin;
    QLineEdit *m_latmax;
    QLineEdit *m_lonmax;
    QLineEdit *m_extensions; // Optionnellement, des extensions permettant d'ajouter librement des éléments XML définis dans un autre espace de noms (namespace) afin d'étendre les capacités du fichier


    void showEvent( QShowEvent* event ) ;

};

#endif // DIALOGAPPREF_H



