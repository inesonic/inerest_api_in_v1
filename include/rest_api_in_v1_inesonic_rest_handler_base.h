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
