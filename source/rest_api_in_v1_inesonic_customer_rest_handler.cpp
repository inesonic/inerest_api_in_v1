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
#include "rest_api_in_v1_authentication_helpers.h"
#include "rest_api_in_v1_customer_data.h"
#include "rest_api_in_v1_json_response.h"
#include "rest_api_in_v1_rest_handler.h"
#include "rest_api_in_v1_inesonic_customer_rest_handler.h"
#include "rest_api_in_v1_inesonic_customer_rest_handler_private.h"

namespace RestApiInV1 {
    InesonicCustomerRestHandler::InesonicCustomerRestHandler(
            CustomerData* customerData
        ):impl(
            new Private(customerData)
        ) {}


    InesonicCustomerRestHandler::~InesonicCustomerRestHandler() {}


    JsonResponse InesonicCustomerRestHandler::processRequest(
            const QString&       path,
            const QJsonDocument& request,
            unsigned             threadId
        ) {
        JsonResponse response(StatusCode::BAD_REQUEST);

        if (request.isObject()) {
            QJsonObject jsonObject = request.object();
            if (jsonObject.size() == 3) {
                QJsonValue cidValue  = jsonObject.value("cid");
                QJsonValue dataValue = jsonObject.value("data");
                QJsonValue hashValue = jsonObject.value("hash");

                if (cidValue.isString() && dataValue.isString() && hashValue.isString()) {
                    QString cid = cidValue.toString();
                    unsigned long customerId = impl->customerId(cid, threadId);
                    if (customerId != 0) {
                        QByteArray secret = impl->customerSecret(customerId, threadId);

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
                                if (checkHash(*decodedData, *decodedHash, secret)) {
                                    QJsonParseError jsonParseError;
                                    QJsonDocument   jsonMessage = QJsonDocument::fromJson(
                                        *decodedData,
                                        &jsonParseError
                                    );
                                    if (jsonParseError.error == QJsonParseError::ParseError::NoError) {
                                        response = processAuthenticatedRequest(
                                            path,
                                            customerId,
                                            jsonMessage,
                                            threadId
                                        );
                                    }
                                } else {
                                    response.setStatusCode(StatusCode::UNAUTHORIZED);
                                }
                            }
                        }
                    } else {
                        response.setStatusCode(StatusCode::FORBIDDEN);
                    }
                }
            }
        }

        return response;
    }
}
