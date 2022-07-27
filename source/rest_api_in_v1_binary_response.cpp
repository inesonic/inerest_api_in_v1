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
* This file implements the \ref RestApiInV1::BinaryResponse class.
***********************************************************************************************************************/

#include <QByteArray>

#include "rest_api_in_v1_response.h"
#include "rest_api_in_v1_binary_response.h"

namespace RestApiInV1 {
    const QByteArray BinaryResponse::defaultContentType("application/octet-stream");

    BinaryResponse::BinaryResponse():Response(BinaryResponse::StatusCode::OK, defaultContentType) {}


    BinaryResponse::BinaryResponse(
            const QByteArray& byteArray
        ):QByteArray(
            byteArray
        ),Response(
            BinaryResponse::StatusCode::OK,
            defaultContentType
        ) {}


    BinaryResponse::BinaryResponse(
            QByteArray&& byteArray
        ):QByteArray(
            byteArray
        ),Response(
            BinaryResponse::StatusCode::OK,
            defaultContentType
        ) {}


    BinaryResponse::BinaryResponse(
            const char* rawData,
            int         size
        ):QByteArray(
            rawData,
            size
        ),Response(
            BinaryResponse::StatusCode::OK,
            defaultContentType
        ) {}


    BinaryResponse::BinaryResponse(
            unsigned size,
            char     ch
        ):QByteArray(
            size,
            ch
        ),Response(
            BinaryResponse::StatusCode::OK,
            defaultContentType
        ) {}


    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode
        ):Response(
            statusCode,
            defaultContentType
        ) {}

    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            const QByteArray&          byteArray
        ):QByteArray(
            byteArray
        ),Response(
            statusCode,
            defaultContentType
        ) {}


    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            QByteArray&&               byteArray
        ):QByteArray(
            byteArray
        ),Response(
            statusCode,
            defaultContentType
        ) {}


    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            const char*                rawData,
            int                        size
        ):QByteArray(
            rawData,
            size
        ),Response(
            statusCode,
            defaultContentType
        ) {}


    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            unsigned                   size,
            char                       ch
        ):QByteArray(
            size,
            ch
        ),Response(
            statusCode,
            defaultContentType
        ) {}


    BinaryResponse::BinaryResponse(
            const QString& contentType
        ):Response(
            BinaryResponse::StatusCode::OK,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(
            const QString&    contentType,
            const QByteArray& byteArray
        ):QByteArray(
            byteArray
        ),Response(
            BinaryResponse::StatusCode::OK,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(
            const QString& contentType,
            QByteArray&&   byteArray
        ):QByteArray(
            byteArray
        ),Response(
            BinaryResponse::StatusCode::OK,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(
            const QString& contentType,
            const char*    rawData,
            int            size
        ):QByteArray(
            rawData,
            size
        ),Response(
            BinaryResponse::StatusCode::OK,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(
            const QString& contentType,
            unsigned       size,
            char           ch
        ):QByteArray(
            size,
            ch
        ),Response(
            BinaryResponse::StatusCode::OK,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            const QString&             contentType
        ):Response(
            statusCode,
            contentType.toLatin1()
        ) {}

    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            const QString&             contentType,
            const QByteArray&          byteArray
        ):QByteArray(
            byteArray
        ),Response(
            statusCode,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            const QString&             contentType,
            QByteArray&&               byteArray
        ):QByteArray(
            byteArray
        ),Response(
            statusCode,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            const QString&             contentType,
            const char*                rawData,
            int                        size
        ):QByteArray(
            rawData,
            size
        ),Response(
            statusCode,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(
            BinaryResponse::StatusCode statusCode,
            const QString&             contentType,
            unsigned                   size,
            char                       ch
        ):QByteArray(
            size,
            ch
        ),Response(
            statusCode,
            contentType.toLatin1()
        ) {}


    BinaryResponse::BinaryResponse(const BinaryResponse& other):QByteArray(other),Response(other) {}


    BinaryResponse::BinaryResponse(BinaryResponse&& other):QByteArray(other),Response(other) {}


    BinaryResponse::~BinaryResponse() {}


    bool BinaryResponse::isBinaryResponse() const {
        return true;
    }


    QByteArray BinaryResponse::asByteArray() const {
        return *this;
    }


    bool BinaryResponse::operator==(const BinaryResponse& other) const {
        return Response::operator==(other) && QByteArray::operator==(other);
    }


    bool BinaryResponse::operator!=(const BinaryResponse& other) const {
        return Response::operator!=(other) || QByteArray::operator!=(other);
    }


    BinaryResponse& BinaryResponse::operator=(const BinaryResponse& other) {
        QByteArray::operator=(other);
        Response::operator=(other);

        return *this;
    }


    BinaryResponse& BinaryResponse::operator=(BinaryResponse&& other) {
        QByteArray::operator=(other);
        Response::operator=(other);

        return *this;
    }
}
