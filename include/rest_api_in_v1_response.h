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
* This file implements the \ref RestApiInV1::Response class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_RESPONSE_H
#define REST_API_IN_V1_RESPONSE_H
#include <QString>
#include <QByteArray>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_handler.h"

namespace RestApiInV1 {
    /**
     * Common base class for each REST API response type.
     */
    class Response {
        public:
            /**
             * Response status codes.
             */
            typedef Handler::StatusCode StatusCode;

            /**
             * Constructor
             *
             * \param[in] statusCode  The status code to be sent.
             *
             * \param[in] contentType The content type
             */
            inline Response(
                    StatusCode        statusCode = StatusCode::OK,
                    const QByteArray& contentType = QByteArray()
                ):currentStatusCode(
                    statusCode
                ),currentContentType(
                    contentType
                ) {}

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to assign to this instance.
             */
            inline Response(
                    const Response& other
                ):currentStatusCode(
                    other.currentStatusCode
                ),currentContentType(
                    other.currentContentType
                ) {}

            virtual ~Response();

            /**
             * Method you can use to determine if this is a binary or JSON response.
             *
             * \return Returns true if this is a binary response.  Returns false if this is a JSON response.
             */
            virtual bool isBinaryResponse() const = 0;

            /**
             * Method you can use to determine if this is a JSON response or binary response.
             *
             * \return Returns true if this is a JSON response.  Returns false if this is a binary response.
             */
            inline bool isJsonResponse() const {
                return !isBinaryResponse();
            }

            /**
             * Method you can use to obtain a strict binary representation of the data.
             *
             * \return Returns a QByteArray holding the result.
             */
            virtual QByteArray asByteArray() const = 0;

            /**
             * Method you can use to set the reported status code.  A value that is not StatusCode::OK will cause the
             * payload to be ignored and a failed response to be sent.
             *
             * \param[in] newStatusCode The new status code.
             */
            inline void setStatusCode(StatusCode newStatusCode) {
                currentStatusCode = newStatusCode;
            }

            /**
             * Method you can use to get the current status code.
             *
             * \return Returns the current status code.
             */
            inline StatusCode statusCode() const {
                return currentStatusCode;
            }

            /**
             * Method you can use to specify the content type for this payload.
             *
             * \param[in] newContentType The new content type to report.
             */
            inline void setContentType(const QByteArray& newContentType) {
                currentContentType = newContentType;
            }

            /**
             * Method you can use to determine the current content type.
             *
             * \return Returns the current content type.
             */
            inline const QByteArray& contentType() const {
                return currentContentType;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare to this instance
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            inline bool operator==(const Response& other) const {
                return currentStatusCode == other.currentStatusCode && currentContentType == other.currentContentType;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare to this instance
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            inline bool operator!=(const Response& other) const {
                return currentStatusCode != other.currentStatusCode || currentContentType != other.currentContentType;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            inline Response& operator=(const Response& other) {
                currentStatusCode  = other.currentStatusCode;
                currentContentType = other.currentContentType;

                return *this;
            }

        private:
            /**
             * The underlying status code value.
             */
            StatusCode currentStatusCode;

            /**
             * The content type to report.
             */
            QByteArray currentContentType;
    };
};

#endif
