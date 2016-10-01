#ifndef ITUTERRAIN_H
#define ITUTERRAIN_H

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
 */

/*!
 * \file ITUTerrain.h
 *
 * ITU Terrain profile classification
 *
 * \ingroup Propagation
 */


#include "GeoUtils.h"


/*!
 * Terrain related calculations from several ITU recommendations.
 *
 * This is based on P.452 and P.1812.
 *
 * \ingroup Propagation Geodetic
 */
class ITUTerrain
{
	public:
	ITUTerrain();
	void reset();
	void pathProfileAnalysis(double f_MHz,
	                         double DELTA_N,
	                         double htg,
	                         double hrg,
	                         const GeoData &data);

	double htg;     //!< Tx antenna height above ground [m]
	double hrg;     //!< Rx antenna height above ground [m]
	double k50;     //!< Median effective earth radius factor
	double ae;      //!< Median value of effective Earth radius [km]
	double hgt;     //!< Ground height above mean sea level (AMSL) at Tx [m]
	double hgr;     //!< Ground height AMSL at Rx [m]
	double dtot;    //!< Total distance between Tx and Rx [m]
	double hts;     //!< Transmitting antenna height AMSL [m]
	double hrs;     //!< Receiving antenna height AMSL [m]
	bool path_trans_horizon; //!< Line-of-Sight or Over-the-Horizon?
	double theta_t; //!< Horizontal elevation angle (above local horizont) at Tx ant OR the elevation angle to the other terminal [mrad]
	double theta_r; //!< Horizontal elevation angle (above local horizont) at Tx ant OR the elevation angle to the other terminal [mrad]
	double theta;   //!< Earth centered, angular distance between Tx & Rx antennas [mrad]
	double hst;     //!< Height of the smooth-earth surface AMSL at Tx station [m]
	double hsr;     //!< Height of the smooth-earth surface AMSL at Rx station [m]
	double hm;      //!< Terrain roughness parameter [m]
	double hte;     //!< Effective height of Tx antenna [m]
	double hre;     //!< Effective height of Rx antenna [m]
	double dlt;     //!< Great circle distance from Tx ant to its horizon point [m]
	double dlr;     //!< Great circle distance from Rx ant to its horizon point [m]

	int index_t;    //!< Index into \ref GeoData::e "data.e" for \ref dlt
	int index_r;    //!< Index into \ref GeoData::e "data.e" for \ref dlr
	int index_hm;   //!< Index into \ref GeoData::e "data.e" for \ref hm
};


#endif
