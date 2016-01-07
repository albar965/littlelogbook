#*****************************************************************************
# Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#****************************************************************************

#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T16:32:20
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Adapt these variables to compile on Windows
win32 {
  QT_BIN=C:\\Qt\\5.5\\mingw492_32\\bin
  GIT_BIN='C:\\Git\\bin\\git'
}

CONFIG *= debug_and_release debug_and_release_target

TARGET = littlelogbook
TEMPLATE = app

# C++ standard 11 used here
#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

# Get the current GIT revision to include it into the code
win32:DEFINES += GIT_REVISION='\\"$$system($${GIT_BIN} rev-parse --short HEAD)\\"'
unix:DEFINES += GIT_REVISION='\\"$$system(git rev-parse --short HEAD)\\"'

# Add dependencies to atools project and its static library to ensure relinking on changes
DEPENDPATH += $$PWD/../atools/src
INCLUDEPATH += $$PWD/../atools/src $$PWD/src

CONFIG(debug, debug|release) {
  LIBS += -L $$PWD/../atools/debug -l atools
  PRE_TARGETDEPS += $$PWD/../atools/debug/libatools.a
}
CONFIG(release, debug|release) {
  LIBS += -L $$PWD/../atools/release -l atools
  PRE_TARGETDEPS += $$PWD/../atools/release/libatools.a
}

SOURCES += src/main.cpp\
    src/gui/mainwindow.cpp \
    src/table/sqlmodel.cpp \
    src/table/controller.cpp \
    src/table/columnlist.cpp \
    src/gui/globalstats.cpp \
    src/table/formatter.cpp \
    src/gui/helphandler.cpp \
    src/gui/constants.cpp \
    src/export/htmlexporter.cpp \
    src/export/csvexporter.cpp \
    src/export/exporter.cpp \
    src/gui/airportinfo.cpp

HEADERS  += src/gui/mainwindow.h \
    src/table/sqlmodel.h \
    src/table/controller.h \
    src/table/columnlist.h \
    src/table/colum.h \
    src/gui/globalstats.h \
    src/table/formatter.h \
    src/gui/helphandler.h \
    src/gui/constants.h \
    src/export/htmlexporter.h \
    src/export/csvexporter.h \
    src/export/exporter.h \
    src/gui/airportinfo.h

FORMS    += src/gui/mainwindow.ui

DISTFILES += \
    help/de/index.html \
    help/en/index.html \
    LICENSE.txt \
    README.txt \
    help/de/images/clearsearch.png \
    help/de/images/complexsearch.png \
    help/de/images/conditions.png \
    help/de/images/grouped.png \
    help/de/images/header.png \
    help/de/images/logbook.png \
    help/de/images/open.png \
    help/de/images/reload.png \
    help/de/images/resetview.png \
    help/de/images/searchbar.png \
    help/de/images/searchbarsmall.png \
    help/de/images/searchtips.png \
    help/de/images/showall.png \
    help/de/images/showsearch.png \
    help/de/images/showstats.png \
    help/de/images/statistics.png \
    help/de/images/statusbar.png \
    help/de/images/tableviewcontext.png \
    help/de/images/ungroup.png \
    help/de/images/gpl-v3-logo.svg \
    help/en/images/clearsearch.png \
    help/en/images/complexsearch.png \
    help/en/images/conditions.png \
    help/en/images/grouped.png \
    help/en/images/header.png \
    help/en/images/logbook.png \
    help/en/images/open.png \
    help/en/images/reload.png \
    help/en/images/resetview.png \
    help/en/images/searchbar.png \
    help/en/images/searchtips.png \
    help/en/images/showall.png \
    help/en/images/showsearch.png \
    help/en/images/showstats.png \
    help/en/images/statistics.png \
    help/en/images/statusbar.png \
    help/en/images/tableviewcontext.png \
    help/en/images/ungroup.png \
    help/en/images/gpl-v3-logo.svg \
    help/de/images/aptooltip.png \
    help/en/images/aptooltip.png \
    uncrustify.cfg \
    htmltidy.cfg \
    BUILD.txt

# Create additional makefile targets to copy help files
unix {
  copydata.commands = cp -avf $$PWD/help $$OUT_PWD
  cleandata.commands = rm -Rvf $$OUT_PWD/help
}

# Windows specific deploy target only for release builds
win32 {
  RC_ICONS = resources/icons/littlelogbook.ico

  # Create backslashed path
  WINPWD=$${PWD}
  WINPWD ~= s,/,\\,g
  WINOUT_PWD=$${OUT_PWD}
  WINOUT_PWD ~= s,/,\\,g
  DEPLOY_DIR_NAME=Little Logbook
  DEPLOY_DIR=\"$${WINPWD}\\deploy\\$${DEPLOY_DIR_NAME}\"

  copydata.commands = xcopy /i /s /e /f /y $${WINPWD}\\help $${WINOUT_PWD}\\help

  cleandata.commands = del /s /q $${WINOUT_PWD}\\help

  deploy.commands = rmdir /s /q $${DEPLOY_DIR} &
  deploy.commands += mkdir $${DEPLOY_DIR} &
  deploy.commands += copy $${WINOUT_PWD}\\littlelogbook.exe $${DEPLOY_DIR} &&
  deploy.commands += copy $${WINPWD}\\README.txt $${DEPLOY_DIR} &&
  deploy.commands += copy $${WINPWD}\\LICENSE.txt $${DEPLOY_DIR} &&
  deploy.commands += xcopy /i /s /e /f /y $${WINPWD}\\help $${DEPLOY_DIR}\\help &&
  deploy.commands += copy $${QT_BIN}\\libgcc*.dll $${DEPLOY_DIR} &&
  deploy.commands += copy $${QT_BIN}\\libstdc*.dll $${DEPLOY_DIR} &&
  deploy.commands += copy $${QT_BIN}\\libwinpthread*.dll $${DEPLOY_DIR} &&
  deploy.commands += $${QT_BIN}\\windeployqt --compiler-runtime $${DEPLOY_DIR} &&
  # Delete some unneeded files copied by windeployqt
  deploy.commands += del /s /q $${DEPLOY_DIR}\\imageformats\\qdds.dll $${DEPLOY_DIR}\\imageformats\\qjp2.dll $${DEPLOY_DIR}\\imageformats\\qtga.dll $${DEPLOY_DIR}\\imageformats\\qtiff.dll &
  deploy.commands += del /s /q $${DEPLOY_DIR}\\sqldrivers\qsqlmysql.dll $${DEPLOY_DIR}\\sqldrivers\qsqlodbc.dll $${DEPLOY_DIR}\\sqldrivers\qsqlpsql.dll
}

QMAKE_EXTRA_TARGETS += deploy

first.depends = $(first) copydata
QMAKE_EXTRA_TARGETS += first copydata

clean.depends = $(clean) cleandata
QMAKE_EXTRA_TARGETS += clean cleandata

RESOURCES += \
    littlelogbook.qrc

TRANSLATIONS = littlelogbook_de.ts
