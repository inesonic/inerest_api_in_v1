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
