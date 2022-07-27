/*-*-c++-*-*************************************************************************************************************
* Copyright 2021 Inesonic, LLC.
*
* This file is licensed under two licenses:
*
* Inesonic Commercial License:
*   All rights reserved.  Unauthorized use is strictly prohibited under the terms of this license.  Inesonic, is
*   authorized to use this code against its own closed source applications and to redistribute this project under
*   different terms.
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
#include "rest_api_in_v1_inesonic_rest_handler_base.h"
#include "rest_api_in_v1_inesonic_rest_handler.h"

namespace RestApiInV1 {
    InesonicRestHandler::InesonicRestHandler(const QByteArray& secret):InesonicRestHandlerBase2(secret) {}


    InesonicRestHandler::~InesonicRestHandler() {}


    Handler::Response InesonicRestHandler::processBulkRequest(
            Handler::Method   /* method */,
            const QString&    path,
            const Headers&    headers,
            const QString&    /* remoteAddress */,
            const QByteArray& receivedData
        ) {
        Response response(QByteArray(), ResponseCode::BAD_REQUEST);

        const QByteArray& contentType = headers.value(contentTypeString, QByteArray());
        if (contentType == applicationJsonString || contentType == textPlainString) {
            QJsonParseError jsonParseError;
            QJsonDocument   json = QJsonDocument::fromJson(receivedData, &jsonParseError);
            if (jsonParseError.error == QJsonParseError::ParseError::NoError && json.isObject()) {
                QJsonObject jsonObject = json.object();
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
                                if (checkHash(*decodedData, *decodedHash)) {
                                    QJsonDocument jsonMessage = QJsonDocument::fromJson(*decodedData, &jsonParseError);
                                    if (jsonParseError.error == QJsonParseError::ParseError::NoError) {
                                        QJsonDocument jsonResponse = processRequest(path, jsonMessage);

                                        response.setResponseCode(ResponseCode::OK);
                                        response.setResponseData(
                                            jsonResponse.toJson(QJsonDocument::JsonFormat::Compact)
                                        );
                                    }
                                } else {
                                    response.setResponseCode(ResponseCode::UNAUTHORIZED);
                                }
                            }
                        }
                    }
                }
            }
        } else {
            response.setResponseCode(ResponseCode::PRECONDITION_FAILED);
        }

        return response;
    }
}
