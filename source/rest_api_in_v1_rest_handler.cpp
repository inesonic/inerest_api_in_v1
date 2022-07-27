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
* This file implements the \ref RestApiInV1::RestHandler class.
***********************************************************************************************************************/

#include <QString>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonDocument>

#include "rest_api_in_v1_session.h"
#include "rest_api_in_v1_json_response.h"
#include "rest_api_in_v1_handler.h"
#include "rest_api_in_v1_rest_handler.h"

namespace RestApiInV1 {
    RestHandler::RestHandler() {}


    RestHandler::~RestHandler() {}


    void RestHandler::session(Session& session) {
        QString path = session.requestUri().path();

        QByteArray contentType = session.headers().value(contentTypeString);
        if (contentType == applicationJsonString || contentType == textPlainString) {
            QByteArray receivedData;
            bool success = session.readData(receivedData);
            if (success) {
                QJsonParseError jsonParseError;
                QJsonDocument   request = QJsonDocument::fromJson(receivedData, &jsonParseError);

                if (jsonParseError.error == QJsonParseError::ParseError::NoError) {
                    JsonResponse response = processRequest(path, request, session.threadId());
                    if (response.statusCode() == StatusCode::OK) {
                        QByteArray responsePayload = response.toJson(QJsonDocument::JsonFormat::Compact);

                        Headers headers;
                        headers.insert(serverString, inesonicBotString);
                        headers.insert(contentTypeString, response.contentType());
                        headers.insert(contentLengthString, QByteArray::number(responsePayload.size()));
                        headers.insert(connectionString, connectionCloseString);

                        success = session.sendResponseHeader(response.statusCode(), headers);
                        if (success) {
                            session.sendData(responsePayload);
                        }
                    } else {
                        session.sendResponseHeader(response.statusCode());
                    }
                } else {
                    session.sendFailedResponse(StatusCode::BAD_REQUEST);
                }
            } else {
                session.sendFailedResponse(StatusCode::INTERNAL_SERVER_ERROR);
            }
        } else {
            session.sendFailedResponse(StatusCode::PRECONDITION_FAILED);
        }
    }
}
