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
