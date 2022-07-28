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

#ifndef REST_API_IN_V1_BINARY_RESPONSE_H
#define REST_API_IN_V1_BINARY_RESPONSE_H

#include <QString>
#include <QByteArray>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_response.h"

namespace RestApiInV1 {
    /**
     * Class you can use to send a response in a raw binary format.
     */
    class BinaryResponse:public QByteArray, public Response {
        public:
            /**
             * The default content type this class will report.
             */
            static const QByteArray defaultContentType;

            BinaryResponse();

            /**
             * Constructor
             *
             * \param[in] byteArray The byte array holding the data to be sent.
             */
            BinaryResponse(const QByteArray& byteArray);

            /**
             * Constructor (move semantics)
             *
             * \param[in] byteArray The byte array holding the data to be sent.
             */
            BinaryResponse(QByteArray&& byteArray);

            /**
             * Constructor
             *
             * \param[in] rawData The raw data as a character array.
             *
             * \param[in] size    The size of the raw data array.  A value of -1 will cause this constructor to
             *                    determine the size by locating an ending NUL character.
             */
            BinaryResponse(const char* rawData, int size = -1);

            /**
             * Constructor
             *
             * \param[in] size The desired byte array size, in bytes.
             *
             * \param[in] ch   The byte value to fill in the array with.
             */
            BinaryResponse(unsigned size, char ch);

            /**
             * Constructor
             *
             * \param[in] statusCode The status code to be sent.
             */
            BinaryResponse(StatusCode statusCode);

            /**
             * Constructor
             *
             * \param[in] statusCode The status code to be sent.
             *
             * \param[in] byteArray  The byte array holding the data to be sent.
             */
            BinaryResponse(StatusCode statusCode, const QByteArray& byteArray);

            /**
             * Constructor (move semantics)
             *
             * \param[in] statusCode The status code to be sent.
             *
             * \param[in] byteArray  The byte array holding the data to be sent.
             */
            BinaryResponse(StatusCode statusCode, QByteArray&& byteArray);

            /**
             * Constructor
             *
             * \param[in] statusCode The status code to be sent.
             *
             * \param[in] rawData    The raw data as a character array.
             *
             * \param[in] size       The size of the raw data array.  A value of -1 will cause this constructor to
             *                       determine the size by locating an ending NUL character.
             */
            BinaryResponse(StatusCode statusCode, const char* rawData, int size = -1);

            /**
             * Constructor
             *
             * \param[in] statusCode The status code to be sent.
             *
             * \param[in] size       The desired byte array size, in bytes.
             *
             * \param[in] ch         The byte value to fill in the array with.
             */
            BinaryResponse(StatusCode statusCode, unsigned size, char ch);

            /**
             * Constructor
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(const QString& contentType);

            /**
             * Constructor
             *
             * \param[in] byteArray   The byte array holding the data to be sent.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(const QString& contentType, const QByteArray& byteArray);

            /**
             * Constructor (move semantics)
             *
             * \param[in] byteArray   The byte array holding the data to be sent.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(const QString& contentType, QByteArray&& byteArray);

            /**
             * Constructor
             *
             * \param[in] rawData     The raw data as a character array.
             *
             * \param[in] size        The size of the raw data array.  A value of -1 will cause this constructor to
             *                        determine the size by locating an ending NUL character.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(const QString& contentType, const char* rawData, int size = -1);

            /**
             * Constructor
             *
             * \param[in] size        The desired byte array size, in bytes.
             *
             * \param[in] ch          The byte value to fill in the array with.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(const QString& contentType, unsigned size, char ch);

            /**
             * Constructor
             *
             * \param[in] statusCode  The status code to be sent.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(StatusCode statusCode, const QString& contentType);

            /**
             * Constructor
             *
             * \param[in] statusCode  The status code to be sent.
             *
             * \param[in] byteArray   The byte array holding the data to be sent.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(StatusCode statusCode, const QString& contentType, const QByteArray& byteArray);

            /**
             * Constructor (move semantics)
             *
             * \param[in] statusCode  The status code to be sent.
             *
             * \param[in] byteArray   The byte array holding the data to be sent.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(StatusCode statusCode, const QString& contentType, QByteArray&& byteArray);

            /**
             * Constructor
             *
             * \param[in] statusCode  The status code to be sent.
             *
             * \param[in] rawData     The raw data as a character array.
             *
             * \param[in] size        The size of the raw data array.  A value of -1 will cause this constructor to
             *                        determine the size by locating an ending NUL character.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(StatusCode statusCode, const QString& contentType, const char* rawData, int size = -1);

            /**
             * Constructor
             *
             * \param[in] statusCode  The status code to be sent.
             *
             * \param[in] size        The desired byte array size, in bytes.
             *
             * \param[in] ch          The byte value to fill in the array with.
             *
             * \param[in] contentType The content type to report.
             */
            BinaryResponse(StatusCode statusCode, const QString& contentType, unsigned size, char ch);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            BinaryResponse(const BinaryResponse& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            BinaryResponse(BinaryResponse&& other);

            ~BinaryResponse() override;

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
            bool operator==(const BinaryResponse& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare to this instance
             *
             * \return Returns true if the instances are not equal.  Returns false if the instances are equal.
             */
            bool operator!=(const BinaryResponse& other) const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            BinaryResponse& operator=(const BinaryResponse& other);

            /**
             * Assignment operator (move semantics)
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            BinaryResponse& operator=(BinaryResponse&& other);
    };
};

#endif
