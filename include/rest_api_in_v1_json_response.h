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

#ifndef REST_API_IN_V1_JSON_RESPONSE_H
#define REST_API_IN_V1_JSON_RESPONSE_H

#include <QString>
#include <QByteArray>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_response.h"

namespace RestApiInV1 {
    /**
     * Class you can use to send a JSON formattedresponse.
     */
    class JsonResponse:public QJsonDocument, public Response {
        public:
            /**
             * The default content type this class will report.
             */
            static const QByteArray defaultContentType;

            JsonResponse();

            /**
             * Constructor
             *
             * \param[in] jsonDocument The JSON document holding the JSON code to be sent.
             */
            inline JsonResponse(
                    const QJsonDocument& jsonDocument
                ):QJsonDocument(
                    jsonDocument
                ),Response(
                    StatusCode::OK,
                    defaultContentType
                ) {}

            /**
             * Constructor (move semantics)
             *
             * \param[in] jsonDocument The JSON document holding the JSON code to be sent.
             */
            inline JsonResponse(
                    QJsonDocument&& jsonDocument
                ):QJsonDocument(
                    jsonDocument
                ),Response(
                    StatusCode::OK,
                    defaultContentType
                ) {}

            /**
             * Constructor
             *
             * \param[in] jsonArray The JSON array holding the JSON code to be sent.
             */
            inline JsonResponse(
                    const QJsonArray& jsonArray
                ):QJsonDocument(
                    jsonArray
                ),Response(
                    StatusCode::OK,
                    defaultContentType
                ) {}

            /**
             * Constructor
             *
             * \param[in] jsonObject The JSON object holding the JSON code to be sent.
             */
            inline JsonResponse(
                    const QJsonObject& jsonObject
                ):QJsonDocument(
                     jsonObject
                ),Response(
                     StatusCode::OK,
                     defaultContentType
                ) {}

            /**
             * Constructor
             *
             * \param[in] statusCode The status code to be sent.
             */
            inline JsonResponse(StatusCode statusCode):Response(statusCode, defaultContentType) {}

            /**
             * Constructor
             *
             * \param[in] statusCode   The status code to be sent.
             *
             * \param[in] jsonDocument The JSON document holding the JSON code to be sent.
             */
            inline JsonResponse(
                    StatusCode           statusCode,
                    const QJsonDocument& jsonDocument
                ):QJsonDocument(
                    jsonDocument
                ),Response(
                    statusCode,
                    defaultContentType
                ) {}

            /**
             * Constructor (move semantics)
             *
             * \param[in] statusCode   The status code to be sent.
             *
             * \param[in] jsonDocument The JSON document holding the JSON code to be sent.
             */
            inline JsonResponse(
                    StatusCode      statusCode,
                    QJsonDocument&& jsonDocument
                ):QJsonDocument(
                    jsonDocument
                ),Response(
                    statusCode,
                    defaultContentType
                ) {}

            /**
             * Constructor
             *
             * \param[in] statusCode The status code to be sent.
             *
             * \param[in] jsonArray The JSON array holding the JSON code to be sent.
             */
            inline JsonResponse(
                    StatusCode        statusCode,
                    const QJsonArray& jsonArray
                ):QJsonDocument(
                    jsonArray
                ),Response(
                    statusCode,
                    defaultContentType
                ) {}

            /**
             * Constructor
             *
             * \param[in] statusCode The status code to be sent.
             *
             * \param[in] jsonObject The JSON object holding the JSON code to be sent.
             */
            inline JsonResponse(
                    StatusCode         statusCode,
                    const QJsonObject& jsonObject
                ):QJsonDocument(
                    jsonObject
                ),Response(
                    statusCode,
                    defaultContentType
                ) {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            inline JsonResponse(
                    const JsonResponse& other
                ):QJsonDocument(
                    other
                ),Response(
                    other
                ) {}

            /**
             * Move constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            inline JsonResponse(
                    JsonResponse&& other
                ):QJsonDocument(
                    other
                ),Response(
                    other
                ) {}

            ~JsonResponse() override;

            /**
             * Method you can use to determine if this is a binary or JSON response.
             *
             * \return Returns true if this is a binary response.  Returns false if this is a JSON response.
             */
            bool isBinaryResponse() const final;

            /**
             * Method you can use to obtain a strict binary representation of the data.
             *
             * \return Returns a QByteArray holding the result.
             */
            QByteArray asByteArray() const final;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare to this instance
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            bool operator==(const JsonResponse& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare to this instance
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            bool operator!=(const JsonResponse& other) const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            JsonResponse& operator=(const JsonResponse& other);

            /**
             * Assignment operator (move semantics)
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            JsonResponse& operator=(JsonResponse&& other);
    };
};

#endif
