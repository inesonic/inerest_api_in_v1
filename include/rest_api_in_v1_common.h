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
* This header provides macros shared across the inbound REST API implementation.
***********************************************************************************************************************/

/* .. sphinx-project inewh */

#ifndef REST_API_IN_V1_COMMON_H
#define REST_API_IN_V1_COMMON_H

#include <QtGlobal>

/** \def REST_API_IN_V1_PUBLIC_API
 *
 * Macro used to define the interface to the container library.  Resolves to __declspec(dllexport) or
 * __declspec(dllimport) on Windows.
 */
#if (defined(INEREST_API_V1_DYNAMIC_BUILD))

    #if (defined(INEREST_API_V1_BUILD))

        #define REST_API_V1_PUBLIC_API Q_DECL_EXPORT

    #else

        #define REST_API_V1_PUBLIC_API Q_DECL_IMPORT

    #endif

#else

#define REST_API_V1_PUBLIC_API

#endif

#endif
