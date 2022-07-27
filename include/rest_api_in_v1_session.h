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
