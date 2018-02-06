
TEMPLATE = subdirs

SUBDIRS = stock \
	3rdparty/cfgfile/generator

stock.depends = 3rdparty/cfgfile/generator
