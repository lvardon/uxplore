#ifndef PROJECTDATAS_H
#define PROJECTDATAS_H

#include "global.h"

#include <QString>
#include <QList>
#include <QVariant>
#include <QStack>
#include <QFile>
#include <QDateTime>

//class dialogCsv;
//#include "dialogCsv.h"

#include "csvDefinition.h"


// http://openclassrooms.com/courses/stocker-et-serialiser-des-objets-avec-qt
// http://www.garmindeveloper.com/schemas/gpx/1.1/oxygen/index.html_h674234604.html
// http://fr.wikipedia.org/wiki/GPX_%28format_de_fichier%29
// http://www.topografix.com/gpx_manual.asp#gpx_req
// ftp://priede.bf.lu.lv/pub/TIS/failu_formaati/GPX/GPX_1.1.htm#type_linkType

#define COL_SORT_ORDER_UP  0
#define COL_SORT_ORDER_DOWN  1


#define COL_WPT_COUNT       24

#define COL_WPT_SELECTED    96
#define COL_WPT_ID          97
#define COL_WPT_NOTE        98
#define COL_WPT_GROUP       99
#define COL_WPT_LAT         100
#define COL_WPT_LON         101
#define COL_WPT_NAME        102
#define COL_WPT_ELE         103
#define COL_WPT_TIME        104
#define COL_WPT_MAGVAR      105
#define COL_WPT_GEOIDHEIGHT 106
#define COL_WPT_CMT         107
#define COL_WPT_DESC        108
#define COL_WPT_SRC         109
#define COL_WPT_FIX         110
#define COL_WPT_SAT         111
#define COL_WPT_DGPSID      112
//QString m_link; // mais aussi un lien vers une page web d'informations additionnelles,
//QString m_linkname; // Texte du lien
#define COL_WPT_LINKS       113
#define COL_WPT_SYM         114
#define COL_WPT_TYPE        115
#define COL_WPT_HDOP        116
#define COL_WPT_VDOP        117
#define COL_WPT_PDOP        118
#define COL_WPT_EXTENTIONS  119


#define EXPORT_FORMAT_GPX   1
#define EXPORT_FORMAT_CSV   2
#define EXPORT_FORMAT_KLM   3
#define EXPORT_FORMAT_KMZ   4

#define IMPORT_FORMAT_GPP   0
#define IMPORT_FORMAT_GPX   1
#define IMPORT_FORMAT_CSV   2
#define IMPORT_FORMAT_KLM   3
#define IMPORT_FORMAT_KMZ   4

#define FILEFORMAT_NOTFOUND    -2
#define FILEFORMAT_UNDEFINED    -1
#define FILEFORMAT_GPP  0
#define FILEFORMAT_GPX  1
#define FILEFORMAT_CSV  2
#define FILEFORMAT_KML  3
#define FILEFORMAT_KMZ  4


class projectDatas
{


public :

    bool dataSaved;
    void updateParent( void );

    QString lastErrorText;
    int importedCount;
    int exportedCount;

    struct gpx
    {
        gpx( const QString creator = "", const QString version = "", const QString schemalocation="" );

        QString m_creator; // O
        QString m_version; // O
        QString m_schemalocation;

    };

    struct metadata
    {
        metadata( const QString name = "", const QString desc = "", const QString author_name="", const QString author_mail="", const QString author_link="", const QString copyright = "", const QList<QString> links = QList<QString>(), const QString time = "", const QString keywords = "", const QString latmin = "", const QString lonmin = "", const QString latmax = "", const QString lonmax = "", const QString extention = "");

        QString m_name ; // Nom
        QString m_desc; // Description
        QString m_author_name; // l'auteur du fichier comprenant son nom, une adresse mail et un lien vers son site web.
        QString m_author_mail;
        QString m_author_link;
        QString m_copyright ; // Licence et année
        QList<QString> m_links;  // un ou plusieurs liens vers des informations additionnelles (page web, photo, vidéo, etc.)
        QString m_time; // la date et l'heure de création du fichier
        QString m_keywords;     //un ou plusieurs mots-clés
        QString m_latmin; //les frontières couvertes par le fichiers GPX (latitudes et longitudes minimum et maximum)
        QString m_lonmin;
        QString m_latmax;
        QString m_lonmax;
        QString m_extensions; // Optionnellement, des extensions permettant d'ajouter librement des éléments XML définis dans un autre espace de noms (namespace) afin d'étendre les capacités du fichier

    };

    // 0 : no import, 1=import, 2=Import and concat
    struct metadataImportFilter
    {
        int m_name ;
        int m_desc;
        int m_author_name;
        int m_author_mail;
        int m_author_link;
        int m_copyright ;
        int m_links;
        int m_time;
        int m_keywords;
        int m_latmin;
        int m_lonmin;
        int m_latmax;
        int m_lonmax;
        int m_extensions;
    };

    //Une liste de points de cheminement ou waypoint
    struct wpt
    {

        wpt( const qlonglong id=0, const bool selected=false, const QString note="", const QString group="", const QString  lat = "", const QString  lon = "", const QString  name = "", const QString  ele = "", const QDateTime time = QDateTime(), const QString  magvar = "", const QString  geoidheight = "", const QString  cmt = "", const QString  desc = "",const QString  src = "", const QString  fix = "", const QString  sat = "", const QString  dgpsid = "", const QList<QString> links = QList<QString>(), const QString  sym = "",  const QString  type = "", const QString  hdop = "", const QString  vdop = "", const QString  pdop = "", const QString  extensions = "" );

        qlonglong m_id;     // id interne
        bool m_selected;    // Select oui/non
        QString m_note;     // Wpt note (stars)
        QString m_group;    // Duplicate group

        QString m_lat; // O
        QString m_lon;  // O
        QString m_name; // un nom
        QString m_ele; // son altitude en mètres
        //QString m_time;  // un horodatage
        QDateTime m_time;  // un horodatage
        QString m_magvar; // l'orientation magnétique du récepteur GPS en degrés
        QString m_geoidheight;  // la hauteur du géoïde ("niveau de la mer") au-dessus de l'ellipsoïde WGS 84, tel que décrit dans la spécification des messages NMEA GGA
        QString m_cmt; // un commentaire
        QString m_desc; // une description
        QString m_src; // le modèle et le fabricant de l'appareil à l'origine de la géolocalisation du point
        QString m_fix; // le type de GPS ayant permis l’obtention du point ((fix) 2D, 3D, DGPS...)
        QString m_sat; // le nombre de satellites acquis lors du calcul des coordonnées
        QString m_dgpsid; // des informations sur les mesures DGPS comme la durée depuis la dernière mise à jour (<ageofdgpsdata>​) ou le l'identifiant de la station
        //QString m_link; // mais aussi un lien vers une page web d'informations additionnelles,
        //QString m_linkname; // Texte du lien
        QList<QString> m_links;
        QString m_sym; // le nom exact du symbole illustrant le point sur l'appareil GPS
        QString m_type; // le type de point (<>​)
        QString m_hdop; // et la précision (<>​, <vdhop>​, <pdop>​ voir Geometric_dilution_of_precision)
        QString m_vdop;
        QString m_pdop;
        QString m_extensions; // optionnellement des extensions d'un autre espace de noms

        wpt& operator =(const wpt& a)
        {
            m_id  = a.m_id;
            m_selected = a.m_selected;
            m_name = a.m_name;
            return *this;
        }

    };

    // Une liste d'itinéraires
    struct rte
    {

        rte( const QString name = "", const QString cmt = "", const QString desc = "", const QString src = "", const QString number = "", const QString link = "", const QString type = "", const QString extention = "");

        QString  m_name; // un nom
        QString  m_cmt; //     un commentaire
        QString  m_desc; //     une description
        QString  m_src; //     le modèle de l'appareil
        QString  m_number; //     le numéro de l'itinéraire
        QString  m_link; //     mais aussi un lien vers une page web d'informations additionnelles
        QString  m_type; // et le type d'itinéraire
        QString  m_extensions; //     optionnellement d'extensions
        //     L'itinéraire est décrit par une liste ordonnée de points (une balise <rtept>​ par point au même format que la balise <wpt>​ décrite précédemment)
        // qui représentent l'emplacement des virages permettant de mener à une destination.
    };

    // Une liste de traces ou track
    struct trk
    {
        trk( const QString name = "", const QString cmt = "", const QString desc = "", const QString src = "", const QString number = "", const QString link = "", const QString type = "", const QString extention = "");

        QString  m_name ; //         un nom
        QString  m_cmt ; //         un commentaire
        QString  m_desc; //         une description
        QString  m_src; //         le modèle de l'appareil
        QString  m_number; //         le numéro de la trace
        QString  m_link; //         mais aussi un lien vers une page web d'informations additionnelles
        QString  m_type; //         le type d'itinéraire
        QString  m_extensions; //         optionnellement des extensions
        //La trace est décrite par des segments de trace (<trkseg>​), le passage d'un segment à un autre indique une extinction du récepteur GPS ou une perte de réception.
        //Un segment de trace est constitué : d'une liste ordonnée de points de trace (<trkpt>​) dont le format est identique à la balise <wpt>​ décrite précédemment
    };

    typedef gpx gpxType;
    typedef metadata metadataType;
    typedef metadataImportFilter metadataImportFilterType;
    typedef wpt wptType;
    typedef QList<wpt> wptListType;
    typedef QList<rte> rteListType;
    typedef QList<trk> trkListType;

    projectDatas ( const QString xmlHeader = "", const gpxType gpx = gpxType(), const metadataType  metadata = metadataType(), const wptListType wptList = wptListType(), const rteListType rteList = rteListType(), const trkListType trkList = trkListType() );   // Constructeur par défaut public

    void deleteAll( void );
    void addTagValue( QStack<QString> tagStack, QString tagName, QString tagType, QString tagValue, gpx* l_gpx, metadata * m_metadata , wpt* l_wpt, rte* l_rte, trk* l_trk );
    void showTags( QStack<QString> tagStack, QString tagName, QString tagType, QString tagValue);
    const wptType *getWptFromId( qlonglong p_id) ;
    wptType *getWptFromId2( qlonglong p_id) ;
    qlonglong getMaxId(void);
    void reIndexwpt(void);
    void removeWptById( qlonglong curentWptId );
    qlonglong addNewWpt( const bool selected, const QString lat , const QString  lon , const QString  name );
    void wptSetPos( qlonglong p_id, QString newlat , QString newlon);
    void sortWpt(int colSort, int sortOrder);
    void sortWptDo(int colSort, int debut, int fin);
    bool sortCompareValues( int colSort ,  wpt * wptSrc,  wpt * wptDst);
    void swapWpt( wptType *wptSrc , wptType *wptDst);
    void copyWpt( wptType *wptSrc , wptType *wptDst);
    QString removeSpecialChars(QString text);
    QString removeAccents(QString s);

    projectDatas (const projectDatas & Copie);    // Constructeur de copie public
    ~projectDatas ();                          // Destructeur public

    static void initStruct ();
    void afficher_metadata () const;                 // Affiche les informations dans la console
    int dumpGpx( QString gpxFileName);
    bool importFromFile( int fileFormat, QString gpxFileName);
    bool importFromFile( int fileFormat, QString gpxFileName, QVector<contextMenuLibElem> *contextMenuLibListe, bool fistLineIsHeader, QString csvSepFieldValue, QString csvSepTextValue );
    bool importFromFileGpx( QString gpxFileName );
    bool importFromFileKml( QString gpxFileName );
    bool importFromFileKmz( QString gpxFileName );
    bool importFromFileCsv( QString csvFileName, QVector<contextMenuLibElem> *contextMenuLibListe ,  bool fistLineIsHeader, QString csvSepFieldValue, QString csvSepTextValue );

    bool exportToFile( int fileFormat, QString gpxFileName);
    bool exportToFile( int fileFormat, QString gpxFileName, QVector<contextMenuLibElem> *contextMenuLibListe, bool fistLineIsHeader, QString csvSepFieldValue, QString csvSepTextValue);
    bool exportToFileCsv( QString csvFileName, QVector<contextMenuLibElem> *contextMenuLibListe , bool fistLineIsHeader, QString csvSepFieldValue, QString csvSepTextValue);
    bool exportToFileCsv( QFile *outputFile, QString csvFileName ) ;
    bool exportToFileGpx( QFile *outputFile, QString gpxFileName ) ;

    bool removeDir(const QString & dirName);

    int guessFileFormat( QString gpxFileName );

    QString         m_xmlHeader;
    gpxType         m_gpx;
    metadataType    m_metadata ;
    wptListType     m_wptList;
    rteListType     m_rteList;
    trkListType     m_trkList;

    qlonglong       m_lastWptId;
    QString         m_iconSymbolRootDir;

    int             m_currentColSort;
    int             m_currentColSortOrder;

private :

    /* Les opérateurs de flux sont des fonctions amies */
    friend QDataStream & operator << (QDataStream &, const projectDatas &);
    friend QDataStream & operator >> (QDataStream &, projectDatas &);

    friend QDataStream & operator << (QDataStream &, const projectDatas::metadata &);
    friend QDataStream & operator >> (QDataStream &, projectDatas::metadata &);

    friend QDataStream & operator << (QDataStream &, const projectDatas::gpx &);
    friend QDataStream & operator >> (QDataStream &, projectDatas::gpx &);

    friend QDataStream & operator << (QDataStream &, const projectDatas::wpt &);
    friend QDataStream & operator >> (QDataStream &, projectDatas::wpt &);

    friend QDataStream & operator << (QDataStream &, const projectDatas::rte &);
    friend QDataStream & operator >> (QDataStream &, projectDatas::rte &);

    friend QDataStream & operator << (QDataStream &, const projectDatas::trk &);
    friend QDataStream & operator >> (QDataStream &, projectDatas::trk &);

};

Q_DECLARE_METATYPE(projectDatas)
QDataStream & operator << (QDataStream & out, const projectDatas & Valeur);
QDataStream & operator >> (QDataStream & in, projectDatas & Valeur);

Q_DECLARE_METATYPE(projectDatas::metadata)
QDataStream & operator << (QDataStream & out, const projectDatas::metadata & Valeur);
QDataStream & operator >> (QDataStream & in, projectDatas::metadata & Valeur);

Q_DECLARE_METATYPE(projectDatas::gpx)
QDataStream & operator << (QDataStream & out, const projectDatas::gpx & Valeur);
QDataStream & operator >> (QDataStream & in, projectDatas::gpx & Valeur);

Q_DECLARE_METATYPE(projectDatas::wpt)
QDataStream & operator << (QDataStream & out, const projectDatas::wpt & Valeur);
QDataStream & operator >> (QDataStream & in, projectDatas::wpt & Valeur);

Q_DECLARE_METATYPE(projectDatas::rte)
QDataStream & operator << (QDataStream & out, const projectDatas::rte & Valeur);
QDataStream & operator >> (QDataStream & in, projectDatas::rte & Valeur);

Q_DECLARE_METATYPE(projectDatas::trk)
QDataStream & operator << (QDataStream & out, const projectDatas::trk & Valeur);
QDataStream & operator >> (QDataStream & in, projectDatas::trk & Valeur);

#endif
