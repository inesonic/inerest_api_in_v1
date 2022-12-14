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

#ifndef REST_API_IN_V1_SERVER_H
#define REST_API_IN_V1_SERVER_H

#include <QtGlobal>
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

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_handler.h"

namespace RestApiInV1 {
    class REST_API_V1_PUBLIC_API TcpServer;
    class REST_API_V1_PUBLIC_API Connection;

    /**
     * Class that implements a simple inbound REST API.
     */
    class REST_API_V1_PUBLIC_API Server:public QObject {
        Q_OBJECT

        friend class TcpServer;
        friend class Connection;

        public:
            /**
             * The default host address to watch.  Maps to QHostAddress::Any.
             */
            static const QHostAddress defaultHostAddress;

            /**
             * The default port to monitor.  Maps to port 8080.
             */
            static const unsigned short defaultPort;

            /**
             * The default maximum number of simultaneous connections.
             */
            static const unsigned defaultMaximumSimultaneousConnections;

            /**
             * Type for functions used to log events.  Note that the function must be fully reentrant and thread safe.
             *
             * \param[in] message The message to be displayed.
             *
             * \param[in] error   If true, the entry is an error.  If false, the entry is informational.
             */
            typedef void (*LoggingFunction)(const QString& message, bool error);

            /**
             * Constructor
             *
             * \param[in] parent      Pointer to the parent object.
             */
            Server(QObject* parent = nullptr);

            /**
             * Constructor
             *
             * \param[in] maximumNumberSimultaneousConnections The desired maximum number of simultaneous connections.
             *
             * \param[in] parent                               Pointer to the parent object.
             */
            Server(unsigned maximumNumberSimultaneousConnections, QObject* parent = nullptr);

            ~Server() override;

            /**
             * Method you can use to set the logging function to be used by this server.
             *
             * \param[in] newLoggingFunction The new logging function to be used.
             */
            void setLoggingFunction(LoggingFunction newLoggingFunction);

            /**
             * Method you can use to determine the logging function being used by this server.
             *
             * \return Returns a pointer to the current logging function.
             */
            LoggingFunction loggingFunction() const;

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
            void registerHandler(Handler* handler, Handler::Method method, const QString& path);

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
            Handler* handler(Handler::Method method, const QString& path) const;

        signals:
            /**
             * Signal that emitted when a session error is reported.
             *
             * \param[out] errorReason The reason provided for the error.
             */
            void sessionError(const QString& errorReason);

        private:
            /**
             * The underlying private implementation.
             */
            class Private;

            /**
             * The underlying TCP server that handles the raw incoming data.
             */
            Private* impl;
    };
};

#endif
