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
* This file implements the \ref RestApiInV1::Handler class.
***********************************************************************************************************************/

#include <QString>
#include <QByteArray>

#include "rest_api_in_v1_handler.h"

namespace RestApiInV1 {
    const QByteArray Handler::contentTypeString("content-type");
    const QByteArray Handler::contentLengthString("content-length");
    const QByteArray Handler::connectionString("connection");
    const QByteArray Handler::xRealIPString("x-real-ip");
    const QByteArray Handler::xForwardedForString("x-forwarded-for");
    const QByteArray Handler::connectionCloseString("close");
    const QByteArray Handler::serverString("server");
    const QByteArray Handler::userAgentString("user-agent");
    const QByteArray Handler::inesonicBotString("InesonicBot");
    const QByteArray Handler::textPlainString("text/plain");
    const QByteArray Handler::textHtmlString("text/html");
    const QByteArray Handler::applicationJsonString("application/json");
    const QByteArray Handler::applicationXmlString("application/xml");
    const QByteArray Handler::applicationOctetStreamString("application/octet-stream");
    const QByteArray Handler::xWWWFormUrlEncodedString("x-www-form-urlencoded");


    Handler::Handler() {}


    Handler::~Handler() {}
}
