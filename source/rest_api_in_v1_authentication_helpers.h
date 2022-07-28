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
* This file implements a small suite of authentication helper functions.
***********************************************************************************************************************/

#ifndef REST_API_IN_V1_AUTHENTICATION_HELPERS_H
#define REST_API_IN_V1_AUTHENTICATION_HELPERS_H

#include <QString>
#include <QByteArray>
#include <QHash>
#include <QJsonDocument>

#include "rest_api_in_v1_common.h"

namespace RestApiInV1 {
    /**
     * The length for secrets before padding.
     */
    extern const unsigned inesonicSecretLength;

    /**
     * The length for secrets after being padded.
     */
    extern const unsigned inesonicSecretPaddedLength;

    /**
     * The length of the generated hashes.
     */
    extern const unsigned inesonicHashLength;

    /**
     * Method that checks our hash.
     *
     * \param[in] receivedData The raw data to be checked.
     *
     * \param[in] receivedHash The received hash to be checked.
     *
     * \param[in] secret       The secret to apply to this calculation.  The secret should be padded to a length of
     *                         length of \ref inesonicSecretPaddedLength.
     *
     * \return Returns true if the hash is correct.  Returns false if the hash is incorrect.
     */
    bool checkHash(
        const QByteArray& receivedData,
        const QByteArray& receivedHash,
        const QByteArray& secret
    );
};

#endif
