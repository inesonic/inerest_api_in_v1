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
