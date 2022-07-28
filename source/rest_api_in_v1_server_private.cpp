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
