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
#include "rest_api_in_v1_json_response.h"
#include "rest_api_in_v1_inesonic_rest_handler_base.h"
#include "rest_api_in_v1_inesonic_rest_handler_base_private.h"
#include "rest_api_in_v1_inesonic_rest_handler.h"

namespace RestApiInV1 {
    InesonicRestHandler::InesonicRestHandler(
            const QByteArray& secret
        ):InesonicRestHandlerBase(
            secret
        ) {}


    InesonicRestHandler::~InesonicRestHandler() {}


    JsonResponse InesonicRestHandler::processRequest(
            const QString&       path,
            const QJsonDocument& request,
            unsigned             threadId
        ) {
        JsonResponse response(StatusCode::BAD_REQUEST);

        if (request.isObject()) {
            QJsonObject jsonObject = request.object();
            if (jsonObject.size() == 2) {
                QJsonValue dataValue = jsonObject.value("data");
                QJsonValue hashValue = jsonObject.value("hash");

                if (dataValue.isString() && hashValue.isString()) {
                    QByteArray::FromBase64Result decodedHash = QByteArray::fromBase64Encoding(
                        hashValue.toString().toUtf8(),
                        QByteArray::Base64Option::Base64Encoding | QByteArray::AbortOnBase64DecodingErrors
                    );

                    if (decodedHash.decodingStatus == QByteArray::Base64DecodingStatus::Ok) {
                        QByteArray::FromBase64Result decodedData = QByteArray::fromBase64Encoding(
                            dataValue.toString().toUtf8(),
                            QByteArray::Base64Option::Base64Encoding | QByteArray::AbortOnBase64DecodingErrors
                        );

                        if (decodedData.decodingStatus == QByteArray::Base64DecodingStatus::Ok) {
                            if (impl->checkHash(*decodedData, *decodedHash)) {
                                QJsonParseError jsonParseError;
                                QJsonDocument   jsonMessage = QJsonDocument::fromJson(*decodedData, &jsonParseError);
                                if (jsonParseError.error == QJsonParseError::ParseError::NoError) {
                                    response = processAuthenticatedRequest(path, jsonMessage, threadId);
                                }
                            } else {
                                response.setStatusCode(StatusCode::UNAUTHORIZED);
                            }
                        }
                    }
                }
            }
        }

        return response;
    }
}
