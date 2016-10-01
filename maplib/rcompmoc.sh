#/bin/bash

/opt/qt5/5.7/gcc_64/bin/qmake -project

/opt/qt5/5.7/gcc_64/bin/moc DemDownloader.h -o DemDownloader.moc
/opt/qt5/5.7/gcc_64/bin/moc Downloader.h -o Downloader.moc
/opt/qt5/5.7/gcc_64/bin/moc LocationMapDialog.h -o LocationMapDialog.moc
/opt/qt5/5.7/gcc_64/bin/moc MapChooseWidget.h -o MapChooseWidget.moc
/opt/qt5/5.7/gcc_64/bin/moc SlippyMapWidget.h -o SlippyMapWidget.moc
/opt/qt5/5.7/gcc_64/bin/moc locations.h -o locations.moc

/opt/qt5/5.7/gcc_64/bin/qmake -project


