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
* This file implements the \ref RestApiInV1::Session class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_SESSION_H
#define REST_API_IN_V1_SESSION_H

#include <QString>
#include <QByteArray>
#include <QHash>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_handler.h"

namespace RestApiInV1 {
    class REST_API_V1_PUBLIC_API Server;

    /**
     * Pure virtual base class that tracks session specific parameters.  You can use this class to receive information
     * specific to the session, and to receive data from and response to the client.
     *
     * Note that the data contained within this instance will not persist past the lifetime of the session with the
     * client.
     */
    class REST_API_V1_PUBLIC_API Session {
        public:
            virtual ~Session() = default;

            /**
             * Method you can use to get the thread ID of this thread.
             *
             * \return Returns the thread ID.
             */
            virtual unsigned threadId() const = 0;

            /**
             * Method you can use to get the next bolus of available data.  This method will block until there is at
             * least some data available.
             *
             * \param[in,out] buffer      The buffer to hold the current receive data.
             *
             * \param[in]     maximumSize The maximum number of bytes of data to obtain.  A value of 0 indicates that
             *                            the current default maximum value should be used.
             *
             * \return Returns true on success or false on error.
             */
            virtual bool readData(QByteArray& buffer, unsigned long maximumSize = 0) = 0;

            /**
             * Method you can use to get a single line of data.  This method will block until a full line is read.
             *
             * \param[in]  maximumSize The maximum number of bytes of data to obtain.  A value of 0 indicates that
             *                         the current default maximum value should be used.
             *
             * \param[out] ok          An optional pointer to a boolean value holding true on success or false on
             *                         error.
             *
             * \return Returns the read line.  The newline character will be automatically removed.
             */
            virtual QByteArray readLine(unsigned long maximumSize = 0, bool* ok = nullptr) = 0;

            /**
             * Method you can use to send response status.  Always call this method before sending any response data.
             *
             * \param[in] statusCode      The response status code.
             *
             * \param[in[ responseHeaders The response headers.
             *
             * \return Returns true on success.  Returns false on error.
             */
            virtual bool sendResponseHeader(
                Handler::StatusCode     statusCode,
                const Handler::Headers& responseHeaders = Handler::Headers()
            ) = 0;

            /**
             * Method you can use to send a failed response including simple response data.
             *
             * \param[in] statusCode      The response status code.
             *
             * \return Returns true on success.  Returns false on error.
             */
            virtual bool sendFailedResponse(Handler::StatusCode statusCode) = 0;

            /**
             * Method you can use to send response data.
             *
             * \param[in] data The raw data to be sent.
             *
             * \return Returns true on success.  Returns false on error.
             */
            virtual bool sendData(const QByteArray& data) = 0;

            /**
             * Method you can use to obtain the current request URI.
             *
             * \return Returns the current request URI.
             */
            virtual const QUrl& requestUri() const = 0;

            /**
             * Method you can use to obtain the current access method.
             *
             * \return Returns the current request URI.
             */
            virtual Handler::Method method() const = 0;

            /**
             * Method you can use to obtain the HTTP version as a string.
             *
             * \return Returns the HTTP version as a string.
             */
            virtual const QString& httpVersion() const = 0;

            /**
             * Method you can use to obtain the current HTTP headers.
             *
             * \return Returns the current HTTP headers.
             */
            virtual const Handler::Headers& headers() const = 0;
    };
};

#endif
