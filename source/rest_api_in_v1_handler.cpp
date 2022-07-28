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
