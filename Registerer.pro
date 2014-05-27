#  Copyright 2013 Mike Bergmann, Bones AG
#
#  This file is part of Registerer.
#
#  Registerer is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  Registerer is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Registerer.  If not, see <http://www.gnu.org/licenses/>.

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Registerer
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
