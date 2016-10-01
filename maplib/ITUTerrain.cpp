#define DEBUGLVL 0
#include "mydebug.h"
//#include <math.h>

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
 * ITU-R Terrain related calculations.
 *
 * All texts like "Equation x" or "Section a.b.c" refer to the ITU-R
 * P.452-13.pdf document. The same equations can be found in ITU P.1812
 * and probably others.
 *
 * On the ITU website, I found a spreadsheet at
 * http://www.itu.int/oth/R0A0400005F/en which contains Visual Basic macros
 * to calculate terrain parameters.
 *
 * I used the pdf and the xls to write the following code. While doing that,
 * I made some minor optimizations, e.g. the VBA code allocates lots of
 * temporary arrays. Here I could achive the same calculation with temporary
 * variables. I also packed this into a class just to have no global
 * variables lying around. This will help when I'm doing parallelization
 * calculations.
 */

/*!
 * \file ITUTerrain.cpp
 *
 * Terrain profile classification
 *
 * \ingroup Propagation
 */


#include <math.h>

#include "ITUTerrain.h"


/*!
 * Factor to convert millirad to degrees.
 */
const double MRAD2DEG = 9.0 / (50.0 * M_PI);


/*!
 * Speed of light in vacuum [m/s]
 *
 * \note The ITU recommendation actually uses just 300000000 m/s.
 */
const double c = 299792458.0;



/*!
 * Constructor.
 *
 * This just calls reset().
 */
ITUTerrain::ITUTerrain()
{
	MYTRACE("ITUTerrain::ITUTerrain");
	reset();
}


/*!
 * Set all class member variables to NAN (not-a-number).
 */
void ITUTerrain::reset()
{
	MYTRACE("ITUTerrain::reset");

	htg     = NAN;
	hrg     = NAN;
	k50     = NAN;
	ae      = NAN;
	hgt     = NAN;
	hgr     = NAN;
	dtot    = NAN;
	hts     = NAN;
	hrs     = NAN;
	theta_t = NAN;
	theta_r = NAN;
	theta   = NAN;
	hst     = NAN;
	hsr     = NAN;
	hm      = NAN;
	hte     = NAN;
	hre     = NAN;
	dlt     = NAN;
	dlr     = NAN;

	index_t  = -1;
	index_r  = -1;
	index_hm = -1;
}



/*!
 * Perform terrain profile analysis according to ITU Recommendation
 * P.452-13/P.452-14 according to Appendix 2 of Annex 1. The same
 * calculations are to be found in ITU P.1812 and probably others.
 *
 * \param f_MHz   frequency in MHz
 * \param DELTA_N The average radio-refractive index lapse-rate throught the
 *                lowest 1 km of the atmosphere. This provides the data
 *                upon which the appropriate effective Earth radius can be
 *                calculated for path profile and diffraction obstacle
 *                analysis.
 * \param new_htg Transmitting antenna centre height above ground level (m)
 * \param new_hrg Receiving antenna centre height above ground level (m)
 * \param data    QVector's containing distances and elevation data
 */
void ITUTerrain::pathProfileAnalysis(double f_MHz,
                                 double DELTA_N,
                                 double new_htg,
                                 double new_hrg,
                                 const GeoData &data)
{
	MYTRACE("ITUTerrain::pathProfileAnalysis(%f, %f, %f, %f, ...)", f_MHz, DELTA_N, htg, hrg);

	MYVERBOSE("n:   %8d         Elevation data items", data.posCount());

	if (!std::isnan(k50))
		reset();

	htg = new_htg;
	hrg = new_hrg;

	/*
	 * 	Median effective earth radius faktor.
	 *
	 * (A	lso median k-factor).
	 *
	 * This faktor is used to artificially enlarge the actual radius, yielding
	 * an effective earth radius. That is the radius of a hypothetical earth for
	 * which the distance to the radio horizon, assuming rectilinear
	 * propagation, is the same as that for the actual Earth with an assumed
	 * uniform vertical gradient of atmospheric refractive index.
	 *
	 * Commonly, the k-factor is set to 4/3. This calculation however uses
	 * \ref DELTA_N as a means to define this value. But see an example at
	 * http://www.hindawi.com/journals/ijap/2010/245070.html how both
	 * \ref DELTA_N and therefore the k-factor can vary.
	 */
	// Equation 5
	k50 = 157 / (157 - DELTA_N);
	MYVERBOSE("k_50: medium effective earth radius factor: %f", k50);

	// Equation 6a
	ae = 6371.0 * k50;
	MYDEBUG("a_e:     %6.1f km    Medium effective earth radius value", ae);

	hgt = data.e.first();
	hgr = data.e.last();
	MYVERBOSE("Ground high AMSL at Tx: %f m", hgt);
	MYVERBOSE("Ground high AMSL at Rx: %f m", hgr);

	dtot = data.d.last();
	MYDEBUG("d:       %6.1f km    Total distance", dtot/1000);

	hts = htg + hgt;
	hrs = hrg + hgr;
	MYDEBUG("h_ts:    %6.1f m     Antenna height AMSL at Tx", hts);
	MYDEBUG("h_rs:    %6.1f m     Antenna height AMSL at Rx", hrs);

	// Equation 143
	double theta_d = (hrs - hts) / (dtot / 1000) - dtot / (2 * ae);
	MYVERBOSE("theta_d: %.1f mrad", theta_d);

	if (data.e.count() <= 2)
		return;

	// Equation 141: Determine theta_max
	double theta_max = -HUGE_VAL;
	theta_r = -HUGE_VAL;
	for (int i = 1; i < data.e.count()-1; i++) {
		// This loop excludes the first and last element

		// Equation 142
		double d_tmp = data.d.at(i);
		double theta_tmp = (data.e.at(i) - hts) / (d_tmp / 1000.0) - d_tmp / (2 * ae);
		//MYVERBOSE("%d: theta at distance %.1f km / height %.1f m: %.1f", i, d_tmp, data.e.at(i), theta_tmp * MRAD2DEG);
		if (theta_tmp > theta_max) {
			theta_max = theta_tmp;
			index_t = i;
		}
	}
	MYVERBOSE("theta_max: %.1f mrad", theta_max);

	// Equation 140
	path_trans_horizon = theta_max > theta_d;
	MYDEBUG("Trans horizon? %d", path_trans_horizon);

	if (path_trans_horizon) {
		// Section 5.1.1, Equation 144
		theta_t = theta_max;
		MYDEBUG("theta_t: %6.1f mrad  Horizontal elevation angle (above local horizon) at Tx (%.1f)", theta_t, theta_t * MRAD2DEG);

		// Section 5.1.2, Equation 145
		dlt = 0;
		dlr = 0;
		if (index_t == -1) {
			qWarning("No maximum antenna horizontal elevation found");
			return;
		} else {
			dlt = data.d.at(index_t);
			MYDEBUG("d_lt:    %6.1f km    Great circle distance from Tx to it's horizon point", dlt/1000);
		}
		for (int i=1; i<data.e.count()-1; i++) {
			double d_tmp = data.d.at(i);
			double theta_tmp = (data.e.at(i) - hrs) / ((dtot - d_tmp) / 1000) - (dtot - d_tmp) / ( 2 * ae);
			//MYVERBOSE("%d: theta2 at distance %.1f km / height %.1f m: %.1f", i, d_tmp, data.e.at(i), theta_tmp * MRAD2DEG);
			if (theta_tmp > theta_r) {
				dlr = dtot - d_tmp;
				theta_r = theta_tmp;
				index_r = i;
			}
		}
		MYDEBUG("d_lr:    %6.1f km    Great circle distance from Rx to it's horizon point", dlr/1000);
		MYDEBUG("theta_r: %6.1f mrad  Horizontal elevation angle (above local horizon) at Rx (%.1f)", theta_r, theta_r * MRAD2DEG);

	} else {
		// TODO: verify those calculations. I couldn't get sample calculations

		// Equation 143
		theta_t = ((hrs - hts) / dtot) - (dtot / (2 * ae));
		MYDEBUG("theta_t: %6.1f mrad  Horizontal elevation angle (above local horizon) at Tx (%.1f)", theta_t, theta_t * MRAD2DEG);

		// Equation 71 from P.1812
		theta_r = ((hts - hrs) / dtot) - (dtot / (2 * ae));
		MYDEBUG("theta_r: %6.1f mrad  Horizontal elevation angle (above local horizon) at Rx (%.1f)", theta_r, theta_r * MRAD2DEG);

		//TODO: calc dlt, dlr
		//XXX i_m50 = diff_loss(1, f, DELTA_N, hrs, hts, 1, 1, 1, 4)
		// p       1
		// f
		// DELTA_N
		// hrs
		// hts
		// L_bfsg  1
		// L_b0p   1
		// BETAo   1
		// Equation 15

		// Section 4.2.1 of Rec P.452-14 (Calculation of Median Diffraction Loss)
		// Calculate the median diffraction loss for the principal (primary) edge

		// Equation 14
		double zeta_m = cos(atan(0.001 * (hrs - hts) / dtot));
		MYVERBOSE("zeta_m:  %8.3f     Correction factor", zeta_m);

		double lambda = c / (f_MHz * 1000000.0);
		MYVERBOSE("lambda: %f", lambda);

		int i_m50 = -1;
		double nu_m50 = -HUGE_VAL;
		for (int i=1; i<data.e.count()-1; i++) {
			double d_i = data.d.at(i) / 1000;

			// Equation 15a
			double H_i = data.e.at(i)
				+ ((d_i * (dtot - d_i)) / (2 * ae))
				- (hts * (dtot - d_i) + hrs * d_i) / dtot;
			MYVERBOSE("H_i:   %8.1f m     Vertical clearance at %d", H_i, i);

			// Equation 15
			// MYVERBOSE("d_tot %.1f, d_i %.1f, lambda %.1f, %f",
			//           dtot,
			//           data.d.at(i),
			//           lambda,
			//           sqrt((2 * dtot) / (lambda * data.d.at(i) * (dtot - data.d.at(i)))) );
			double nu_m50_tmp =
				zeta_m *
				H_i *
				sqrt((2 * dtot) / (lambda * data.d.at(i) * (dtot - data.d.at(i))));
			MYVERBOSE("nu_m50 %8.1f       Diffraction parameter at %d", nu_m50_tmp, i);

			if (nu_m50_tmp > nu_m50) {
				nu_m50 = nu_m50_tmp;
				i_m50 = i;
			}
		}
		MYDEBUG("nu_m50:  %8.3f     Diffraction parameter", nu_m50);
		MYDEBUG("i_m50: %6d", i_m50);

		dlt = data.d.at(i_m50);        ;
		dlr = dtot - dlt;
		MYDEBUG("d_lt:    %6.1f km    Great circle distance from Tx to it's horizon point", dlt / 1000);
		MYDEBUG("d_lr:    %6.1f km    Great circle distance from Rx to it's horizon point", dlr / 1000);
		index_t = i_m50;
		index_r = i_m50;
	}
	theta = dtot / ae + theta_t + theta_r;
	MYDEBUG("theta:   %6.1f mrad  Earth centered angular distance between Tx and Rx (%.1f)", theta, theta * MRAD2DEG);


	// Section 5.1.6.3: Derive the smooth-earth surface
	double ha = 0;
	for (int i=0; i<data.d.count()-1; i++) {
		double di = data.d.at(i+1);
		double di_1 = data.d.at(i);
		double hi = data.e.at(i+1);
		double hi_1 = data.e.at(i);
		// Equation 152b
		double h = (di-di_1) * (hi+hi_1);
		//MYVERBOSE("%3d: %6.1f %6.1f %6.1f %6.1f %6.1f", i, di, di_1, hi, hi_1, h);
		ha += h;
	}
	ha /= (2 * dtot);
	MYVERBOSE("h_a:     %6.1f m     Mean of the real path heights", ha);


	// Equation 151b
	double m = 0;
	for (int i=0; i<data.d.count()-1; i++) {
		double di = data.d.at(i+1);
		double di_1 = data.d.at(i);
		double hi = data.e.at(i+1);
		double hi_1 = data.e.at(i);
		m += 3 *
			(di - di_1) *
			(di + di_1 - dtot) *
			(hi + hi_1 - 2*ha) + pow(di - di_1, 2) *
			(hi - hi-1);
	}
	m = 1 / pow(dtot, 3) * m;
	MYVERBOSE("m:       %6.1f m/km  slope of least-squares surface relative to sea level", m*1000);


	// Equation 153
	hst = ha - m * dtot/2;
	MYDEBUG("h_st:    %6.1f m     Height of smooth-earth AMSL at Tx", hst);

	// Equation 154
	hsr = hst + m * dtot;
	MYDEBUG("h_sr:    %6.1f m     Height of smooth-earth AMSL at Rx", hsr);

	// Equation 155a
	bool corrected = false;
	if (hst > hgt) {
		corrected = true;
		hst = hgt;
		MYDEBUG("h_st:    %6.1f m     Height of smooth-earth AMSL at Tx", hst);
	}

	// Equation 155b
	if (hsr > hgr) {
		corrected = true;
		hsr = hgr;
		MYDEBUG("h_sr:    %6.1f m     Height of smooth-earth AMSL at Rx", hsr);
	}

	// Equation 156
	if (corrected) {
		double m_tmp = (hsr - hst) / dtot;
		MYVERBOSE("correct m from %f to %f", m, m_tmp);
		m = m_tmp;
	}

	hte = hts - hst;
	hre = hrs - hsr;
	MYDEBUG("h_te:    %6.1f m     Effective height of Tx antenna", hte);
	MYDEBUG("h_re:    %6.1f m     Effective heigth of Rx antenna", hre);


	// Section 5.1.6.4 - Calculate terrain roughness
	hm = -HUGE_VAL;
	for (int i=index_t; i<=index_r; i++) {
		// Equation 157
		double hm_tmp = data.e.at(i) - (hst + m * data.d.at(i));
		if (hm_tmp > hm) {
			hm = hm_tmp;
			index_hm = i;
		}
	}
	MYDEBUG("h_m:     %6.1f m     Terrain roughness", hm);
}
