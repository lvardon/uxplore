#define DEBUGLVL 3
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
 */

#include <QApplication>
#include <QVBoxLayout>
#include <QComboBox>

#include "SlippyMapWidget.h"
#include "GeoUtils.h"
#include "MapChooseWidget.h"






int main(int argc, char **argv)
{
#if defined(Q_WS_X11)
    QApplication::setGraphicsSystem("raster");
#endif

    QApplication app(argc, argv);
    // app.setApplicationName("splat2");
    // app.setApplicationVersion("0.0");

    QWidget *w = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSpacing(0);

    MapChooseWidget *c = new MapChooseWidget(w);
    layout->addWidget(c);

    SlippyMapWidget *map = new SlippyMapWidget(w);
    layout->addWidget(map);

    SlippyCenterMark *mark = new SlippyCenterMark(map);
    map->appendPart(mark);

    SlippyZoomButtons *buttons = new SlippyZoomButtons(map);
    map->appendPart(buttons);

    w->setLayout(layout);

    w->connect(c, SIGNAL(setMap(const QString &, const QString &, int, int)),
               map, SLOT(setMap(const QString &, const QString &, int, int)) );


    w->show();
    w->resize(440, 560);

    int res = app.exec();
    delete w;
    return res;
}
