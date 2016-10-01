#ifndef DIALOGWPTPROP_H
#define DIALOGWPTPROP_H

#include "global.h"
#include "mainWindow.h"

#include <QDialog>
#include <QCheckBox>


class dialogWptProp : public QDialog
{
    Q_OBJECT

public:

    bool dataSaved;

    dialogWptProp();

    qlonglong m_id;
    QString m_name;
    QString m_note;
    QString m_lat;
    QString m_lon;

    QString m_ele;
    QString m_time;  // un horodatage
    QString m_magvar; // l'orientation magnétique du récepteur GPS en degrés
    QString m_geoidheight;  // la hauteur du géoïde ("niveau de la mer") au-dessus de l'ellipsoïde WGS 84, tel que décrit dans la spécification des messages NMEA GGA
    QString m_cmt; // un commentaire
    QString m_desc; // une description
    QString m_src; // le modèle et le fabricant de l'appareil à l'origine de la géolocalisation du point
    QString m_fix; // le type de GPS ayant permis l'obtention du point ((fix) 2D, 3D, DGPS...)
    QString m_sat; // le nombre de satellites acquis lors du calcul des coordonnées
    QString m_dgpsid; // des informations sur les mesures DGPS comme la durée depuis la dernière mise à jour (<ageofdgpsdata>) ou le l'identifiant de la station
    QString m_links;
    QString m_sym; // le nom exact du symbole illustrant le point sur l'appareil GPS
    QString m_type; // le type de point (<>)
    QString m_hdop; // et la précision (<>, <vdhop>, <pdop> voir Geometric_dilution_of_precision)
    QString m_vdop;
    QString m_pdop;
    QString m_extensions; // optionnellement des extensions d'un autre espace de noms

    qlonglong curentWptId;
    projectDatas *m_projectDatas;

    projectDatas::wptType *wpt;

    QString iconSymbolRootDir;

public slots:
    void okClicked();
    void cancelClicked();
    void onMlinksClicked();
    void onMlinksViewClicked();
    void onSymQIconClicked();

private :

    void updateParent( void );

    QLineEdit *m_idQ;
    QLineEdit *m_nameQ;
    QLineEdit *m_groupQ;
    QLineEdit *m_noteQ;
    QLineEdit *m_latQ;
    QLineEdit *m_lonQ;
    QLineEdit *m_eleQ;
    //QLineEdit *m_timeQ;  // un horodatage
    QDateTimeEdit *m_timeQ;
    QLineEdit *m_magvarQ; // l'orientation magnétique du récepteur GPS en degrés
    QLineEdit *m_geoidheightQ;  // la hauteur du géoïde ("niveau de la mer") au-dessus de l'ellipsoïde WGS 84, tel que décrit dans la spécification des messages NMEA GGA
    QTextEdit *m_cmtQ; // un commentaire
    QTextEdit *m_descQ; // une description
    QLineEdit *m_srcQ; // le modèle et le fabricant de l'appareil à l'origine de la géolocalisation du point
    QLineEdit *m_fixQ; // le type de GPS ayant permis l'obtention du point ((fix) 2D, 3D, DGPS...)
    QLineEdit *m_satQ; // le nombre de satellites acquis lors du calcul des coordonnées
    QLineEdit *m_dgpsidQ; // des informations sur les mesures DGPS comme la durée depuis la dernière mise à jour (<ageofdgpsdata>) ou le l'identifiant de la station
    QTableWidget *m_linksQ;
    QLineEdit *m_symQ; // le nom exact du symbole illustrant le point sur l'appareil GPS
    QPushButton* m_symQIcon ;
    QLineEdit *m_typeQ; // le type de point (<>)
    QLineEdit *m_hdopQ; // et la précision (<>, <vdhop>, <pdop> voir Geometric_dilution_of_precision)
    QLineEdit *m_vdopQ;
    QLineEdit *m_pdopQ;
    QLineEdit *m_extensionsQ; // optionnellement des extensions d'un autre espace de noms

    void showEvent( QShowEvent* event ) ;
    void appendLinkTableLine( QTableWidget *tbl,  int idexLine, QString editBtnLabel, QString viewBtnLabel, QString valueLink);

};

#endif // DIALOGWPTPROP_H



