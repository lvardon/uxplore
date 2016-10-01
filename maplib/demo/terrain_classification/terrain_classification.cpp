#define DEBUGLVL 1
#include "mydebug.h"

/*
 * Copyright (C) 2010 by H.Schurig, Germany, Wöllstadt (DH3HS)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 ************************************************************************
 *
 * Demo application for the ITUTerrain class.
 */


#include <QApplication>
#include <QPainter>

#include "ITUTerrain.h"
#include "SRTMv4.h"
#include "ProfileWidget.h"
#include "DrawITUTerrain.h"



#if 0
void beta0(const QPointF &t, const QPointF &r, double dtm, double dlm)
{
	// Section 3.2.1

	// Equation 3a
	double tau = 1 - exp(-(0.000412 * pow(dlm, 2.41)));
	MYVERBOSE("tau: %.10f", tau);

	// Equation 3
	double m1 = pow(10, (-dtm / (16 - 6.6 * tau)));
	double m2 = pow(10, -(0.496 + 0.354 * tau));
	m2 = pow(m2, 5);
	double mu1 = pow(m1 + m2, 0.2);

	if (mu1 > 1)
		mu1 = 1;
	MYVERBOSE("mu1: %.10f", mu1);

	// This gives us a rought radio climatic zone
	double th = fabs((r.y() + t.y()) / 2);
	MYVERBOSE("theta: %.3f", th);

	// Equation 2 and 4
	double mu4;
	double beta0;
	//TODO: something here is still wrong,
	if (th <= 70) {
		mu4   = pow(10, (-0.935 + 0.0176 * th) * log10(mu1));
		beta0 = pow(10, -0.015 * th + 1.67) * mu1 * mu4;
	} else {
		mu4   = pow(10, 0.3 * log10(mu1));
		beta0 = 4.17 * mu1 * mu4;
	}
	MYVERBOSE("beta0: %f", beta0);

}
#endif



int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

#if 1
	// This is the data from the spreadsheet at http://www.itu.int/oth/R0A0400005F/en
	const double d[] = {
		0.0,	40.0,
		1.0,	24.0,
		2.0,	35.0,
		3.0,	38.0,
		4.0,	41.0,
		5.0,	39.0,
		6.0,	41.0,
		7.0,	41.0,
		8.0,	44.0,
		9.0,	35.0,
		10.0,	39.0,
		11.0,	24.0,
		12.0,	26.0,
		13.0,	32.0,
		14.0,	36.0,
		15.0,	43.0,
		16.0,	49.0,
		17.0,	48.0,
		18.0,	48.0,
		19.0,	45.0,
		20.0,	41.0,
		21.0,	35.0,
		22.0,	36.0,
		23.0,	36.0,
		24.0,	42.0,
		25.0,	43.0,
		26.0,	52.0,
		27.0,	55.0,
		28.0,	73.0,
		29.0,	67.0,
		30.0,	68.0,
		31.0,	53.0,
		32.0,	50.0,
		33.0,	14.0,
		34.0,	1.0,
		35.0,	0.0,
		36.0,	0.0,
		37.0,	0.0,
		38.0,	0.0,
		39.0,	0.0,
		40.0,	0.0,
		41.0,	0.0,
		42.0,	0.0,
		43.0,	0.0,
		44.0,	0.0,
		45.0,	0.0,
		46.0,	0.0,
		47.0,	0.0,
		48.0,	0.0,
		49.0,	0.0,
		50.0,	0.0,
		51.0,	0.0,
		52.0,	0.0,
		53.0,	0.0,
		54.0,	0.0,
		55.0,	0.0,
		56.0,	0.0,
		57.0,	0.0,
		58.0,	0.0,
		59.0,	0.0,
		60.0,	0.0,
		61.0,	0.0,
		62.0,	0.0,
		63.0,	0.0,
		64.0,	0.0,
		65.0,	0.0,
		66.0,	0.0,
		67.0,	0.0,
		68.0,	0.0,
		69.0,	0.0,
		70.0,	0.0,
		71.0,	0.0,
		72.0,	0.0,
		73.0,	0.0,
		74.0,	0.0,
		75.0,	0.0,
		76.0,	0.0,
		77.0,	0.0,
		78.0,	45.0,
		79.0,	54.0,
		80.0,	63.0,
		81.0,	79.0,
		82.0,	85.0,
		83.0,	39.0,
		84.0,	21.0,
		85.0,	15.0,
		86.0,	37.0,
		87.0,	42.0,
		88.0,	63.0,
		89.0,	51.0,
		90.0,	35.0,
		91.0,	68.0,
		92.0,	68.0,
		93.0,	53.0,
		94.0,	63.0,
		95.0,	75.0,
		96.0,	92.0,
		97.0,	174.0,
		98.0,	184.0,
		99.0,	145.0,
		100.0,	158.0,
		101.0,	122.0,
		102.0,	129.0,
		103.0,	97.0,
		104.0,	100.0,
		105.0,	91.0,
		106.0,	144.0,
		107.0,	119.0,
		108.0,	154.0,
		109.0,	183.0};

	GeoData data;
	for (size_t i=0; i<sizeof(d)/sizeof(float)/2; i += 2) {
		data.d.append( d[i] * 1000 ); // convert km to m
		data.e.append( d[i+1] );
	}

	ITUTerrain itu;
	itu.pathProfileAnalysis(200, // Step 1
	                        53,  // Step 3
	                        10,  // Step 1
	                        10,  // Step 1
	                        data);

#define VERIFY(variable, should) \
	if (fabs(itu.variable - should) > 0.1) \
		qWarning(#variable " should be %f, but is %f", should, itu.variable);
	// else
	// 	qDebug(#variable " is %f", itu.variable);
	VERIFY(ae, 9617.8);
	VERIFY(dtot, 109000.0);
	VERIFY(hts, 50.0);
	VERIFY(hrs, 193.0);
	VERIFY(theta_t, -0.6);
	VERIFY(theta_r, -1.4);
	VERIFY(theta, 9.3);
	VERIFY(hst, 5.4);
	VERIFY(hsr, 71.1);
	VERIFY(hm, 119.5);
	VERIFY(hte, 44.6);
	VERIFY(hre, 121.9);
	VERIFY(dlt, 28000.0);
	VERIFY(dlr, 11000.0);
#else
	const QPointF DH3HS(8.77485, 50.28425);
	const QPointF DB0FT(8.45741, 50.23153);
	const QPointF DB0ZAV(360-351.2117, 50.2128);
	const QPointF DB0WAS(9.93654, 50.4987);

	SRTMv4 srtm("../../data");
	GeoData data;
	MakePath(DH3HS, DB0WAS, data);
	for (int i=0; i<data.posCount(); i++)
	 	data.e.append(srtm.get( QPointF(data.lon.at(i), data.lat.at(i)) ));

	ITUTerrain itu;
	itu.pathProfileAnalysis(144,
	                        45,
	                        10,
	                        10,
	                        data);
#endif


#if 0
	// This isn't finished yet ...  and besides I'm about to use ITU-R P.452 only
	// for terrain classification
	QPointF lonLat_t(0.6, 51.2);
	QPointF lonLat_r(1.98, 50.73);
	beta0(lonLat_t, lonLat_r, 34.5, 6);
#endif

	ProfileWidget *pl = new ProfileWidget(0, &data);
	DrawITUTerrain *itu_terr = new DrawITUTerrain(&itu, pl);

	pl->resize(640, 240);
	pl->parts.append(itu_terr);
	pl->show();

	return app.exec();
}
