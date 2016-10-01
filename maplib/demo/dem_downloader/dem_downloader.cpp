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


#include <QCoreApplication>

#include "DemDownloader.h"


int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    DemDownloader dem("../../data/");

    /*
     * Note that this works only when the file isn't there. If the file
     * then tileFilename() emits useSRTM(QString) before app.exec() runs, this the
     * signal vanishes into nirvana.
     */
    QObject::connect(&dem, SIGNAL(useSRTM(const QString &)), qApp, SLOT(quit()) );

    QPointF p(8.77485, 50.28425);
    QString fn = dem.tileFilename(p);
    qDebug("tile file name: %s", qPrintable(fn));

    return app.exec();
}
