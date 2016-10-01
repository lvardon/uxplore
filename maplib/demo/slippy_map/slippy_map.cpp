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

#include <QApplication>

#include "SlippyMapWidget.h"
#include "SlippyLocations.h"
#include "locations.h"
#include "GeoUtils.h"

int main(int argc, char **argv)
{
#if defined(Q_WS_X11)
    QApplication::setGraphicsSystem("raster");
#endif

    QApplication app(argc, argv);
    // app.setApplicationName("splat2");
    // app.setApplicationVersion("0.0");

    SlippyMapWidget w;

    SlippyCenterMark *mark = new SlippyCenterMark(&w);
    w.appendPart(mark);

    SlippyZoomButtons *buttons = new SlippyZoomButtons(&w);
    w.appendPart(buttons);

    loadLocations("../../data/locations.csv");
    SlippyLocations *loc = new SlippyLocations(&w);
    w.appendPart(loc);

    QPointF db0ft(9.93654, 50.4987);
    QPointF db0was(8.45748, 50.2322);
    GeoData d;
    MakePath(db0ft, db0was, d);
    SlippyPath *path = new SlippyPath(&d, &w);
    w.appendPart(path);

    w.show();
    w.resize(440, 560);

    return app.exec();
}
