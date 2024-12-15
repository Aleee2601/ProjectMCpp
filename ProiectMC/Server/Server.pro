TEMPLATE = lib
QT += core network

# Include și surse
HEADERS += \
    include/ActiveClientGuard.h \
    include/Bomb.h \
    include/Bullet.h \
    include/ClientFunctions.h \
    include/GameSession.h \
    include/map.h \
    include/Player.h \
    include/Server.h \
    include/Weapon.h \
    include/Event.h \
    include/GameEndEvent.h \
    include/PlayerEliminatedEvent.h \
    include/PowerUpEvent.h

SOURCES += \
    src/ActiveClientGuard.cpp \
    src/Bomb.cpp \
    src/Bullet.cpp \
    src/ClientFunctions.cpp \
    src/GameSession.cpp \
    src/map.cpp \
    src/Player.cpp \
    src/Server.cpp \
    src/Weapon.cpp \
    src/GameEndEvent.cpp \
    src/PlayerEliminatedEvent.cpp \
    src/PowerUpEvent.cpp

# Calea către directoare include
INCLUDEPATH += include
INCLUDEPATH += "D:/Facultate/Anul2Sem1/MC/vcpkg/installed/x64-windows/include"

# Biblioteci pentru linkare
LIBS += -L"D:/Facultate/Anul2Sem1/MC/vcpkg/installed/x64-windows/lib" -lcurl
LIBS += -L"D:/Facultate/Anul2Sem1/MC/vcpkg/installed/x64-windows/lib" -lcpr


CONFIG += x86_64
QMAKE_CXXFLAGS += /machine:x64
QMAKE_LFLAGS += /machine:x64