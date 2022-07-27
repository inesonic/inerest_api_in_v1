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
* This file implements the \ref RestApiInV1::TimeDeltaHandler class.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_TIME_DELTA_HANDLER_H
#define REST_API_IN_V1_TIME_DELTA_HANDLER_H

#include <QString>
#include <QByteArray>
#include <QHash>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"
#include "rest_api_in_v1_json_response.h"
#include "rest_api_in_v1_rest_handler.h"

namespace RestApiInV1 {
    /**
     * Class you can overload to provide a time delta correction REST API.
     */
    class REST_API_V1_PUBLIC_API TimeDeltaHandler final:public RestHandler {
        friend class Server;

        public:
            /**
             * The default time delta handler endpoint.
             */
            static const QString defaultEndpoint;

            /**
             * Constructor
             */
            TimeDeltaHandler();

            ~TimeDeltaHandler() final;

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
    };
};

#endif
