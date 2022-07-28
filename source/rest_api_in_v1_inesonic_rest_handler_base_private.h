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
* This file implements the \ref RestApiInV1::InesonicRestHandlerBase::Private class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_INESONIC_REST_HANDLER_BASE_PRIVATE_H
#define REST_API_IN_V1_INESONIC_REST_HANDLER_BASE_PRIVATE_H

#include <QString>
#include <QByteArray>
#include <QHash>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_authentication_helpers.h"
#include "rest_api_in_v1_inesonic_rest_handler_base.h"

namespace RestApiInV1 {
    /**
     * Private implementation of the \ref InesonicRestHandlerBase class.
     */
    class REST_API_V1_PUBLIC_API InesonicRestHandlerBase::Private {
        public:
            /**
             * Constructor
             *
             * \param[in] secret The secret used to authenticate incoming messages.
             */
            Private(const QByteArray& secret);

            ~Private();

            /**
             * Method you can use to set the Inesonic authentication secret.
             *
             * \param[in] newSecret The new secret to be used.  The secret must be the length prescribed by the
             *                      constant \ref inesonicSecretLength.
             */
            void setSecret(const QByteArray& newSecret);

            /**
             * Method that checks our hash.
             *
             * \param[in] receivedData The raw data to be checked.
             *
             * \param[in] receivedHash The received hash to be checked.
             *
             * \return Returns true if the hash is correct.  Returns false if the hash is incorrect.
             */
            inline bool checkHash(
                    const QByteArray& receivedData,
                    const QByteArray& receivedHash
                ) const {
                return RestApiInV1::checkHash(receivedData, receivedHash, currentSecret);
            }

        private:
            /**
             * The secret used to authenticate the incoming message.
             */
            QByteArray currentSecret;
    };
};

#endif
