/*-*-c++-*-*************************************************************************************************************
* Copyright 2021 - 2022 Inesonic, LLC.
*
* MIT License:
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
*   documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
*   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
*   Software.
*   
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
*   WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
*   OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
*   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
