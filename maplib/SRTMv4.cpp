#ifndef DEBUGLVL
#define DEBUGLVL 0
#endif
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
 * This is a module to parse SRTM data and to extract heights from it.
 *
 * http://www.cgiar-csi.org/data/elevation/item/45-srtm-90m-digital-elevation-database-v41
 */

/*!
 * \file SRTMv4.cpp
 *
 * Quick access to SRTM V.4 data files.
 *
 * http://www.cgiar-csi.org/data/elevation/item/45-srtm-90m-digital-elevation-database-v41
 *
 *
 * \ingroup Storage
 */


#include <math.h>


#include "SRTMv4.h"
#include "GeoUtils.h" // for nodata, invdata


/*!
 * \class SRTMv4
 * \ingroup Storage Geodetic
 *
 * Access elevation data from SRTM V.4 files.
 *
 *
 *
 * \var SRTMv4::ok
 *
 * Indicates that SRTMv4::parse() was successful.
 *
 * \var SRTMv4::dataPath
 *
 * Path to a directory that contains the SRTMv4 files. It is used by
 * SRTMv4::get(const QPointF &lonLat) when currently no SRTM file has been
 * loaded.
 *
 * \var SRTMv4::dataofs
 *
 * When the SRTM V.4 files fullfills certain criteria, then all elevation
 * data is stored in adjacent bytes inside the file. This variable contains
 * the offset into the file of this area.
 *
 * \var SRTMv4::length
 *
 * Length (or height) of the "image" inside the TIFF file. In other wide,
 * the SRTM V.4 file contains \ref length lines of elevation data.
 *
 * \var SRTMv4::width
 *
 * Width of one line inside the TIFF file. Each line contains \ref width
 * elevation points.
 *
 * \var SRTMv4::map
 *
 * For faster access, the elevation data area indicated by \ref dataofs
 * will be memory mapped. This speeds up \ref get(quint32 x, quint32 y).
 *
 * \var SRTMv4::modelTiepoint
 *
 * Storage place for some GeoTIFF related data.
 *
 * \var SRTMv4::geoDoubleParams
 *
 * Storage place for some GeoTIFF related data.
 *
 * \var SRTMv4::f
 *
 * QFile to an opened (by SRTMv4::parse()) file.
 *
 * \fn bool SRTMv4::isOk() const
 *
 * Retrieve parsing status.
 *
 * \return True if \ref parse() succeeded.
 */

/*!
 * Constructor
 *
 * \param datapath   Path to a directory that contains the SRTMv4 files.
 *                   It is used by SRTMv4::get(const QPointF &lonLat) when
 *                   currently no SRTM file has been loaded.
 * \param parent     Pointer to QObject
 */
SRTMv4::SRTMv4(const QString &datapath, QObject *parent)
	: QObject(parent)
	, ok(false)
	, map(0)
{
	MYTRACE("SRTMv4::SRTM");

	// make sure that a non-empty datapath ends with an /
	dataPath = datapath;
	if (!dataPath.isEmpty())
		if (dataPath.at(dataPath.count()-1) != '/')
			dataPath.append('/');
	MYVERBOSE("   dataPath: '%s'", qPrintable(dataPath));
}


/*!
 * Parses an SRTMv4 file (to make sure it has expected properties).
 *
 * This is a specialized parser to read in GeoTIFF-files with SRTM data,
 * especially the SRTMv4.1 data from CGIAR. You can get those files from:
 *
 * - ftp://srtm.csi.cgiar.org/SRTM_V41/SRTM_Data_GeoTiff/
 * - http://srtm.csi.cgiar.org/SRT-ZIP/SRTM_V41/SRTM_Data_GeoTiff/
 * - http://droppr.org/srtm/v4.1/6_5x5_TIFs/
 * - ftp://xftp.jrc.it/pub/srtmV4/tiff/
 * - http://webbuster.dyndns.info/xcsoar/mapgen_data/terrain/srtm/
 * -------------
 *
 * Why am I using a self-written parser and not simply GDAL, you may ask?.
 * Because GDAL is very slow when accessing individual pixels. Look at
 * http://cullenking.com/2010/6/2/converting-dems-from-geotiff-to-gridfloat
 * for a reference.
 *
 * Even using libtiff directly would be a bit slower than this parser. TIFF
 * is a very versatile format, and as such quite complex. For example, TIFFs
 * can have at least 7 different compression methods, contain several
 * picture, monochrome-, greyscale, RGB or YCbCrB pictures. Name a feature,
 * it will be supported. This and it's internal format makes it complicated
 * and "slow" to access individual stuff: you will always need to handle
 * another special case.
 *
 * Now it happens to be the case that the SRTMv4.1 files are actually quite
 * simple. At some data-offset, there is a consecutive array of 6000*6000
 * qint16's with all the elevation data. "Parsing" this TIFF file is now
 * reduced to a mere checking that the file indeed has the structure we
 * expect ... and then we can seek to the proper file-position and quickly
 * read the needed data. Voila!
 *
 * \param path  File name (including directory) of the SRTM V.4 file to parse.
 *
 * \return true if parsing succeeded and the SRTM V.4 file has all the properties
 * expected to quickly mmap it.
 */
bool SRTMv4::parse(const QString &path)
{
	MYTRACE("SRTMv4::parse(%s)", qPrintable(path));

    // Structures found inside a TIFF file
	struct tiffHeader {
		quint16 byteOrder;
		quint16 theNumber;
		quint32 firstIFD;
	};

	struct dirEntry {
		quint16 tag;
		quint16 typ;
		quint32 cnt;
		quint32 n;
	};

	struct geoDirEntry {
		quint16 key;
		quint16 loc;
		quint16 cnt;
		quint16 ofs;
	};

	map = 0;
	dataofs = 0;
	ok = false;

	if (f.isOpen()) {
		if (map) {
			f.unmap(map);
			map = 0;
		}
		f.close();
	}

	f.setFileName(path);
	if (!f.open(QIODevice::ReadOnly)) {
		QString s("Cannot open TIFF '%1'");
		return err(qPrintable(s.arg(path)));
	}

	// Read tiff header
	struct tiffHeader hdr;
	if (f.read((char*) &hdr, sizeof(hdr)) != sizeof(hdr))
		return err("TIFF too small");
	if (hdr.byteOrder != 0x4949)
		return err("TIFF with wrong byte-order");
	if (hdr.theNumber != 42)
		return err("Not a TIFF");
	if (!f.seek(hdr.firstIFD))
		return err("TIFF too small");

	quint16 dirCount;
	if (f.read((char*) &dirCount, sizeof(dirCount)) != sizeof(dirCount))
		return err("TIFF too small");

	enum dir_types {
	                tByte = 1,
	                tAscii,
	                tShort,
	                tLong,
	                tRational,
	                tSByte,
	                tUndefined,
	                tSShort,
	                tSLong,
	                tSRational,
	                tFloat,
	                tDouble
	};

	length = 0;
	width = 0;
	quint32 stripOffsetsOfs = 0;
	quint32 stripByteCountsOfs = 0;
	quint32 geoTiffDirectoryOfs = 0;
	quint32 geoTiffDirectoryLen = 0;
	quint32 geoDoubleParamsOfs = 0;
#if 0
	quint32 geoAsciiParamsOfs = 0;
	quint32 geoAsciiParamsLen = 0;
#endif
	quint32 modelTiepointOfs = 0;
	quint32 modelPixelScaleOfs = 0;
	quint32 gdalNodataOfs = 0;
	quint32 gdalNodataLen = 0;
	unsigned char data[256];

	for (int i=0; i<dirCount; i++) {
		struct dirEntry dir;
		if (f.read((char*) &dir, sizeof(dir)) != sizeof(dir))
			return err("TIFF too small");

		MYVERBOSE("dir %2d tag 0x%04x (%5d), typ %4d, cnt %6d, n %8d",
		          i,
		          dir.tag, dir.tag,
		          dir.typ,
		          dir.cnt,
		          dir.n);

		// check "PhotometricInterpretation", must be 1 (BlackIsZero), TIFF6.pdf page 17
		if (dir.tag == 0x106 && dir.typ == tShort) {
			if (dir.cnt != 1 || dir.n != 1)
				return err("PhotometricInterpretation != BlackIsZero");
		} else

		// check "Compression", must be 1 (no compression), TIFF6.pdf page 17
		if (dir.tag == 0x103 && dir.typ == tShort) {
			if (dir.cnt != 1 || dir.n != 1)
				return err("Compression != none");
		} else

		// ImageLength, TIFF6.pdf page 18
		if (dir.tag == 0x101 && (dir.typ == tShort || dir.typ == tLong)) {
			//MYVERBOSE("length %d", dir.n);
			length = dir.n;
		} else

		// ImageWidth, TIFF6.pdf page 18
		if (dir.tag == 0x100 && (dir.typ == tShort || dir.typ == tLong)) {
			//MYVERBOSE("width %d", dir.n);
			width = dir.n;
		} else

		// check "RowsPerStrip", must be 1, TIFF6.pdf page 19
		if (dir.tag == 0x116 && (dir.typ == tShort || dir.typ == tLong)) {
			if (dir.cnt != 1 || dir.n != 1)
				return err("RowsPerStrip != 1");
		} else

		// StripOffsets, points into file, TIFF6.pdf page 19
		if (dir.tag == 0x111 && (dir.typ == tShort || dir.typ == tLong)) {
			if (dir.cnt != length)
				return err("Wrong number of StripOffsets");
			//MYVERBOSE("strip offsets %d", dir.n);
			stripOffsetsOfs = dir.n;
		} else

		// StripByteCounts, points into file, TIFF6.pdf page 19
		if (dir.tag == 0x117 && (dir.typ == tShort || dir.typ == tLong)) {
			//MYVERBOSE("strip byte counts %d", dir.n);
			if (dir.cnt != length)
				return err("Wrong number of StripByteCounts");
			stripByteCountsOfs = dir.n;
		} else

		// check "BitsPerSample", must be 16, TIFF6.pdf page 22
		if (dir.tag == 0x102 && dir.typ == tShort) {
			if (dir.cnt != 1 || dir.n != 16)
				return err("BitsPerSample != 16");
		} else

		// check "SamplesPerPixel", must be 1, TIFF6.pdf page 24
		if (dir.tag == 0x115 && dir.typ == tShort) {
			if (dir.cnt != 1 || dir.n != 1)
				return err("SamplesPerPixel != 1");
		} else

		// check "PlanarConfiguration", must be 1 (chunky), TIFF6.pdf page 38
		if (dir.tag == 0x11c && dir.typ == tShort) {
			if (dir.cnt != 1 || dir.n != 1)
				return err("PlanarConfiguration != chunky");
		} else

		// TileWidth, TileLength, TileOffsets, TileByteCounts, TIFF6.pdf page 67f
		if (dir.tag >= 0x142 && dir.tag <= 0x145) {
			return err("Tiled TIFFs are unsupported");
		} else

		// check "SampleFormat", must be 2 (two's complement signed
		// integer data), TIFF6.pdf page 80
		if (dir.tag == 0x153 && dir.typ == tShort) {
			if (dir.cnt != 1 || dir.n != 2)
				return err("SampleFormat != short");
		} else

		// GeoKeyDirectoryTag, Geotiff 2.4
		if (dir.tag == 0x87af && dir.typ == tShort) {
			//MYVERBOSE("geo key directory %d", dir.n);
			geoTiffDirectoryOfs = dir.n;
			geoTiffDirectoryLen = dir.cnt;
			if (dir.cnt < 4)
				return err("Not enought elements in GeoKeyDirectory");
		} else

		// GeoDoubleParamsTag. Geotiff 2.4
		if (dir.tag == 0x87b0 && dir.typ == tDouble) {
			geoDoubleParamsOfs = dir.n;
			geoDoubleParams.resize(dir.cnt);
		} else

		// GeoAsciiParamsTag, Geotiff 2.4
		if (dir.tag == 0x87b1 && dir.typ == tAscii) {
#if 0
			//MYVERBOSE("get ascii params %d", dir.n);
			geoAsciiParamsOfs = dir.n;
			geoAsciiParamsLen = dir.cnt;
#endif
		} else

		// ModelTiepointTag, Geotiff 2.6.1
		if (dir.tag == 0x8482 && dir.typ == tDouble) {
			//MYVERBOSE("model tiepoint %d", dir.n);
			modelTiepointOfs = dir.n;
			if (dir.cnt != 6)
				return err("Unsupported elements in ModelTiepoint");
		} else

		// ModelPixelScaleTag, Geotiff 2.6.1
		if (dir.tag == 0x830e && dir.typ == tDouble) {
			//MYVERBOSE("model pixel scale %d", dir.n);
			modelPixelScaleOfs = dir.n;
			if (dir.cnt != 3)
				return err("Unsupported elements in ModelPixelScale");
		} else

		// GDAL_NODATA, http://www.awaresystems.be/imaging/tiff/tifftags/private.html
		if (dir.tag == 0xa481 && dir.typ == tAscii) {
			gdalNodataOfs = dir.n;
			gdalNodataLen = dir.cnt;
			if (gdalNodataLen > sizeof(data))
				return err("GDAL_NODATA entry too long");
		}

		else {
			MYDEBUG("unsupported dir %2d tag 0x%04x (%5d), typ %4d, cnt %6d, n %8d",
		          i,
		          dir.tag, dir.tag,
		          dir.typ,
		          dir.cnt,
		          dir.n);
		}
	}

	// check if there are more than one directories in the file.
	quint32 nextIFD;
	if (f.read((char *) &nextIFD, sizeof(nextIFD)) != sizeof(nextIFD))
		return err("TIFF too small");
	if (nextIFD != 0)
		return err("Cannot handle more than one directory entry");

	// Make sure GDAL_NODATA exists and is -32768
	if (!gdalNodataLen)
		return err("No GDAL_NODATA");
	if (!f.seek(gdalNodataOfs))
		return err("TIFF too small");
	if (f.read((char *) data, gdalNodataLen) != gdalNodataLen)
		return err("TIFF too small");
	/*
	  srtm_50_23.tif contains junk
	if (qstrcmp((char*)data, "-32768") != 0) {
		dump(data, gdalNodataLen);
		return err("Unsupported GDAL_NODATA value");
	}
	*/

	double modelPixelScale[3];
	if (!modelPixelScaleOfs)
		return err("No ModelPixelScale tag");
	if (!f.seek(modelPixelScaleOfs))
		return err("TIFF too small");
	if (f.read((char*)&modelPixelScale, sizeof(modelPixelScale)) != sizeof(modelPixelScale))
	 	return err("TIFF too small");
	for (int i=0; i<3; i++)
		MYDEBUG("ModelPixelScale %d: %e", i, modelPixelScale[i]);
	// 6000 pixels / 5 degree = 0.00083333333333 pixel per degree
	if (fabs(modelPixelScale[0] - 8.333333e-04) > 1e-5)
		return err("Unsupported modelPixelScale[0]");
	if (fabs(modelPixelScale[1] - 8.333333e-04) > 1e-5)
		return err("Unsupported modelPixelScale[1]");
	if (modelPixelScale[2] != 0)
		return err("Unsupported modelPixelScale[2]");


	if (!modelTiepointOfs)
		return err("No ModelTiepoint tag");
	if (!f.seek(modelTiepointOfs))
		return err("TIFF too small");
	if (f.read((char*)&modelTiepoint, sizeof(modelTiepoint)) != sizeof(modelTiepoint))
	 	return err("TIFF too small");
	for (int i=0; i<6; i++)
		MYDEBUG("ModelTiepoint %d: %f", i, modelTiepoint[i]);
	if (modelTiepoint[0] != 0 || modelTiepoint[1] != 0)
		return err("modelTiepoint not anchorded to 0,0");
	if (modelTiepoint[2] != 0)
		return err("Unsupported modelTiepoint");
	if (modelTiepoint[5] != 0)
		return err("Unsupported modelTiepoint");

	if (geoDoubleParamsOfs) {
		if (!f.seek(geoDoubleParamsOfs))
			return err("TIFF too small");
		qint64 sz = sizeof(double)*geoDoubleParams.size();
		if (f.read((char*)geoDoubleParams.data(), sz) != sz)
			return err("TIFF too small");
		for (int i=0; i<2; i++) {
			MYVERBOSE("  geoDoubleParams %d: %f", i, geoDoubleParams.at(i));
		}
	}

	if (!geoTiffDirectoryOfs)
		return err("No GeoTiffDirectory tag");
	if (!f.seek(geoTiffDirectoryOfs))
		return err("TIFF too small");
	for (quint16 i=0; i < geoTiffDirectoryLen/4; i++) {
		struct geoDirEntry dir;
		if (f.read((char*) &dir, sizeof(dir)) != sizeof(dir))
			return err("TIFF too small");

		MYVERBOSE("geo key %d: key 0x%04x %5d, loc 0x%04x, %5d, cnt %5d, ofs %5d 0x%04x", i,
		          dir.key, dir.key,
		          dir.loc, dir.loc,
		          dir.cnt,
		          dir.ofs, dir.ofs);

		if (i == 0) {
			if (dir.key != 1 ||
			    dir.loc != 1 || dir.cnt != 0)
			    return err("Unsupported GeoTiffDirectory version");
			if (dir.ofs != geoTiffDirectoryLen/4-1)
				return err("Mismatch in GeoTiffDirectory length");
		} else

		// GTModelTypeGeoKey, http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.1.1
		if (dir.key == 1024) {
			if (dir.loc != 0)
				return err("Unsupported key location");
			if (dir.cnt != 1)
				return err("Unsupported key count");
			if (dir.ofs != 2)
				return err("GTModelTypeGeoKey != Geographic");
		} else

		// GTRasterTypeGeoKey, http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.1.2
		if (dir.key == 1025) {
			if (dir.loc != 0)
				return err("Unsupported key location");
			if (dir.cnt != 1)
				return err("Unsupported key count");
			if (dir.ofs != 1)
				return err("GTRasterTypeGeoKey != RasterPixelIsArea");
		} else

		// GeographicTypeGeoKey, http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.2.1
		if (dir.key == 2048) {
			if (dir.loc != 0)
				return err("Unsupported key location");
			if (dir.cnt != 1)
				return err("Unsupported key count");
			if (dir.ofs != 4326)
				return err("GTRasterTypeGeoKey != GCS_WGS_84");
		} else

		// GeogCitationGeoKey
		if (dir.key == 2049) {
			if (dir.loc != 0x87b1)
				return err("GeogCitationGeoKey stored in unknown location");
			// dir.cnt: length
			// dir.ofs: offset into the GeoAsciiParams
		} else

		// GeogAngularUnitsGeoKey, http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.1.4
		if (dir.key == 2054) {
			if (dir.loc != 0)
				return err("Unsupported key location");
			if (dir.cnt != 1)
				return err("Unsupported key count");
			if (dir.ofs != 9102)
				return err("GeogAngularUnitsGeoKey != Angular_Degree");
		} else

		// GeogSemiMajorAxisGeoKey
		if (dir.key == 2057 && dir.loc == 0x87b0) {
			if (dir.ofs > geoDoubleParams.size())
				return err("Wrong GeogSemiMajorAxisGeo offset");
			MYDEBUG("GeogSemiMajorAxisGeo %f", geoDoubleParams.at(dir.ofs));
		} else

		// GeogInvFlatteningGeoKey
		if (dir.key == 2059 && dir.loc == 0x87b0) {
			if (dir.ofs > geoDoubleParams.size())
				return err("Wrong GeogInvFlatteningGeoKey offset");
			MYDEBUG("GeogInvFlatteningGeoKey %f", geoDoubleParams.at(dir.ofs));
		} else

		{
			MYDEBUG("unsupported geo %d: key 0x%04x %5d, loc 0x%04x, %5d, cnt %5d, ofs %5d", i,
			        dir.key, dir.key,
			        dir.loc, dir.loc, dir.cnt, dir.ofs);
		}
	}

	// Check that every strip of data has the right width
	if (!stripByteCountsOfs)
		return err("No stripByteCounts");
	if (!f.seek(stripByteCountsOfs))
		return err("TIFF too small");
	QVector<quint32> stripByteCounts(length);
	qint64 sz = stripByteCounts.size() * sizeof(quint32);
	if (f.read((char *) stripByteCounts.data(), sz) != sz)
		return err("TIFF too small");
	for (int i=0; i<stripByteCounts.size(); i++) {
		// each strip contains width * sizeof(short) bytes
		if (stripByteCounts.at(i) != width*2)
			return err("Unsupported width in stripByteCounts");
	}

	// Check that the data is in adjacent positions
	if (!stripOffsetsOfs)
		return err("No stripOffsets");
	if (!f.seek(stripOffsetsOfs))
		return err("TIFF too small");
	QVector<quint32> stripOffsets(length);
	sz = stripOffsets.size() * sizeof(quint32);
	if (f.read((char *) stripOffsets.data(), sz) != sz)
		return err("TIFF too small");
	// check if all strips are adjacent
	quint32 ofs = stripOffsets.at(0);
	dataofs = ofs;
	MYVERBOSE("data starts at %d (0x%x)", dataofs, dataofs);
	for (int i=1; i<stripOffsets.size(); i++) {
		quint32 n = stripOffsets.at(i);
		if ((n-ofs) != 12000)
			return err("Strips not adjacent");
		ofs = n;
	}

	/*
	 * Now we know exactly that we have ...
	 * * uncompressed data
	 * * with signed short heigh data
	 * * all adjacent
	 * * starting at dataofs.
	 * That would us now allow to read in the data in one-go, or to open the
	 * file as necessary and seek into the needed bytes.
	 */

	/*
	 * To further speed up access, we can now mmap it. If the map fails, we
	 * simple get back a 0. No harm is done then.
	 */
	map = f.map(dataofs, length * width * sizeof(quint16));
	if (!map)
		return err("could not memory map");

	ok = true;
	return true;
}


/*!
 * Retrieve an elevation data from pixels
 *
 * \param x  x-coordinate in pixels of elevation data
 * \param y  y-coordinate in pixels of elevation data
 * \returns  - elevation in meters
 *           - nodata if no elevation data is present
 *           - invdata if pixel coordinates are invalid
 */
qint16 SRTMv4::get(quint32 x, quint32 y)
{
	MYVERBOSE("SRTMv4::get(%d, %d)", x, y);

	if (x >= width || y >= length)
		return invdata;

	if (!ok)
		return nodata;

	quint32 pos = y*width + x;
	pos *= sizeof(quint16);

	/* Is the file memory mapped? */
	if (map)
		return * ((qint16*) (map+pos));

	pos += dataofs;
	MYVERBOSE("  pos %d", pos);
	if (!f.seek(pos))
	    return nodata;
	qint16 data;
	if (!f.read((char*) &data, sizeof(data)) == sizeof(data))
		return nodata;
	return data;
}


/*!
 * Retrieve an elevation data
 *
 * \param lonLat  geographical coordinates in degrees
 * \returns  One of the following
 *           - elevation in meters
 *           - \ref nodata if no elevation data is present
 *           - \ref invdata if pixel coordinates are invalid
 */
qint16 SRTMv4::get(const QPointF &lonLat)
{
	MYTRACE("SRTMv4::get(%f, %f)", lonLat.x(), lonLat.y());

	if (!area().contains(lonLat)) {
		QString fn = dataPath + baseFileName(lonLat) + ".tif";
		if (!parse(fn))
			return nodata;
	}

	return get(nearbyint((lonLat.x() - modelTiepoint[3]) * ppd),
	           nearbyint((modelTiepoint[4] - lonLat.y()) * ppd));
}


/*!
 * Returns the area covered by this SRTM V.4.
 *
 * \return Geographical coordinates covered by this SRTM measured in degrees
 * (east is positive).
 */
QRectF SRTMv4::area() const
{
	if (!ok)
		return QRect();
	return QRectF(modelTiepoint[3], modelTiepoint[4], 5, -5);
}


/*!
 * Determine SRTM V.4 file name
 *
 * This function is true for SRTMv4.1 data only. It calculates the
 * tile numbers for a given longitude/latitude.
 *
 * \param lonLat  geographical coordinates in degrees
 * \returns       File name that the TIFF should have, excluding any
 *                extension.
 */
QString SRTMv4::baseFileName(const QPointF &lonLat)
{
	MYTRACE("lonLatToSRTM(%f,%f)", lonLat.x(), lonLat.y());
	int x = (lonLat.x() + 180) / 5 + 1;
	int y = (60 - lonLat.y()) / 5 + 1;
	if (x < 1 || x > 72 || y < 1 || y > 24) {
		qWarning("No SRTMv4.1 data for long/lat");
		return QString::null;
	}
	QString srtmBase = QString("srtm_%1_%2")
		.arg(x, 2, 10, QLatin1Char('0'))
		.arg(y, 2, 10, QLatin1Char('0'));
	MYVERBOSE("  x,y %d,%d '%s'", x, y, qPrintable(srtmBase));

	return srtmBase;
}
