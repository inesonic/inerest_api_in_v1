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
* This file implements the \ref RestApiInV1::InesonicBinaryRestHandler class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_INESONIC_BINARY_REST_HANDLER_H
#define REST_API_IN_V1_INESONIC_BINARY_REST_HANDLER_H

#include <QString>
#include <QByteArray>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_handler.h"
#include "rest_api_in_v1_response.h"
#include "rest_api_in_v1_inesonic_rest_handler_base.h"

namespace RestApiInV1 {
    class REST_API_V1_PUBLIC_API Session;

    /**
     * Pure virtual class you can overload to receive inbound messages and send responses using binary formats with
     * authentication using Inesonic's rolling authentication algorithm.
     *
     * The secret must be the length in bytes prescribed by the constant
     *  \ref InesonicRestHandler::inesonicSecretLength.
     *
     * This class is best for use with large payloads and/or higher bandwidth operations.
     */
    class REST_API_V1_PUBLIC_API InesonicBinaryRestHandler:public Handler, public InesonicRestHandlerBase {
        public:
            /**
             * Value holding the default maximum payload size.
             */
            static constexpr unsigned long defaultMaximumPayloadSize = 1048576;
            /**
             * Constructor
             *
             * \param[in] secret The secret used to authenticate incoming messages.
             */
            InesonicBinaryRestHandler(const QByteArray& secret = QByteArray());

            ~InesonicBinaryRestHandler() override;

        protected:
            /**
             * Method you can overload to receive a request and send a return response.  This method will only be
             * triggered if the message meets the authentication requirements.
             *
             * \param[in] path     The request path.
             *
             * \param[in] request  The request data in raw binary format.
             *
             * \param[in] threadId The ID used to uniquely identify this thread while in flight.
             *
             * \return The response to return.  You can use either \ref BinaryResponse or \ref JsonResponse to encode
             *         the data in your favorite format.  The response will be deleted automatically.
             */
            virtual Response* processAuthenticatedRequest(
                const QString&    path,
                const QByteArray& request,
                unsigned          threadId
            ) = 0;

            /**
             * Method you can overload to set the maximum allowed payload size.
             *
             * \return Returns the maximum allowed payload size.  The default implementation will impose the default
             *         maximum payload size.
             */
            virtual unsigned long long maximumPayloadSize() const;

        private:
            /**
             * Method you can overload to handle a session for this endpoint.  Note that this method will be called
             * from multiple threads and must therefore be fully reentrant.
             *
             * Session specific information is contained in the suppled session object.  You can use this session
             * object to send response data back to the client.  Always send the header first followed by any data.
             *
             * The session will be closed gracefully when you exit this method.
             *
             * \param[in] session A reference to the session object tied to this session.
             */
            void session(Session& session) final;
    };
};

#endif
