# -------------------------------------------------------------------
# This file contains shared rules used both when building WebCore
# itself, and by targets that use WebCore.
#
# See 'Tools/qmake/README' for an overview of the build system
# -------------------------------------------------------------------

SOURCE_DIR = $${ROOT_WEBKIT_DIR}/Source/WebCore

QT *= network
haveQt(5): QT *= core-private gui-private

WEBCORE_GENERATED_SOURCES_DIR = $${ROOT_BUILD_DIR}/Source/WebCore/$${GENERATED_SOURCES_DESTDIR}

INCLUDEPATH += \
    $$SOURCE_DIR \
    $$SOURCE_DIR/Modules/filesystem \
    $$SOURCE_DIR/Modules/geolocation \
    $$SOURCE_DIR/Modules/indexeddb \
    $$SOURCE_DIR/Modules/navigatorcontentutils \
    $$SOURCE_DIR/Modules/notifications \
    $$SOURCE_DIR/Modules/quota \
    $$SOURCE_DIR/Modules/webaudio \
    $$SOURCE_DIR/Modules/webdatabase \
    $$SOURCE_DIR/Modules/websockets \
    $$SOURCE_DIR/accessibility \
    $$SOURCE_DIR/bindings \
    $$SOURCE_DIR/bindings/generic \
    $$SOURCE_DIR/bridge \
    $$SOURCE_DIR/bridge/qt \
    $$SOURCE_DIR/css \
    $$SOURCE_DIR/dom \
    $$SOURCE_DIR/dom/default \
    $$SOURCE_DIR/editing \
    $$SOURCE_DIR/fileapi \
    $$SOURCE_DIR/history \
    $$SOURCE_DIR/html \
    $$SOURCE_DIR/html/canvas \
    $$SOURCE_DIR/html/parser \
    $$SOURCE_DIR/html/shadow \
    $$SOURCE_DIR/html/track \
    $$SOURCE_DIR/inspector \
    $$SOURCE_DIR/loader \
    $$SOURCE_DIR/loader/appcache \
    $$SOURCE_DIR/loader/archive \
    $$SOURCE_DIR/loader/cache \
    $$SOURCE_DIR/loader/icon \
    $$SOURCE_DIR/mathml \
    $$SOURCE_DIR/page \
    $$SOURCE_DIR/page/animation \
    $$SOURCE_DIR/page/qt \
    $$SOURCE_DIR/page/scrolling \
    $$SOURCE_DIR/platform \
    $$SOURCE_DIR/platform/animation \
    $$SOURCE_DIR/platform/audio \
    $$SOURCE_DIR/platform/graphics \
    $$SOURCE_DIR/platform/graphics/cpu/arm \
    $$SOURCE_DIR/platform/graphics/cpu/arm/filters \
    $$SOURCE_DIR/platform/graphics/filters \
    $$SOURCE_DIR/platform/graphics/filters/texmap \
    $$SOURCE_DIR/platform/graphics/opengl \
    $$SOURCE_DIR/platform/graphics/opentype \
    $$SOURCE_DIR/platform/graphics/qt \
    $$SOURCE_DIR/platform/graphics/surfaces \
    $$SOURCE_DIR/platform/graphics/texmap \
    $$SOURCE_DIR/platform/graphics/transforms \
    $$SOURCE_DIR/platform/image-decoders \
    $$SOURCE_DIR/platform/image-decoders/bmp \
    $$SOURCE_DIR/platform/image-decoders/ico \
    $$SOURCE_DIR/platform/image-decoders/gif \
    $$SOURCE_DIR/platform/image-decoders/jpeg \
    $$SOURCE_DIR/platform/image-decoders/png \
    $$SOURCE_DIR/platform/image-decoders/webp \
    $$SOURCE_DIR/platform/leveldb \
    $$SOURCE_DIR/platform/mock \
    $$SOURCE_DIR/platform/network \
    $$SOURCE_DIR/platform/network/qt \
    $$SOURCE_DIR/platform/qt \
    $$SOURCE_DIR/platform/sql \
    $$SOURCE_DIR/platform/text \
    $$SOURCE_DIR/platform/text/transcoder \
    $$SOURCE_DIR/plugins \
    $$SOURCE_DIR/rendering \
    $$SOURCE_DIR/rendering/mathml \
    $$SOURCE_DIR/rendering/style \
    $$SOURCE_DIR/rendering/svg \
    $$SOURCE_DIR/storage \
    $$SOURCE_DIR/svg \
    $$SOURCE_DIR/svg/animation \
    $$SOURCE_DIR/svg/graphics \
    $$SOURCE_DIR/svg/graphics/filters \
    $$SOURCE_DIR/svg/properties \
    $$SOURCE_DIR/testing \
    $$SOURCE_DIR/websockets \
    $$SOURCE_DIR/workers \
    $$SOURCE_DIR/xml \
    $$SOURCE_DIR/xml/parser \
    $$SOURCE_DIR/../ThirdParty

INCLUDEPATH += \
    $$SOURCE_DIR/bridge/jsc \
    $$SOURCE_DIR/bindings/js \
    $$SOURCE_DIR/bridge/c \
    $$SOURCE_DIR/testing/js

INCLUDEPATH += $$WEBCORE_GENERATED_SOURCES_DIR

contains(DEFINES, ENABLE_XSLT=1) {
    contains(DEFINES, WTF_USE_LIBXML2=1) {
        mac {
            INCLUDEPATH += /usr/include/libxml2
            LIBS += -lxml2 -lxslt
        } else {
            PKGCONFIG += libxslt
        }
    } else {
        QT *= xmlpatterns
    }
}

contains(DEFINES, WTF_USE_LIBXML2=1) {
    PKGCONFIG += libxml-2.0
}

contains(DEFINES, WTF_USE_ZLIB=1) {
    LIBS += -lz
}

contains(DEFINES, ENABLE_NETSCAPE_PLUGIN_API=1) {
    unix {
        mac {
            INCLUDEPATH += platform/mac
            # Note: XP_MACOSX is defined in npapi.h
        } else {
            xlibAvailable() {
                CONFIG *= x11
                LIBS += -lXrender
                DEFINES += MOZ_X11
            }
            DEFINES += XP_UNIX
            DEFINES += ENABLE_NETSCAPE_PLUGIN_METADATA_CACHE=1
        }
    }
    win32-* {
        LIBS += \
            -ladvapi32 \
            -lgdi32 \
            -lshell32 \
            -lshlwapi \
            -luser32 \
            -lversion
    }
}

contains(DEFINES, ENABLE_GEOLOCATION=1) {
    CONFIG *= mobility
    MOBILITY *= location
}

contains(DEFINES, ENABLE_QT_SHARED_COOKIEJAR=1) {
    QT *= sql
}

contains(DEFINES, ENABLE_ORIENTATION_EVENTS=1)|contains(DEFINES, ENABLE_DEVICE_ORIENTATION=1) {
    haveQt(5) {
        QT += sensors
    } else {
        CONFIG *= mobility
        MOBILITY *= sensors
    }
}

contains(DEFINES, WTF_USE_QT_MOBILITY_SYSTEMINFO=1) {
     CONFIG *= mobility
     MOBILITY *= systeminfo
}

contains(DEFINES, ENABLE_GAMEPAD=1) {
    INCLUDEPATH += \
        $$SOURCE_DIR/platform/linux \
        $$SOURCE_DIR/Modules/gamepad
    PKGCONFIG += libudev
}

contains(DEFINES, WTF_USE_GSTREAMER=1) {
    DEFINES += ENABLE_GLIB_SUPPORT=1
    PKGCONFIG += glib-2.0 gio-2.0 gstreamer-0.10 gstreamer-app-0.10 gstreamer-base-0.10 gstreamer-interfaces-0.10 gstreamer-pbutils-0.10 gstreamer-plugins-base-0.10 gstreamer-video-0.10
}

contains(DEFINES, ENABLE_VIDEO=1) {
    contains(DEFINES, WTF_USE_QTKIT=1) {
        INCLUDEPATH += $$SOURCE_DIR/platform/graphics/mac

        LIBS += -framework AppKit -framework AudioUnit \
                -framework AudioToolbox -framework CoreAudio \
                -framework QuartzCore -framework QTKit \
                -framework Security -framework IOKit

        # We can know the Mac OS version by using the Darwin major version
        DARWIN_VERSION = $$split(QMAKE_HOST.version, ".")
        DARWIN_MAJOR_VERSION = $$first(DARWIN_VERSION)
        equals(DARWIN_MAJOR_VERSION, "12") {
            LIBS += $${ROOT_WEBKIT_DIR}/WebKitLibraries/libWebKitSystemInterfaceMountainLion.a
        } else:equals(DARWIN_MAJOR_VERSION, "11") {
            LIBS += $${ROOT_WEBKIT_DIR}/WebKitLibraries/libWebKitSystemInterfaceLion.a
        } else:equals(DARWIN_MAJOR_VERSION, "10") {
            LIBS += $${ROOT_WEBKIT_DIR}/WebKitLibraries/libWebKitSystemInterfaceSnowLeopard.a
        } else:equals(DARWIN_MAJOR_VERSION, "9") {
            LIBS += $${ROOT_WEBKIT_DIR}/WebKitLibraries/libWebKitSystemInterfaceLeopard.a
        }
    } else:contains(DEFINES, WTF_USE_GSTREAMER=1) {
        INCLUDEPATH += $$SOURCE_DIR/platform/graphics/gstreamer
    } else:contains(DEFINES, WTF_USE_QT_MULTIMEDIA=1) {
        CONFIG   *= mobility
        MOBILITY *= multimedia
    }
}

contains(DEFINES, ENABLE_WEB_AUDIO=1) {
    contains(DEFINES, WTF_USE_GSTREAMER=1) {
        DEFINES += WTF_USE_WEBAUDIO_GSTREAMER=1
        INCLUDEPATH += $$SOURCE_DIR/platform/audio/gstreamer
        PKGCONFIG += gstreamer-audio-0.10 gstreamer-fft-0.10
    }
}

contains(DEFINES, WTF_USE_3D_GRAPHICS=1) {
    win32: {
        win32-g++: {
            # Make sure OpenGL libs are after the webcore lib so MinGW can resolve symbols
            contains(QT_CONFIG, opengles2) {
                LIBS += $$QMAKE_LIBS_OPENGL_ES2
            } else {
                LIBS += $$QMAKE_LIBS_OPENGL
            }
        }
    } else {
        contains(QT_CONFIG, opengles2): CONFIG += egl
    }
    haveQt(4): QT *= opengl
}

contains(DEFINES, WTF_USE_GRAPHICS_SURFACE=1) {
    mac: LIBS += -framework IOSurface -framework CoreFoundation
    linux-*: {
        LIBS += -lXcomposite -lXrender
        CONFIG *= x11
    }
}

contains(DEFINES, HAVE_SQLITE3=1) {
    mac {
        LIBS += -lsqlite3
    } else {
        PKGCONFIG += sqlite3
    }
} else {
    SQLITE3SRCDIR = $$(SQLITE3SRCDIR)
    isEmpty(SQLITE3SRCDIR): SQLITE3SRCDIR = ../../../qtbase/src/3rdparty/sqlite/
    exists($${SQLITE3SRCDIR}/sqlite3.c) {
        INCLUDEPATH += $${SQLITE3SRCDIR}
        DEFINES += SQLITE_CORE SQLITE_OMIT_LOAD_EXTENSION SQLITE_OMIT_COMPLETE
    } else {
        INCLUDEPATH += $${SQLITE3SRCDIR}
        LIBS += -lsqlite3
    }
}

haveQt(5) {
    # Qt5 allows us to use config tests to check for the presence of these libraries
    config_libjpeg {
        DEFINES += WTF_USE_LIBJPEG=1
        LIBS += -ljpeg
    } else {
        warning("JPEG library not found! QImageDecoder will decode JPEG images.")
    }
    config_libpng {
        DEFINES += WTF_USE_LIBPNG=1
        LIBS += -lpng
    } else {
        warning("PNG library not found! QImageDecoder will decode PNG images.")
    }
    config_libwebp {
        DEFINES += WTF_USE_WEBP=1
        LIBS += -lwebp
    }
} else {
    !win32-*:!mac {
        DEFINES += WTF_USE_LIBJPEG=1 WTF_USE_LIBPNG=1
        LIBS += -ljpeg -lpng
    }
    contains(DEFINES, HAVE_LIBWEBP=1) {
        DEFINES += WTF_USE_WEBP=1
        LIBS += -lwebp
    }
}

mac {
    LIBS += -framework Carbon -framework AppKit -framework IOKit
}

win32 {
    INCLUDEPATH += $$SOURCE_DIR/platform/win

    wince* {
        # see https://bugs.webkit.org/show_bug.cgi?id=43442
        DEFINES += HAVE_LOCALTIME_S=0

        LIBS += -lmmtimer
        LIBS += -lole32
    }
    else {
        LIBS += -lgdi32
        LIBS += -lole32
        LIBS += -luser32
    }
}

# Remove whole program optimizations due to miscompilations
win32-msvc2005|win32-msvc2008|win32-msvc2010|wince*:{
    QMAKE_CFLAGS_LTCG -= -GL
    QMAKE_CXXFLAGS_LTCG -= -GL

    # Disable incremental linking for windows 32bit OS debug build as WebKit is so big
    # that linker failes to link incrementally in debug mode.
    ARCH = $$(PROCESSOR_ARCHITECTURE)
    WOW64ARCH = $$(PROCESSOR_ARCHITEW6432)
    equals(ARCH, x86):{
        isEmpty(WOW64ARCH): QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
    }
}

mac {
    LIBS_PRIVATE += -framework Carbon -framework AppKit
}

# -ffunction-section conflicts with -pg option
!contains(CONFIG, gprof) {
    unix:!mac:*-g++*:QMAKE_CXXFLAGS += -ffunction-sections
}
unix:!mac:*-g++*:QMAKE_CXXFLAGS += -fdata-sections
unix:!mac:*-g++*:QMAKE_LFLAGS += -Wl,--gc-sections
linux*-g++*:QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF

unix|win32-g++* {
    QMAKE_PKGCONFIG_REQUIRES = QtCore QtGui QtNetwork
    haveQt(5): QMAKE_PKGCONFIG_REQUIRES += QtWidgets
}

contains(DEFINES, ENABLE_OPENCL=1) {
    LIBS += -lOpenCL

    INCLUDEPATH += $$SOURCE_DIR/platform/graphics/gpu/opencl
}

# Disable C++0x mode in WebCore for those who enabled it in their Qt's mkspec
*-g++*:QMAKE_CXXFLAGS -= -std=c++0x -std=gnu++0x

enable_fast_mobile_scrolling: DEFINES += ENABLE_FAST_MOBILE_SCROLLING=1

contains(DEFINES, HAVE_FONTCONFIG=1): PKGCONFIG += fontconfig
