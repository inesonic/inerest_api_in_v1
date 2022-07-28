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
* This file implements the \ref RestApiInV1::Connection class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_CONNECTION_H
#define REST_API_IN_V1_CONNECTION_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QSemaphore>
#include <QMutex>
#include <QQueue>
#include <QHostAddress>
#include <QTcpSocket>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_session.h"
#include "rest_api_in_v1_server_private.h"

namespace RestApiInV1 {
    /**
     * Class that manages communcation for a single session in a separate thread.
     */
    class REST_API_V1_PUBLIC_API Connection:public QThread, public Session {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] serverPrivate     The server instance.
             *
             * \param[in] socketDescriptor  The socket descriptor for the socket to tie to this session.
             *
             * \param[in] threadId          A value used to uniquely identify this thread.   Note that values are
             *                              recycled but will always be unique while in flight.
             *
             * \param[in] maximumBufferSize The maximum amount of data we are allowed to read at once.
             *
             * \param[in] loggingFunction   The function to be used for logging.
             *
             * \param[in] parent            The pointer to the parent object.
             */
            Connection(
                Server::Private*        server,
                qintptr                 socketDescriptor,
                unsigned                threadId,
                unsigned long           maximumBufferSize,
                Server::LoggingFunction loggingFunction,
                QObject*                parent = nullptr
            );

            ~Connection() override;

            /**
             * Method you can use to get the thread ID of this thread.
             *
             * \return Returns the thread ID.
             */
            unsigned threadId() const final;

            /**
             * Method you can use to get the next bolus of available data.  This method will block until there is at
             * least some data available.
             *
             * \param[in,out] buffer      The buffer to hold the current receive data.
             *
             * \param[in]     maximumSize The maximum number of bytes of data to obtain.  A value of 0 indicates that
             *                            the current default maximum value should be used.
             *
             * \return Returns true on success or false on error.
             */
            bool readData(QByteArray& buffer, unsigned long maximumSize = 0) final;

            /**
             * Method you can use to get a single line of data.  This method will block until a full line is read.
             *
             * \param[in]  maximumSize The maximum number of bytes of data to obtain.  A value of 0 indicates that
             *                         the current default maximum value should be used.
             *
             * \param[out] ok          An optional pointer to a boolean value holding true on success or false on
             *                         error.
             *
             * \return Returns the read line.  The newline character will be automatically removed.
             */
            QByteArray readLine(unsigned long maximumSize = 0, bool* ok = nullptr) final;

            /**
             * Method you can use to send response status.  Always call this method before sending any response data.
             *
             * \param[in] statusCode      The response status code.
             *
             * \param[in[ responseHeaders The response headers.
             *
             * \return Returns true on success.  Returns false on error.
             */
            bool sendResponseHeader(
                Handler::StatusCode     statusCode,
                const Handler::Headers& responseHeaders = Handler::Headers()
            ) final;

            /**
             * Method you can use to send a failed response including simple response data.
             *
             * \param[in] statusCode      The response status code.
             *
             * \return Returns true on success.  Returns false on error.
             */
            bool sendFailedResponse(Handler::StatusCode statusCode) final;

            /**
             * Method you can use to send response data.
             *
             * \param[in] data The raw data to be sent.
             *
             * \return Returns true on success.  Returns false on error.
             */
            bool sendData(const QByteArray& data) final;

            /**
             * Method you can use to obtain the current request URI.
             *
             * \return Returns the current request URI.
             */
            const QUrl& requestUri() const final;

            /**
             * Method you can use to obtain the current access method.
             *
             * \return Returns the current request URI.
             */
            Handler::Method method() const final;

            /**
             * Method you can use to obtain the HTTP version as a string.
             *
             * \return Returns the HTTP version as a string.
             */
            const QString& httpVersion() const final;

            /**
             * Method you can use to obtain the current HTTP headers.
             *
             * \return Returns the current HTTP headers.
             */
            const Handler::Headers& headers() const final;

        signals:
            /**
             * Signal that is emitted when the thread finishes.
             *
             * \param[out] connection A pointer to this instance.
             */
            void sessionClosed(Connection* connection);

            /**
             * Signal that is emitted if we could not bind to this socket.
             *
             * \param[out] errorReason The reason provided for the error.
             */
            void sessionError(const QString& errorReason);

        public slots:
            /**
             * Method you can use to set the underlying socket descriptor and to start processing of the request.
             *
             * \param[in] socketDescriptor The socket descriptor to be processed.
             */
            void startSession(qintptr socketDescriptor);

        private:
            /**
             * Hash table used to convert handler methods into strings.
             */
            static const QHash<QByteArray, Handler::Method> handlerMethodsByString;

            /**
             * Hash table of response reason phrases by status code.
             */
            static const QHash<Handler::StatusCode, QByteArray> reasonPhraseByStatusCode;

            /**
             * The default reason phrase to use if a status code is not recognized.
             */
            static const QByteArray defaultReasonPhrase;

            /**
             * A preconstructed byte array containing a single space.
             */
            static const QByteArray space;

            /**
             * A preconstructed byte array containing a newline.
             */
            static const QByteArray newline;

            /**
             * A preconstructed byte array containing a single colon.
             */
            static const QByteArray colon;

            /**
             * Method that manages the underlying connection.
             */
            void run() override;

            /**
             * Method that handles the incoming request and outgoing response.
             */
            void processRequest();

            /**
             * Method that writes a log entry.
             *
             * \param[in] message The log message to be included.
             *
             * \param[in] error   If true, then this is an error message.
             */
            void writeLog(const QString& message, bool error) const;

            /**
             * Method that parses a method string into a handler method.
             *
             * \param[in] methodString The method string to be parsed.
             *
             * \return Returns the method enumerated value.  The value Handler::Method::NUMBER_METHODS is returned if
             *         the supplied method is unknown.
             */
            static Handler::Method toMethod(const QByteArray& methodString);

            /**
             * The underlying server private instance.
             */
            Server::Private* currentServerPrivate;

            /**
             * The current socket descriptor.
             */
            qintptr currentSocketDescriptor;

            /**
             * The current thread ID of this thread.
             */
            unsigned currentThreadId;

            /**
             * The current maximum read buffer size.
             */
            unsigned long currentMaximumBufferSize;

            /**
             * Value holding the current number of bytes available for reading.
             */
            unsigned long long bytesAvailable;

            /**
             * Mutex used to gate this session.
             */
            QSemaphore currentSessionSemaphore;

            /**
             * The active socket.
             */
            QTcpSocket* currentSocket;

            /**
             * The request URI for this connection.
             */
            QUrl currentRequestUri;

            /**
             * The current connection method.
             */
            Handler::Method currentMethod;

            /**
             * The current HTTP version.
             */
            QString currentHttpVersion;

            /**
             * The current request headers.
             */
            Handler::Headers currentHeaders;

            /**
             * The returned status code.
             */
            Handler::StatusCode returnedStatusCode;

            /**
             * The current logging function.
             */
            Server::LoggingFunction currentLoggingFunction;
    };
};

#endif
