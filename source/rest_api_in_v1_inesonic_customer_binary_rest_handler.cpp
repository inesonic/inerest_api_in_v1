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

#include "rest_api_in_v1_session.h"
#include "rest_api_in_v1_customer_data.h"
#include "rest_api_in_v1_binary_response.h"
#include "rest_api_in_v1_authentication_helpers.h"
#include "rest_api_in_v1_handler.h"
#include "rest_api_in_v1_inesonic_customer_binary_rest_handler.h"
#include "rest_api_in_v1_inesonic_customer_binary_rest_handler_private.h"

namespace RestApiInV1 {
    InesonicCustomerBinaryRestHandler::InesonicCustomerBinaryRestHandler(
            CustomerData* customerData
        ):impl(
            new Private(customerData)
        ) {}


    InesonicCustomerBinaryRestHandler::~InesonicCustomerBinaryRestHandler() {}


    void InesonicCustomerBinaryRestHandler::session(Session& session) {
        StatusCode statusCode = StatusCode::BAD_REQUEST;
        QByteArray payload;
        QString    responseContentType;

        QString path = session.requestUri().path();

        QByteArray contentType = session.headers().value(contentTypeString);
        if (contentType == applicationJsonString) {
            bool               success;
            QByteArray         contentLengthArray = session.headers().value(contentLengthString);
            unsigned long long contentLength      = contentLengthArray.toULongLong(&success);

            QByteArray receivedData;
            while (success && static_cast<unsigned long long>(receivedData.size()) < contentLength) {
                success = session.readData(receivedData);
            }

            if (success) {
                QJsonParseError jsonParseError;
                QJsonDocument   document = QJsonDocument::fromJson(receivedData, &jsonParseError);
                if (jsonParseError.error == QJsonParseError::ParseError::NoError && document.isObject()) {
                    QJsonObject jsonObject = document.object();
                    if (jsonObject.size() == 3) {
                        QJsonValue cidValue  = jsonObject.value("cid");
                        QJsonValue dataValue = jsonObject.value("data");
                        QJsonValue hashValue = jsonObject.value("hash");
                        unsigned   threadId  = session.threadId();

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
                                          QByteArray::Base64Option::Base64Encoding
                                        | QByteArray::AbortOnBase64DecodingErrors
                                    );

                                    if (decodedData.decodingStatus == QByteArray::Base64DecodingStatus::Ok) {
                                        if (checkHash(*decodedData, *decodedHash, secret)) {
                                            QJsonParseError jsonParseError;
                                            QJsonDocument   jsonMessage = QJsonDocument::fromJson(
                                                *decodedData,
                                                &jsonParseError
                                            );
                                            if (jsonParseError.error == QJsonParseError::ParseError::NoError) {
                                                BinaryResponse response = processAuthenticatedRequest(
                                                    path,
                                                    customerId,
                                                    jsonMessage,
                                                    threadId
                                                );

                                                statusCode          = response.statusCode();
                                                responseContentType = response.contentType();
                                                payload             = response.asByteArray();
                                            }
                                        } else {
                                            statusCode = StatusCode::UNAUTHORIZED;
                                        }
                                    }
                                }
                            } else {
                                statusCode = StatusCode::FORBIDDEN;
                            }
                        }
                    }
                }
            } else {
                statusCode = StatusCode::INTERNAL_SERVER_ERROR;
            }
        } else {
            statusCode = StatusCode::PRECONDITION_FAILED;
        }

        if (statusCode == StatusCode::OK) {
            Headers headers;
            headers.insert(serverString, inesonicBotString);
            headers.insert(contentTypeString, responseContentType.toUtf8());
            headers.insert(contentLengthString, QByteArray::number(payload.size()));
            headers.insert(connectionString, connectionCloseString);

            bool success = session.sendResponseHeader(statusCode, headers);
            if (success) {
                session.sendData(payload);
            }
        } else {
            session.sendFailedResponse(statusCode);
        }
    }
}
