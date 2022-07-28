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
