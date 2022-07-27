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
* This file implements the \ref RestApiInV1::Server::Private class.
***********************************************************************************************************************/

#include <QString>
#include <QTcpServer>
#include <QQueue>
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>

#include <iostream>

#include "rest_api_in_v1_connection.h"
#include "rest_api_in_v1_server.h"
#include "rest_api_in_v1_server_private.h"

namespace RestApiInV1 {
    QMutex Server::Private::loggingMutex;

    Server::Private::Private(unsigned maximumNumberSimultanousConnections, QObject* parent):QTcpServer(parent) {
        for (unsigned i=0 ; i<static_cast<unsigned>(Handler::Method::NUMBER_METHODS) ; ++i) {
            currentHandlersByPathByMethod.append(QHash<QString, Handler*>());
        }

        currentMaximumAllowedConnections = 0;
        setMaximumSimultaneousConnections(maximumNumberSimultanousConnections);

        currentLoggingFunction = &Server::Private::logWrite;
    }


    Server::Private::~Private() {}


    void Server::Private::setLoggingFunction(Server::LoggingFunction newLoggingFunction) {
        currentLoggingFunction = newLoggingFunction;
    }


    Server::LoggingFunction Server::Private::loggingFunction() const {
        return currentLoggingFunction;
    }


    void Server::Private::setMaximumSimultaneousConnections(unsigned newMaximumNumberConnections) {
        if (newMaximumNumberConnections > currentMaximumAllowedConnections) {
            unsigned numberAdditionalConnections = newMaximumNumberConnections - currentMaximumAllowedConnections;

            threadIdQueueMutex.lock();
            for (  unsigned threadId = currentMaximumAllowedConnections
                 ; threadId < newMaximumNumberConnections
                 ; ++threadId
                ) {
                threadIdQueue.enqueue(threadId);
            }
            threadIdQueueMutex.unlock();

            availableConnectionSemaphore.release(numberAdditionalConnections);

            currentMaximumAllowedConnections = newMaximumNumberConnections;
        } else if (newMaximumNumberConnections < currentMaximumAllowedConnections) {
            pauseAccepting();

            while (currentMaximumAllowedConnections > newMaximumNumberConnections) {
                availableConnectionSemaphore.acquire();

                threadIdQueueMutex.lock();
                threadIdQueue.dequeue();
                threadIdQueueMutex.unlock();

                --currentMaximumAllowedConnections;
            }

            resumeAccepting();
        }
    }


    unsigned Server::Private::maximumSimultaneousConnections() const {
        return currentMaximumAllowedConnections;
    }


    bool Server::Private::reconfigure(const QHostAddress& hostAddress, unsigned short port) {
        if (isListening()) {
            close();
        }

        return listen(hostAddress, port);
    }


    void Server::Private::incomingConnection(qintptr socketDescriptor) {
        availableConnectionSemaphore.acquire();

        threadIdQueueMutex.lock();
        unsigned nextThreadId = threadIdQueue.dequeue();
        threadIdQueueMutex.unlock();

        Connection* connection = new Connection(
            this,
            socketDescriptor,
            nextThreadId,
            maximumBufferSize,
            currentLoggingFunction,
            this
        );

        connect(connection, &Connection::finished, this, &Server::Private::sessionFinished);
        connection->start();
    }


    void Server::Private::sessionFinished() {
        Connection* finishedConnection = dynamic_cast<Connection*>(sender());
        unsigned    threadId           = finishedConnection->threadId();

        threadIdQueueMutex.lock();
        threadIdQueue.enqueue(threadId);
        threadIdQueueMutex.unlock();

        availableConnectionSemaphore.release();

        finishedConnection->deleteLater();
    }


    QString Server::Private::cleanPath(const QString& path) {
        QString cleanedPath = path;
        if (path.endsWith('/')) {
            cleanedPath = cleanedPath.left(cleanedPath.length() - 1);
        }

        if (!path.startsWith('/')) {
            cleanedPath = QString("/") + cleanedPath;
        }

        return cleanedPath;
    }


    void Server::Private::logWrite(const QString& message, bool error) {
        QMutexLocker mutexLocker(&loggingMutex);

        QString dateTime = QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate);

        if (error) {
            QString logEntry = QString("%1: *** %2").arg(dateTime, message);
            std::cerr << logEntry.toLocal8Bit().data() << std::endl;
        } else {
            QString logEntry = QString("%1: %2").arg(dateTime, message);
            std::cout << logEntry.toLocal8Bit().data() << std::endl;

        }
    }
}
