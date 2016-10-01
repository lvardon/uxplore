#define CSV_MODE_IMPORT 1
#define CSV_MODE_EXPORT 2

#define CSV_CTXMENU_SELECTALL       0
#define CSV_CTXMENU_UNSELECTALL     1
#define CSV_CTXMENU_MOVELEFT        3
#define CSV_CTXMENU_MOVERIGHT       4
#define CSV_CTXMENU_SELECTCURRENT   5


// Colonnes à exporter / importer (cible)
struct contextMenuLibElem
{
    int id;
    int order; // -1 : ignore
    QString name;
    bool colvisible;
    QString format;
    QString tooltip;
    int len;
    bool operator<(const contextMenuLibElem &other) const
    {
        return (name < other.name);

    };

};

