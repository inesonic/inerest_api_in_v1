/*-*-c++-*-*************************************************************************************************************
* Copyright 2021 Inesonic, LLC.
*
* This file is licensed under two licenses:
*
* Inesonic Commercial License:
*   All rights reserved.  Unauthorized use is strictly prohibited under the terms of this license.  Inesonic, is
*   authorized to use this code against its own closed source applications and to redistribute this project under
*   Inesonic's preferred terms.
*
* GPLv3:
*   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation, version 3.
*
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*   details.
*
*   You should have received a copy of the GNU General Public License along with this program. If not, see
*   <http://www.gnu.org/licenses/>.
********************************************************************************************************************//**
* \file
*
* This file implements the \ref RestApiInV1::Server class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QCoreApplication>
#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QHash>
#include <QPointer>
#include <QDateTime>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "rest_api_in_v1_handler.h"
#include "rest_api_in_v1_server.h"
#include "rest_api_in_v1_server_private.h"

namespace RestApiInV1 {
    const QHostAddress   Server::defaultHostAddress                    = QHostAddress::Any;
    const unsigned short Server::defaultPort                           = 8080;
    const unsigned       Server::defaultMaximumSimultaneousConnections = 4;

    Server::Server(QObject* parent):QObject(parent) {
        impl = new Private(defaultMaximumSimultaneousConnections);
        connect(impl, &Server::Private::sessionError, this, &Server::sessionError);
    }


    void Server::setLoggingFunction(Server::LoggingFunction newLoggingFunction) {
        impl->setLoggingFunction(newLoggingFunction);
    }


    Server::LoggingFunction Server::loggingFunction() const {
        return impl->loggingFunction();
    }


    Server::Server(unsigned maximumNumberSimultaneousConnections, QObject* parent):QObject(parent) {
        impl = new Private(maximumNumberSimultaneousConnections);
        connect(impl, &Server::Private::sessionError, this, &Server::sessionError);
    }


    Server::~Server() {
        delete impl;
    }


    void Server::setMaximumSimultaneousConnections(unsigned int newMaximumNumberConnections) {
        impl->setMaximumSimultaneousConnections(newMaximumNumberConnections);
    }


    unsigned Server::maximumSimultaneousConnections() const {
        return impl->maximumSimultaneousConnections();
    }


    bool Server::reconfigure(const QHostAddress& hostAddress, unsigned short port) {
        return impl->reconfigure(hostAddress, port);
    }


    void Server::registerHandler(Handler* handler, Handler::Method method, const QString& path) {
        impl->registerHandler(handler, method, path);
    }


    Handler* Server::handler(Handler::Method method, const QString& path) const {
        return impl->handler(method, path);
    }
}
