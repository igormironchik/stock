
HEADERS += $$PWD/tcp_socket.hpp \
	$$PWD/datagrams.hpp \
	$$PWD/constants.hpp \
	$$PWD/exceptions.hpp \
	$$PWD/buffer.hpp

SOURCES += $$PWD/tcp_socket.cpp \
	$$PWD/datagrams.cpp \
	$$PWD/exceptions.cpp \
	$$PWD/buffer.cpp

OTHER_FILES += $$PWD/messages.cfgfile

INCLUDEPATH += $$PWD/..
