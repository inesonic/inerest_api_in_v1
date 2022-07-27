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
* This file implements the \ref RestApiInV1::InesonicCustomerRestHandler class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_INESONIC_CUSTOMER_REST_HANDLER_H
#define REST_API_IN_V1_INESONIC_CUSTOMER_REST_HANDLER_H

#include <QString>
#include <QByteArray>
#include <QHash>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_customer_data.h"
#include "rest_api_in_v1_json_response.h"
#include "rest_api_in_v1_rest_handler.h"

namespace RestApiInV1 {
    /**
     * Pure virtual class you can overload to receive inbound messages and send responses using JSON format with
     * authentication using Inesonic's rolling authentication algorithm.  This version supports customer unique
     * secrets.
     */
    class REST_API_V1_PUBLIC_API InesonicCustomerRestHandler:public RestHandler {
        friend class Server;

        public:
            /**
             * Constructor
             *
             * \param[in] customerData The customer data instance used to obtain customer settings.  Note that this
             *                         class does not take ownership of the \ref CustomerData instance.
             */
            InesonicCustomerRestHandler(CustomerData* customerData);

            ~InesonicCustomerRestHandler() override;

        protected:
            /**
             * Method you can overload to receive a request and send a return response.  This method will only be
             * triggered if the message meets the authentication requirements.
             *
             * \param[in] path       The request path.
             *
             * \param[in] customerId The customer ID of the customer in question.
             *
             * \param[in] request    The request data encoded as a JSON document.
             *
             * \param[in] threadId   The ID used to uniquely identify this thread while in flight.
             *
             * \return The response to return, also encoded as a JSON document.
             */
            virtual JsonResponse processAuthenticatedRequest(
                const QString&       path,
                unsigned long        customerId,
                const QJsonDocument& request,
                unsigned             threadId
            ) = 0;

        private:
            /**
             * Method you can overload to receive a request and send a return response.
             *
             * \param[in] path     The request path.
             *
             * \param[in] request  The request data encoded as a JSON document.
             *
             * \param[in] threadId The ID used to uniquely identify this thread while in flight.
             *
             * \return The response to return, also encoded as a JSON document.
             */
            JsonResponse processRequest(const QString& path, const QJsonDocument& request, unsigned threadId) final;

            /**
             * The private implementation.
             */
            class Private;

            /**
             * The private implementation;
             */
            Private* impl;
    };
};

#endif
