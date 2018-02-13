
HEADERS += $$PWD/tcp_socket.hpp \
	$$PWD/udp_socket.hpp \
	$$PWD/datagrams.hpp \
	$$PWD/constants.hpp \
	$$PWD/exceptions.hpp

SOURCES += $$PWD/tcp_socket.cpp \
	$$PWD/udp_socket.cpp \
	$$PWD/datagrams.cpp \
	$$PWD/exceptions.cpp

OTHER_FILES += $$PWD/messages.cfgfile

INCLUDEPATH += $$PWD/..
