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
* This file implements the \ref RestApiInV1::CustomerData class.
***********************************************************************************************************************/

#include "rest_api_in_v1_authentication_helpers.h"
#include "rest_api_in_v1_customer_data.h"

namespace RestApiInV1 {
    const unsigned CustomerData::secretLength       = RestApiInV1::inesonicSecretLength;
    const unsigned CustomerData::paddedSecretLength = RestApiInV1::inesonicSecretPaddedLength;

    CustomerData::CustomerData() {}


    CustomerData::~CustomerData() {}
}

