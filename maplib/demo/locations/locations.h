#ifndef LOCATIONS_H
#define LOCATIONS_H


/*!
 * \file locations.h
 * This file has been auto-generated from \a locations.yaml.
 * \ingroup Storage
 */


#include <QString>
#include <QList>
#include <QAbstractTableModel>
#include <QTableView>
#include <QDialog>


class QLineEdit;
class QFormLayout;
class QDialogButtonBox;


/*!
 * Stores a QTH location.
 */
class Location {
public:
	//! Internal number, corresponds to the position in the file. Mostly used for sorting in the LocationsView.
	quint32 number;
	//! Name of this location.
	QString name;
	//! Longitude (in degrees) of this position. East is positive, west is negative. This uses the WGS 84 datum --- the same as GPS.
	double longitude;
	//! Longitude (in degrees) of this position. North is positive, south is negative. This uses the WGS 84 datum --- the same as GPS.
	double latitude;
};


/*!
 * A container for QTH records.
 *
 * \sa saveLocations()
 * \sa loadLocations()
 * \sa LocationDialog
 * \sa LocationsView
 */
extern QList<Location> locations;

bool saveLocations(const QString &fname);
bool loadLocations(const QString &fname);


/*!
 * A QAbstractTableModel to view and edit the contents of \ref locations 
 * \note This class has been auto-generated from \a locations.yaml.
 */
class LocationsModel : public QAbstractTableModel
{
public:
	LocationsModel(QObject *parent=0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex() ) const;

	QVariant headerData(int section, Qt::Orientation orientation,
	                    int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role) const;

	// Sort support:
	virtual void sort(int column, Qt::SortOrder order=Qt::AscendingOrder);

	// In-Table edit support:
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value,
	             int role = Qt::EditRole);
	virtual bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
	virtual bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
};


/*!
 * A QTableView to view and edit the contents of \ref locations 
 * \note This class has been auto-generated from \a locations.yaml.
*/
class LocationsView : public QTableView
{
	Q_OBJECT
public:
	LocationsView(QWidget *parent=0);
public slots:
	virtual void keyPressEvent(QKeyEvent *event);
public slots:
	void slotEdit(const QModelIndex &index);
	virtual void setModel(QAbstractItemModel *model);
};


/*!
 * Class to edit / change records of type Location.
 *
 * \note This class has been auto-generated from \a locations.yaml.
 */
class LocationDialog : public QDialog
{
public:
	LocationDialog(Location *record, QWidget *parent=0, Qt::WindowFlags f=0);
	virtual void accept();

	//! Pointer to a Location record.
	Location *m;

	//! Widget to edit m->name
	QLineEdit *editName;
	//! Widget to edit m->longitude
	QLineEdit *editLongitude;
	//! QValidator to make sure the user can't enter impossible degree values.
	QDoubleValidator *editLongitudeValidator;
	//! Widget to edit m->latitude
	QLineEdit *editLatitude;
	//! QValidator to make sure the user can't enter impossible degree values.
	QDoubleValidator *editLatitudeValidator;

	//! OK and CANCEL buttons.
	QDialogButtonBox *okCancel;
	//! QFormLayout to make all the widgets and labels align nicely.
	QFormLayout *formLayout;
};


#endif
