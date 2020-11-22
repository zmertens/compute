TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -Wall

#INCLUDEPATH += /home/lateralus/repo/glm

LIBS += -lSDL2 -lX11 -lGL

SOURCES += \
    Main.cpp \
    Player.cpp \
    extlibs/gl_core_4_5.c \
    Camera.cpp \
    GlUtils.cpp \
    Light.cpp \
    Material.cpp \
    SdlManager.cpp \
    Shader.cpp \
    Transform.cpp

HEADERS += \
    Player.hpp \
    extlibs/gl_core_4_5.h \
    extlibs/stb_image.h \
    Camera.hpp \
    Config.hpp \
    GlUtils.hpp \
    Light.hpp \
    Material.hpp \
    SdlManager.hpp \
    Shader.hpp \
    Transform.hpp \
    Utils.hpp \
    Sphere.hpp \
    Plane.hpp

DISTFILES += \
    ../shaders/raytracer.cs.glsl \
    ../shaders/raytracer.frag.glsl \
    ../shaders/raytracer.vert.glsl

