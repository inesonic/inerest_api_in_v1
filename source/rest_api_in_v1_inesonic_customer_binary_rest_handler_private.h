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
* This file implements the \ref RestApiInV1::InesonicCustomerBinaryRestHandler::Private class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_INESONIC_CUSTOMER_BINARY_REST_HANDLER_PRIVATE_H
#define REST_API_IN_V1_INESONIC_CUSTOMER_BINARY_REST_HANDLER_PRIVATE_H

#include <QString>
#include <QByteArray>
#include <QHash>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_inesonic_customer_binary_rest_handler.h"

namespace RestApiInV1 {
    /**
     * Private implementation of the \ref InesonicCustomerBiarnyRestHandler class.
     */
    class REST_API_V1_PUBLIC_API InesonicCustomerBinaryRestHandler::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] customerData The customer data instance used to obtain customer settings.  Note that this
             *                         class does not take ownership of the \ref CustomerData instance.
             */
            inline Private(CustomerData* customerData):currentCustomerData(customerData) {}

            inline ~Private() {}

            /**
             * Method you should overload to map customer identifiers to an internal numeric customer ID.
             *
             * \param[in] customerIdentifier The customer identifier to be mapped.
             *
             * \param[in] threadId           The thread ID of the thread we're executing under.
             *
             * \return Returns the internal customer ID associated with the customer identifier.  Returning
             *         a customer ID of 0 will cause a 401 UNAUTHORIZED to be returned.
             */
            inline unsigned long customerId(const QString& customerIdentifier, unsigned threadId) {
                return currentCustomerData->customerId(customerIdentifier, threadId);
            }

            /**
             * Method you should overload to map customer IDs to customer secrets.
             *
             * \param[in] customerId The internal customer ID to be mapped.
             *
             * \param[in] threadId   The thread ID of the thread we're executing under.
             *
             * \return Returns the secret associated with the customer identifier.  The secret should be
             *         padded to \ref InesonicRestHandlerLength::inesonicSecretPaddedLength.
             */
            inline QByteArray customerSecret(unsigned long customerId, unsigned threadId) {
                return currentCustomerData->customerSecret(customerId, threadId);
            }

        private:
            /**
             * The underlying CustomerData instance.
             */
            CustomerData* currentCustomerData;
    };
};

#endif
