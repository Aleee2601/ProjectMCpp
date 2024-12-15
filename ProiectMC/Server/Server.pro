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
    # src/Event.cpp \
    src/GameEndEvent.cpp \
    src/PlayerEliminatedEvent.cpp \
    src/PowerUpEvent.cpp

# Calea către directoare
INCLUDEPATH += include
