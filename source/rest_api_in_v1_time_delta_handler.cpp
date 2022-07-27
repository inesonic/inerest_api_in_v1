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

#include <QString>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>

#include "rest_api_in_v1_handler.h"
#include "rest_api_in_v1_json_response.h"
#include "rest_api_in_v1_time_delta_handler.h"

namespace RestApiInV1 {
    const QString TimeDeltaHandler::defaultEndpoint("/td");

    TimeDeltaHandler::TimeDeltaHandler() {}


    TimeDeltaHandler::~TimeDeltaHandler() {}


    JsonResponse TimeDeltaHandler::processRequest(
            const QString&       /* path */,
            const QJsonDocument& request,
            unsigned             /* threadId */
        ) {
        bool      success   = false;
        long long timeDelta = 0;

        if (request.isObject()) {
            QJsonObject object = request.object();
            if (object.contains("timestamp") && object.size() == 1) {
                QJsonValue timestampValue = object.value("timestamp");
                if (timestampValue.isDouble()) {
                    double timestampAsDouble = timestampValue.toDouble();
                    if (timestampAsDouble > 0                                          ||
                        timestampAsDouble < static_cast<double>(0x7FFFFFFFFFFFFFFFULL)    ) {
                        long long timestamp = static_cast<unsigned long long>(timestampAsDouble + 0.5);

                        timeDelta = QDateTime::currentSecsSinceEpoch() - timestamp;
                        success = true;
                    }
                }
            }
        }

        QJsonObject response;
        if (success) {
            response.insert("status", "OK");
            response.insert("time_delta", static_cast<double>(timeDelta));
        } else {
            response.insert("status", "failed");
        }

        return JsonResponse(response);
    }
}
