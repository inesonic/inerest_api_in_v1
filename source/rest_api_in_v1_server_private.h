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

#ifndef REST_API_IN_V1_SERVER_PRIVATE_H
#define REST_API_IN_V1_SERVER_PRIVATE_H

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QHostAddress>
#include <QList>
#include <QQueue>
#include <QSemaphore>
#include <QMutex>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_server.h"

namespace RestApiInV1 {
    class REST_API_V1_PUBLIC_API Handler;
    class REST_API_V1_PUBLIC_API Connection;

    /**
     * Private class used to accept and process incoming TCP connections.
     */
    class REST_API_V1_PUBLIC_API Server::Private:public QTcpServer {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] maximumNumberSimultaneousConnections The desired maximum number of simultaneous connections.
             *
             * \param[in] parent                               The pointer to the parent object.
             */
            Private(unsigned maximumNumberSimultaneousConnections, QObject* parent = nullptr);

            ~Private() override;

            /**
             * Method you can use to set the logging function to be used by this server.
             *
             * \param[in] newLoggingFunction The new logging function to be used.
             */
            void setLoggingFunction(Server::LoggingFunction newLoggingFunction);

            /**
             * Method you can use to determine the logging function being used by this server.
             *
             * \return Returns a pointer to the current logging function.
             */
            Server::LoggingFunction loggingFunction() const;

            /**
             * Method you can use to set the number of allowed simultaneous connections.
             *
             * \param[in] newMaximumNumberConnections The new maximum number of connections.
             */
            void setMaximumSimultaneousConnections(unsigned newMaximumNumberConnections);

            /**
             * Method you can use to determine the current number of allowed simultaneous connections.
             *
             * \return Returns the current number of allowed connections.
             */
            unsigned maximumSimultaneousConnections() const;

            /**
             * Method you can use to reconfigure this server instance.
             *
             * \param[in] hostAddress The host address to be monitored.
             *
             * \param[in] port        The port address to be monitoroed.
             *
             * \return Returns true if the reconfiguration was successful.  Returns false if the reconfiguration
             *         failed.
             */
            bool reconfigure(
                const QHostAddress& hostAddress = defaultHostAddress,
                unsigned short      port = defaultPort
            );

            /**
             * Method you can use to register an handler with this server.  If a handler for the path already exists,
             * it will be replaced.
             *
             * \param[in] handler A pointer the REST API handler.  Not that this class will *not* take ownership.
             *
             * \param[in] method  The method required to trigger this handler.
             *
             * \param[in] path    The path under the server that will trigger this handler.
             */
            inline void registerHandler(Handler* handler, Handler::Method method, const QString& path) {
                currentHandlersByPathByMethod[static_cast<unsigned>(method)].insert(cleanPath(path), handler);
            }

            /**
             * Method that gets the handler for a given path.
             *
             * \param[in] method The method required to trigger this handler.
             *
             * \param[in] path   The path to get the handler for.
             *
             * \return Returns a pointer to the handler.  A null pointer is returned if there is no path tied to the
             *         requested handler.
             */
            inline Handler* handler(Handler::Method method, const QString& path) const {
                return currentHandlersByPathByMethod.at(
                    static_cast<unsigned>(method)
                ).value(
                    cleanPath(path),
                    nullptr
                );
            }

        signals:
            /**
             * Signal that is emitted when a session error is reported.
             *
             * \param[out] errorReason The reason provided for the error.
             */
            void sessionError(const QString& errorReason);

        public slots:
            /**
             * Slot that is triggered when a session has finished.
             */
            void sessionFinished();

        protected:
            /**
             * Slot you can trigger when a new connection is available.
             *
             * \param[in] socketDescriptor The underlying OS socket descriptor to use for the incoming TCP socket.
             */
            void incomingConnection(qintptr socketDescriptor) override;

        private:
            /**
             * Value indicating the maximum per-connection buffer size, in bytes.
             */
            static constexpr unsigned long maximumBufferSize = 65536;

            /**
             * Method that cleans a path.
             *
             * \param[in] path The path to be cleaned.
             *
             * \return Returns the path with a forced leading slash and ending slashes removed.
             */
            static QString cleanPath(const QString& path);

            /**
             * Function you can use to write a log entry.
             *
             * \param[in] message The log message.
             *
             * \param[in] error   If true, the data is reporting an error.
             */
            static void logWrite(const QString& message, bool error = false);

            /**
             * Queue used to keep track of available thread IDs.
             */
            QQueue<unsigned> threadIdQueue;

            /**
             * Mutex used to guarantee atomic access to the thread ID queue.
             */
            QMutex threadIdQueueMutex;

            /**
             * A list of underlying handlers.
             */
            QList<QHash<QString, Handler*>> currentHandlersByPathByMethod;

            /**
             * Semaphore used to track the available connections.
             */
            QSemaphore availableConnectionSemaphore;

            /**
             * The current count of allowed connections.
             */
            unsigned currentMaximumAllowedConnections;

            /**
             * The current logging function.
             */
            Server::LoggingFunction currentLoggingFunction;

            /**
             * Mutex used to support logging across threads.
             */
            static QMutex loggingMutex;
    };
};

#endif
