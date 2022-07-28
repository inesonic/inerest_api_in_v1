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
