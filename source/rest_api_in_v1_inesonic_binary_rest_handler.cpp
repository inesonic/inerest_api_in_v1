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
