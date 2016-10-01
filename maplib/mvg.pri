isEmpty(QMAKE_MVG) {
    MVG_BIN = $$TOPDIR/mvg.py
    QMAKE_MVG = python $$MVG_BIN
}

isEmpty(MVG_DIR):MVG_DIR = .
isEmpty(MVG_SOURCES_DIR):MVG_SOURCES_DIR = $$MVG_DIR
isEmpty(MVG_HEADERS_DIR):MVG_HEADERS_DIR = $$MVG_DIR

mvg_decl.commands = $$QMAKE_MVG --decl --dir $$MVG_HEADERS_DIR $$MVG_OPTIONS ${QMAKE_FILE_IN}
#mvg_decl.dependency_type = TYPE_UI
mvg_decl.output = $$MVG_HEADERS_DIR/${QMAKE_FILE_BASE}$${first(QMAKE_EXT_H)}
mvg_decl.depends = $$MVG_BIN
mvg_decl.input = MVG_YAML
mvg_decl.CONFIG += no_link target_predeps
mvg_decl.variable_out = GENERATED_FILES MVG_HEADERS
mvg_decl.name = MVG Decl ${QMAKE_FILE_IN}
silent:mvg_decl.commands = @echo mvg header ${QMAKE_FILE_IN} && $$mvg_decl.commands
QMAKE_EXTRA_COMPILERS += mvg_decl

mvg_impl.commands = $$QMAKE_MVG --impl --dir $$MVG_HEADERS_DIR $$MVG_OPTIONS ${QMAKE_FILE_IN}
#mvg_impl.dependency_type = TYPE_UI
mvg_impl.output = $$MVG_SOURCES_DIR/${QMAKE_FILE_BASE}$${first(QMAKE_EXT_CPP)}
mvg_impl.depends = $$MVG_HEADERS_DIR/${QMAKE_FILE_BASE}$${first(QMAKE_EXT_H)} $$MVG_BIN
mvg_impl.input = MVG_YAML
mvg_impl.variable_out = GENERATED_SOURCES
mvg_impl.name = MVG Impl ${QMAKE_FILE_IN}
silent:mvg_impl.commands = @echo mvg source ${QMAKE_FILE_IN} && $$mvg_impl.commands
QMAKE_EXTRA_COMPILERS += mvg_impl

load(moc)
mvg_moc.commands = $$moc_header.commands -nw
mvg_moc.output = $$moc_header.output
mvg_moc.depends = $$MVG_HEADERS_DIR/${QMAKE_FILE_BASE}$${first(QMAKE_EXT_H)}
mvg_moc.input = MVG_HEADERS
mvg_moc.variable_out = GENERATED_SOURCES
mvg_moc.name = $$moc_header.name
QMAKE_EXTRA_COMPILERS += mvg_moc
