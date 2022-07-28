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
* This file implements the \ref RestApiInV1::RestHandler class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_REST_HANDLER_H
#define REST_API_IN_V1_REST_HANDLER_H

#include <QString>
#include <QByteArray>
#include <QHash>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_json_response.h"
#include "rest_api_in_v1_handler.h"

namespace RestApiInV1 {
    /**
     * Pure virtual class you can overload to receive inbound messages and send responses using JSON format.
     */
    class REST_API_V1_PUBLIC_API RestHandler:public Handler {
        public:
            /**
             * Constructor
             */
            RestHandler();

            ~RestHandler() override;

        protected:
            /**
             * Pure virtual method you should overload to receive a request and send a return response.
             *
             * \param[in] path     The request path.
             *
             * \param[in] request  The request data encoded as a JSON document.
             *
             * \param[in] threadId The ID used to uniquely identify this thread while in flight.
             *
             * \return The response to return.
             */
            virtual JsonResponse processRequest(
                const QString&       path,
                const QJsonDocument& request,
                unsigned             threadId
            ) = 0;

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
