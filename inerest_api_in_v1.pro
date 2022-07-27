##-*-makefile-*-########################################################################################################
# Copyright 2021 Inesonic, LLC.
#
# This file is licensed under two licenses:
#
# Inesonic Commercial License:
#   All rights reserved.  Unauthorized use is strictly prohibited under the terms of this license.  Inesonic, is
#   authorized to use this code against its own closed source applications and to redistribute this project under
#   Inesonic's preferred terms.
#
# GPLv3:
#   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation, version 3.
#
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
#   details.
#
#   You should have received a copy of the GNU General Public License along with this program. If not, see
#   <http://www.gnu.org/licenses/>.
########################################################################################################################

########################################################################################################################
# Basic build characteristics
#

TEMPLATE = lib

QT += core network
QT -= gui

CONFIG += static c++14

########################################################################################################################
# Public includes:
#

INCLUDEPATH += include
API_HEADERS = include/rest_api_in_v1_common.h \
              include/rest_api_in_v1_server.h \
              include/rest_api_in_v1_session.h \
              include/rest_api_in_v1_handler.h \
              include/rest_api_in_v1_rest_handler.h \
              include/rest_api_in_v1_inesonic_rest_handler_base.h \
              include/rest_api_in_v1_time_delta_handler.h \
              include/rest_api_in_v1_response.h \
              include/rest_api_in_v1_json_response.h \
              include/rest_api_in_v1_binary_response.h \
              include/rest_api_in_v1_customer_data.h \
              include/rest_api_in_v1_inesonic_rest_handler.h \
              include/rest_api_in_v1_inesonic_binary_rest_handler.h \
              include/rest_api_in_v1_inesonic_customer_rest_handler.h \
              include/rest_api_in_v1_inesonic_customer_binary_rest_handler.h \

########################################################################################################################
# Source files:
#

SOURCES = source/rest_api_in_v1_server.cpp \
          source/rest_api_in_v1_connection.cpp \
          source/rest_api_in_v1_server_private.cpp \
          source/rest_api_in_v1_authentication_helpers.cpp \
          source/rest_api_in_v1_handler.cpp \
          source/rest_api_in_v1_rest_handler.cpp \
          source/rest_api_in_v1_inesonic_rest_handler_base.cpp \
          source/rest_api_in_v1_inesonic_rest_handler_base_private.cpp \
          source/rest_api_in_v1_time_delta_handler.cpp \
          source/rest_api_in_v1_response.cpp \
          source/rest_api_in_v1_json_response.cpp \
          source/rest_api_in_v1_binary_response.cpp \
          source/rest_api_in_v1_customer_data.cpp \
          source/rest_api_in_v1_inesonic_rest_handler.cpp \
          source/rest_api_in_v1_inesonic_binary_rest_handler.cpp \
          source/rest_api_in_v1_inesonic_customer_rest_handler.cpp \
          source/rest_api_in_v1_inesonic_customer_binary_rest_handler.cpp \

########################################################################################################################
# Private includes:
#

INCLUDEPATH += source
PRIVATE_HEADERS = source/rest_api_in_v1_connection.h \
                  source/rest_api_in_v1_server_private.h \
                  source/rest_api_in_v1_authentication_helpers.h \
                  source/rest_api_in_v1_inesonic_rest_handler_base_private.h \
                  source/rest_api_in_v1_inesonic_customer_rest_handler_private.h \
                  source/rest_api_in_v1_inesonic_customer_binary_rest_handler_private.h \

########################################################################################################################
# All libraries and sources:
#

HEADERS = $${API_HEADERS} $${PRIVATE_HEADERS}

########################################################################################################################
# Libraries
#

INCLUDEPATH += $${INECRYPTO_INCLUDE}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = inerest_api_in_v1

CONFIG(debug, debug|release) {
    unix:DESTDIR = build/debug
    win32:DESTDIR = build/Debug
} else {
    unix:DESTDIR = build/release
    win32:DESTDIR = build/Release
}

OBJECTS_DIR = $${DESTDIR}/objects
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui

