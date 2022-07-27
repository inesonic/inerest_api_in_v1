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
* This file implements the \ref RestApiInV1::Connection class.
***********************************************************************************************************************/

#include <QString>
#include <QByteArray>
#include <QTcpServer>
#include <QHash>
#include <QUrl>

#include <utility>
#include <iostream>

#include "rest_api_in_v1_server.h"
#include "rest_api_in_v1_server_private.h"
#include "rest_api_in_v1_handler.h"
#include "rest_api_in_v1_connection.h"

namespace RestApiInV1 {
    const QHash<QByteArray, Handler::Method> Connection::handlerMethodsByString(
        {
            std::pair<QByteArray, Handler::Method>(QByteArray("delete"),      Handler::Method::DELETE),
            std::pair<QByteArray, Handler::Method>(QByteArray("get"),         Handler::Method::GET),
            std::pair<QByteArray, Handler::Method>(QByteArray("head"),        Handler::Method::HEAD),
            std::pair<QByteArray, Handler::Method>(QByteArray("post"),        Handler::Method::POST),
            std::pair<QByteArray, Handler::Method>(QByteArray("put"),         Handler::Method::PUT),
            std::pair<QByteArray, Handler::Method>(QByteArray("connect"),     Handler::Method::CONNECT),
            std::pair<QByteArray, Handler::Method>(QByteArray("options"),     Handler::Method::OPTIONS),
            std::pair<QByteArray, Handler::Method>(QByteArray("trace"),       Handler::Method::TRACE),
            std::pair<QByteArray, Handler::Method>(QByteArray("copy"),        Handler::Method::COPY),
            std::pair<QByteArray, Handler::Method>(QByteArray("lock"),        Handler::Method::LOCK),
            std::pair<QByteArray, Handler::Method>(QByteArray("mkcol"),       Handler::Method::MKCOL),
            std::pair<QByteArray, Handler::Method>(QByteArray("move"),        Handler::Method::MOVE),
            std::pair<QByteArray, Handler::Method>(QByteArray("propfind"),    Handler::Method::PROPFIND),
            std::pair<QByteArray, Handler::Method>(QByteArray("proppatch"),   Handler::Method::PROPPATCH),
            std::pair<QByteArray, Handler::Method>(QByteArray("search"),      Handler::Method::SEARCH),
            std::pair<QByteArray, Handler::Method>(QByteArray("unlock"),      Handler::Method::UNLOCK),
            std::pair<QByteArray, Handler::Method>(QByteArray("bind"),        Handler::Method::BIND),
            std::pair<QByteArray, Handler::Method>(QByteArray("rebind"),      Handler::Method::REBIND),
            std::pair<QByteArray, Handler::Method>(QByteArray("unbind"),      Handler::Method::UNBIND),
            std::pair<QByteArray, Handler::Method>(QByteArray("acl"),         Handler::Method::ACL),
            std::pair<QByteArray, Handler::Method>(QByteArray("report"),      Handler::Method::REPORT),
            std::pair<QByteArray, Handler::Method>(QByteArray("mkactivity"),  Handler::Method::MKACTIVITY),
            std::pair<QByteArray, Handler::Method>(QByteArray("checkout"),    Handler::Method::CHECKOUT),
            std::pair<QByteArray, Handler::Method>(QByteArray("merge"),       Handler::Method::MERGE),
            std::pair<QByteArray, Handler::Method>(QByteArray("msearch"),     Handler::Method::MSEARCH),
            std::pair<QByteArray, Handler::Method>(QByteArray("notify"),      Handler::Method::NOTIFY),
            std::pair<QByteArray, Handler::Method>(QByteArray("subscribe"),   Handler::Method::SUBSCRIBE),
            std::pair<QByteArray, Handler::Method>(QByteArray("unsubscribe"), Handler::Method::UNSUBSCRIBE),
            std::pair<QByteArray, Handler::Method>(QByteArray("patch"),       Handler::Method::PATCH),
            std::pair<QByteArray, Handler::Method>(QByteArray("purge"),       Handler::Method::PURGE),
            std::pair<QByteArray, Handler::Method>(QByteArray("mkcalendar"),  Handler::Method::MKCALENDAR),
            std::pair<QByteArray, Handler::Method>(QByteArray("link"),        Handler::Method::LINK),
            std::pair<QByteArray, Handler::Method>(QByteArray("unlink"),      Handler::Method::UNLINK)
        }
    );

    const QHash<Handler::StatusCode, QByteArray> Connection::reasonPhraseByStatusCode(
        {
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::CONTINUE,
                QByteArray("Continue")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::SWITCH_PROTOCOLS,
                QByteArray("Switching Protocols")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::OK,
                QByteArray("OK")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::CREATED,
                QByteArray("Created")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::ACCEPTED,
                QByteArray("Accepted")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::NON_AUTHORITATIVE_INFORMATION,
                QByteArray("Non-Authoritative Information")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::NO_CONTENT,
                QByteArray("No Content")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::RESET_CONTENT,
                QByteArray("Reset Content")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::PARTIAL_CONTENT,
                QByteArray("Partial Content")),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::MULTI_STATUS,
                QByteArray("Multi-Status")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::MULTIPLE_CHOICES,
                QByteArray("Multiple Choices")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::MOVED_PERMANENTLY,
                QByteArray("Moved Permanently")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::FOUND,
                QByteArray("Found")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::SEE_OTHER,
                QByteArray("See Other")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::NOT_MODIFIED,
                QByteArray("Not Modified")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::USE_PROXY,
                QByteArray("Use Proxy")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::TEMPORARY_REDIRECT,
                QByteArray("Temporary Redirect")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::BAD_REQUEST,
                QByteArray("Bad Request")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::UNAUTHORIZED,
                QByteArray("Unauthorized")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::PAYMENT_REQUIRED,
                QByteArray("Payment Required")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::FORBIDDEN,
                QByteArray("Forbidden")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::NOT_FOUND,
                QByteArray("Not Found")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::METHOD_NOT_ALLOWED,
                QByteArray("Method Not Allowed")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::NOT_ACCEPTABLE,
                QByteArray("Not Acceptable")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::PROXY_AUTHENTICATION_REQUIRED,
                QByteArray("Proxy Authentication Required")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::REQUEST_TIMEOUT,
                QByteArray("Request Time-out")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::CONFLICT,
                QByteArray("Conflict")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::GONE,
                QByteArray("Gone")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::LENGTH_REQUIRED,
                QByteArray("Length Required")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::PRECONDITION_FAILED,
                QByteArray("Precondition Failed")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::REQUEST_ENTITY_TOO_LARGE,
                QByteArray("Request Entity Too Large")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::REQUEST_URI_TOO_LONG,
                QByteArray("Request-URI Too Large")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::REQUEST_UNSUPPORTED_MEDIA_TYPE,
                QByteArray("Unsupported Media Type")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::REQUESTED_RANGE_NOT_SATISFIABLE,
                QByteArray("Requested range not satisfiable")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::EXPECTATION_FAILED,
                QByteArray("Expectation Failed")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::INTERNAL_SERVER_ERROR,
                QByteArray("Internal Server Error")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::NOT_IMPLEMENTED,
                QByteArray("Not Implemented")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::BAD_GATEWAY,
                QByteArray("Bad Gateway")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::SERVICE_UNAVAILABLE,
                QByteArray("Service Unavailable")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::GATEWAY_TIMEOUT,
                QByteArray("Gateway Time-out")
            ),
            std::pair<Handler::StatusCode, QByteArray>(
                Handler::StatusCode::HTTP_VERSION_NOT_SUPPORTED,
                QByteArray("HTTP Version not supported")
            )
        }
    );

    const QByteArray Connection::defaultReasonPhrase("Undefined Status");
    const QByteArray Connection::space(" ");
    const QByteArray Connection::newline("\r\n");
    const QByteArray Connection::colon(":");

    Connection::Connection(
            Server::Private*        serverPrivate,
            qintptr                 socketDescriptor,
            unsigned                threadId,
            unsigned long           maximumBufferSize,
            Server::LoggingFunction loggingFunction,
            QObject*                parent
        ):QThread(
            parent
        ),currentServerPrivate(
            serverPrivate
        ),currentSocketDescriptor(
            socketDescriptor
        ),currentThreadId(
            threadId
        ),currentMaximumBufferSize(
            maximumBufferSize
        ),bytesAvailable(
            0
        ),returnedStatusCode(
            Handler::StatusCode::OK
        ),currentLoggingFunction(
            loggingFunction
        ) {}


    Connection::~Connection() {}


    unsigned Connection::threadId() const {
        return currentThreadId;
    }


    bool Connection::readData(QByteArray& buffer, unsigned long maximumSize) {
        if (maximumSize == 0) {
            maximumSize = currentMaximumBufferSize;
        }

        unsigned long long spaceRemaining = std::max(0LL, static_cast<long long>(maximumSize - buffer.size()));

        bool success = true;
        if (bytesAvailable == 0) {
            success = currentSocket->waitForReadyRead();
            if (success) {
                bytesAvailable = currentSocket->bytesAvailable();
                unsigned long bytesToRead = std::min(bytesAvailable, spaceRemaining);
                buffer.append(currentSocket->read(bytesToRead));
                bytesAvailable -= bytesToRead;
            }
        } else {
            unsigned long bytesToRead = std::min(bytesAvailable, spaceRemaining);
            buffer.append(currentSocket->read(bytesToRead));
            bytesAvailable -= bytesToRead;
        }

        return success;
    }


    QByteArray Connection::readLine(unsigned long maximumSize, bool* ok) {
        QByteArray result;
        char       lastCharacter        = 0;
        bool       success              = true;
        bool       lastWasCarrageReturn = false;

        if (maximumSize == 0) {
            maximumSize = currentMaximumBufferSize;
        }

        while (success && lastCharacter != '\n' && static_cast<unsigned long>(result.size()) < maximumSize) {
            if (bytesAvailable == 0) {
                success = currentSocket->waitForReadyRead();
                if (success) {
                    bytesAvailable = currentSocket->bytesAvailable();
                    success = currentSocket->getChar(&lastCharacter);
                }
            } else {
                success = currentSocket->getChar(&lastCharacter);
            }

            if (success) {
                if (lastCharacter != '\n' && lastCharacter != '\r') {
                    if (lastWasCarrageReturn) {
                        result.append('\r');
                        lastWasCarrageReturn = false;
                    }

                    result.append(lastCharacter);
                } else if (lastCharacter == '\r') {
                    lastWasCarrageReturn = true;
                }

                --bytesAvailable;
            }
        }

        if (ok != nullptr) {
            *ok = success;
        }

        return result;
    }


    bool Connection::sendResponseHeader(Handler::StatusCode statusCode, const Handler::Headers& responseHeaders) {
        const QByteArray& reasonPhrase = reasonPhraseByStatusCode.value(statusCode, defaultReasonPhrase);

        // Send the status line.
        bool success = (
               sendData(currentHttpVersion.toUtf8())
            && sendData(space)
            && sendData(QByteArray::number(static_cast<unsigned>(statusCode)))
            && sendData(space)
            && sendData(reasonPhrase)
            && sendData(newline)
        );

        // And then the headers.
        Handler::Headers::const_iterator headerIterator    = responseHeaders.constBegin();
        Handler::Headers::const_iterator headerEndIterator = responseHeaders.constEnd();
        while (success && headerIterator != headerEndIterator) {
            success = (
                   sendData(headerIterator.key())
                && sendData(colon)
                && sendData(headerIterator.value())
                && sendData(newline)
            );
            ++headerIterator;
        }

        // A blank line indicates the end of the header.
        success = success && sendData(newline);

        returnedStatusCode = statusCode;

        return success;
    }


    bool Connection::sendFailedResponse(Handler::StatusCode statusCode) {
        QByteArray body(
            "<html>"
              "<head>"
                "<title>{{ status_code }} {{ reason_phrase }}</title>"
              "</head>"
              "<body>"
                "<p align=\"center\">{{ status_code }} {{ reason_phrase }}</p>"
              "</body>"
            "</html>\n"
        );

        const QByteArray& reasonPhrase = reasonPhraseByStatusCode.value(statusCode, defaultReasonPhrase);
        body = body.replace("{{ status_code }}", QByteArray::number(static_cast<unsigned>(statusCode)))
                   .replace("{{ reason_phrase }}", reasonPhrase);

        Handler::Headers headers;
        headers.insert(Handler::contentLengthString, QByteArray::number(body.size()));
        headers.insert(Handler::contentTypeString, Handler::textHtmlString);
        headers.insert(Handler::connectionString, Handler::connectionCloseString);

        return sendResponseHeader(statusCode, headers) && sendData(body);
    }


    bool Connection::sendData(const QByteArray& data) {
        return currentSocket->write(data) == data.size();
    }


    const QUrl& Connection::requestUri() const {
        return currentRequestUri;
    }


    Handler::Method Connection::method() const {
        return currentMethod;
    }


    const QString& Connection::httpVersion() const {
        return currentHttpVersion;
    }


    const Handler::Headers& Connection::headers() const {
        return currentHeaders;
    }


    void Connection::startSession(qintptr socketDescriptor) {
        currentSocketDescriptor = socketDescriptor;
        currentSessionSemaphore.release(1);
    }


    void Connection::run() {
        QTcpSocket socket;
        bool success = socket.setSocketDescriptor(currentSocketDescriptor);
        if (success) {
            currentSocket = &socket;
            processRequest();

            socket.disconnectFromHost();
            if (socket.state() != QTcpSocket::SocketState::UnconnectedState) {
                socket.waitForDisconnected();
            }
        } else {
            currentServerPrivate->sessionError(
                QString("Could not bind to socket: %1").arg(socket.errorString())
            );
        }
    }


    void Connection::processRequest() {
        bool         success;
        QByteArray   requestLine     = readLine(0, &success);
        if (success) {
            QList<QByteArray> requestLineParts = requestLine.split(' ');
            unsigned          numberParts      = static_cast<unsigned>(requestLineParts.size());

            if (numberParts >= 3) {
                const QByteArray* methodString     = nullptr;
                const QByteArray* requestUriString = nullptr;
                const QByteArray* httpVersion      = nullptr;

                unsigned nextField = 0;
                unsigned index     = 0;
                while (success && index<numberParts) {
                    const QByteArray& field = requestLineParts.at(index);
                    if (field.size() > 0) {
                        if (nextField == 0) {
                            methodString = &field;
                        } else if (nextField == 1) {
                            requestUriString = &field;
                        } else if (nextField == 2) {
                            httpVersion = &field;
                        } else {
                            success = false;
                            sendFailedResponse(Handler::StatusCode::BAD_REQUEST);

                            QString message = QString("Malformed HTTP request line: %1")
                                              .arg(QString::fromUtf8(requestLine));

                            writeLog(message, false);
                            currentServerPrivate->sessionError(message);
                        }

                        ++nextField;
                    }

                    ++index;
                }

                if (success && nextField < 3) {
                    success = false;
                    sendFailedResponse(Handler::StatusCode::BAD_REQUEST);

                    QString message = QString("Malformed HTTP request line: %1").arg(QString::fromUtf8(requestLine));
                    writeLog(message, false);
                    currentServerPrivate->sessionError(message);
                }

                if (success) {
                    currentRequestUri.setUrl(QString::fromUtf8(*requestUriString));
                    currentMethod      = toMethod(*methodString);
                    currentHttpVersion = QString::fromUtf8(*httpVersion);

                    currentHeaders.clear();
                    QByteArray receivedHeader;
                    do {
                        receivedHeader = readLine(0, &success).trimmed();
                        if (!success) {
                            QString message = QString("Falied to read content: %1").arg(currentSocket->errorString());

                            sendFailedResponse(Handler::StatusCode::BAD_REQUEST);

                            writeLog(message, false);
                            currentServerPrivate->sessionError(message);
                        }

                        if (success && !receivedHeader.isEmpty()) {
                            int splitIndex = receivedHeader.indexOf(':');
                            if (splitIndex >= 0) {
                                QByteArray headerName  = receivedHeader.left(splitIndex);
                                QByteArray headerValue = receivedHeader.mid(splitIndex + 1);
                                currentHeaders.insert(headerName.toLower(), headerValue.trimmed());
                            } else {
                                currentHeaders.insert(receivedHeader, QByteArray());
                            }
                        }
                    } while (success && !receivedHeader.isEmpty());

                    if (success) {
                        QString  path    = currentRequestUri.path();
                        Handler* handler = currentServerPrivate->handler(currentMethod, path);

                        if (handler != nullptr) {
                            handler->session(*this);
                            writeLog(path, false);
                        } else {
                            // When we're behind a proxy, we lose our host and scheme which screws up the path calculation above.  We address that here.

                            QString host = currentRequestUri.host();
                            path    = QString("/") + host + path;
                            handler = currentServerPrivate->handler(currentMethod, path);

                            if (handler != nullptr) {
                                handler->session(*this);
                            } else {
                                success = false;
                                sendFailedResponse(Handler::StatusCode::NOT_FOUND);

                                QString message = QString("Invalid URI: %1").arg(QString::fromUtf8(*requestUriString));
                                writeLog(message, false);
                                currentServerPrivate->sessionError(message);
                            }
                        }

                    }
                }
            } else {
                sendFailedResponse(Handler::StatusCode::BAD_REQUEST);

                QString message = QString("Malformed HTTP request line: %1").arg(QString::fromUtf8(requestLine));
                writeLog(message, false);
                currentServerPrivate->sessionError(message);

                success = false;
            }
        } else {
            QString message = QString("Falied to read content: %1").arg(currentSocket->errorString());

            sendFailedResponse(Handler::StatusCode::BAD_REQUEST);

            writeLog(message, false);
            currentServerPrivate->sessionError(message);

            success = false;
        }
    }


    void Connection::writeLog(const QString& message, bool error) const {
        if (currentLoggingFunction != nullptr) {
            QString                          peerAddress;
            Handler::Headers::const_iterator headerIterator = currentHeaders.find(Handler::xRealIPString);
            if (headerIterator != currentHeaders.constEnd()) {
                peerAddress = QString::fromUtf8(headerIterator.value());
            } else {
                headerIterator = currentHeaders.find(Handler::xForwardedForString);
                if (headerIterator != currentHeaders.constEnd()) {
                    peerAddress = QString::fromUtf8(headerIterator.value());
                } else {
                    peerAddress = currentSocket->peerAddress().toString();
                }
            }

            QString logEntry = QString("%2 -> %3: %4")
                               .arg(peerAddress)
                               .arg(static_cast<unsigned>(returnedStatusCode))
                               .arg(message);

            (*currentLoggingFunction)(logEntry, error);
        }
    }


    Handler::Method Connection::toMethod(const QByteArray& methodString) {
        return handlerMethodsByString.value(methodString.toLower(), Handler::Method::NUMBER_METHODS);
    }
}
