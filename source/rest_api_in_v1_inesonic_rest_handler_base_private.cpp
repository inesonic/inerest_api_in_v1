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

#include <QByteArray>

#include <cstring>
#include <cstdint>

#include "rest_api_in_v1_authentication_helpers.h"
#include "rest_api_in_v1_inesonic_rest_handler_base.h"
#include "rest_api_in_v1_inesonic_rest_handler_base_private.h"

namespace RestApiInV1 {
    InesonicRestHandlerBase::Private::Private(
            const QByteArray& secret
        ):currentSecret(
            inesonicSecretPaddedLength,
            0
        ) {
        // Confirm we're on a little-endian architecture.  Will assert for big-endian architectures.
        // Note the code in checkHash that relies on our system being little endian.
        assert(*reinterpret_cast<const std::uint16_t*>("\x00\xFF") == 0xFF00);

        if (secret.size() > 0) {
            setSecret(secret);
        }
    }


    InesonicRestHandlerBase::Private::~Private() {
        assert(static_cast<unsigned>(currentSecret.size()) == inesonicSecretPaddedLength);
        std::memset(currentSecret.data(), 0, inesonicSecretPaddedLength);
    }


    void InesonicRestHandlerBase::Private::setSecret(const QByteArray& newSecret) {
        assert(static_cast<unsigned>(newSecret.size()) == inesonicSecretLength);
        assert(static_cast<unsigned>(currentSecret.size()) == inesonicSecretPaddedLength);

        std::memcpy(currentSecret.data(), newSecret.constData(), inesonicSecretLength);
    }
}
