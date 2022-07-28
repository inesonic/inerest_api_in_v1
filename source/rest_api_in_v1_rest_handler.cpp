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
