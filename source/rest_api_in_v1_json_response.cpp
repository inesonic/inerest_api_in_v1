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
