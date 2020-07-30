
defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

!minQtVersion(5, 15, 0) {
    message("Cannot build Stock with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.15.0.")
}

TEMPLATE = subdirs

SUBDIRS = stock \
	3rdparty/cfgfile/generator

stock.depends = 3rdparty/cfgfile/generator

OTHER_FILES += README.md \
	LICENSE
