/*!
 * \file locations.cpp
 * This file has been auto-generated from \a locations.yaml.
 * \ingroup Storage
 */

#include "locations.h"


#include <QFile>
#include <QTextStream>
#include "ParseCSV.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "LocationMapDialog.h"
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>


QList<Location> locations;


/*!
 * Stores ::locations into a file as comma-seperated values.
 *
 * \param fname Name of file to write
 *
 * \return \a true if file was created, \a false otherwise.
 */
bool saveLocations(const QString &fname)
{
	QFile file(fname);
	if (! file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	QTextStream f(&file);
	foreach (Location m, locations) {
		f << '"'<< m.name.replace('"',"\\\"") << '"'
		  << ", " << m.longitude
		  << ", " << m.latitude << "\n";
	}
	file.close();
	return true;
}


/*!
 * A parser to parse CSV-files (comma seperated values) with Location data.
 * \note This class has been auto-generated from \a locations.yaml.
 * \sa loadLocations()
 */
class LocationsParser : public ParseCSV
{
public:
	/*!
	 * Constructor
	 * \param fname Name of file to parse.
	 */
	LocationsParser(const QString &fname) : ParseCSV(fname) {};
	virtual void setData(int field, const QString &item);
	virtual void saveRecord();

/*!
 * Record of type Location.
 *
 * This is used as an intermediate storage place while parse() parses a
 * line from the CSV file.
 */
	Location m;
};

/*!
 * Called by parse() for every data item read from the CSV file.
 *
 * This implementation stores every item into the proper \ref m field.
 *
 * \param field Indicator of the field inside \ref m.
 *
 * \param item Data \a item to be stored. Will be automatically converted to the
 * target type in Location.
 */
void LocationsParser::setData(int field, const QString &item)
{
	switch (field) {
	case 0: m.name = item; break;
	case 1: m.longitude = item.toDouble(); break;
	case 2: m.latitude = item.toDouble(); break;
	}
}

/*!
 * Called by parse() for every finshed line.
 *
 * This implementation appends \ref m to ::locations.
 */
void LocationsParser::saveRecord()
{
	// START custom code
	m.number = locations.count()+1;
	// END custom code
	locations.append(m);
}


/*!
 * Parses the specified filed and stores it's contents in ::locations.
 *
 * \param fname Name of file to parse.
 * \return The result of LocationsParser::parse(), that is \a true if
 * the file was read successfully, \a false otherwise.
 */
bool loadLocations(const QString &fname)
{
	LocationsParser parser(fname);
	return parser.parse();
}



//! Integer representing column index of Location::number in the model.
const int COL_LOCATIONSMODEL_NUMBER = 0;
//! Integer representing column index of Location::name in the model.
const int COL_LOCATIONSMODEL_NAME = 1;
//! Integer representing column index of Location::longitude in the model.
const int COL_LOCATIONSMODEL_LONGITUDE = 2;
//! Integer representing column index of Location::latitude in the model.
const int COL_LOCATIONSMODEL_LATITUDE = 3;
//! Last column index of the model.
const int COL_LOCATIONSMODEL_LAST = 3;


/*!
 * Current sort order.
 *
 * Can be changed by clicking on the column headers.
 */
static Qt::SortOrder sortOrder = Qt::AscendingOrder;


/*!
 * Constructor.
 * \param parent Parent object.
 */
LocationsModel::LocationsModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

/*!
 * Returns the number of rows.
 * \param parent From where the rows should be counted.
 * \return Number of entries in ::locations if parent isn't invalid, else 0.
 */
int LocationsModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	return locations.count();
}

/*!
 * Returns the number of columns.
 * \param parent From where the columns should be counted.
 * \return Number of columns in this model, always
 * ::COL_LOCATIONSMODEL_LAST + 1
 */
int LocationsModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return COL_LOCATIONSMODEL_LAST + 1;
}

/*!
 * Return text for column header.
 * \param section Section (column) index.
 * \param orientation If data should be returned for the vertical or horizontal header.
 * \param role role (kind of data) that should be returned.
 * \return Header text for each colum when role == Qt::DisplayRole and
 * orientation orientation != Qt::Vertical. Else an empty QVariant().
 */
QVariant LocationsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Vertical) {
		return trUtf8(" ");
	} else {
		switch (section) {
		case COL_LOCATIONSMODEL_NUMBER:
			return trUtf8("Number");
		case COL_LOCATIONSMODEL_NAME:
			return trUtf8("Name");
		case COL_LOCATIONSMODEL_LONGITUDE:
			return trUtf8("Longitude");
		case COL_LOCATIONSMODEL_LATITUDE:
			return trUtf8("Latitude");
		}
	}
	return QVariant();
}

/*!
 * Return text for cell data.
 * \param index Index to the cell for which text should be returned.
 * \param role role (kind of data) that should be returned.
 * \return Contents of the matching ::locations record if role is
 * Qt::DisplayRole or Qt::EditRole. Alignment if role
 * Qt::TextAlignmentRole for those fields that have an alignment defined.
 * An empty QVariant otherwise.
 */
QVariant LocationsModel::data(const QModelIndex &index, int role) const
{
	int row = index.row();
	if (row < 0 || row >= locations.count())
		return QVariant();

	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		switch (index.column()) {
		case COL_LOCATIONSMODEL_NUMBER:
			return locations[row].number;
		case COL_LOCATIONSMODEL_NAME:
			return locations[row].name;
		case COL_LOCATIONSMODEL_LONGITUDE:
			return locations[row].longitude;
		case COL_LOCATIONSMODEL_LATITUDE:
			return locations[row].latitude;
		}
	} else
	if (role == Qt::TextAlignmentRole) {
		switch (index.column()) {
		case COL_LOCATIONSMODEL_LONGITUDE:
			return (int)(Qt::AlignRight | Qt::AlignVCenter);
		case COL_LOCATIONSMODEL_LATITUDE:
			return (int)(Qt::AlignRight | Qt::AlignVCenter);
		}
	}

	return QVariant();
}

/*!
 * Helper function for LocationsModel::sort().
 * \param one First Number to compare.
 * \param two Second Number to compare.
 * \return \a true or \a false, depending on the two values and sortOrder.
 */
static bool sortLocationsModelNumber(const Location &one, const Location &two)
{
	if (sortOrder == Qt::AscendingOrder)
		return one.number < two.number;
	else
		return one.number > two.number;
}

/*!
 * Helper function for LocationsModel::sort().
 * \param one First Name to compare.
 * \param two Second Name to compare.
 * \return \a true or \a false, depending on the two values and sortOrder.
 */
static bool sortLocationsModelName(const Location &one, const Location &two)
{
	if (sortOrder == Qt::AscendingOrder)
		return one.name < two.name;
	else
		return one.name > two.name;
}

/*!
 * Helper function for LocationsModel::sort().
 * \param one First Longitude to compare.
 * \param two Second Longitude to compare.
 * \return \a true or \a false, depending on the two values and sortOrder.
 */
static bool sortLocationsModelLongitude(const Location &one, const Location &two)
{
	if (sortOrder == Qt::AscendingOrder)
		return one.longitude < two.longitude;
	else
		return one.longitude > two.longitude;
}

/*!
 * Helper function for LocationsModel::sort().
 * \param one First Latitude to compare.
 * \param two Second Latitude to compare.
 * \return \a true or \a false, depending on the two values and sortOrder.
 */
static bool sortLocationsModelLatitude(const Location &one, const Location &two)
{
	if (sortOrder == Qt::AscendingOrder)
		return one.latitude < two.latitude;
	else
		return one.latitude > two.latitude;
}

/*!
 * Sorts the model.
 * \param column Column to sort.
 * \param order Order (ascending, descending to sort.
 * \sa sortLocationsModelNumber
 * \sa sortLocationsModelName
 * \sa sortLocationsModelLongitude
 * \sa sortLocationsModelLatitude
 */
void LocationsModel::sort(int column, Qt::SortOrder order)
{
	sortOrder = order;

	switch (column) {
	case COL_LOCATIONSMODEL_NUMBER:
		qStableSort(locations.begin(), locations.end(), sortLocationsModelNumber);
		break;
	case COL_LOCATIONSMODEL_NAME:
		qStableSort(locations.begin(), locations.end(), sortLocationsModelName);
		break;
	case COL_LOCATIONSMODEL_LONGITUDE:
		qStableSort(locations.begin(), locations.end(), sortLocationsModelLongitude);
		break;
	case COL_LOCATIONSMODEL_LATITUDE:
		qStableSort(locations.begin(), locations.end(), sortLocationsModelLatitude);
		break;
	default:
		return;
	}
	emit dataChanged(index(0, 0), index(locations.count(), COL_LOCATIONSMODEL_LAST));
}

/*!
 * Returns the item \a flags for the given \a index.
 * \param index Index to the cell for which flags should be returned.
 * \return Proper Qt::ItemFlags based on the field definition.
 */
Qt::ItemFlags LocationsModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	//flags |= Qt::ItemIsEnabled;

	if (!index.isValid())
		return flags;

	switch (index.column()) {
	case COL_LOCATIONSMODEL_NAME:
		flags |= Qt::ItemIsEditable;
		break;
	case COL_LOCATIONSMODEL_LONGITUDE:
		flags |= Qt::ItemIsEditable;
		break;
	case COL_LOCATIONSMODEL_LATITUDE:
		flags |= Qt::ItemIsEditable;
		break;
	}
	return flags;
}

/*!
 * Stores data into the model (or it's underlying container).
 * \param index \a index to the cell whose data should be changed.
 * \param value New \a value for this cell.
 * \param role The role for which the data should be changed, normally Qt::EditRole.
 * \return Returns \a true if successful; otherwise \a returns false.
 */
bool LocationsModel::setData(const QModelIndex &index, const QVariant &value,
	int role)
{
	int row = index.row();
	if (row < 0 || row >= locations.count())
		return false;

	if (role == Qt::EditRole || role == Qt::DisplayRole) {
		switch (index.column()) {
		case COL_LOCATIONSMODEL_NAME:
			locations[row].name = value.toString();
			break;
		case COL_LOCATIONSMODEL_LONGITUDE:
			locations[row].longitude = value.toDouble();
			break;
		case COL_LOCATIONSMODEL_LATITUDE:
			locations[row].latitude = value.toDouble();
			break;
		default:
			return false;
		}
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

/*!
 * Inserts rows into the model.
 * \param row New rows will be inserted before this \a row.
 * \param count Number of new rows to insert.
 * \param parent Points to some model cell.
 * \return Returns \a true if the rows could have been inserted; otherwise returns \a false.
 */
bool LocationsModel::insertRows(int row, int count, const QModelIndex &parent)
{
	if (count < 1 || row < 0 || row > rowCount(parent))
		return false;

	beginInsertRows(QModelIndex(), row, row + count - 1);
	Location m;
	for(int r = 0; r < count; ++r)
		locations.insert(row, m);
	endInsertRows();
	return true;

}

/*!
 * Removes rows from the model.
 * \param row Rows will be deleted starting from this \a row.
 * \param count Number of new rows to delete.
 * \param parent Points to some model cell.
 * \return Returns \a true if the rows could be deleted; otherwise  returns false.
 */
bool LocationsModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
		return false;

	beginRemoveRows(QModelIndex(), row, row + count - 1);
	for(int r = 0; r < count; ++r)
		locations.removeAt(row);
	endRemoveRows();
	return true;

}


/*!
 * Constructor
 *
 * \param parent Parent widget.
 */
LocationsView::LocationsView(QWidget *parent)
	: QTableView(parent)
{
	// START custom code
	setAlternatingRowColors(true);
	verticalHeader()->hide();
	horizontalHeader()->setSectionsMovable(true);
	setTabKeyNavigation(false);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	// END custom code

	// sort support
	connect(horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
	        SLOT(sortByColumn(int)) );
	setSortingEnabled(true);
	sortByColumn(0, Qt::AscendingOrder);
	setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed);

	// edit support
	connect(this, SIGNAL(activated(const QModelIndex &)), SLOT(slotEdit(const QModelIndex &)) );

}

/*!
 * Reacts to insert and delete key press events.
 *
 * Insert is handled by LocationDialog and
 * LocationsModel::insertRows(),
 * Delete is handled by LocationsModel::removeRows().
 *
 * \param event Pointer to a QKeyEvent.
 */
void LocationsView::keyPressEvent(QKeyEvent *event)
{

    /*
	switch (event->key()) {
	case Qt::Key_Insert: {
		Location m;
		m.number = locations.count()+1;
		m.longitude = 0.0;
		m.latitude = 0.0;
		QDialog *dia = new LocationMapDialog(&m, this);
		if (dia->exec() == QDialog::Accepted) {
			int row = 0;
			if (currentIndex().isValid())
				row = currentIndex().row();
			if (model()->insertRow(row))
				setCurrentIndex(model()->index(row+1, 0));
			locations[row] = m;
		}
		delete dia;
		return;
		}
	case Qt::Key_Delete:
		if (!currentIndex().isValid())
			return;
		model()->removeRows(currentIndex().row(), 1);
		return;
	}
	QAbstractItemView::keyPressEvent(event);
	*/
}

/*!
 * Edit current Location.
 *
 * This slot is connected to the signal activated(). It uses
 * LocationDialog to let the user edit the current Location from
 * ::locations.
 * \param index QModelIndex, which denotes the row.
 */
void LocationsView::slotEdit(const QModelIndex &index)
{
	int row = index.row();
	Location m = locations[row];
	QDialog *dia = new LocationMapDialog(&m, this);
	if (dia->exec() == QDialog::Accepted) {
		locations[row] = m;
		QModelIndex left = model()->index(row, 0);
		QModelIndex right = model()->index(row, 99);
		dataChanged(left, right);
	}
	delete dia;
}


/*!
 * Sets the \a model for the view to present.
 * This calls QTableView::setModel() and then it resizes and moves the
 * columns around. This cannot be done in the \ref
 * LocationsView::LocationsView "constructor", because at construction
 * time there aren't any columns defined. That would make calls to
 * setColumnWidth() etc. no-ops.
 * \param model Pointer to the model that should be used.
 */
void LocationsView::setModel(QAbstractItemModel *model)
{
	QTableView::setModel(model);
	setColumnWidth(0, 100);
	setColumnWidth(2, 100);
	setColumnWidth(3, 100);
	// START custom code
	horizontalHeader()->moveSection(COL_LOCATIONSMODEL_NAME, COL_LOCATIONSMODEL_LAST);
	horizontalHeader()->setStretchLastSection(true);
	// END custom code
}


/*!
 * Constructor
 *
 * \param record Pointer to the Location record that this dialog should edit.
 * \param parent Parent widget.
 * \param f (default 0) Window flags, if any.
 */
LocationDialog::LocationDialog(Location *record, QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f)
	, m(record)
{
	setWindowTitle(trUtf8("Location"));
	resize(500, 250);

	editName = new QLineEdit(this);
	editLongitude = new QLineEdit(this);
	editLongitudeValidator = new QDoubleValidator(editLongitude);
	editLongitudeValidator->setBottom(-180);
	editLongitudeValidator->setTop(180);
	editLongitude->setValidator(editLongitudeValidator);
	editLatitude = new QLineEdit(this);
	editLatitudeValidator = new QDoubleValidator(editLatitude);
	editLatitudeValidator->setBottom(-75);
	editLatitudeValidator->setTop(75);
	editLatitude->setValidator(editLatitudeValidator);

	formLayout = new QFormLayout;
	formLayout->addRow(trUtf8("&Name"), editName);
	formLayout->addRow(trUtf8("L&ongitude"), editLongitude);
	formLayout->addRow(trUtf8("L&atitude"), editLatitude);

	editName->setText(m->name);
	editLongitude->setText(QString::number(m->longitude));
	editLatitude->setText(QString::number(m->latitude));

	okCancel = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(okCancel, SIGNAL(accepted()), this, SLOT(accept()));
	connect(okCancel, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(formLayout);
	mainLayout->addWidget(okCancel);
	setLayout(mainLayout);
}

/*!
 * Stores the contents of the edit widgets into the record \ref m.
 */
void LocationDialog::accept()
{
	m->name = editName->text();
	if (!editLongitude->hasAcceptableInput()) {
		editLongitude->setFocus(Qt::OtherFocusReason);
		return;
	}
	m->longitude = editLongitude->text().toDouble();
	if (!editLatitude->hasAcceptableInput()) {
		editLatitude->setFocus(Qt::OtherFocusReason);
		return;
	}
	m->latitude = editLatitude->text().toDouble();
	QDialog::accept();
}
