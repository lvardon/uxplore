#include <QApplication>
#include <QDir>

#include "SRTMv4.h"
#include "mydebug.h"
#include "GeoUtils.h"
#include "ProfileWidget.h"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	SRTMv4 srtm("../../data");

	QPointF nidda(8.795535, 50.283194);
	QPointF DH3HS(8.774939, 50.284208);
	QPointF DB0FT(8.457471, 50.23153);
	QPointF DB0WAS(9.936593, 50.498648);

	GeoData data;
	MakePath(DH3HS, DB0WAS, data, 100);

	for (int i=0; i<data.posCount(); i++)
	 	data.e.append(srtm.get( QPointF(data.lon.at(i), data.lat.at(i)) ));


	ProfileWidget *pl = new ProfileWidget(0, &data);
	DrawLoS *los = new DrawLoS(pl);

	pl->resize(640, 240);
	pl->parts.append(los);
	pl->show();

	return app.exec();
}
