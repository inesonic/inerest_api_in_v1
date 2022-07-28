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
* This file implements the \ref RestApiInV1::InesonicRestHandler class.
***********************************************************************************************************************/

#include <QString>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>

#include <cstring>
#include <cstdint>

#include "rest_api_in_v1_handler.h"
#include "rest_api_in_v1_session.h"
#include "rest_api_in_v1_response.h"
#include "rest_api_in_v1_json_response.h"
#include "rest_api_in_v1_binary_response.h"
#include "rest_api_in_v1_authentication_helpers.h"
#include "rest_api_in_v1_inesonic_rest_handler_base.h"
#include "rest_api_in_v1_inesonic_rest_handler_base_private.h"
#include "rest_api_in_v1_inesonic_binary_rest_handler.h"

namespace RestApiInV1 {
    InesonicBinaryRestHandler::InesonicBinaryRestHandler(const QByteArray& secret):InesonicRestHandlerBase(secret) {}


    InesonicBinaryRestHandler::~InesonicBinaryRestHandler() {}


    unsigned long long InesonicBinaryRestHandler::maximumPayloadSize() const {
        return defaultMaximumPayloadSize;
    }


    void InesonicBinaryRestHandler::session(Session& session) {
        QString path = session.requestUri().path();

        QByteArray contentType = session.headers().value(contentTypeString);
        if (contentType == applicationOctetStreamString) {
            bool               success;
            QByteArray         contentLengthArray = session.headers().value(contentLengthString);
            unsigned long long contentLength      = contentLengthArray.toULongLong(&success);
            unsigned long      maximumBufferSize  = InesonicBinaryRestHandler::maximumPayloadSize();

            if (contentLength <= maximumBufferSize) {
                QByteArray receivedData;
                while (success && static_cast<unsigned long long>(receivedData.size()) < contentLength) {
                    success = session.readData(receivedData, maximumBufferSize);
                }

                if (success) {
                    QByteArray rawHash = receivedData.right(RestApiInV1::inesonicHashLength);
                    QByteArray rawData = receivedData.left(receivedData.size() - inesonicHashLength);

                    if (impl->checkHash(rawData, rawHash)) {
                        Response* response = processAuthenticatedRequest(path, rawData, session.threadId());
                        if (response != nullptr) {
                            QByteArray responsePayload = response->asByteArray();

                            Headers headers;
                            headers.insert(serverString, inesonicBotString);
                            headers.insert(contentTypeString, response->contentType());
                            headers.insert(contentLengthString, QByteArray::number(responsePayload.size()));
                            headers.insert(connectionString, connectionCloseString);

                            success = session.sendResponseHeader(response->statusCode(), headers);
                            if (success) {
                                success = session.sendData(responsePayload);
                            }

                            delete response;
                        } else {
                            session.sendFailedResponse(StatusCode::INTERNAL_SERVER_ERROR);
                        }
                    } else {
                        session.sendFailedResponse(StatusCode::UNAUTHORIZED);
                    }
                }
            } else {
                session.sendFailedResponse(StatusCode::PRECONDITION_FAILED);
            }
        } else {
            session.sendFailedResponse(StatusCode::PRECONDITION_FAILED);
        }
    }
}
