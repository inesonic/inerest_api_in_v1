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
* This file implements the \ref RestApiInV1::JsonResponse class.
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

#include "rest_api_in_v1_response.h"
#include "rest_api_in_v1_json_response.h"

namespace RestApiInV1 {
    const QByteArray JsonResponse::defaultContentType("application/json");

    JsonResponse::JsonResponse():Response(JsonResponse::StatusCode::OK, JsonResponse::defaultContentType) {}


    JsonResponse::~JsonResponse() {}


    bool JsonResponse::isBinaryResponse() const {
        return false;
    }


    QByteArray JsonResponse::asByteArray() const {
        return toJson(JsonFormat::Compact);
    }

    bool JsonResponse::operator==(const JsonResponse& other) const {
        return Response::operator==(other) && QJsonDocument::operator==(other);
    }


    bool JsonResponse::operator!=(const JsonResponse& other) const {
        return Response::operator!=(other) || QJsonDocument::operator!=(other);
    }


    JsonResponse& JsonResponse::operator=(const JsonResponse& other) {
        QJsonDocument::operator=(other);
        Response::operator=(other);

        return *this;
    }


    JsonResponse& JsonResponse::operator=(JsonResponse&& other) {
        QJsonDocument::operator=(other);
        Response::operator=(other);

        return *this;
    }
}
