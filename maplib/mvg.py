#!/usr/bin/python
# -*- coding: iso-latin-1 -*-

#
# mvg - Model-View-Generator for Qt 4.x
#
# ------------------------------------------------------------------
#
# Copyright (C) 2010 by H.Schurig, Germany, Wöllstadt (DH3HS)
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#
# ------------------------------------------------------------------
#
# This script takes a *.yaml file and creates a *.h and *.cpp file that contain all the
# boilerplate for a Qt Model/View table which stores it's data in QList<struct>.
#
# Currently possible:
# * Displaying
# * in-place edit
# * adding records
# * deleting records
# * editing records via QDialog
#


import sys, yaml, os

col_count = 0
col_prefix = ""
c_col = []
has_sort = False

h_include = []
h_classes = []
c_include = []
h_struct = []
h_container = []
c_container = []
h_model = []
c_model = []
h_view = []
c_view = []
h_dialog = []
c_dialog = []


types = {
	"double":  { #"widget": ("QDoubleSpinBox", "setValue", "value"),
	             "widget": ("QLineEdit", "setText(QString::number(%))", "text().toDouble()"),
	             "validator": "QDoubleValidator",
	             "load": ".toDouble()",
	             "default": "0.0", },
	"int":	   { "widget": ("QSpinBox", "setValue(%)", "value()"),
	             "load": ".toInt()",
	             "default:": 0, },
	"quint32": { "widget": ("QSpinBox", "setValue(%)", "value()"),
	             "load": ".toUInt()",
	             "default:": 0, },
	"bool":    { "widget": ("QCheckBox", "setChecked(%)", "checked()"),
	             "load": ".toInt()",
	             "default:": 0, },
	"QString": { "widget": ("QLineEdit", "setText(%)", "text()"),
	             "load": "",
	             "fromVariant": ".toString()",
	             "preSave": """<< '"'""",
	             "postSave": """.replace('"',"\\\\\\"") << '\"'""",
	             "default": ""},
	}

class MyException(Exception):
	def _get_message(self):
		return self._message
	def _set_message(self, message):
		self._message = message
	message = property(_get_message, _set_message)



##############################################################


class Holder(object):
	def __init__(self, name, data):
		"""Creates a new folder object:

		"""
		self._inherit_from = []
		self.name = name
		self._accesses = {}
		self.__dict__.update(data)

	def keys(self):
		"""Return list of stored variables.

		>>> h = Holder(bar=1, baz="test")
		>>> print sorted(h.keys())
		['bar', 'baz']
		"""
		return filter(lambda x: x[0] != '_', self.__dict__.keys())

	def has_key(self, key):
		self._accesses[key] = True
		return self.__dict__.has_key(key)

	def get(self, key, value=None):
		if not self.__dict__.has_key(key):
			return value
		return self.__dict__[key]

	def setDefault(self, key, value):
		self._accesses[key] = True
		if not self.__dict__.has_key(key):
			self.__dict__[key] = value

	def __getitem__(self, key):
		"""Allows access to the variables via obj[name] syntax.

		>>> h = Holder()
		>>> h.foo = "Hello"
		>>> print h["foo"]
		Hello
		"""
		#print "get %s.%s" % (self.name, key)
		self._accesses[key] = True
		try:
			return self.__dict__[key]
		except:
			pass
		for inh in self._inherit_from:
			try:
				return inh[key]
			except:
				pass
			raise KeyError(key)

		__getattr__ = __getitem__

	def __setitem__(self, key, val):
		"""Allows access to the variables via obj[name] syntax.

		>>> h = Holder()
		>>> h["foo"] = "Hello"
		>>> print h.foo
		Hello
		"""
		self._accesses[key] = True
		self.__dict__[key] = val

	def inheritFrom(self, holder):
		"""
		This allows on Holder to inherit settings from another holder.

		>>> h1 = Holder(a=1, b=2)
		>>> h2 = Holder(c=3)
		>>> h2.inheritFrom(h1)
		>>> print h2.c
		3
		>>> print h2["b"]
		2
		>>> print h2.a
		1
		"""
		self._inherit_from.append(holder)

	def __repr__(self):
		return "<%s object '%s': %s>" % (self.__class__.__name__, self.name, ",".join(self.keys()))

	def accesses(self):
		return self._accesses.keys()

# Place-Holger, will be populated with a Holder() object:
data = {}
cont = {}
model = {}
view = {}
dia = {}


##############################################################




def get(dict, var, default=None):
	"""Returns @dict[@var]" if it exists, otherwise returns the default."""

	if dict.has_key(var):
		return dict[var]
	else:
		return default


def appendCode(dest, indent, text):
	"""Adds a pice of source code from @text into @dest. The code
	get's formatted somewhat, e.g. @indent is added before each
	line."""

	dest.append("%s// START custom code" % indent)
	for s in text.rstrip().split("\n"):
		dest.append("%s%s" % (indent, s))
	dest.append("%s// END custom code" % indent)


def addInclude(arr, header, local=False):
	"""Adds an '#include <>' line to @arr, but only if it doesn't yet
	exist there."""

	if local:
		header = "#include \"%s\"" % header
	else:
		header = "#include <%s>" % header
	if not header in arr:
		arr.append(header)


def addClassRef(clazz):
	"""Adds an 'class @clazz;' line to h_classes, but only if it
	doesn't yet exist there."""

	global h_classes
	header = "class %s;" % clazz
	if not header in h_classes:
		h_classes.append(header)


def generateStruct():
	"Creates the struct that contains the data fields"

	global h_struct
	desc = data.get("desc")
	if not desc is None:
		h_struct.append("/*!")
		h_struct.append(" * %s" % desc)
		h_struct.append(" */")
	h_struct.append("class %s {" % data.name)
	h_struct.append("public:")
	global h_include
	for field in data.fields:
		try:
			desc = get(field, "desc")
			if not desc is None:
				h_struct.append("\t//! %s" % desc)
			typ = field["type"]
			if typ[0] == "Q":
				addInclude(h_include, typ)
			h_struct.append("\t%s %s;" % (typ, field["name"]))
		except KeyError:
			pass
	h_struct.append("};")


def generateContainer(options):
	"Creates the container (usually QList) that contains the structs."

	global h_include
	addInclude(h_include, cont["type"])
	global h_container
	h_container.append("/*!")
	desc = cont.get("desc")
	if not desc is None:
		h_container.append(" * %s" % desc)
	else:
		h_container.append(" * A container for %s records." % data.name)
	h_container.append(" *")
	if options.save and cont.get("save"):
		h_container.append(" * \sa save%s()" % cont["name"].capitalize() )
	if options.load and cont.get("load"):
		h_container.append(" * \sa load%s()" % cont["name"].capitalize() )
	if not dia is None:
		h_container.append(" * \sa %s" % dia["name"] )
	if not view is None:
		h_container.append(" * \sa %s" % view["name"] )
	h_container.append(" */")
	h_container.append("extern %s<%s> %s;\n" % (cont["type"], data.name, cont["name"]) )
	global c_container
	c_container.append("%s<%s> %s;\n" % ( cont["type"], data.name, cont["name"]) )
	if options.save and cont.get("save"):
		generateContainerSave()
	if options.load and cont.get("load"):
		generateContainerLoad()


def generateContainerSave():
	name = cont["name"].capitalize()
	global h_include
	addInclude(h_include, "QString")
	global c_include
	addInclude(c_include, "QFile")
	addInclude(c_include, "QTextStream")
	global h_container
	h_container.append("bool save%s(const QString &fname);" % name)
	global c_container
	c_container.append("")
	c_container.append("/*!")
	c_container.append(" * Stores ::%s into a file as comma-seperated values." % cont["name"])
	c_container.append(" *")
	c_container.append(" * \\param fname Name of file to write")
	c_container.append(" *")
	c_container.append(" * \\return \\a true if file was created, \\a false otherwise.")
	c_container.append(" */")
	c_container.append("bool save%s(const QString &fname)" % name)
	c_container.append("{")
	c_container.append("\tQFile file(fname);")
	c_container.append("\tif (! file.open(QIODevice::WriteOnly | QIODevice::Text))")
	c_container.append("\t\treturn false;")
	c_container.append("\tQTextStream f(&file);")
	c_container.append("\tforeach (%s m, %s) {" % (data.name, cont["name"]))
	n = 0
	for field in data.fields:
		if not get(field, "save", True):
			continue
		typ = get(field, "type")
		if not typ:
			continue
		if n:
			comma = "\t\t  << \", \" "
		else:
			comma = "\t\tf "
		save = ["<< m.%s" % field["name"],]
		try:
			save.insert(0, types[typ]["preSave"] )
		except:
			pass
		try:
			save.append( types[typ]["postSave"] )
		except:
			pass
		c_container.append("%s%s" % (comma, "".join(save)))
		n += 1
	c_container[-1] = "%s << \"\\n\";" % c_container[-1]
	c_container.append("\t}")
	c_container.append("\tfile.close();")
	c_container.append("\treturn true;")
	c_container.append("}")
	c_container.append("")


def generateContainerLoad():
	code = data.code
	name = cont["name"].capitalize()
	global c_include
	addInclude(c_include, "ParseCSV.h", local=True)
	global h_container
	h_container.append("bool load%s(const QString &fname);" % name)
	global c_container

	c_container.append("")
	c_container.append("/*!")
	c_container.append(" * A parser to parse CSV-files (comma seperated values) with %s data." % data.name)
	c_container.append(" * \\note This class has been auto-generated from \\a %s." % os.path.basename(args[0]))
	c_container.append(" * \sa load%s()" % cont["name"].capitalize() )
	c_container.append(" */")
	c_container.append("class %sParser : public ParseCSV" % name);
	c_container.append("{");
	c_container.append("public:");
	c_container.append("\t/*!")
	c_container.append("\t * Constructor")
	c_container.append("\t * \param fname Name of file to parse.")
	c_container.append("\t */")
	c_container.append("\t%sParser(const QString &fname) : ParseCSV(fname) {};" % name);
	c_container.append("\tvirtual void setData(int field, const QString &item);");
	c_container.append("\tvirtual void saveRecord();");
	c_container.append("");
	c_container.append("/*!")
	c_container.append(" * Record of type %s." % data.name)
	c_container.append(" *")
	c_container.append(" * This is used as an intermediate storage place while parse() parses a")
	c_container.append(" * line from the CSV file.")
	c_container.append(" */")
	c_container.append("\t%s m;" % data.name);
	c_container.append("};");
	c_container.append("");
	c_container.append("/*!")
	c_container.append(" * Called by parse() for every data item read from the CSV file.")
	c_container.append(" *")
	c_container.append(" * This implementation stores every item into the proper \\ref m field.")
	c_container.append(" *")
	c_container.append(" * \\param field Indicator of the field inside \\ref m.")
	c_container.append(" *")
	c_container.append(" * \\param item Data \\a item to be stored. Will be automatically converted to the")
	c_container.append(" * target type in %s." % data.name)
	c_container.append(" */")
	c_container.append("void %sParser::setData(int field, const QString &item)" %name);
	c_container.append("{");
	c_container.append("\tswitch (field) {");
	n = 0
	for field in data.fields:
		if not get(field, "save", True):
			continue
		typ = get(field, "type")
		if not typ:
			continue
		load = types[typ]["load"]
		c_container.append("\tcase %d: m.%s = item%s; break;" % (n, field["name"], load));
		n += 1
	c_container.append("\t}");
	c_container.append("}");
	c_container.append("");
	c_container.append("/*!")
	c_container.append(" * Called by parse() for every finshed line.")
	c_container.append(" *")
	c_container.append(" * This implementation appends \\ref m to ::%s." % cont["name"])
	c_container.append(" */")
	c_container.append("void %sParser::saveRecord()" % name);
	c_container.append("{");
	if cont.has_key("load_code"):
		appendCode(c_container, "\t", code[cont["load_code"]])

	c_container.append("\t%s.append(m);" % cont["name"]);
	c_container.append("}");
	c_container.append("");
	c_container.append("");
	c_container.append("/*!")
	c_container.append(" * Parses the specified filed and stores it's contents in ::%s." % cont["name"])
	c_container.append(" *")
	c_container.append(" * \\param fname Name of file to parse.")
	c_container.append(" * \\return The result of %sParser::parse(), that is \\a true if" % name)
	c_container.append(" * the file was read successfully, \\a false otherwise.")
	c_container.append(" */")
	c_container.append("bool load%s(const QString &fname)" % name)
	c_container.append("{");
	c_container.append("\t%sParser parser(fname);" % name);
	c_container.append("\treturn parser.parse();");
	c_container.append("}");
	c_container.append("")


def generateColumnConsts():
	"""Generates "const int COL_xxx = n;" constants for easier
	referencing to the columns."""

	global c_col
	global col_prefix
	global col_count
	col_prefix = "COL_%s" % model["name"].upper()
	col_count = 0
	for field in data.fields:
		if not field.has_key("head"):
			continue
		try:
			c_col.append("//! Integer representing column index of %s::%s in the model." % (data.name, field["name"]))
			c_col.append("const int %s_%s = %d;" % (col_prefix, field["name"].upper(), col_count))
			col_count += 1
		except KeyError:
			pass
	c_col.append("//! Last column index of the model.")
	c_col.append("const int %s_LAST = %d;" % (col_prefix, col_count-1))
	c_col.append("\n")


def generateModelContructor():
	"Creates the (boring) constructor of the model."

	code = data.code
	global h_model
	h_model.append("\t%s(QObject *parent=0);" % model["name"])
	h_model.append("")
	global c_model
	c_model.append("/*!")
	c_model.append(" * Constructor.")
	c_model.append(" * \\param parent Parent object.")
	c_model.append(" */")
	c_model.append("%s::%s(QObject *parent)" % (model["name"], model["name"]) )
	c_model.append("\t: %s(parent)" % model["type"])
	c_model.append("{")
	name = "construct" + model["name"]
	if code.has_key(name):
		appendCode(c_model, "\t", code[name])
	c_model.append("}\n")


def generateModelRowCount():
	"""Creates an interface function that simply returns number of
	elements in the container."""

	global h_model
	h_model.append("\tint rowCount(const QModelIndex &parent = QModelIndex()) const;")
	global c_model
	c_model.append("/*!")
	c_model.append(" * Returns the number of rows.")
	c_model.append(" * \\param parent From where the rows should be counted.")
	c_model.append(" * \\return Number of entries in ::%s if parent isn't invalid, else 0." % cont["name"])
	c_model.append(" */")
	c_model.append("int %s::rowCount(const QModelIndex &parent) const" % model["name"])
	c_model.append("{")
	c_model.append("\tif (parent.isValid())")
	c_model.append("\t\treturn 0;\n")
	c_model.append("\treturn %s.count();" % cont["name"])
	c_model.append("}\n")


def generateModelColumnCount():
	"""Creates a (boring) function that simply return the number of
	columns. We can use our COL_xxx constant from above."""

	global h_model
	h_model.append("\tint columnCount(const QModelIndex &parent = QModelIndex() ) const;")
	global c_model
	c_model.append("/*!")
	c_model.append(" * Returns the number of columns.")
	c_model.append(" * \\param parent From where the columns should be counted.")
	c_model.append(" * \\return Number of columns in this model, always")
	c_model.append(" * ::%s_LAST + 1" % col_prefix)
	c_model.append(" */")
	c_model.append("int %s::columnCount(const QModelIndex &parent) const" % model["name"])
	c_model.append("{")
	c_model.append("\tQ_UNUSED(parent);\n")
	c_model.append("\treturn %s_LAST + 1;" % col_prefix)
	c_model.append("}\n")


def generateModelHeaderData():
	"""Create the ::headerData() method, which returns the row
	headers. Any field that has both a 'head' and 'name' entry is
	eligible for display."""

	global h_model
	h_model.append("")
	h_model.append("\tQVariant headerData(int section, Qt::Orientation orientation,")
	h_model.append("\t                    int role = Qt::DisplayRole) const;")
	global c_model
	c_model.append("/*!")
	c_model.append(" * Return text for column header.");
	c_model.append(" * \\param section Section (column) index.")
	c_model.append(" * \\param orientation If data should be returned for the vertical or horizontal header.")
	c_model.append(" * \\param role role (kind of data) that should be returned.")
	c_model.append(" * \\return Header text for each colum when role == Qt::DisplayRole and")
	c_model.append(" * orientation orientation != Qt::Vertical. Else an empty QVariant().")
	c_model.append(" */")
	c_model.append("QVariant %s::headerData(int section, Qt::Orientation orientation, int role) const" % model["name"])
	c_model.append("{")
	c_model.append("\tif (role != Qt::DisplayRole)")
	c_model.append("\t\treturn QVariant();")
	c_model.append("")
	c_model.append("\tif (orientation == Qt::Vertical) {")
	rowtext = model.get("rowtext", "Row %1")
	if rowtext.find("%") != -1:
		rowargs = ".arg(section)"
	else:
		rowargs = ""
	c_model.append("\t\treturn tr(\"%s\")%s;" % (rowtext, rowargs))
	c_model.append("\t} else {")
	c_model.append("\t\tswitch (section) {")
	for field in data.fields:
		if field.has_key("head") and field.has_key("name"):
			c_model.append("\t\tcase %s_%s:" % (col_prefix, field["name"].upper() ))
			c_model.append("\t\t\treturn tr(\"%s\");" % field["head"].replace("&", "") )
	c_model.append("\t\t}")
	c_model.append("\t}")
	c_model.append("\treturn QVariant();")
	c_model.append("}\n")


def generateModelData():
	"""Create the ::data() method, which returns the current data item. Can produce calculated
	results (in the presence of a 'data_code' field entry) or checkboxes."""

	code = data.code
	global h_model
	h_model.append("\tQVariant data(const QModelIndex &index, int role) const;")
	global c_model
	c_model.append("/*!")
	c_model.append(" * Return text for cell data.");
	c_model.append(" * \\param index Index to the cell for which text should be returned.")
	c_model.append(" * \\param role role (kind of data) that should be returned.")
	c_model.append(" * \\return Contents of the matching ::%s record if role is" % cont["name"])
	c_model.append(" * Qt::DisplayRole or Qt::EditRole. Alignment if role")
	c_model.append(" * Qt::TextAlignmentRole for those fields that have an alignment defined.")
	c_model.append(" * An empty QVariant otherwise.")
	c_model.append(" */")
	c_model.append("QVariant %s::data(const QModelIndex &index, int role) const" % model["name"])
	c_model.append("{")
	c_model.append("\tint row = index.row();")
	c_model.append("\tif (row < 0 || row >= %s.count())" % cont["name"])
	c_model.append("\t\treturn QVariant();\n")
	c_model.append("\tif (role == Qt::DisplayRole || role == Qt::EditRole) {")
	c_model.append("\t\tswitch (index.column()) {")
	col_checkbox = False
	col_align = False
	for field in data.fields:
		if not field.has_key("head"):
			continue
		if field.has_key("halign") or field.has_key("valign"):
			col_align = True
		if get(field, "table_type") == "checkbox":
			col_checkbox = True
			continue
		else:
			c_model.append("\t\tcase %s_%s:" % (col_prefix, field["name"].upper()) )
		if field.has_key("data_code"):
			c_model.append("\t\t\t{")
			appendCode(c_model, "\t\t\t", code[field["data_code"]])
			c_model.append("\t\t\t}")
		elif field.has_key("type"):
			c_model.append("\t\t\treturn %s[row].%s;" % (
					cont["name"],
					field["name"]))
		else:
			c_model.append("\t\t\treturn QVariant(); //TODO")
	c_model.append("\t\t}")
	if col_checkbox:
		c_model.append("\t} else")
		c_model.append("\tif (role == Qt::CheckStateRole) {")
		c_model.append("\t\tswitch (index.column()) {")
		for field in data.fields:
			if get(field, "table_type") != "checkbox":
				continue
			c_model.append("\t\tcase %s_%s: {" % (col_prefix, field["name"].upper()) )
			c_model.append("\t\t\tbool checked = %s[row].%s;" % (cont["name"], field["name"]))
			c_model.append("\t\t\treturn checked ? Qt::Checked : Qt::Unchecked;")
			c_model.append("\t\t\t}")
		c_model.append("\t\t}")
	if col_align:
		c_model.append("\t} else")
		c_model.append("\tif (role == Qt::TextAlignmentRole) {")
		c_model.append("\t\tswitch (index.column()) {")
		for field in data.fields:
			halign = get(field, "halign", "left").capitalize()
			valign = get(field, "valign", "vcenter").capitalize()
			if halign == "Left" and valign == "Vcenter":
				continue
			if valign == "Vcenter": valign = "VCenter"
			c_model.append("\t\tcase %s_%s:" % (col_prefix, field["name"].upper()) )
			c_model.append("\t\t\treturn (int)(Qt::Align%s | Qt::Align%s);" % (halign.capitalize(), valign))
		c_model.append("\t\t}")
	c_model.append("\t}")
	c_model.append("")
	c_model.append("\treturn QVariant();")
	c_model.append("}\n")


def generateModelSort():
	"""Sorting colums is most tedious when written by hand. This is
	where this generater excells! Can use code for custom sorting
	('sort_code') or a different default sort order ('sort_order)."""

	code = data.code
	global h_model
	h_model.append("")
	h_model.append("\t// Sort support:")
	h_model.append("\tvirtual void sort(int column, Qt::SortOrder order=Qt::AscendingOrder);")

	global c_model

	# First the static sort functions
	for field in data.fields:
		if not field.has_key("head"):
			continue
		c_model.append("/*!")
		c_model.append(" * Helper function for %s::sort()." % model["name"])
		c_model.append(" * \\param one First %s to compare." % field["name"].capitalize())
		c_model.append(" * \\param two Second %s to compare." % field["name"].capitalize())
		c_model.append(" * \\return \\a true or \\a false, depending on the two values and sortOrder.")
		c_model.append(" */")
		c_model.append("static bool sort%s%s(const %s &one, const %s &two)" % (
				model["name"], field["name"].capitalize(),
				name, name))
		c_model.append("{")
		order = get(field, "sort_order", "ascending")
		if order == "ascending":
			order = "Qt::AscendingOrder"
		elif order == "descending":
			order = "Qt::DescendingOrder";
		s1 = "one.%s" % field["name"]
		s2 = "two.%s" % field["name"]
		if field.has_key("sort_code"):
			s1 = "s1"
			s2 = "s2"
			appendCode(c_model, "\t", code[field["sort_code"]])
		c_model.append("\tif (sortOrder == %s)" % order)
		c_model.append("\t\treturn %s < %s;" % (s1, s2))
		c_model.append("\telse")
		c_model.append("\t\treturn %s > %s;" % (s1, s2))
		c_model.append("}\n")

	# and then the class member:
	c_model.append("/*!")
	c_model.append(" * Sorts the model.")
	c_model.append(" * \\param column Column to sort.")
	c_model.append(" * \\param order Order (ascending, descending to sort.")
	for field in data.fields:
		if not field.has_key("head"):
			continue
		c_model.append(" * \\sa sort%s%s" % (model["name"], field["name"].capitalize()))
	c_model.append(" */")
	c_model.append("void %s::sort(int column, Qt::SortOrder order)" % model["name"])
	c_model.append("{")
	c_model.append("\tsortOrder = order;")
	c_model.append("")
	c_model.append("\tswitch (column) {")
	for field in data.fields:
		if not field.has_key("head"):
			continue
		c_model.append("\tcase %s_%s:" % (col_prefix, field["name"].upper()) )
		c_model.append("\t\tqStableSort(%s.begin(), %s.end(), sort%s%s);" % (
				cont["name"], cont["name"],
				model["name"], field["name"].capitalize(),
				))
		c_model.append("\t\tbreak;")
	c_model.append("\tdefault:")
	c_model.append("\t\treturn;")
	c_model.append("\t}")
	c_model.append("\temit dataChanged(index(0, 0), index(%s.count(), %s_LAST));" % (cont["name"], col_prefix))
	c_model.append("}\n")


def generateModelEdit():
	"""This calls generators for all functions that are needed for in-place edit."""

	global h_model
	h_model.append("")
	h_model.append("\t// In-Table edit support:")
	generateModelFlags()
	generateModelSetData()


def generateModelFlags():
	"""This return the Qt::ItemFlags for a field. The .yaml can define
	a field as 'readonly', set the 'table_type' or 'checkbox'."""

	global h_model
	h_model.append("\tQt::ItemFlags flags(const QModelIndex &index) const;");
	global c_model
	c_model.append("/*!")
	c_model.append(" * Returns the item \\a flags for the given \\a index.")
	c_model.append(" * \\param index Index to the cell for which flags should be returned.")
	c_model.append(" * \\return Proper Qt::ItemFlags based on the field definition.");
	c_model.append(" */")
	c_model.append("Qt::ItemFlags %s::flags(const QModelIndex &index) const" % model["name"])
	c_model.append("{")
	c_model.append("\tQt::ItemFlags flags = QAbstractItemModel::flags(index);")
	c_model.append("\t//flags |= Qt::ItemIsEnabled;\n")
	c_model.append("\tif (!index.isValid())")
	# c_model.append("\t\treturn Qt::ItemIsEnabled;")
	c_model.append("\t\treturn flags;\n")
	c_model.append("\tswitch (index.column()) {")
	for field in data.fields:
		if not field.has_key("head") or get(field, "readonly"):
			continue
		c_model.append("\tcase %s_%s:" % (col_prefix, field["name"].upper()) )
		if get(field, "table_type") == "checkbox":
			c_model.append("\t\tflags |= Qt::ItemIsUserCheckable;")
		c_model.append("\t\tflags |= Qt::ItemIsEditable;")
		c_model.append("\t\tbreak;")
	c_model.append("\t}")
	c_model.append("\treturn flags;")
	c_model.append("}\n")


def generateModelSetData():
	"""This genererates the ::setData() method, which takes the
	in-table edited user input and updates the structure in the
	container."""

	global h_model
	h_model.append("\tbool setData(const QModelIndex &index, const QVariant &value,")
	h_model.append("\t             int role = Qt::EditRole);");
	global c_model
	c_model.append("/*!")
	c_model.append(" * Stores data into the model (or it's underlying container).")
	c_model.append(" * \\param index \\a index to the cell whose data should be changed.")
	c_model.append(" * \\param value New \\a value for this cell.")
	c_model.append(" * \\param role The role for which the data should be changed, normally Qt::EditRole.")
	c_model.append(" * \\return Returns \\a true if successful; otherwise \\a returns false.")
	c_model.append(" */")
	c_model.append("bool %s::setData(const QModelIndex &index, const QVariant &value," % model["name"])
	c_model.append("\tint role)")
	c_model.append("{")
	c_model.append("\tint row = index.row();")
	c_model.append("\tif (row < 0 || row >= %s.count())" % cont["name"])
	c_model.append("\t\treturn false;\n")
	c_model.append("\tif (role == Qt::EditRole || role == Qt::DisplayRole) {")
	c_model.append("\t\tswitch (index.column()) {")
	has_checkbox = False
	for field in data.fields:
		if not field.has_key("head") or get(field, "readonly"):
			continue
		if get(field, "table_type") == "checkbox":
			has_checkbox = True
			continue
		c_model.append("\t\tcase %s_%s:" % (col_prefix, field["name"].upper()) )
		try:
			conv = types[field["type"]]["fromVariant"]
		except:
			conv = types[field["type"]]["load"]
		c_model.append("\t\t\t%s[row].%s = value%s;" % (
				cont["name"],
				field["name"],
				conv,
				))
		c_model.append("\t\t\tbreak;")
	c_model.append("\t\tdefault:")
	c_model.append("\t\t\treturn false;")
	c_model.append("\t\t}")
	c_model.append("\t\temit dataChanged(index, index);")
	c_model.append("\t\treturn true;")
	c_model.append("\t}")
	if has_checkbox:
		c_model.append("\tif (role == Qt::CheckStateRole) {")
		for field in data.fields:
			if not field.has_key("head") or get(field, "readonly"):
				continue
			if get(field, "table_type") != "checkbox":
				continue
			c_model.append("\t\tswitch (index.column()) {")
			c_model.append("\t\tcase %s_%s:" % (col_prefix, field["name"].upper()) )
			c_model.append("\t\t\t%s[row].enabled = value.toBool();" % cont["name"])
			c_model.append("\t\t\temit dataChanged(index, index);")
			c_model.append("\t\t\treturn true;")
		c_model.append("\t\t}")
		c_model.append("\t}")
	c_model.append("\treturn false;")
	c_model.append("}\n")


def generateModelInsertRows():
	global h_model
	h_model.append("\tvirtual bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());")
	global c_model

	c_model.append("/*!")
	c_model.append(" * Inserts rows into the model.")
	c_model.append(" * \\param row New rows will be inserted before this \\a row.")
	c_model.append(" * \\param count Number of new rows to insert.");
	c_model.append(" * \\param parent Points to some model cell.");
	c_model.append(" * \\return Returns \\a true if the rows could have been inserted; otherwise returns \\a false.")
	c_model.append(" */")
	c_model.append("bool %s::insertRows(int row, int count, const QModelIndex &parent)" % model["name"]);
	c_model.append("{")
	c_model.append("\tif (count < 1 || row < 0 || row > rowCount(parent))")
	c_model.append("\t\treturn false;\n")
	c_model.append("\tbeginInsertRows(QModelIndex(), row, row + count - 1);");
	c_model.append("\t%s m;" % data.name)
	c_model.append("\tfor(int r = 0; r < count; ++r)")
	c_model.append("\t\t%s.insert(row, m);" % cont["name"])
	c_model.append("\tendInsertRows();");
	c_model.append("\treturn true;\n")
	c_model.append("}\n")


def generateModelRemoveRows():
	global h_model
	h_model.append("\tvirtual bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());")
	global c_model
	c_model.append("/*!")
	c_model.append(" * Removes rows from the model.")
	c_model.append(" * \\param row Rows will be deleted starting from this \\a row.")
	c_model.append(" * \\param count Number of new rows to delete.");
	c_model.append(" * \\param parent Points to some model cell.");
	c_model.append(" * \\return Returns \\a true if the rows could be deleted; otherwise \a returns false.")
	c_model.append(" */")
	c_model.append("bool %s::removeRows(int row, int count, const QModelIndex &parent)" % model["name"]);
	c_model.append("{")
	c_model.append("\tif (count <= 0 || row < 0 || (row + count) > rowCount(parent))")
	c_model.append("\t\treturn false;\n")
	c_model.append("\tbeginRemoveRows(QModelIndex(), row, row + count - 1);");
	c_model.append("\tfor(int r = 0; r < count; ++r)")
	c_model.append("\t\t%s.removeAt(row);" % cont["name"])
	c_model.append("\tendRemoveRows();");
	c_model.append("\treturn true;\n")
	c_model.append("}")


def generateModel():
	"""This is the high-level function which generates the whole model."""

	if not model["type"] in ("QAbstractTableModel",):
		raise MyException("cannot handle model type %s" % model["type"])
	global h_include
	addInclude(h_include, model["type"])

	global h_model
	h_model.append("/*!")
	h_model.append(" * A %s to view and edit the contents of \\ref %s "
	               % (model["type"], cont["name"]))
	h_model.append(" * \\note This class has been auto-generated from \\a %s." % os.path.basename(args[0]))
	h_model.append(" */")
	h_model.append("class %s : public %s" % (model["name"], model["type"]) )
	h_model.append("{")
	h_model.append("public:")

	generateColumnConsts()
	generateModelContructor()
	generateModelRowCount()
	generateModelColumnCount()
	generateModelHeaderData()
	generateModelData()
	if model.get("sort", True):
		global has_sort
		has_sort = True
		generateModelSort()
	if model.get("edit_table", True):
		generateModelEdit()
	if view.get("insert"):
		generateModelInsertRows()
	if view.get("delete"):
		generateModelRemoveRows()
	h_model.append("};")


def generateViewWidthSupport():
	code = data.code
	global h_view
	h_view.append("\tvirtual void setModel(QAbstractItemModel *model);")
	global c_view
	c_view.append("\n")

	c_view.append("/*!")
	c_view.append(" * Sets the \\a model for the view to present.")
	c_view.append(" * This calls QTableView::setModel() and then it resizes and moves the")
	c_view.append(" * columns around. This cannot be done in the \\ref")
	c_view.append(" * %s::%s \"constructor\", because at construction" % (view["name"], view["name"]))
	c_view.append(" * time there aren't any columns defined. That would make calls to")
	c_view.append(" * setColumnWidth() etc. no-ops.")
	c_view.append(" * \\param model Pointer to the model that should be used.")
	c_view.append(" */")
	c_view.append("void %s::setModel(QAbstractItemModel *model)" % view["name"]);
	c_view.append("{")
	c_view.append("\tQTableView::setModel(model);")
	n = 0
	for field in data.fields:
		if field.has_key("width"):
			c_view.append("\tsetColumnWidth(%d, %d);" % (n, field["width"]))
		n += 1
	if view.has_key("setmodel_code"):
		appendCode(c_view, "\t", code[view["setmodel_code"]])
	c_view.append("}")


def generateView():
	"""This creates a simple view."""

	code = data.code
	global h_include
	addInclude(h_include, view["type"])
	global h_view
	h_view.append("/*!")
	h_view.append(" * A %s to view and edit the contents of \\ref %s "
	               % (view["type"], cont["name"]))
	h_view.append(" * \\note This class has been auto-generated from \\a %s." % os.path.basename(args[0]))
	h_view.append("*/")
	h_view.append("class %s : public %s" % (view["name"], view["type"]))
	h_view.append("{")
	h_view.append("\tQ_OBJECT")
	h_view.append("public:")
	h_view.append("\t%s(QWidget *parent=0);" % view["name"])

	global c_include
	addInclude(c_include, "QHeaderView")

	global c_view
	c_view.append("/*!")
	c_view.append(" * Constructor")
	c_view.append(" *")
	c_view.append(" * \\param parent Parent widget.")
	c_view.append(" */")
	c_view.append("%s::%s(QWidget *parent)" % (view["name"],view["name"]))
	c_view.append("\t: %s(parent)" % view["type"])
	c_view.append("{")
	if view.has_key("init_code"):
		appendCode(c_view, "\t", code[view["init_code"]]);
		c_view.append("")
	if view.get("sort", True):
		c_view.append("\t// sort support")
		c_view.append("\tconnect(horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),")
		c_view.append("\t        SLOT(sortByColumn(int)) );")
		c_view.append("\tsetSortingEnabled(true);")
		c_view.append("\tsortByColumn(0, Qt::AscendingOrder);")
		c_view.append("\tsetEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed);")
		c_view.append("")
	if data.has_key("dialog"):
		c_view.append("\t// edit support")
		c_view.append("\tconnect(this, SIGNAL(activated(const QModelIndex &)), SLOT(slotEdit(const QModelIndex &)) );")
		c_view.append("")
	c_view.append("}\n")

	if view.get("delete") or view.get("insert"):
		if data.has_key("container"):
			generateViewInsertDelete()
		else:
			print "Warning: cannot generate view inser/delete function without container"

	if data.has_key("dialog"):
		if data.has_key("container"):
			generateViewSlotEdit()
		else:
			print "Warning: cannot generate edit slot in view without container"

	setmodel_code = False
	for field in data.fields:
		if field.has_key("width"):
			setmodel_code = True
			break
	if setmodel_code:
		generateViewWidthSupport()

	h_view.append("};")


def generateViewSlotEdit():
	"""Adds code to handle the call of the editor dialog due after a
	double-click."""

	if view.has_key("dialog_include"):
		global c_include
		addInclude(c_include, view["dialog_include"], local=True)

	global h_view
	h_view.append("public slots:")
	h_view.append("\tvoid slotEdit(const QModelIndex &index);")

	global c_view
	c_view.append("/*!")
	c_view.append(" * Edit current %s." % data.name)
	c_view.append(" *")
	c_view.append(" * This slot is connected to the signal activated(). It uses")
	c_view.append(" * %s to let the user edit the current %s from" % (dia["name"], data.name))
	c_view.append(" * ::%s." % cont["name"])
	c_view.append(" * \\param index QModelIndex, which denotes the row.")
	c_view.append(" */")
	c_view.append("void %s::slotEdit(const QModelIndex &index)" % view["name"])
	c_view.append("{")
	c_view.append("\tint row = index.row();")
	c_view.append("\t%s m = %s[row];" % (data.name, cont["name"]))
	c_view.append("\tQDialog *dia = new %s(&m, this);" %
				  view.get("dialog_name", dia["name"]))
	c_view.append("\tif (dia->exec() == QDialog::Accepted) {")
	c_view.append("\t\t%s[row] = m;" % cont["name"])
	c_view.append("\t\tQModelIndex left = model()->index(row, 0);")
	c_view.append("\t\tQModelIndex right = model()->index(row, 99);")
	c_view.append("\t\tdataChanged(left, right);")
	c_view.append("\t}")
	c_view.append("\tdelete dia;")
	c_view.append("}")


def generateViewInsertDelete():
	global c_include
	addInclude(c_include, "QKeyEvent")

	if view.has_key("dialog_include"):
		addInclude(c_include, view["dialog_include"], local=True)

	global h_view
	h_view.append("public slots:")
	h_view.append("\tvirtual void keyPressEvent(QKeyEvent *event);")

	global c_view
	c_view.append("/*!")
	c_view.append(" * Reacts to insert and delete key press events.")
	c_view.append(" *")
	c_view.append(" * Insert is handled by %s and" % dia["name"])
	c_view.append(" * %s::insertRows()," % model["name"])
	c_view.append(" * Delete is handled by %s::removeRows()." % model["name"])
	c_view.append(" *")
	c_view.append(" * \\param event Pointer to a QKeyEvent.")
	c_view.append(" */")
	c_view.append("void %s::keyPressEvent(QKeyEvent *event)" % view["name"])
	c_view.append("{")
	c_view.append("\tswitch (event->key()) {")
	if view.get("insert"):
		if dia:
			c_view.append("\tcase Qt::Key_Insert: {")
			c_view.append("\t\t%s m;" % name)
			# Initialize field. A simple memset() doesn't work because of possible QStrings
			for field in data.fields:
				typ = get(field, "type")
				if not typ:
					continue
				default_code = get(field, "default_code")
				default = get(field, "default")
				if default_code:
					default = default_code
				elif default:
					if typ == "QString":
						default = "\"%s\"" % default
				else:
					default = types[typ]["default"]
				if default:
					c_view.append("\t\tm.%s = %s;" % (field["name"], default) )
			c_view.append("\t\tQDialog *dia = new %s(&m, this);" % #dia["name"])
						  view.get("dialog_name", dia["name"]))
			c_view.append("\t\tif (dia->exec() == QDialog::Accepted) {")
			c_view.append("\t\t\tint row = 0;")
			c_view.append("\t\t\tif (currentIndex().isValid())")
			c_view.append("\t\t\t\trow = currentIndex().row();")
			c_view.append("\t\t\tif (model()->insertRow(row))")
			c_view.append("\t\t\t\tsetCurrentIndex(model()->index(row+1, 0));");
			c_view.append("\t\t\t%s[row] = m;" % cont["name"])
			c_view.append("\t\t}")
			c_view.append("\t\tdelete dia;")
			c_view.append("\t\treturn;")
			c_view.append("\t\t}")
		else:
			print "Warning: cannot generate insert view without dialog"
	if view.get("delete"):
		c_view.append("\tcase Qt::Key_Delete:")
		c_view.append("\t\tif (!currentIndex().isValid())")
		c_view.append("\t\t\treturn;")
		# TODO: ask for confirmation?
		# TODO: check selection
		c_view.append("\t\tmodel()->removeRows(currentIndex().row(), 1);")
		c_view.append("\t\treturn;")
	c_view.append("\t}");
	c_view.append("\tQAbstractItemView::keyPressEvent(event);")
	c_view.append("}\n")


def generateDialog():
	"""This hefty generator function generates a QDialog-derived class
	to out-of-table editing of data records."""

	global c_include

	title = dia.get("head")
	if title:
		title = "tr(\"%s\")" % title
	else:
		title = "QString::null"

	global h_dialog;
	addInclude(h_include, "QDialog")
	h_dialog.append("/*!")
	h_dialog.append(" * Class to edit / change records of type %s." % data.name)
	h_dialog.append(" *")
	h_dialog.append(" * \\note This class has been auto-generated from \\a %s." % os.path.basename(args[0]))
	h_dialog.append(" */")
	h_dialog.append("class %s : public QDialog" % dia["name"])
	h_dialog.append("{")
	h_dialog.append("public:")
	h_dialog.append("\t%s(%s *record, QWidget *parent=0, Qt::WindowFlags f=0);" % (dia["name"], data.name))
	h_dialog.append("\tvirtual void accept();")
	h_dialog.append("")
	h_dialog.append("\t//! Pointer to a %s record." % data.name)
	h_dialog.append("\t%s *m;\n" % data.name)

	global c_dialog
	c_dialog.append("/*!")
	c_dialog.append(" * Constructor")
	c_dialog.append(" *")
	c_dialog.append(" * \\param record Pointer to the %s record that this dialog should edit." % data.name)
	c_dialog.append(" * \\param parent Parent widget.")
	c_dialog.append(" * \\param f (default 0) Window flags, if any.")
	c_dialog.append(" */")
	c_dialog.append("%s::%s(%s *record, QWidget *parent, Qt::WindowFlags f)"
					% (dia["name"], dia["name"], data.name))
	c_dialog.append("\t: QDialog(parent, f)")
	c_dialog.append("\t, m(record)")
	c_dialog.append("{")
	c_dialog.append("\tsetWindowTitle(%s);" % title)
	xsize = dia.get("xsize")
	ysize = dia.get("ysize")
	if xsize and ysize:
		c_dialog.append("\tresize(%s, %s);" % (xsize, ysize))
	c_dialog.append("")

	c_layout = []
	c_get	 = []
	c_set	 = []
	for field in data.fields:
		if not (field.has_key("type") and field.has_key("name") and field.has_key("head")):
			continue
		if get(field, "readonly"):
			continue
		name = "edit%s" % field["name"].capitalize()
		typ = get(field, "dia_type", field["type"])
		(typ, setter, getter) = types[typ]["widget"]
		if typ == "":
			print "Unknown type '%s' for dialog" % field["type"]
			continue
		if typ[0] == "Q":
			addInclude(c_include, typ)
			addClassRef(typ)
		# TODO: allow include of local header?
		h_dialog.append("\t//! Widget to edit m->%s" % field["name"])
		h_dialog.append("\t%s *%s;" % (typ, name) )

		c_dialog.append("\t%s = new %s(this);" % (name, typ))
		try:
			valid = types[field["type"]]["validator"]
			h_dialog.append("\t//! QValidator to make sure the user can't enter impossible degree values.")
			h_dialog.append("\t%s *%sValidator;" % (valid, name));
			c_dialog.append("\t%sValidator = new %s(%s);" % (name, valid, name))
			c_get.append("\tif (!%s->hasAcceptableInput()) {" % name);
			c_get.append("\t\t%s->setFocus(Qt::OtherFocusReason);" % name)
			c_get.append("\t\treturn;")
			c_get.append("\t}")
			min = get(field, "min", None)
			if not min is None:
				c_dialog.append("\t%sValidator->setBottom(%s);" % (name, min))
			max = get(field, "max", None)
			if not min is None:
				c_dialog.append("\t%sValidator->setTop(%s);" % (name, max))
			c_dialog.append("\t%s->setValidator(%sValidator);" % (name, name))
			addInclude(c_include, valid, false)
		except:
			pass
		if setter.find("%") != -1:
			setter = setter.replace("%", "m->%s" % field["name"])
		c_set.append("\t%s->%s;" % (name, setter))
		label = get(field, "dia_label", get(field, "head"))
		if label:
			label = "tr(\"%s\"), " % label
		c_layout.append("\tformLayout->addRow(%s%s);" % (label, name))

		c_get.append("\tm->%s = %s->%s;" % (field["name"], name, getter))

	h_dialog.append("")
	h_dialog.append("\t//! OK and CANCEL buttons.");
	h_dialog.append("\tQDialogButtonBox *okCancel;");
	h_dialog.append("\t//! QFormLayout to make all the widgets and labels align nicely.");
	h_dialog.append("\tQFormLayout *formLayout;")
	h_dialog.append("};")

	addClassRef("QFormLayout")
	addInclude(c_include, "QFormLayout")
	c_dialog.append("")
	c_dialog.append("\tformLayout = new QFormLayout;")
	c_dialog.extend(c_layout)
	c_dialog.append("")
	c_dialog.extend(c_set)
	c_dialog.append("")

	addClassRef("QDialogButtonBox")
	addInclude(c_include, "QDialogButtonBox")
	c_dialog.append("\tokCancel = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);")
	c_dialog.append("\tconnect(okCancel, SIGNAL(accepted()), this, SLOT(accept()));")
	c_dialog.append("\tconnect(okCancel, SIGNAL(rejected()), this, SLOT(reject()));")
	c_dialog.append("")
	c_dialog.append("\tQVBoxLayout *mainLayout = new QVBoxLayout(this);")
	c_dialog.append("\tmainLayout->addLayout(formLayout);")
	c_dialog.append("\tmainLayout->addWidget(okCancel);")
	c_dialog.append("\tsetLayout(mainLayout);")
	c_dialog.append("}\n")

	c_dialog.append("/*!")
	c_dialog.append(" * Stores the contents of the edit widgets into the record \\ref m.")
	c_dialog.append(" */")
	c_dialog.append("void %s::accept()" % dia["name"])
	c_dialog.append("{")
	c_dialog.extend(c_get)
	c_dialog.append("\tQDialog::accept();")
	c_dialog.append("}")



#
# Very simple option parsing
#
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-d", "--dir", dest="destdir", default=".",
	help="destination directory", metavar="DIR")
parser.add_option("--decl",
	action="store_true", dest="decl", default=False,
	help="generate declaration (*.h)")
parser.add_option("--impl",
	action="store_true", dest="impl", default=False,
	help="generate implementation (*.cpp)")
parser.add_option("--no-model",
	action="store_false", dest="model", default=True,
	help="don't generate a model")
parser.add_option("--no-view",
	action="store_false", dest="view", default=True,
	help="don't generate a view")
parser.add_option("--no-dialog",
	action="store_false", dest="dialog", default=True,
	help="don't generate an edit dialog")
parser.add_option("--no-load",
	action="store_false", dest="load", default=True,
	help="don't generate CSV loading code")
parser.add_option("--no-save",
	action="store_false", dest="save", default=True,
	help="don't generate CSV saving code")
parser.add_option("--debug",
	action="store_true", dest="debug", default=False,
	help="show which elements get accessed")
(options, args) = parser.parse_args()
options.both = not options.decl and not options.impl
if len(args) != 1:
	print "Usage: %s -o <output> <filename>.yaml" % sys.argv[0]


#
# Load the .yaml file into a python structures
#
stream = open(args[0])
alldata = yaml.load(stream, Loader=yaml.CLoader)


#
# Loop over all entries and create arrays of string in various h_* /
# c_* files (for header & source code code segments)
#
for name in alldata:
	data = Holder("data", alldata[name])
	data.name = name
	try:
		fields = data.fields
	except:
		print "Warning: can't do anything without fields"
		continue

	# convert fields to Holder instances
	n = 0
	newfields = []
	for f in fields:
		newfields.append( Holder(f["name"], f) )

	data.fields = newfields
	if not data.has_key("code"):
		data.code = {}
	cont = Holder("cont", data.container)
	model = Holder("model", data.model)
	view = Holder("view", data.get("view"))
	if options.dialog and data.has_key("dialog"):
		dia = Holder("dialog", data.get("dialog"))
	else:
		dia = None
	generateStruct()
	if data.has_key("container"):
		generateContainer(options)
	if options.model and data.has_key("model"):
		if not data.has_key("container"):
			print "Warning: cannot generate model without container"
		else:
			generateModel()
	if options.view and data.has_key("view"):
		#if not data.has_key("model"):
		#	 print "Warning: cannot generate view without model"
		generateView()
	if options.dialog and data.has_key("dialog"):
		generateDialog()
	if options.debug:
		print
		print "Element accesses"
		print "----------------"
		print "data accesses:"
		print "  ", ", ".join(sorted(data.accesses()))
		print "container accesses:"
		print "  ", ", ".join(sorted(cont.accesses()))
		print "model accesses:"
		print "  ", ", ".join(sorted(model.accesses()))
		print "view accesses:"
		print "  ", ", ".join(sorted(view.accesses()))
		if dia:
			print "dialog accesses:"
			print "  ", ", ".join(sorted(dia.accesses()))
		s  = set()
		for f in data.fields:
			s.update(f._accesses.keys())
		print "field accesses:"
		print "  ", ", ".join(sorted(s))


def writeArr(f, arr):
	if arr:
		f.write("\n\n")
		f.write("\n".join(arr))
		f.write("\n")


#
# Now write this into the two files
#
basename = os.path.splitext(os.path.basename(args[0]))[0]
if options.decl or options.both:
	f = open("%s/%s.h" % (options.destdir, basename), "w")
	f.write("#ifndef %s_H\n" % basename.upper())
	f.write("#define %s_H\n" % basename.upper())
	f.write("\n\n")
	f.write("/*!\n")
	f.write(" * \\file %s.h\n" % basename)
	f.write(" * This file has been auto-generated from \\a %s.yaml.\n" % basename)
	f.write(" * \ingroup Storage\n")
	f.write(" */\n")
	writeArr(f, h_include)
	writeArr(f, h_classes)
	writeArr(f, h_struct)
	writeArr(f, h_container)
	writeArr(f, h_model)
	writeArr(f, h_view)
	writeArr(f, h_dialog)
	f.write("\n\n")
	f.write("#endif\n")

if options.impl or options.both:
	f = open("%s/%s.cpp" % (options.destdir, basename), "w")
	f.write("/*!\n")
	f.write(" * \\file %s.cpp\n" % basename)
	f.write(" * This file has been auto-generated from \\a %s.yaml.\n" % basename)
	f.write(" * \ingroup Storage\n")
	f.write(" */\n\n")
	f.write("#include \"%s.h\"\n" % basename)
	writeArr(f, c_include)
	writeArr(f, c_container)
	writeArr(f, c_col)
	if has_sort:
		f.write("/*!\n")
		f.write(" * Current sort order.\n")
		f.write(" *\n")
		f.write(" * Can be changed by clicking on the column headers.\n")
		f.write(" */\n")
		f.write("static Qt::SortOrder sortOrder = Qt::AscendingOrder;\n")
	writeArr(f, c_model)
	writeArr(f, c_view)
	writeArr(f, c_dialog)
