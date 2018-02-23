
TEMPLATE = subdirs

SUBDIRS = stock \
	3rdparty/cfgfile/generator

stock.depends = 3rdparty/cfgfile/generator

OTHER_FILES += README.md \
	LICENSE
