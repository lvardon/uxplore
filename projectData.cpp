#include "global.h"
#include "projectData.h"
#include "maplib/3rdparty/qzip/qzipreader.h"

#include "mainWindow.h"

#include <iostream>
#include <QDebug>
#include <QFile>
#include <QStack>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QRegExp>
#include <QStringList>

#include <QObject>
#include <QMessageBox>


projectDatas::gpx::gpx( const QString creator, const QString version, const QString schemalocation )
{
    m_creator = creator;
    m_version = version;
    m_schemalocation = schemalocation;
}


projectDatas::metadata::metadata(const QString name, const QString desc, const QString author_name, const QString author_mail, const QString author_link, const QString copyright, const QList<QString> links, const QString time, const QString keywords, const QString latmin , const QString lonmin , const QString latmax , const QString lonmax , const QString extention)
{
    m_name = name;
    m_desc = desc;
    m_author_name = m_author_name;
    m_author_mail = m_author_mail;
    m_author_link = m_author_link;
    m_copyright = copyright;
    m_links = links;
    m_time = time;
    m_keywords = keywords;
    m_latmin = latmin;
    m_lonmin = lonmin;
    m_latmax = latmax ;
    m_lonmax = lonmax;
    m_extensions = m_extensions;
}

projectDatas::rte::rte( const QString name, const QString cmt, const QString desc, const QString src, const QString number, const QString link, const QString type, const QString extension)
{
    m_name = name;
    m_cmt = cmt;
    m_desc = desc;
    m_src = src;
    m_number = number;
    m_link = link;
    m_type = type;
    m_extensions = extension;
}

projectDatas::trk::trk( const QString name, const QString cmt, const QString desc, const QString src, const QString number, const QString link, const QString type, const QString extension)
{
    m_name = name;
    m_cmt = cmt;
    m_desc = desc;
    m_src = src;
    m_number = number;
    m_link = link;
    m_type = type;
    m_extensions = extension;

}

//projectDatas::wpt::wpt( const qlonglong id, const bool selected, const QString note, const QString group, const QString lat , const QString  lon , const QString  name, const QString  ele , const QString  time , const QString  magvar , const QString  geoidheight , const QString  cmt , const QString  desc ,const QString  src , const QString  fix , const QString  sat , const QString  dgpsid , const QList<QString> links, const QString  sym ,  const QString  type , const QString  hdop , const QString  vdop , const QString  pdop , const QString  extensions )
projectDatas::wpt::wpt( const qlonglong id, const bool selected, const QString note, const QString group, const QString lat , const QString  lon , const QString  name, const QString  ele , const QDateTime time , const QString  magvar , const QString  geoidheight , const QString  cmt , const QString  desc ,const QString  src , const QString  fix , const QString  sat , const QString  dgpsid , const QList<QString> links, const QString  sym ,  const QString  type , const QString  hdop , const QString  vdop , const QString  pdop , const QString  extensions )
{

    m_id = id;
    m_selected = selected;
    m_note = note;
    m_group = group;
    m_lat = lat;
    m_lon = lon;
    m_name = name;
    m_ele = ele;
    m_time =  time;
    m_magvar = magvar;
    m_geoidheight = geoidheight;
    m_cmt = cmt;
    m_desc = desc;
    m_src = src;
    m_fix = fix;
    m_sat = sat;
    m_dgpsid = dgpsid;
    m_links = links;
    m_sym = sym;
    m_type = type;
    m_hdop = hdop;
    m_vdop = vdop;
    m_pdop = pdop;
    m_extensions = extensions;

}


projectDatas::projectDatas ( const QString  xmlHeader, const gpxType gpx, const metadataType metadata, const wptListType wptList, const rteListType rteList, const trkListType trkList )   // Constructeur par défaut public )
    :   m_xmlHeader(xmlHeader) , m_gpx(gpx) ,m_metadata(metadata) , m_wptList(wptList), m_rteList(rteList), m_trkList(trkList)
{}

projectDatas::~projectDatas()
{}

projectDatas::projectDatas(const projectDatas & Copie)
{
    m_xmlHeader = Copie.m_xmlHeader;
    m_gpx = Copie.m_gpx;
    m_metadata = Copie.m_metadata;
    m_wptList = Copie.m_wptList;
    m_rteList = Copie.m_rteList;
    m_trkList = Copie.m_trkList;
}


void projectDatas::afficher_metadata () const
{
    debugp( "dump project datas : " );
    debugp( "xmlheader=" + m_xmlHeader );
    debugp( "gpx.creator=" + m_gpx.m_creator  );
    debugp( "gpx.version=" + m_gpx.m_version  );
    debugp( "gpx.m_schemalocation=" + m_gpx.m_schemalocation );
    debugp( "name=" + m_metadata.m_name  );
    debugp( "desc="+ m_metadata.m_desc  );
    debugp( "author name="+ m_metadata.m_author_name  );
    debugp( "author mail="+ m_metadata.m_author_mail  );
    debugp( "author link ="+ m_metadata.m_author_link  );
    debugp( "copyright="+ m_metadata.m_copyright  );
    debugp( "keywords="+ m_metadata.m_keywords  );
    debugp( "time="+ m_metadata.m_time  );
    debugp( "min/max lat="+ m_metadata.m_latmin + "," + m_metadata.m_latmax  );
    debugp( "min/max lon="+ m_metadata.m_lonmin + "," + m_metadata.m_lonmax  );
    debugp( "extensions="+ m_metadata.m_extensions );

    QString linkCurr;
    foreach(linkCurr, m_metadata.m_links)
    debugp( "metadata link(s)=" + linkCurr );


    wpt wptCurr;
    foreach(wptCurr, m_wptList)
    {
        debugp( "--- WPT id:" + wptCurr.m_id );
        debugp( "name=" + wptCurr.m_name );
        debugp( "Lat,lon=" + wptCurr.m_lat + ", " + wptCurr.m_lon );
        debugp( "ele=" + wptCurr.m_ele );
        debugp( "time=" + wptCurr.m_time.toString() );
        debugp( "magvar=" + wptCurr.m_magvar );
        debugp( "geoheight=" + wptCurr.m_geoidheight );
        debugp( "cmt=" + wptCurr.m_cmt );
        debugp( "desc=" + wptCurr.m_desc );
        debugp( "src=" + wptCurr.m_src );
        debugp( "fix=" + wptCurr.m_fix );
        debugp( "sat=" + wptCurr.m_sat );
        debugp( "dgpsid=" + wptCurr.m_dgpsid );
        debugp( "sym=" + wptCurr.m_sym );
        debugp( "type=" + wptCurr.m_type );
        debugp( "hdop=" + wptCurr.m_hdop );
        debugp( "mvdop=" + wptCurr.m_vdop );
        debugp( "pdop=" + wptCurr.m_pdop );

        QString linkCurr;
        foreach(linkCurr, wptCurr.m_links)
        debugp( "link(s)=" + linkCurr );


    }
    //m_extensions; // optionnellement des extensions d'un autre espace de noms

    rte rteCurr;
    foreach(rteCurr, m_rteList)
    debugp( "rte:" + rteCurr.m_name) ; //<< ","<< rteCurr.m_lat << "," << rteCurr.m_lon;

    trk trkCurr;
    foreach(trkCurr, m_trkList)
    debugp ("rtk:" + trkCurr.m_name) ; //<< ","<< trkCurr.m_lat << "," << trkCurr.m_lon;

}

void projectDatas::initStruct ()
{
    qRegisterMetaTypeStreamOperators<projectDatas::metadata>("projectDatas::metadata");
    qRegisterMetaTypeStreamOperators<projectDatas::gpx>("projectDatas::gpx");
    qRegisterMetaTypeStreamOperators<projectDatas::wpt>("projectDatas::wpt");
    qRegisterMetaTypeStreamOperators<projectDatas::trk>("projectDatas::trk");
    qRegisterMetaTypeStreamOperators<projectDatas::rte>("projectDatas::rte");
    qRegisterMetaTypeStreamOperators<projectDatas>("projectDatas");
    qMetaTypeId<projectDatas::metadata>();
    qMetaTypeId<projectDatas::gpx>();
    qMetaTypeId<projectDatas::wpt>();
    qMetaTypeId<projectDatas::rte>();
    qMetaTypeId<projectDatas::trk>();
    qMetaTypeId<projectDatas>();             // Teste la validité de la classe projectDatas

}

QDataStream & operator << (QDataStream & out, const projectDatas & Valeur)
{

    debugp("--- write CLASS formatgpx "+ Valeur.m_metadata.m_name );

    out << Valeur.m_xmlHeader
        << Valeur.m_metadata
        << Valeur.m_gpx
        << Valeur.m_wptList
        << Valeur.m_rteList
        << Valeur.m_trkList;

    return out;
}
QDataStream & operator >> (QDataStream & in, projectDatas & Valeur)
{

    in >> Valeur.m_xmlHeader;
    in >> Valeur.m_metadata;
    in >> Valeur.m_gpx;
    in >> Valeur.m_wptList;
    in >> Valeur.m_rteList;
    in >> Valeur.m_trkList;

    debugp ("--- read CLASS formatgpx " + Valeur.m_metadata.m_name );

    return in;
}

QDataStream & operator << (QDataStream & out, const projectDatas::gpx & Valeur)
{
    debugp( "--- write CLASS gpx " );
    out << Valeur.m_creator
        << Valeur.m_version
        << Valeur.m_schemalocation;

    return out;
}

QDataStream & operator >> (QDataStream & in, projectDatas::gpx & Valeur)
{

    in >> Valeur.m_creator ;
    in >> Valeur.m_version;
    in >> Valeur.m_schemalocation;

    debugp("--- read CLASS gpx " );

    return in;
}

QDataStream & operator << (QDataStream & out, const projectDatas::metadata & Valeur)
{
    debugp( "--- write CLASS metadata "  + Valeur.m_name );

    out << Valeur.m_name
        << Valeur.m_desc
        << Valeur.m_author_name
        << Valeur.m_author_mail
        << Valeur.m_author_link
        << Valeur.m_copyright
        << Valeur.m_links
        << Valeur.m_time
        << Valeur.m_keywords
        << Valeur.m_latmin
        << Valeur.m_lonmin
        << Valeur.m_latmax
        << Valeur.m_lonmax
        << Valeur.m_extensions;

    return out;
}

QDataStream & operator >> (QDataStream & in, projectDatas::metadata & Valeur)
{


    in >> Valeur.m_name ;
    in >> Valeur.m_desc;
    in >> Valeur.m_author_name;
    in >> Valeur.m_author_mail;
    in >> Valeur.m_author_link;
    in >> Valeur.m_copyright;
    in >> Valeur.m_links;
    in >> Valeur.m_time;
    in >> Valeur.m_keywords;
    in >> Valeur.m_latmin;
    in >> Valeur.m_lonmin;
    in >> Valeur.m_latmax;
    in >> Valeur.m_lonmax;
    in >> Valeur.m_extensions;

    debugp("--- read CLASS metadata " + Valeur.m_name );

    return in;
}


QDataStream & operator << (QDataStream & out, const projectDatas::wpt & Valeur)
{

    debugp("--- write CLASS wpt "  + Valeur.m_name );

    out << Valeur.m_id
        << Valeur.m_selected
        << Valeur.m_note
        << Valeur.m_group
        << Valeur.m_lat
        << Valeur.m_lon
        << Valeur.m_name
        << Valeur.m_ele
        << Valeur.m_time
        << Valeur.m_magvar
        << Valeur.m_geoidheight
        << Valeur.m_cmt
        << Valeur.m_desc
        << Valeur.m_src
        << Valeur.m_fix
        << Valeur.m_sat
        << Valeur.m_dgpsid
        << Valeur.m_links
        << Valeur.m_sym
        << Valeur.m_type
        << Valeur.m_hdop
        << Valeur.m_vdop
        << Valeur.m_pdop
        << Valeur.m_extensions;

    return out;
}
QDataStream & operator >> (QDataStream & in, projectDatas::wpt & Valeur)
{
    in >> Valeur.m_id;
    in >> Valeur.m_selected;
    in >> Valeur.m_note;
    in >> Valeur.m_group;
    in >> Valeur.m_lat;
    in >> Valeur.m_lon;
    in >> Valeur.m_name;
    in >> Valeur.m_ele;
    in >> Valeur.m_time;
    in >> Valeur.m_magvar;
    in >> Valeur.m_geoidheight;
    in >> Valeur.m_cmt;
    in >> Valeur.m_desc;
    in >> Valeur.m_src;
    in >> Valeur.m_fix;
    in >> Valeur.m_sat;
    in >> Valeur.m_dgpsid;
    in >> Valeur.m_links;
    in >> Valeur.m_sym;
    in >> Valeur.m_type;
    in >> Valeur.m_hdop;
    in >> Valeur.m_vdop;
    in >> Valeur.m_pdop;
    in >> Valeur.m_extensions;

    debugp("--- read CLASS wpt " + Valeur.m_name + " id=" + QString::number(Valeur.m_id) );

    return in;
}

QDataStream & operator << (QDataStream & out, const projectDatas::rte & Valeur)
{

    debugp("--- write CLASS rte "  + Valeur.m_name );

    out << Valeur.m_name
        << Valeur.m_cmt
        << Valeur.m_desc
        << Valeur.m_src
        << Valeur.m_number
        << Valeur.m_link
        << Valeur.m_type
        << Valeur.m_extensions;

    return out;
}
QDataStream & operator >> (QDataStream & in, projectDatas::rte & Valeur)
{
    in  >> Valeur.m_name;
    in >> Valeur.m_cmt;
    in >> Valeur.m_desc;
    in >> Valeur.m_src;
    in >> Valeur.m_number;
    in >> Valeur.m_link;
    in >> Valeur.m_type;
    in >> Valeur.m_extensions;

    debugp("--- read CLASS rte " + Valeur.m_name );

    return in;
}


QDataStream & operator << (QDataStream & out, const projectDatas::trk & Valeur)
{

    debugp("--- write CLASS trk "  + Valeur.m_name );

    out << Valeur.m_name
        << Valeur.m_cmt
        << Valeur.m_desc
        << Valeur.m_src
        << Valeur.m_number
        << Valeur.m_link
        << Valeur.m_type
        << Valeur.m_extensions;

    return out;
}
QDataStream & operator >> (QDataStream & in, projectDatas::trk & Valeur)
{
    in >> Valeur.m_name;
    in >> Valeur.m_cmt;
    in >> Valeur.m_desc;
    in >> Valeur.m_src;
    in >> Valeur.m_number;
    in >> Valeur.m_link;
    in >> Valeur.m_type;
    in >> Valeur.m_extensions;

    debugp("--- read CLASS trk " + Valeur.m_name );

    return in;
}

void projectDatas::wptSetPos( qlonglong p_id, QString newlat , QString newlon)
{

    wptType *m_wpt;

    m_wpt = getWptFromId2( p_id) ;
    if (m_wpt==NULL)
    {
        debugp( "m_wpt is NULL" );
        return;
    }
    m_wpt->m_lat = newlat;
    m_wpt->m_lon = newlon;

    QDateTime cdt = QDateTime::currentDateTime();

    m_wpt->m_time = cdt;

    updateParent();


}

//class mainWindow;
//#include <QApplication>

qlonglong projectDatas::addNewWpt( const bool selected, const QString lat , const QString  lon , const QString  name )
{

    qlonglong id;
    wptType m_wpt;

    m_wpt.m_selected = selected;
    m_wpt.m_lat = lat;
    m_wpt.m_lon = lon;
    m_wpt.m_name = name;

    QDateTime cdt = QDateTime::currentDateTime();
    //m_wpt.m_time = cdt.toUTC().toString();
    m_wpt.m_time = cdt;
    m_wpt.m_cmt = QString::fromUtf8("Créé ") + " le " + cdt.toUTC().toString();

    m_wptList.append( m_wpt );

    reIndexwpt();

    id = m_wptList.count();

    debugp("add newpt " + name );

    updateParent();

    return id;

}

void projectDatas::updateParent( void )
{

    debugp("updateParent");

    dataSaved = false;

    mainWindow *p_mainWindow;

    p_mainWindow = dynamic_cast<mainWindow*>(QApplication::activeWindow());;
    if (p_mainWindow == 0)
    {
        debugp("Parent is not MainWindow !");
    }
    else
        p_mainWindow->OnProjectDataUnSaved();

}

void projectDatas::deleteAll( void )
{
    m_xmlHeader = "";

    m_gpx = gpxType();

    m_metadata = metadataType();

    m_wptList.clear();
    m_rteList.clear();
    m_trkList.clear();

    updateParent();

}

int  projectDatas::dumpGpx( QString gpxFileName)
{
    debugp("Enter dump GPX" );

    QFile file(gpxFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        debugp("File open error:" + file.errorString()  + " File = " + gpxFileName);
        return 1;
    }
    QXmlStreamReader inputStream(&file);
    while (!inputStream.atEnd() && !inputStream.hasError())
    {
        inputStream.readNext();
        if (inputStream.isStartElement())
        {
            QString name = inputStream.name().toString();
            if (name == "wpt")
                debugp("lon:" + inputStream.attributes().value("lon").toString() + " Lat:" + inputStream.attributes().value("lat").toString() );
        }
    }
    return 0;
}



int projectDatas::guessFileFormat( QString gpxFileName )
{

    int fileFormat;
    int elementLevel;
    QString tagName;

    QFileInfo check_file(gpxFileName);
    if ( !check_file.exists() || !check_file.isFile())
    {
        return FILEFORMAT_NOTFOUND;
    }

    fileFormat = FILEFORMAT_UNDEFINED;
    lastErrorText="";

    QFile file(gpxFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        lastErrorText = "File open error:"  +  file.errorString()  + " File = " + gpxFileName;
        debugp(lastErrorText);
        return FILEFORMAT_UNDEFINED;
    }

    QXmlStreamReader xml(&file);

    elementLevel = 0;
    while (!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            elementLevel++;
            tagName = xml.name().toString();
            xml.readNext();
            if ( xml.text().toString().trimmed() != "" )
            {
                debugp ( "*  start element name/text : '" + QString::number(elementLevel) + " : " + tagName + "' , '" + xml.text().toString() + "'" );
            }
            else
            {
                debugp ( "*  start element name : '" + QString::number(elementLevel) + " : "+ tagName + "'");
            }

            if ( elementLevel == 1 && tagName == "kml")
            {
                debugp ( "kml file format detect");
                return FILEFORMAT_KML;
            }
            if ( elementLevel == 1 && tagName == "gpx")
            {
                debugp ( "gpx file format detect");
                return FILEFORMAT_GPX;
            }

        }
        else if (xml.hasError())
        {
            lastErrorText = xml.errorString();
            debugp (lastErrorText);
        }
        else if (xml.isEndElement())
        {
            elementLevel--;

        }
    }

    int lineIndex=0;

    // GPP ?
    QFile inputFile(gpxFileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd() && lineIndex <= 0)
        {
            QString line = in.readLine();
            debugp( "LINE 0 is = " + line + "left = " + line.left(10));
            if (line.left(10) == "[gppdatas]")
            {
                inputFile.close();
                return FILEFORMAT_GPP;
            }
            lineIndex++;

        }
        inputFile.close();
    }


    // Kmz ?
    // https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html
    /*
    PK 50 4B 03 04
    PK 50 4B 05 06(empty archive)
    PK 50 4B 07 08 (spanned archive)
    */
    QFile fileB(gpxFileName);
    char fileByte[6];

    if (!fileB.open(QIODevice::ReadOnly))
    {
        debugp( "Erreur de lecture binaire");

    }
    //Buffer=fileB.readAll();
    fileB.read(&fileByte[0],sizeof(char));
    fileB.read(&fileByte[1],sizeof(char));
    fileB.read(&fileByte[2],sizeof(char));
    fileB.read(&fileByte[3],sizeof(char));
    fileB.read(&fileByte[4],sizeof(char));
    fileB.read(&fileByte[5],sizeof(char));

    debugp("Byte 0 =" + QString::number(static_cast<unsigned char>(fileByte[0]), 16).toUpper() );
    debugp("Byte 1 =" + QString::number(static_cast<unsigned char>(fileByte[1]), 16).toUpper() );
    debugp("Byte 2 =" + QString::number(static_cast<unsigned char>(fileByte[2]), 16).toUpper() );
    debugp("Byte 3 =" + QString::number(static_cast<unsigned char>(fileByte[3]), 16).toUpper() );
    debugp("Byte 4 =" + QString::number(static_cast<unsigned char>(fileByte[4]), 16).toUpper() );
    debugp("Byte 5 =" + QString::number(static_cast<unsigned char>(fileByte[5]), 16).toUpper() );

    //if (fileByte[0] == 'P' && fileByte[1] == 'K' && fileByte[3] == 0x03 && fileByte[4] == 0x04) fileFormat = FILEFORMAT_KMZ;
    if (
        QString::number(static_cast<unsigned char>(fileByte[0]), 16).toUpper() == "50" &&
        QString::number(static_cast<unsigned char>(fileByte[1]), 16).toUpper() == "4B" &&
        QString::number(static_cast<unsigned char>(fileByte[2]), 16).toUpper() == "3" &&
        QString::number(static_cast<unsigned char>(fileByte[3]), 16).toUpper() == "4"
    )
    {
        fileFormat = FILEFORMAT_KMZ;
        return fileFormat ;
        fileB.close();
    }



    //debugp( "fileByte[1]=" + fileByte[1]) ;
    //debugp( "fileByte[2]=" + fileByte[2]) ;

    fileB.close();

    // CSV ?
    int lineColCount;
    lineColCount = 0;
    QRegExp rx("(\\;|\\t)");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd() && lineIndex >= 0)
        {
            QString line = in.readLine();
            //debugp( "LINE csv = " + line) ;
            QStringList colCollection = line.split(rx);
            if (lineColCount == 0 ) lineColCount = colCollection.size();
            else
            {
                if (lineColCount == colCollection.size())
                {
                    fileFormat = FILEFORMAT_CSV;
                    lineIndex = -1;
                }
            }

            lineIndex++;
        }
        inputFile.close();
    }
    if (fileFormat != FILEFORMAT_UNDEFINED) return FILEFORMAT_CSV;


    return fileFormat;

}


bool projectDatas::removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists())
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            if (info.isDir())
            {
                result = removeDir(info.absoluteFilePath());
            }
            else
            {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result)
            {
                return result;
            }
        }
        result = QDir().rmdir(dirName);
    }
    return result;
}

bool projectDatas::importFromFile( int fileFormat, QString gpxFileName)
{
    bool trtResult;

    debugp("fileFormat=" + QString::number(fileFormat));

    importedCount = 0;

    trtResult= 1;
    switch (fileFormat)
    {
    case IMPORT_FORMAT_KLM :
        trtResult = importFromFileKml( gpxFileName );
        break;
    case IMPORT_FORMAT_GPX :
        trtResult = importFromFileGpx( gpxFileName );
        break;
    case IMPORT_FORMAT_KMZ :
        trtResult = importFromFileKmz( gpxFileName );
        break;
        //case IMPORT_FORMAT_CSV :
        //    trtResult = importFromFileCsv( gpxFileName );
        //    break;
    }

    return trtResult;
}
bool projectDatas::importFromFile( int fileFormat, QString gpxFileName, QVector<contextMenuLibElem> *contextMenuLibListe, bool fistLineIsHeader, QString csvSepFieldValue, QString csvSepTextValue )
{
    bool trtResult;

    debugp("fileFormat=" + QString::number(fileFormat));

    importedCount = 0;

    trtResult= 1;
    switch (fileFormat)
    {
    case IMPORT_FORMAT_CSV :
        trtResult = importFromFileCsv( gpxFileName, contextMenuLibListe , fistLineIsHeader, csvSepFieldValue, csvSepTextValue);
        break;
    }

    return trtResult;
}

bool  projectDatas::importFromFileCsv( QString gpxFileName, QVector<contextMenuLibElem> *contextMenuLibListe , bool fistLineIsHeader, QString csvSepFieldValue, QString csvSepTextValue)
{
    bool importResult;
    int m_lastWptId;
    metadata    l_metadata ;
    wpt         l_wpt;
    QString fieldFmt;

    importResult = false;

    m_lastWptId = getMaxId();

    importResult = false;
    importedCount = 0;

    int rowData;
    //int rowDisplay;

    QFile file(gpxFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    rowData=0;
    //rowDisplay=0;
    while( !in.atEnd()  )
    {
        QString line = in.readLine();
        debugp("Line #" + QString::number(rowData) + " =" + line);

        if ( rowData == 0 && fistLineIsHeader == true)
        {
            debugp("Skip Line 0 ");
            rowData++;
        }
        else
        {
            rowData++;
            QStringList fields = line.split(QRegExp(csvSepFieldValue), QString::KeepEmptyParts );

            int fieldPos = 0;

            foreach (const QString &field, fields)
            {
                fieldPos++;
                //debugp(QString::number(fieldPos) + " =" + field );
                for (int j = 0 ; j< COL_WPT_COUNT ; j++)
                {

                    if (contextMenuLibListe->at(j).order == fieldPos-1)
                    {

                        QString debugStr ;
                        fieldFmt = field;

                        //debugp( field + " ->" + contextMenuLibListe->at(j).name  + "   order=" + QString::number(contextMenuLibListe->at(j).order) + "   Format=" + contextMenuLibListe->at(j).format  + "  field len = " + QString::number(fieldFmt.length()) );

                        if (fieldFmt.length() >= csvSepTextValue.length() &&
                                fieldFmt.mid(0,csvSepTextValue.length()) == csvSepTextValue &&
                                fieldFmt.mid(fieldFmt.length() - 1 , csvSepTextValue.length()) == csvSepTextValue)
                        {
                            fieldFmt.remove(0, csvSepTextValue.length());
                            fieldFmt.chop( csvSepTextValue.length() );
                        }

                        switch (contextMenuLibListe->at(j).id)
                        {

                        case  COL_WPT_NAME :
                            l_wpt.m_name  = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;
                            break;
                        case COL_WPT_NOTE :
                            l_wpt.m_note  = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;
                            break;
                        case COL_WPT_GROUP :
                            l_wpt.m_group = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;
                        case COL_WPT_LAT :
                            l_wpt.m_lat = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_LON :
                            l_wpt.m_lon = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_ELE :
                            l_wpt.m_ele = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_TIME :
                            l_wpt.m_time = QDateTime::fromString(fieldFmt, contextMenuLibListe->at(j).format );
                            //QLocale locale(QLocale::English, QLocale::UnitedStates);
                            //QDateTime dt = locale.toDateTime("Jun 18 2015", "MMM dd yyyy");
                            debugStr = QDateTime::fromString(fieldFmt, contextMenuLibListe->at(j).format ) .toString();
                            debugp("dte formated = " + debugStr ) ;
                            break;

                        case COL_WPT_MAGVAR :
                            l_wpt.m_magvar = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_GEOIDHEIGHT :
                            l_wpt.m_geoidheight = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_DESC :
                            l_wpt.m_desc = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_CMT :
                            l_wpt.m_cmt = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_SRC :
                            l_wpt.m_src = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_FIX :
                            l_wpt.m_fix = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_SAT :
                            l_wpt.m_sat = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_DGPSID :
                            l_wpt.m_dgpsid = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_LINKS :
                            //m_linksCsv="";
                            //for (QStringList::iterator it = m_wpt->m_links.begin(); it != m_wpt->m_links.end(); ++it)
                            //{
                            //    QString current = *it;
                            //    m_linksCsv += current + "," ;
                            //
                            // }
                            l_wpt.m_links.append(field);
                            break;

                        case COL_WPT_SYM :
                            l_wpt.m_sym = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_TYPE :
                            l_wpt.m_type = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_HDOP :
                            l_wpt.m_hdop = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_VDOP :
                            l_wpt.m_vdop = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_PDOP :
                            l_wpt.m_pdop = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;

                        case COL_WPT_EXTENTIONS :
                            l_wpt.m_extensions = QString(contextMenuLibListe->at(j).format ).arg( fieldFmt ) ;;
                            break;
                        }
                    }
                }

            } // end foreach line fields

            m_lastWptId++;
            l_wpt.m_id = m_lastWptId;
            m_wptList.append(l_wpt);
            l_wpt = wpt();

            //debugp( "doImport : id=" + QString::number(contextMenuLib->at(j).id) + " name=" + contextMenuLib->at(j).name + "   order=" + QString::number(contextMenuLib->at(j).order) + "   Format=" + contextMenuLib->at(j).format );
            importedCount++;

        }
    }

    file.close();

    if (importedCount) importResult = true;
    debugp("Import CSV done : " + gpxFileName);


    return importResult;
}

bool  projectDatas::importFromFileKmz( QString gpxFileName )
{

    QStack<QString> tagStack;
    QString tagName;

    QString     l_xmlHeader;
    metadata    l_metadata ;
    gpx         l_gpx;
    wpt         l_wpt;
    rte         l_rte;
    trk         l_trk;

    lastErrorText = "";

    tagName = "";

    debugp("Enter import KMZ : " + gpxFileName   );

    QFile file(gpxFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        lastErrorText = "File open error:"  +  file.errorString()  + " File = " + gpxFileName;
        debugp(lastErrorText);
        return false;
    }

    QString tmpPath = QDir::tempPath() ;
    QTemporaryFile *tmpFile = new QTemporaryFile( QString ( "XXXXXX" ) );
    //tmpDir-> autoRemove = true;    is ON by default Qt4.8
    tmpFile->open();
    QFileInfo fi( tmpFile->fileName() );
    QString tmpDir = tmpPath + "/" + fi.fileName();
    tmpFile->remove();
    QDir dir(tmpDir );
    if (!dir.exists())
    {
        dir.mkpath(tmpDir);
        debugp("tmpPath=" + tmpPath);
        debugp("tmpDir=" + tmpDir ) ;
        debugp("kmz gpxFileName=" + gpxFileName );

        QZipReader zip( gpxFileName, QIODevice::ReadOnly);
        for(int i=0; i<zip.count(); i++)
        {
            QZipReader::FileInfo fi = zip.entryInfoAt(i);
            QFileInfo fileInfo( fi.filePath );
            QString fileKmlUncompressed = QString(  tmpDir + "/" + fileInfo.baseName( ) + "." + fileInfo.suffix() );
            debugp(" check entry : " +  fi.filePath +" to " + fileKmlUncompressed);
            if (fileInfo.suffix() == "kml" && fileInfo.baseName().trimmed() != "" )
            {
                debugp("unzip this file and load... : " + fi.filePath);
                QByteArray data = zip.fileData(fi.filePath);
                if (data.isEmpty())
                {
                    lastErrorText = "Err: No data in extracted file";
                    debugp( lastErrorText );
                    return false;
                }
                else
                {
                    debugp("decompressing file to : " + fileKmlUncompressed );
                    QFile out(fileKmlUncompressed );
                    if (!out.open(QIODevice::WriteOnly))
                    {
                        lastErrorText = "Err : Could not open zip stream";
                        debugp( lastErrorText );
                        return false;
                    }

                    if (out.write(data) != data.count())
                    {
                        lastErrorText = "Err : Could not write extracted data";
                        debugp( lastErrorText );
                        return false;
                    }

                    importFromFileKml(fileKmlUncompressed);

                }
            }
        }
        removeDir(tmpDir);
    }
    else
    {
        lastErrorText = "Err : impossible de créer un dossier temporaire : " + tmpDir;
        return false;
    }

    return true;
}

// http://geoportal-arcal.github.io/Google%20Earth/2001/01/04/understanding-kml-structure/
bool  projectDatas::importFromFileKml( QString gpxFileName )
{

    QStack<QString> tagStack;
    QString tagName;

    QString     l_xmlHeader;
    metadata    l_metadata ;
    wpt         l_wpt;
    gpx         l_gpx;
    rte         l_rte;
    trk         l_trk;
    bool        metadataAdded = false;
    int m_lastWptId;

    m_lastWptId = getMaxId();
    lastErrorText = "";
    tagName = "";

    debugp("Enter import KML : " + gpxFileName   );

    QFile file(gpxFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        lastErrorText = "File open error:"  +  file.errorString()  + " File = " + gpxFileName;
        debugp(lastErrorText);
        return false;
    }

    QXmlStreamReader xml(&file);
    QString tabChar("------------");

    while (!xml.atEnd() && !xml.hasError())
    {
        //debugp( "start loop");
        xml.readNext();

        if (xml.isEndElement())
        {
            int tagIndex=tagStack.count()-1;
            if ( tagIndex >= 0 && tagStack.at(tagIndex) == "Placemark" )
            {
                debugp("CREATE poi: " + tagStack.at(tagIndex) );
                debugp("m_name = " + l_wpt.m_name);
                debugp("m_des = " + l_wpt.m_desc);
                debugp("m_lat = " + l_wpt.m_lat);
                debugp("m_lon = " + l_wpt.m_lon);
                m_lastWptId++;
                l_wpt.m_id = m_lastWptId;
                m_wptList.append(l_wpt);
                importedCount++;
            }

            if (tagStack.count() > 0)
                tagStack.pop();

        }
        else if (xml.isCDATA() )
        {
            debugp("CDATA" + xml.text().toString());
            int tagIndex=tagStack.count()-1;
            if ( tagIndex >1 && tagStack.at(tagIndex-1) =="Placemark" && tagStack.at(tagIndex) == "description")
            {
                debugp("found description = " + xml.text().toString());
                l_wpt.m_desc = xml.text().toString();
            }
            if ( tagIndex >1 && tagStack.at(tagIndex-1) =="Placemark" && tagStack.at(tagIndex) == "name")
            {
                debugp("found name : " + tagStack.at(tagIndex) + " = " + xml.text().toString() + "'");
                l_wpt.m_name = xml.text().toString();

            }
        }
        else if (xml.isStartElement() )
        {
            //elementLevel++;
            //debugp( "startElement");

            tagName = xml.name().toString();
            xml.readNext();

            tagStack.push(tagName);

            int tagIndex=0;

            QString tagStr;
            tagStr="";
            while ( tagIndex < tagStack.count())
            {
                if (tagIndex +1 == tagStack.count())
                {
                    if ( xml.text().toString().trimmed() != "" )
                    {
                        //debugp (  QString::number(tagIndex) + ":'"  + tagName + "' , '" + xml.text().toString() + "'" );
                        tagStr = tagStr + QString::number(tagIndex) + ":'"  + tagName + "' v= '" + xml.text().toString() + "'" ;
                    }
                    else
                    {
                        //debugp ( QString::number(tagIndex) + ":'"+ tagName + "'");
                        tagStr = tagStr + QString::number(tagIndex) + ":'"+ tagName + "'";
                    }
                }
                else
                {
                    //debugp( QString::number(tagIndex) + ":" + tagStack.at(tagIndex) );
                    tagStr = tagStr + QString::number(tagIndex) + ":" + tagStack.at(tagIndex);
                }
                tagIndex++;
                tagStr = tagStr + " - ";
            }

            debugp(tagStr);

            QXmlStreamAttributes attributes = xml.attributes();
            foreach(QXmlStreamAttribute attribute, attributes)
            {
                debugp("    Attribute: name/values = " + attribute.name().toString() + ", attv=" + attribute.value().toString() );
            }

            //debugp( "" );

            tagIndex=tagStack.count()-1;

            if ( tagIndex >= 0 && tagStack.at(tagIndex) == "Placemark" )
            {
                debugp("int poi: " + tagStack.at(tagIndex) );
                l_wpt = wpt();
            }

            if ( tagIndex >1 && tagStack.at(tagIndex-1) =="Placemark" && tagStack.at(tagIndex) == "description")
            {

                debugp("ADD des: " + tagStack.at(tagIndex) + " = " + xml.text().toString() + "'");
                l_wpt.m_desc = xml.text().toString();
            }

            if ( tagIndex >1 && tagStack.at(tagIndex-1) =="Placemark" && tagStack.at(tagIndex) == "name")
            {
                debugp("ADD name : " + tagStack.at(tagIndex) + " = " + xml.text().toString() + "'");
                l_wpt.m_name = xml.text().toString();

            }
            if ( tagIndex >2 && tagStack.at(tagIndex-2) =="Placemark" && tagStack.at(tagIndex-1) =="Point" && tagStack.at(tagIndex) == "coordinates")
            {
                debugp("ADD coo : " + tagStack.at(tagIndex) + " = " + xml.text().toString() + "'");
                QStringList lonlat = xml.text().toString().split(",");
                l_wpt.m_lon = lonlat.at(0);
                l_wpt.m_lat = lonlat.at(1);
            }

        } // end isStartElement()
        else if (xml.hasError())
        {
            lastErrorText = xml.errorString() + " Ligne=" + QString::number(xml.lineNumber()) + ", Col=" + QString::number(xml.columnNumber() ) ;
            debugp (lastErrorText);
            return false;
            /*
            errorStream << XmlStreamLint::tr(
                           "Error: %1 in file %2 at line %3, column %4.\n").arg(
                               reader.errorString(), inputFilePath,
                               QString::number(reader.lineNumber()),
                               QString::number(reader.columnNumber()));
                               */
        }

        //debugp( "end loop");

    } // while

    debugp( "done loop");
    if (xml.hasError())
    {

        lastErrorText = xml.errorString();
        debugp (lastErrorText);
        return false;

    }
    else if (xml.atEnd())
    {
        debugp ("Parse done");
    }

    if (metadataAdded == false)
    {
        m_metadata = l_metadata;
    }

    return true;
}

bool  projectDatas::importFromFileGpx( QString gpxFileName )
{
    QStack<QString> tagStack;
    QString     l_xmlHeader;
    metadata    l_metadata ;
    gpx         l_gpx;
    wpt         l_wpt;
    rte         l_rte;
    trk         l_trk;
    bool        metadataAdded = false;

    int elementLevel;

    debugp("Enter import GPX : " + gpxFileName   );

    QFile file(gpxFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        debugp("File open error:"  +  file.errorString()  + " File = " + gpxFileName);
        return false;
    }

    QXmlStreamReader inputStream(&file);

    elementLevel=0;
    while (!inputStream.atEnd() && !inputStream.hasError())
    {
        elementLevel++;
        inputStream.readNext();

        if (inputStream.isStartElement())
        {
            elementLevel=1;
            debugp ("* push " + inputStream.name().toString() );
            tagStack.push( inputStream.name().toString() );
            QXmlStreamAttributes allAttributes = inputStream.attributes();
            foreach(const QXmlStreamAttribute &att, allAttributes)
            {
                addTagValue( tagStack, att.name().toString(), "att", att.value().toString(), &l_gpx, &l_metadata, &l_wpt, &l_rte, &l_trk);
                //showTags( tagStack, att.name().toString(), "att", att.value().toString() );
            }
        }

        if (inputStream.isEndElement())
        {
            elementLevel--;
            debugp("end element detected. " + inputStream.name().toString() + " elementLevel=" + elementLevel );

            // Empty elements are also reported as StartElement, followed directly by EndElement.
            if (elementLevel==1)
            {
                debugp("end element detected. [" + inputStream.name().toString() + "]" + " -> empty" );
                addTagValue( tagStack, "" , "txt" , "" , &l_gpx, &l_metadata , &l_wpt, &l_rte, &l_trk );
                tagStack.pop();
                continue;
            }
            else
            {
                debugp("end element detected. [" +  inputStream.name().toString() + "]" );
            }

            if (inputStream.name().toString() == "metadata")
            {
                m_metadata = l_metadata;
                metadataAdded=true;
            }
            if (inputStream.name().toString() == "gpx")
            {
                m_gpx = l_gpx;
            }
            if (inputStream.name().toString() == "wpt")
            {
                m_wptList.append(l_wpt);
                l_wpt = wpt();
                importedCount++;
            }
            tagStack.pop();
        }

        if ( inputStream.isCharacters() )
        {
            if ( inputStream.text().toString().trimmed() != "")
            {
                debugp ("addTagValue txt " +inputStream.text().toString().trimmed() );
                addTagValue( tagStack, "" , "txt" , inputStream.text().toString().trimmed() , &l_gpx, &l_metadata , &l_wpt, &l_rte, &l_trk );
                //showTags( tagStack , "", "txt", inputStream.text().toString() );
            }
        }

    }

    if (metadataAdded == false)
    {
        m_metadata = l_metadata;
    }

    /* Error handling. */
    if(inputStream.hasError())
    {
        debugp( "Xml error : " + inputStream.errorString() );
        inputStream.clear();
        return false;
    }
    else if (inputStream.atEnd())
    {
        debugp("end xml ok ");
    }

    inputStream.clear();

    debugp ("***************** end parse ");

    updateParent();

    return true;
}


bool projectDatas::exportToFile( int fileFormat, QString gpxFileName)
{
    bool trtResult;

    exportedCount = 0;

    QFile outputFile(gpxFileName);
    outputFile.open(QIODevice::WriteOnly);

    /* Check it opened OK */
    if(!outputFile.isOpen())
    {
        debugp("Error, unable to open file : " + gpxFileName + " for output" );
        return false;
    }

    trtResult = 1;
    switch (fileFormat)
    {
    //case EXPORT_FORMAT_CSV :
    //    trtResult = exportToFileCsv( &outputFile, gpxFileName );
    //    break;
    case EXPORT_FORMAT_GPX :
        trtResult = exportToFileGpx( &outputFile, gpxFileName );
        break;
    }

    outputFile.close();
    return trtResult;
}


bool projectDatas::exportToFile( int fileFormat, QString gpxFileName, QVector<contextMenuLibElem> *contextMenuLibListe, bool fistLineIsHeader, QString csvSepFieldValue, QString csvSepTextValue )
{
    bool trtResult;

    trtResult = false;

    trtResult = exportToFileCsv(  gpxFileName , contextMenuLibListe, fistLineIsHeader, csvSepFieldValue, csvSepTextValue);

    return trtResult;
}

bool  projectDatas::exportToFileCsv( QString csvFileName, QVector<contextMenuLibElem> *contextMenuLibListe , bool fistLineIsHeader, QString csvSepFieldValue, QString csvSepTextValue)
{

    QFile outputFile(csvFileName);
    outputFile.open(QIODevice::WriteOnly);

    lastErrorText = "";

    /* Check it opened OK */
    if(!outputFile.isOpen())
    {
        debugp("Error, unable to open file : " + csvFileName + " for output" );
        lastErrorText = "Ouverture impossible de : " + csvFileName + ".";
        return false;
    }

    /* Point a QTextStream object at the file */
    QTextStream outStream(&outputFile);

    QString m_linksCsv;
    projectDatas::wptType wptCurr;
    QList<projectDatas::wptType>::iterator it = m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    int row=0;
    QString lineOut;
    QString lineHead;
    while (it != m_wptList.end())
    {
        //if (m_wpt->m_selected == false) continue;

        lineOut = "";
        lineHead = "";

        for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
        {

            if (contextMenuLibListe->at(j).order == -1 )
                continue;

            lineOut += csvSepTextValue;
            lineHead += csvSepTextValue;

            switch (contextMenuLibListe->at(j).id)
            {

            case  COL_WPT_NAME :
                lineOut += m_wptList[row].m_name ;
                lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_NOTE :
                 lineOut += m_wptList[row].m_note ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_GROUP :
                 lineOut += m_wptList[row].m_group ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_LAT :
                 lineOut += m_wptList[row].m_lat ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_LON :
                 lineOut += m_wptList[row].m_lon ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_ELE :
                 lineOut += m_wptList[row].m_ele ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_TIME :
                 lineOut += m_wptList[row].m_time.toString( contextMenuLibListe->at(j).format );
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_MAGVAR :
                 lineOut += m_wptList[row].m_magvar;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_GEOIDHEIGHT :
                 lineOut += m_wptList[row].m_geoidheight ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_DESC :
                 lineOut += m_wptList[row].m_desc ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_CMT :
                 lineOut += m_wptList[row].m_cmt ;
                 lineHead += contextMenuLibListe->at(j).name;

                break;

            case COL_WPT_SRC :
                 lineOut += m_wptList[row].m_src ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_FIX :
                 lineOut += m_wptList[row].m_fix ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_SAT :
                 lineOut += m_wptList[row].m_sat ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_DGPSID :
                 lineOut += m_wptList[row].m_dgpsid;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_LINKS :
                m_linksCsv="";
                for (QStringList::iterator itLinsks = m_wptList[row].m_links.begin(); itLinsks != m_wptList[row].m_links.end(); ++itLinsks)
                {
                    QString current = *itLinsks;
                    m_linksCsv += current + "," ;
                }

                 lineOut += m_linksCsv ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_SYM :
                 lineOut += m_wptList[row].m_sym ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_TYPE :
                 lineOut += m_wptList[row].m_type ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_HDOP :
                 lineOut += m_wptList[row].m_hdop;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_VDOP :
                 lineOut += m_wptList[row].m_vdop ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_PDOP :
                 lineOut += m_wptList[row].m_pdop ;
                 lineHead += contextMenuLibListe->at(j).name;
                break;

            case COL_WPT_EXTENTIONS :
                //return m_wpt->m_extensions;
                break;
            }

            lineOut += csvSepTextValue + csvSepFieldValue;
            lineHead += csvSepTextValue + csvSepFieldValue;

        }

        debugp( lineOut );

        if ( row==0 && fistLineIsHeader==true) {
            outStream << lineHead ;
            outStream << '\n';
        }


        outStream << lineOut ;
        outStream << '\n';

        ++it;
        index_list++;
        row++;

        exportedCount++;
    }

    outputFile.close();

    return true;

}

bool projectDatas::exportToFileCsv( QFile *outputFile, QString gpxFileName )
{

    QString elemSeparator;

    elemSeparator = '\t';

    /* Point a QTextStream object at the file */
    QTextStream outStream(outputFile);

    debugp("Exporting CSV .... ");

    // header line
    outStream << "id"  << elemSeparator
              << "name" << elemSeparator
              << "note" << elemSeparator
              << "group" << elemSeparator
              << "lat" << elemSeparator
              << "lon" << elemSeparator
              << "ele" << elemSeparator
              << "time" << elemSeparator
              << "magvar" << elemSeparator
              << "geoheight" << elemSeparator
              << "cmt" << elemSeparator
              << "desc" << elemSeparator
              << "src" << elemSeparator
              << "fix" << elemSeparator
              << "sat" << elemSeparator
              << "dgpsid" << elemSeparator
              << "sym" << elemSeparator
              << "type" << elemSeparator
              << "hdop" << elemSeparator
              << "mdop" << elemSeparator
              << "pdop" ;

    outStream << '\n';

    projectDatas::wptType wptCurr;
    QList<projectDatas::wptType>::iterator it = m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    int row=0;
    while (it != m_wptList.end())
    {
        outStream << m_wptList[row].m_id << elemSeparator
                  << m_wptList[row].m_name << elemSeparator
                  << m_wptList[row].m_note << elemSeparator
                  << m_wptList[row].m_group << elemSeparator
                  << m_wptList[row].m_lat << elemSeparator
                  << m_wptList[row].m_lon << elemSeparator
                  << m_wptList[row].m_ele << elemSeparator
                  << m_wptList[row].m_time.toString() << elemSeparator
                  << m_wptList[row].m_magvar << elemSeparator
                  << m_wptList[row].m_geoidheight << elemSeparator
                  << m_wptList[row].m_cmt << elemSeparator
                  << m_wptList[row].m_desc << elemSeparator
                  << m_wptList[row].m_src << elemSeparator
                  << m_wptList[row].m_fix << elemSeparator
                  << m_wptList[row].m_sat << elemSeparator
                  << m_wptList[row].m_dgpsid << elemSeparator
                  << m_wptList[row].m_sym << elemSeparator
                  << m_wptList[row].m_type << elemSeparator
                  << m_wptList[row].m_hdop << elemSeparator
                  << m_wptList[row].m_vdop << elemSeparator
                  << m_wptList[row].m_pdop ;

        outStream << '\n';

        ++it;
        index_list++;
        row++;

        exportedCount++;
    }

    return true;
}

bool projectDatas::exportToFileGpx( QFile *outputFile, QString gpxFileName )
{
    debugp("Exporting Gpx .... " + gpxFileName );

    QXmlStreamWriter xmlWriter;

    xmlWriter.setDevice(outputFile);
    QTextStream outStream(outputFile);

    xmlWriter.writeStartDocument();
    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeStartElement("gpx");
    xmlWriter.writeAttribute("version", "1.1");
    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeStartElement("metadata");
    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeStartElement("name");
    xmlWriter.writeCharacters ( m_metadata.m_name  );
    xmlWriter.writeEndElement();
    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeStartElement("creator");
    xmlWriter.writeCharacters (  m_metadata.m_author_name );
    xmlWriter.writeEndElement();
    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeStartElement("time");
    xmlWriter.writeCharacters (  m_metadata.m_time );
    xmlWriter.writeEndElement();
    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeStartElement("bounds");
    xmlWriter.writeAttribute("minlat", m_metadata.m_latmin);
    xmlWriter.writeAttribute("minlon", m_metadata.m_latmax);
    xmlWriter.writeAttribute("maxlat", m_metadata.m_lonmin);
    xmlWriter.writeAttribute("maxlon", m_metadata.m_lonmax);
    xmlWriter.writeEndElement();
    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeStartElement("desc");
    xmlWriter.writeCharacters (  m_metadata.m_desc );
    xmlWriter.writeEndElement();
    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeStartElement("keywords");
    xmlWriter.writeCharacters (  m_metadata.m_keywords );
    xmlWriter.writeEndElement();
    xmlWriter.writeCharacters ( "\n"  );

    for (QStringList::iterator it = m_metadata.m_links.begin();
            it != m_metadata.m_links.end(); ++it)
    {
        QString current = *it;
        xmlWriter.writeStartElement("link", current);
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );
    }

    xmlWriter.writeEndElement(); // end metadata

    projectDatas::wptType wptCurr;
    QList<projectDatas::wptType>::iterator it = m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    int row=0;
    while (it != m_wptList.end())
    {
        xmlWriter.writeStartElement("wpt");
        xmlWriter.writeAttribute("lat", m_wptList[row].m_lat);
        xmlWriter.writeAttribute("lon", m_wptList[row].m_lon);
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("ele");
        xmlWriter.writeCharacters ( m_wptList[row].m_ele  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("time");
        xmlWriter.writeCharacters ( m_wptList[row].m_time.toString() );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("magvar");
        xmlWriter.writeCharacters ( m_wptList[row].m_magvar  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("geoidheight");
        xmlWriter.writeCharacters ( m_wptList[row].m_geoidheight  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("name");
        xmlWriter.writeCharacters ( m_wptList[row].m_name  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("cmt");
        xmlWriter.writeCharacters ( m_wptList[row].m_cmt  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("desc");
        xmlWriter.writeCharacters ( m_wptList[row].m_desc  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("src");
        xmlWriter.writeCharacters ( m_wptList[row].m_src  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        QString linkCurr;
        foreach(linkCurr, wptCurr.m_links)
        {
            xmlWriter.writeStartElement("link");
            xmlWriter.writeCharacters ( linkCurr  );
            xmlWriter.writeEndElement();
            xmlWriter.writeCharacters ( "\n"  );
        }

        xmlWriter.writeStartElement("sym");
        xmlWriter.writeCharacters ( m_wptList[row].m_sym  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("type");
        xmlWriter.writeCharacters ( m_wptList[row].m_type  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("fix");
        xmlWriter.writeCharacters ( m_wptList[row].m_fix  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("sat");
        xmlWriter.writeCharacters ( m_wptList[row].m_sat  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("hdop");
        xmlWriter.writeCharacters ( m_wptList[row].m_hdop  );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("vdop");
        xmlWriter.writeCharacters ( m_wptList[row].m_vdop  );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("pdop");
        xmlWriter.writeCharacters ( m_wptList[row].m_pdop  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("ageofdgpsdata");
        //xmlWriter.writeCharacters ( m_wptList[row].m_age  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("dgpsid");
        xmlWriter.writeCharacters ( m_wptList[row].m_dgpsid  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeStartElement("extensions");
        xmlWriter.writeCharacters ( m_wptList[row].m_extensions  );
        xmlWriter.writeEndElement();
        xmlWriter.writeCharacters ( "\n"  );

        xmlWriter.writeEndElement(); // </wpt>
        xmlWriter.writeCharacters ( "\n"  );

        ++it;
        index_list++;
        row++;

        exportedCount++;
    }

    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeEndElement(); // </gpx>

    xmlWriter.writeCharacters ( "\n"  );

    xmlWriter.writeEndDocument();

    return true;
}

void projectDatas::showTags( QStack<QString> tagStack, QString tagName, QString tagType, QString tagValue="" )
{

    QString tagPath;

    if (tagValue== "") return;

    tagPath  = "";
    for (int i = 0; i < tagStack.size(); ++i)
    {
        tagPath += " > " + tagStack.at(i);

    }
    debugp("showTags:size=" + QString::number(tagStack.size()) + ", "  + tagPath + " (" + tagType + ":" + tagName + ") =" +  tagValue );

}

void projectDatas::addTagValue( QStack<QString> tagStack, QString tagName, QString tagType, QString tagValue, gpx* l_gpx, metadata* l_metadata, wpt* l_wpt, rte* l_rte, trk* l_trk )
{


    if ( tagStack.size() == 1 && tagStack.at(0)== "gpx" && tagType == "att" && tagName == "creator" )
    {
        l_gpx->m_creator = tagValue;
        return;
    }
    if ( tagStack.size() == 1 && tagStack.at(0)== "gpx" && tagType == "att" && tagName == "version" )
    {
        l_gpx->m_version = tagValue;
        return;
    }
    if ( tagStack.size() == 1 && tagStack.at(0)== "gpx" && tagType == "att" && tagName == "schemaLocation" )
    {
        l_gpx->m_schemalocation = tagValue;
        return;
    }

    // -------------------------------------------------

    if ( (tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "metadata" && tagStack.at(2) == "name" && tagType == "txt" )
            ||
            (tagStack.size() == 2 && tagStack.at(0)== "gpx" && tagStack.at(1) == "name" && tagType == "txt" ))
    {
        l_metadata->m_name = tagValue;
        return;
    }
    if ( (tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "metadata" && tagStack.at(2) == "desc" && tagType == "txt" )
            ||
            ( tagStack.size() == 2 && tagStack.at(0)== "gpx" && tagStack.at(1) == "desc" && tagType == "txt" ))
    {
        l_metadata->m_desc = tagValue;
        return;
    }
    if ( (tagStack.size() == 4 && tagStack.at(0)== "gpx" && tagStack.at(1) == "metadata" && tagStack.at(2) == "author" && tagStack.at(3) == "name" && tagType == "txt" )
            ||
            ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "author" && tagStack.at(2) == "name" && tagType == "txt" ) )
    {
        l_metadata->m_author_name = tagValue;
        return;
    }
    if ( (tagStack.size() == 4 && tagStack.at(0)== "gpx" && tagStack.at(1) == "metadata" && tagStack.at(2) == "author" && tagStack.at(3) == "link" && tagType == "txt" )
            ||
            ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "author" && tagStack.at(2) == "link" && tagType == "txt" ) )
    {
        l_metadata->m_author_link = tagValue;
        return;
    }
    if ( (tagStack.size() == 4 && tagStack.at(0)== "gpx" && tagStack.at(1) == "metadata" && tagStack.at(2) == "author" && tagStack.at(3) == "link" && tagType == "att" && tagName == "href" )
            ||
            (tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "author" && tagStack.at(2) == "link" && tagType == "att" && tagName == "href" ))
    {
        //l_metadata-> LIST m_links
        l_metadata->m_links.append( tagValue );
        return;
    }
    if ( ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "metadata" && tagStack.at(2) == "copyright" && tagType == "txt" )
            ||
            ( tagStack.size() == 2 && tagStack.at(0)== "gpx" && tagStack.at(1) == "copyright" && tagType == "txt" ) )
    {
        l_metadata->m_copyright = tagValue;
        return;
    }
    // here : m_links list
    // here  : extentions

    // -------------------------------------------------

    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "metadata" && tagStack.at(2) == "time" && tagType == "txt" )
    {
        l_metadata->m_time = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "metadata" && tagStack.at(2) == "keywords" && tagType == "txt" )
    {
        l_metadata->m_keywords = tagValue;
        return;
    }
    if ( tagStack.size() == 2 && tagStack.at(0)== "gpx" && tagStack.at(1) == "bounds" && tagType == "att" && tagName == "minlat" )
    {
        l_metadata->m_latmin = tagValue;
        return;
    }
    if ( tagStack.size() == 2 && tagStack.at(0)== "gpx" && tagStack.at(1) == "bounds" && tagType == "att" && tagName == "maxlat" )
    {
        l_metadata->m_latmax = tagValue;
        return;
    }
    if ( tagStack.size() == 2 && tagStack.at(0)== "gpx" && tagStack.at(1) == "bounds" && tagType == "att" && tagName == "minlon" )
    {
        l_metadata->m_lonmin = tagValue;
        return;
    }
    if ( tagStack.size() == 2 && tagStack.at(0)== "gpx" && tagStack.at(1) == "bounds" && tagType == "att" && tagName == "maxlon" )
    {
        l_metadata->m_lonmax = tagValue;
        return;
    }

    // -------------------------------------------------

    if ( tagStack.size() == 2  && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagType == "att" && tagName == "lat" )
    {
        l_wpt->m_lat = tagValue;
        return;
    }
    if ( tagStack.size() == 2 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagType == "att" && tagName == "lon" )
    {
        l_wpt->m_lon = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "name" && tagType == "txt" )
    {
        debugp( "add , NAME = " + tagValue + " id = " + m_lastWptId );

        l_wpt->m_name = tagValue;
        m_lastWptId++;
        l_wpt->m_id = m_lastWptId;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "desc" && tagType == "txt" )
    {
        l_wpt->m_desc = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "ele" && tagType == "txt" )
    {
        l_wpt->m_ele = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "link" && tagType == "att" && tagName == "href")
    {
        l_wpt->m_links.append(tagValue);
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "sym" && tagType == "txt" )
    {
        l_wpt->m_sym = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "time" && tagType == "txt" )
    {
        QDateTime cdt = QDateTime::fromString(tagValue);
        l_wpt->m_time = cdt;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "magvar" && tagType == "txt" )
    {
        l_wpt->m_magvar = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "geoheight" && tagType == "txt" )
    {
        l_wpt->m_geoidheight = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "cmt" && tagType == "txt" )
    {
        l_wpt->m_cmt = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "src" && tagType == "txt" )
    {
        l_wpt->m_src = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "fix" && tagType == "txt" )
    {
        l_wpt->m_fix = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "sat" && tagType == "txt" )
    {
        l_wpt->m_sat = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "dgpsid" && tagType == "txt" )
    {
        l_wpt->m_dgpsid = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "link" && tagType == "txt" )
    {
        l_wpt->m_links.append(tagValue);
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "utlname" && tagType == "txt" )
    {
        //l_wpt->m_linkname = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "type" && tagType == "txt" )
    {
        l_wpt->m_type= tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "hdop" && tagType == "txt" )
    {
        l_wpt->m_hdop = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "vdop" && tagType == "txt" )
    {
        l_wpt->m_vdop = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "pdop" && tagType == "txt" )
    {
        l_wpt->m_pdop = tagValue;
        return;
    }
    if ( tagStack.size() == 3 && tagStack.at(0)== "gpx" && tagStack.at(1) == "wpt" && tagStack.at(2) == "extensions" && tagType == "txt" )
    {
        //l_wpt->m_hdop = tagValue;
        return;
    }

}

qlonglong  projectDatas::getMaxId( void )
{
    qlonglong retMaxId;

    retMaxId = 0;
    QList<projectDatas::wptType>::iterator it = m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    while (it != m_wptList.end())
    {
        if ( m_wptList[index_list].m_id > retMaxId ) retMaxId = m_wptList[index_list].m_id ;
        ++it;
        index_list++;
    }
    return retMaxId;
}


projectDatas::wptType *projectDatas::getWptFromId2( qlonglong p_id)
{

    QList<projectDatas::wptType>::iterator it = m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    while (it != m_wptList.end())
    {

        if ( m_wptList[index_list].m_id == p_id)
            return &m_wptList[index_list];
        ++it;
        index_list++;
    }

    return NULL;
}
const projectDatas::wptType *projectDatas::getWptFromId( qlonglong p_id)
{
    QList<projectDatas::wptType>::iterator it = m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    while (it != m_wptList.end())
    {
        if (m_wptList.at(index_list).m_id == p_id)
            return &m_wptList.at(index_list);
        ++it;
        index_list++;
    }
    return NULL;
}

void projectDatas::removeWptById( qlonglong p_id )
{
    QList<projectDatas::wptType>::iterator it = m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    while (it != m_wptList.end())
    {
        if (m_wptList[index_list].m_id == p_id)
        {
            m_wptList.removeAt( index_list );
            break;
        }
        ++it;
        index_list++;
    }

    updateParent();
}

void projectDatas::reIndexwpt(void)
{

    QList<projectDatas::wptType>::iterator it = m_wptList.begin();
    qlonglong index_list;
    index_list = 0;
    qlonglong  idx=1;
    while (it != m_wptList.end())
    {
        m_wptList[index_list].m_id = idx;
        ++it;
        index_list++;
        idx++;
    }


}

bool projectDatas::sortCompareValues( int colSort ,  wpt * wptSrc,  wpt * wptDst)
{
    bool doSwap=false;
    QString strFmtSrc;
    QString strFmtDst;

    switch( colSort )
    {

    case COL_WPT_ID:
        if (m_currentColSortOrder == COL_SORT_ORDER_DOWN &&  wptSrc->m_id <  wptDst->m_id )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_id >  wptDst->m_id ) doSwap = true;
        break;
    case COL_WPT_SELECTED:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_selected <  wptDst->m_selected )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_selected >  wptDst->m_selected )  doSwap = true;
        break;
    case COL_WPT_NOTE:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_note <  wptDst->m_note )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_note>   wptDst->m_note ) doSwap = true;
        break;
    case COL_WPT_GROUP:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN  && wptSrc->m_group <  wptDst->m_group )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_group>   wptDst->m_group ) doSwap = true;
        break;
    case COL_WPT_LAT:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_lat.toFloat() <  wptDst->m_lat.toFloat() )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_lat.toFloat() >   wptDst->m_lat.toFloat() ) doSwap = true;
        break;
    case COL_WPT_LON:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_lon.toFloat() <  wptDst->m_lon.toFloat() )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_lon.toFloat() >   wptDst->m_lon.toFloat() ) doSwap = true;
        break;
    case COL_WPT_NAME:
        //strFmtSrc = removeSpecialChars(wptSrc->m_name);
        //strFmtDst = removeSpecialChars(wptDst->m_name);
        strFmtSrc = removeAccents(wptSrc->m_name);
        strFmtDst = removeAccents(wptDst->m_name);
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && QString::compare( strFmtSrc, strFmtDst) < 0 ) doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && QString::compare( strFmtSrc, strFmtDst, Qt::CaseInsensitive) > 0 ) doSwap = true;
        break;
    case COL_WPT_ELE:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN  && wptSrc->m_ele <  wptDst->m_ele )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_ele>   wptDst->m_ele ) doSwap = true;
        break;
    case COL_WPT_TIME:
        debugp( "sort COL_WPT_TIME" + wptSrc->m_time.toString() + " vs " +  wptDst->m_time.toString() );
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_time <  wptDst->m_time )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_time>   wptDst->m_time ) doSwap = true;
        break;
    case COL_WPT_MAGVAR:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN  && wptSrc->m_magvar <  wptDst->m_magvar )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_magvar>   wptDst->m_magvar ) doSwap = true;
        break;
    case COL_WPT_GEOIDHEIGHT:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_geoidheight <  wptDst->m_geoidheight )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_geoidheight>   wptDst->m_geoidheight ) doSwap = true;
        break;
    case COL_WPT_CMT:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_cmt <  wptDst->m_cmt )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_cmt>   wptDst->m_cmt ) doSwap = true;
        break;
    case COL_WPT_DESC:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_desc <  wptDst->m_desc )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_desc>   wptDst->m_desc ) doSwap = true;
        break;
    case COL_WPT_SRC:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_src <  wptDst->m_src )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_src>   wptDst->m_src ) doSwap = true;
        break;
    case COL_WPT_FIX:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_fix <  wptDst->m_fix )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_fix>   wptDst->m_fix ) doSwap = true;
        break;
    case COL_WPT_SAT:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_sat <  wptDst->m_sat )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_sat>   wptDst->m_sat ) doSwap = true;
        break;
    case COL_WPT_DGPSID:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_dgpsid <  wptDst->m_dgpsid )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_dgpsid>   wptDst->m_dgpsid ) doSwap = true;
        break;
    case COL_WPT_LINKS:
        //if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN )
        //{
        //    if ( wptSrc->m_links <  wptDst->m_links )  doSwap = true;
        //}
        //else if ( wptSrc->m_links>   wptDst->m_links ) doSwap = true;
        break;
    case COL_WPT_SYM:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_sym <  wptDst->m_sym )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_sym>   wptDst->m_sym ) doSwap = true;
        break;
    case COL_WPT_TYPE:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_type <  wptDst->m_type )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_type>   wptDst->m_type ) doSwap = true;
        break;
    case COL_WPT_HDOP:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_hdop <  wptDst->m_hdop )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_hdop>   wptDst->m_hdop ) doSwap = true;
        break;
    case COL_WPT_VDOP:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_vdop <  wptDst->m_vdop )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_vdop>   wptDst->m_vdop ) doSwap = true;
        break;
    case COL_WPT_PDOP:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN  && wptSrc->m_pdop <  wptDst->m_pdop )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_pdop>   wptDst->m_pdop ) doSwap = true;
        break;
    case COL_WPT_EXTENTIONS:
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN && wptSrc->m_extensions <  wptDst->m_extensions )  doSwap = true;
        if ( m_currentColSortOrder == COL_SORT_ORDER_UP && wptSrc->m_extensions>   wptDst->m_extensions ) doSwap = true;
        break;
    }

    return doSwap;
}

void projectDatas::sortWpt(int colSort, int sortOrder)
{
    int debut = 0;
    int taille = m_wptList.count()-1;

    if (taille < 1 ) return;

    // Sort order
    if ( m_currentColSort == colSort)
    {
        if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN)
            m_currentColSortOrder = COL_SORT_ORDER_UP;
        else
            m_currentColSortOrder = COL_SORT_ORDER_DOWN;
    }
    else m_currentColSortOrder = COL_SORT_ORDER_DOWN;
    m_currentColSort = colSort;

    debugp("enter sort " + QString::number(debut) + " -> " + QString::number(taille));

    if ( m_currentColSortOrder == COL_SORT_ORDER_DOWN)
    {
        debugp ("DOWN" );
    }
    else
    {
        debugp("Up");
    }

    sortWptDo( colSort, debut, taille );

    debugp("end sort");

    reIndexwpt();

    debugp ("end reindex");


    updateParent();

}

void projectDatas::sortWptDo(int colSort, int debut, int fin)
{
    int  pivot, gauche, droite;
    wpt *wptSrc;
    wpt *wptDst;

    pivot = debut;
    gauche = debut;
    droite = fin;
    do
    {
        wptSrc = &m_wptList[gauche];
        wptDst = &m_wptList[droite];
        if ( sortCompareValues( colSort,  wptSrc,  wptDst) == true  )
        {
            swapWpt( wptSrc, wptDst);
        }
        if (pivot == gauche) droite--;
        else gauche++;

    }
    while ( gauche != droite );

    if (debut < gauche - 1) sortWptDo(colSort, debut, gauche - 1); //appel récursif sur la partie gauche
    if (fin > droite + 1) sortWptDo(colSort, droite + 1, fin); //appel récursif sur la partie droite
}


QString projectDatas::removeSpecialChars(QString message)
{
    QString s=message.toLower();

    // debugp( "s=" << s;
    // É-é
    // Château Blanc (80)
    // s.replace( QString::fromUtf8("â"), "a");

    s.replace( QString::fromUtf8("â"), "a");
    s.replace( QString::fromUtf8("ä"), "a");
    s.replace( QString::fromUtf8("á"), "a");
    s.replace( QString::fromUtf8("à"), "a");
    s.replace( QString::fromUtf8("ã"), "a");
    s.replace( QString::fromUtf8("å"), "a"); //a
    s.replace( QString::fromUtf8("ç"), "c"); //c
    s.replace( QString::fromUtf8("ê"), "e");
    s.replace( QString::fromUtf8("ë"), "e");
    s.replace( QString::fromUtf8("é"), "e");
    s.replace( QString::fromUtf8("è"), "e"); //e
    s.replace( QString::fromUtf8("î"), "i");
    s.replace( QString::fromUtf8("ï"), "i");
    s.replace( QString::fromUtf8("í"), "i");
    s.replace( QString::fromUtf8("ì"), "i"); //i
    s.replace( QString::fromUtf8("ñ"), "n"); //n
    s.replace( QString::fromUtf8("ô"), "o");
    s.replace( QString::fromUtf8("ö"), "o");
    s.replace( QString::fromUtf8("ó"), "o");
    s.replace( QString::fromUtf8("ò"), "o");
    s.replace( QString::fromUtf8("õ"), "o");
    s.replace( QString::fromUtf8("ø"), "o"); //o
    s.replace( QString::fromUtf8("ß"), "ss"); //ß -> ss
    s.replace( QString::fromUtf8("û"), "u");
    s.replace( QString::fromUtf8("ü"), "u");
    s.replace( QString::fromUtf8("ú"), "u");
    s.replace( QString::fromUtf8("ù"), "u"); //u
    s.replace( QString::fromUtf8("æ"), "ae");

    return s;
}

QString diacriticLetters_;
QStringList  noDiacriticLetters_;

QString projectDatas::removeAccents(QString s)
{
    if (diacriticLetters_.isEmpty())
    {
        diacriticLetters_ = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
        noDiacriticLetters_ << "S"<<"OE"<<"Z"<<"s"<<"oe"<<"z"<<"Y"<<"Y"<<"u"<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<"C"<<"E"<<"E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"D"<<"N"<<"O"<<"O"<<"O"<<"O"<<"O"<<"O"<<"U"<<"U"<<"U"<<"U"<<"Y"<<"s"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"e"<<"e"<<"e"<<"e"<<"i"<<"i"<<"i"<<"i"<<"o"<<"n"<<"o"<<"o"<<"o"<<"o"<<"o"<<"o"<<"u"<<"u"<<"u"<<"u"<<"y"<<"y";
    }

    QString output = "";
    for (int i = 0; i < s.length(); i++)
    {
        QChar c = s[i];
        int dIndex = diacriticLetters_.indexOf(c);
        if (dIndex < 0)
        {
            output.append(c);
        }
        else
        {
            QString replacement = noDiacriticLetters_[dIndex];
            output.append(replacement);
        }
    }

    return output;
}


void projectDatas::swapWpt( wptType *wptSrc , wptType *wptDst)
{
    wptType wptTemp;

    copyWpt( wptSrc , &wptTemp);
    copyWpt( wptDst , wptSrc);
    copyWpt( &wptTemp , wptDst);

}

void projectDatas::copyWpt( wptType *wptSrc , wptType *wptDst)
{


    wptDst->m_id = wptSrc->m_id;
    wptDst->m_selected = wptSrc->m_selected;
    wptDst->m_note = wptSrc->m_note;
    wptDst->m_group = wptSrc->m_group;
    wptDst->m_lat = wptSrc->m_lat;
    wptDst->m_lon = wptSrc->m_lon;
    wptDst->m_name = wptSrc->m_name;
    wptDst->m_ele = wptSrc->m_ele;
    wptDst->m_time = wptSrc->m_time;
    wptDst->m_magvar = wptSrc->m_magvar;
    wptDst->m_geoidheight = wptSrc->m_geoidheight;
    wptDst->m_cmt = wptSrc->m_cmt;
    wptDst->m_desc = wptSrc->m_desc;
    wptDst->m_src = wptSrc->m_src;
    wptDst->m_fix = wptSrc->m_fix;
    wptDst->m_sat = wptSrc->m_sat;
    wptDst->m_dgpsid = wptSrc->m_dgpsid;
    //QString m_link; // mais aussi un lien vers une page web d'informations additionnelles,
    //QString m_linkname; // Texte du lien
    wptDst->m_links = wptSrc->m_links;
    wptDst->m_sym = wptSrc->m_sym;
    wptDst->m_type = wptSrc->m_type;
    wptDst->m_hdop = wptSrc->m_hdop;
    wptDst->m_vdop = wptSrc->m_vdop;
    wptDst->m_pdop = wptSrc->m_pdop;
    wptDst->m_extensions = wptSrc->m_extensions;

}

