# -------------------------------------------------------------------
# Project file for the QtWebKit C++ APIs
#
# See 'Tools/qmake/README' for an overview of the build system
# -------------------------------------------------------------------

TEMPLATE = lib
TARGET = QtWebKit

WEBKIT_DESTDIR = $${ROOT_BUILD_DIR}/lib

haveQt(5) {
    # Use Qt5's module system
    load(qt_build_config)
    MODULE = webkit
    MODULE_PRI = ../Tools/qmake/qt_webkit.pri

    # ---------------- Custom developer-build handling -------------------
    #
    # The assumption for Qt developer builds is that the module file
    # will be put into qtbase/mkspecs/modules, and the libraries into
    # qtbase/lib, with rpath/install_name set to the Qt lib dir.
    #
    # For WebKit we don't want that behavior for the libraries, as we want
    # them to be self-contained in the WebKit build dir.
    #
    CONFIG += force_independent

    BASE_TARGET = $$TARGET

    load(qt_module)

    # Make sure the module config doesn't override our preferred build config.
    debug_and_release:if(!debug|!release) {
        # Removing debug_and_release causes issues with lib suffixes when building debug on Windows.
        # Work around it by only removing build_all, and still create the Makefiles for both configurations.
        win32*: CONFIG -= build_all
        else: CONFIG -= debug_and_release
    }

    # Allow doing a debug-only build of WebKit (not supported by Qt)
    macx:!debug_and_release:debug: TARGET = $$BASE_TARGET

    # Make sure the install_name of the QtWebKit library point to webkit
    macx {
        # We do our own absolute path so that we can trick qmake into
        # using the webkit build path instead of the Qt install path.
        CONFIG -= absolute_library_soname
        QMAKE_LFLAGS_SONAME = $$QMAKE_LFLAGS_SONAME$$WEBKIT_DESTDIR/

        !debug_and_release|build_pass {
            # We also have to make sure the install_name is correct when
            # the library is installed.
            change_install_name.depends = install_target

            # The install rules generated by qmake for frameworks are busted in
            # that both the debug and the release makefile copy QtWebKit.framework
            # into the install dir, so whatever changes we did to the release library
            # will get overwritten when the debug library is installed. We work around
            # that by running install_name on both, for both configs.
            change_install_name.commands = framework_dir=\$\$(dirname $(TARGETD)); \
                for file in \$\$(ls $$[QT_INSTALL_LIBS]/\$\$framework_dir/$$BASE_TARGET*); do \
                    install_name_tool -id \$\$file \$\$file; \
                done
            default_install_target.target = install
            default_install_target.depends += change_install_name

            QMAKE_EXTRA_TARGETS += change_install_name default_install_target
        }
    }
} else {
    MODULE_PRI = ../Tools/qmake/qt_webkit.pri
    include($$MODULE_PRI)
    VERSION = $$QT.webkit.VERSION
    DESTDIR = $$WEBKIT_DESTDIR
}

runSyncQt() # Generate forwarding headers for the QtWebKit API

WEBKIT += wtf javascriptcore webcore

# Ensure that changes to the WebKit1 and WebKit2 API will trigger a qmake of this
# file, which in turn runs syncqt to update the forwarding headers.
!no_webkit1: QMAKE_INTERNAL_INCLUDED_FILES *= WebKit/WebKit1.pro
!no_webkit2: QMAKE_INTERNAL_INCLUDED_FILES *= WebKit2/Target.pri

contains(DEFINES, WTF_USE_3D_GRAPHICS=1): WEBKIT += angle

# This is the canonical list of dependencies for the public API of
# the QtWebKit library, and will end up in the library's prl file.
QT_API_DEPENDS = core gui network

# We want the QtWebKit API forwarding includes to live in the root build dir.
MODULE_BASE_DIR = $$_PRO_FILE_PWD_
MODULE_BASE_OUTDIR = $$ROOT_BUILD_DIR

QMAKE_DOCS = $$PWD/qtwebkit.qdocconf

!no_webkit1: WEBKIT += webkit1
!no_webkit2: WEBKIT += webkit2

# Resources have to be included directly in the final binary for MSVC.
# The linker won't pick them from a static library since they aren't referenced.
win* {
    RESOURCES += $$PWD/WebCore/WebCore.qrc
    include_webinspector {
        # WEBCORE_GENERATED_SOURCES_DIR is defined in WebCore.pri, included by
        # load(webkit_modules) if WEBKIT contains webcore.
        RESOURCES += \
            $$PWD/WebCore/inspector/front-end/WebKit.qrc \
            $${WEBCORE_GENERATED_SOURCES_DIR}/InspectorBackendCommands.qrc
    }
}

# ------------- Install rules -------------

haveQt(5) {
    # Install rules handled by Qt's module system
} else {
    # For Qt4 we have to set up install rules manually
    modulefile.files = $${ROOT_WEBKIT_DIR}/Tools/qmake/qt_webkit.pri
    mkspecs = $$[QMAKE_MKSPECS]
    mkspecs = $$split(mkspecs, :)
    modulefile.path = $$last(mkspecs)/modules
    INSTALLS += modulefile

    # Syncqt has already run at this point, so we can use headers.pri
    # as a basis for our install-rules
    HEADERS_PRI = $${ROOT_BUILD_DIR}/include/$${QT.webkit.name}/headers.pri
    !include($$HEADERS_PRI): error(Failed to resolve install headers)

    headers.files = $$SYNCQT.HEADER_FILES $$SYNCQT.HEADER_CLASSES
    !isEmpty(INSTALL_HEADERS): headers.path = $$INSTALL_HEADERS/$${TARGET}
    else: headers.path = $$[QT_INSTALL_HEADERS]/$${TARGET}
    INSTALLS += headers

    !isEmpty(INSTALL_LIBS): target.path = $$INSTALL_LIBS
    else: target.path = $$[QT_INSTALL_LIBS]
    INSTALLS += target

    unix {
        CONFIG += create_pc create_prl
        QMAKE_PKGCONFIG_LIBDIR = $$target.path
        QMAKE_PKGCONFIG_INCDIR = $$headers.path
        QMAKE_PKGCONFIG_DESTDIR = pkgconfig
        lib_replace.match = $$re_escape($$DESTDIR)
        lib_replace.replace = $$[QT_INSTALL_LIBS]
        QMAKE_PKGCONFIG_INSTALL_REPLACE += lib_replace
    }

    mac {
        !static:contains(QT_CONFIG, qt_framework) {
            # Build QtWebKit as a framework, to match how Qt was built
            CONFIG += lib_bundle qt_no_framework_direct_includes qt_framework

            # For debug_and_release configs, only copy headers in release
            !debug_and_release|if(build_pass:CONFIG(release, debug|release)) {
                FRAMEWORK_HEADERS.version = Versions
                FRAMEWORK_HEADERS.files = $${headers.files}
                FRAMEWORK_HEADERS.path = Headers
                QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
            }
        }

        QMAKE_LFLAGS_SONAME = "$${QMAKE_LFLAGS_SONAME}$${DESTDIR}$${QMAKE_DIR_SEP}"
    }
}

qnx {
    # see: https://bugs.webkit.org/show_bug.cgi?id=93460
    # the gcc 4.4.2 used in the qnx bbndk cannot cope with
    # the linkage step of libQtWebKit, adding a dummy .cpp
    # file fixes this though - so do this here
    dummyfile.target = dummy.cpp
    dummyfile.commands = touch $$dummyfile.target
    QMAKE_EXTRA_TARGETS += dummyfile
    GENERATED_SOURCES += $$dummyfile.target
}
