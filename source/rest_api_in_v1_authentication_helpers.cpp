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
* This file implements  the \ref RestApiInV1::InesonicRestHandlerBase class.
***********************************************************************************************************************/

#include <QString>
#include <QByteArray>
#include <QDateTime>

#include <cstring>
#include <cstdint>

#include <crypto_hmac.h>

#include "rest_api_in_v1_authentication_helpers.h"

namespace RestApiInV1 {
    static const Crypto::Hmac::Algorithm hashAlgorithm       = Crypto::Hmac::Algorithm::Sha256;
    static const unsigned                hmacBlockSize       = Crypto::Hmac::blockSize(hashAlgorithm);
    static const unsigned                hmacDigestSize      = Crypto::Hmac::digestSize(hashAlgorithm);
    static const unsigned                hmacDigestChunkSize = hmacDigestSize / sizeof(std::uint64_t);
    static const unsigned                timestampLength     = 8;

    const unsigned inesonicSecretLength       = hmacBlockSize - timestampLength;
    const unsigned inesonicSecretPaddedLength = hmacBlockSize;
    const unsigned inesonicHashLength         = hmacDigestSize;

    static bool compareHash(const QByteArray& receivedHash, const QByteArray& expectedHash) {
        // Implementation below is designed to be fast and constant time.
        const std::uint64_t* expectedHashData = reinterpret_cast<const std::uint64_t*>(expectedHash.constData());
        const std::uint64_t* receivedHashData = reinterpret_cast<const std::uint64_t*>(receivedHash.constData());
        std::uint64_t        result           = 0;
        for (unsigned i=0 ; i<hmacDigestChunkSize ; ++i) {
            result |= (expectedHashData[i] - receivedHashData[i]);
        }

        return result == 0;
    }


    bool checkHash(
            const QByteArray& receivedData,
            const QByteArray& receivedHash,
            const QByteArray& secret
        ) {
        bool success;

        unsigned long long currentTimestamp = QDateTime::currentSecsSinceEpoch();
        QByteArray         fullSecret       = secret;
        std::uint64_t*     rawSecret        = reinterpret_cast<std::uint64_t*>(fullSecret.data());

        // Round 1: No time offset
        unsigned long long hashSuffix       = (currentTimestamp / 30) + 0;
        rawSecret[inesonicSecretLength / 8] = hashSuffix;

        Crypto::Hmac hmac(fullSecret, receivedData, hashAlgorithm);
        QByteArray   expectedHash     = hmac.digest();

        if (compareHash(receivedHash, expectedHash)) {
            success = true;
        } else {
            // Round 2: +1
            unsigned long long hashSuffix       = (currentTimestamp / 30) + 1;
            rawSecret[inesonicSecretLength / 8] = hashSuffix;

            Crypto::Hmac hmac(fullSecret, receivedData, hashAlgorithm);
            QByteArray   expectedHash     = hmac.digest();
            if (compareHash(receivedHash, expectedHash)) {
                success = true;
            } else {
                // Round 3: -1
                unsigned long long hashSuffix       = (currentTimestamp / 30) - 1;
                rawSecret[inesonicSecretLength / 8] = hashSuffix;

                Crypto::Hmac hmac(fullSecret, receivedData, hashAlgorithm);
                QByteArray   expectedHash     = hmac.digest();

                success = compareHash(receivedHash, expectedHash);
            }
        }

        return success;
    }
}
