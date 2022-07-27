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
* This file implements the \ref RestApiInV1::InesonicRestHandlerBase class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_INESONIC_REST_HANDLER_BASE_H
#define REST_API_IN_V1_INESONIC_REST_HANDLER_BASE_H

#include <QString>
#include <QByteArray>
#include <QHash>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"

namespace RestApiInV1 {
    class REST_API_V1_PUBLIC_API InesonicRestHandler;
    class REST_API_V1_PUBLIC_API InesonicBinaryRestHandler;

    /**
     * Base class for the Inesonic REST API handlers.  You will generally never need to use this class directly.
     *
     * The secret must be the length in bytes prescribed by the constant
     *  \ref InesonicRestHandler::secretLength.
     */
    class REST_API_V1_PUBLIC_API InesonicRestHandlerBase {
        friend class InesonicRestHandler;
        friend class InesonicBinaryRestHandler;

        public:
            /**
             * The required length for secrets you provide.
             */
            static const unsigned secretLength;

            /**
             * Constructor
             *
             * \param[in] secret The secret used to authenticate incoming messages.
             */
            InesonicRestHandlerBase(const QByteArray& secret = QByteArray());

            ~InesonicRestHandlerBase();

            /**
             * Method you can use to set the Inesonic authentication secret.
             *
             * \param[in] newSecret The new secret to be used.  The secret must be the length prescribed by the
             *                      constant \ref secretLength.
             */
            void setSecret(const QByteArray& newSecret);

        private:
            /**
             * The private implementation.
             */
            class Private;

            /**
             * The private implementation.
             */
            Private* impl;
    };
};

#endif
