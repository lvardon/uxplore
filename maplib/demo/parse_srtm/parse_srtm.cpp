#include <QCoreApplication>
#include <QDir>

#include "SRTMv4.h"
#include "mydebug.h"


int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	SRTMv4 srtm("../../data");
	srtm.parse(argv[1]);
	qDebug("ok: %d", srtm.isOk());

	QPointF nidda(8.795535, 50.283194);
	QPointF DH3HS(8.774939, 50.284208);
	QPointF DB0FT(8.457471, 50.23153);
	qDebug("Nidda (Fluss):      %d m", srtm.get(nidda));
	qDebug("Woellstadt (DH3HS): %d m", srtm.get(DH3HS));
	qDebug("Feldberg (DB0FT):   %d m", srtm.get(DB0FT));

    return 0;
}
