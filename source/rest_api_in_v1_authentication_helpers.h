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
