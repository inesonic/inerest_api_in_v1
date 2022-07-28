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

#ifndef REST_API_IN_V1_HANDLER_H
#define REST_API_IN_V1_HANDLER_H

#include <QString>
#include <QByteArray>
#include <QHash>

#include "rest_api_in_v1_common.h"

namespace RestApiInV1 {
    class REST_API_V1_PUBLIC_API Connection;
    class REST_API_V1_PUBLIC_API Session;

    /**
     * Pure virtual base class that provides a handler for an endpoint.  The handler class is generic enough that it
     * can be used both to issue content and to act as a handle for a REST API.
     *
     * The handler provides two distinct APIs.  The first is a bulk data handler API that expects the entire incoming
     * message to be read before being processed.  The second is a streaming API that allows for processing of data of
     * unlimited length.  Overload the method \ref createSession to indicate which approach to be used.
     */
    class REST_API_V1_PUBLIC_API Handler {
        friend class Connection;

        public:
            /**
             * The supported HTTP methods.  Values taken from the qhttp project found at
             *     https://github.com/azadkuh/qhttp
             */
            enum class Method {
                /**
                 * Delete
                 */
                DELETE = 0,

                /**
                 * Get
                 */
                GET = 1,

                /**
                 * Head
                 */
                HEAD = 2,

                /**
                 * Post
                 */
                POST = 3,

                /**
                 * Put
                 */
                PUT = 4,

                /**
                 * Pathological -- Connect
                 */
                CONNECT = 5,

                /**
                 * Pathological -- Options
                 */
                OPTIONS = 6,

                /**
                 * Pathological -- Trace
                 */
                TRACE = 7,

                /**
                 * WebDAV -- Copy
                 */
                COPY = 8,

                /**
                 * WebDAV -- Lock
                 */
                LOCK = 9,

                /**
                 * WebDAV -- MKCOL
                 */
                MKCOL = 10,

                /**
                 * WebDAV -- Move
                 */
                MOVE = 11,

                /**
                 * WebDAV -- PropFind
                 */
                PROPFIND = 12,

                /**
                 * WebDAV -- PropPatch
                 */
                PROPPATCH = 13,

                /**
                 * WebDAV -- Search
                 */
                SEARCH = 14,

                /**
                 * WebDAV -- Unlock
                 */
                UNLOCK = 15,

                /**
                 * WebDAV -- Bind
                 */
                BIND = 16,

                /**
                 * WebDAV -- Rebind
                 */
                REBIND = 17,

                /**
                 * WebDAV -- Unbind
                 */
                UNBIND = 18,

                /**
                 * WebDAV -- ACL
                 */
                ACL = 19,

                /**
                 * Subversion -- Report
                 */
                REPORT = 20,

                /**
                 * Subversion -- MkActivity
                 */
                MKACTIVITY = 21,

                /**
                 * Subversion -- Checkout
                 */
                CHECKOUT = 22,

                /**
                 * Subversion -- Merge
                 */
                MERGE = 23,

                /**
                 * UPnP -- MSEARCH
                 */
                MSEARCH = 24,

                /**
                 * UPnP -- NOTIFY
                 */
                NOTIFY = 25,

                /**
                 * UPnP -- Subscribe
                 */
                SUBSCRIBE = 26,

                /**
                 * UPnP -- Unsubscribe
                 */
                UNSUBSCRIBE = 27,

                /**
                 * RFC-5789 -- Patch
                 */
                PATCH = 28,

                /**
                 * RFC-5789 -- Purge
                 */
                PURGE = 29,

                /**
                 * CalDAV -- MkCalendar
                 */
                MKCALENDAR = 30,

                /**
                 * RFC-2068, Section 19.6.1.2 -- Link
                 */
                LINK = 31,

                /**
                 * RFC-2068, Section 19.6.1.2 -- Link
                 */
                UNLINK = 32,

                /**
                 * Value indicating the number of defined methods.
                 */
                NUMBER_METHODS
            };

            /**
             * Supported return status codes.
             */
            enum class StatusCode {
                /**
                 * HTTP continue response -- indicates no errors, continue.
                 */
                CONTINUE = 100,

                /**
                 * HTTP switch protocol response -- Indicates the server is switching to a different protocol.
                 */
                SWITCH_PROTOCOLS = 101,

                /**
                 * HTTP OK response -- Meaning depends on the method used.  Generally means success.
                 */
                OK = 200,

                /**
                 * HTTP created response -- Indicates success with a new resource created.
                 */
                CREATED = 201,

                /**
                 * HTTP accepted response - Indicates the request has been received but has not been acted on.
                 */
                ACCEPTED = 202,

                /**
                 * HTTP non-authoratative information response -- Indicates data is close to but not guaranteed to
                 * match what was originally sent.  Primarily used for mirrors or backups.
                 */
                NON_AUTHORITATIVE_INFORMATION = 203,

                /**
                 * HTTP no-content response -- No information provided in body but headers may be of use.
                 */
                NO_CONTENT = 204,

                /**
                 * HTTP reset content response -- The client should reset the document that sent the request.
                 */
                RESET_CONTENT = 205,

                /**
                 * HTTP partial content response -- Used with the Range header is included, indicates content is
                 * incomplete.
                 */
                PARTIAL_CONTENT = 206,

                /**
                 * HTTP multi-status response -- Used with WebDAV.  Indicates multiple responses would be more
                 * appropriate.
                 */
                MULTI_STATUS = 207,

                /**
                 * HTTP multiple choices response -- Indicates that there are multiple choices for a response and the
                 * client should choose one specific response.
                 */
                MULTIPLE_CHOICES = 300,

                /**
                 * HTTP moved permanently response -- Indicates the resource is now at a different address.
                 */
                MOVED_PERMANENTLY = 301,

                /**
                 * HTTP found response -- Indicates that the resource has been temporarily relocated.
                 */
                FOUND = 302,

                /**
                 * HTTP see other response -- Indicates that the client should go to another location for the requested
                 * data.  Intended to be used with GET.
                 */
                SEE_OTHER = 303,

                /**
                 * HTTP not-modified response -- Indicates that previously cached content has not been changed.
                 */
                NOT_MODIFIED = 304,

                /**
                 * HTTP use proxy response -- Deprecated, do not use.
                 */
                USE_PROXY = 305,

                /**
                 * HTTP temporary request response -- Indicates that the client should get the requested resourse at
                 * another URI using the same method as the prior request.  Similar to 302 except that the method of
                 * access should not be changed.
                 */
                TEMPORARY_REDIRECT = 307,

                /**
                 * HTTP bad request response -- Indicates that the request could not be understood due to invalid
                 * syntax.
                 */
                BAD_REQUEST = 400,

                /**
                 * HTTP unauthorized response -- Indicates that the request could not be authenticated.
                 */
                UNAUTHORIZED = 401,

                /**
                 * HTTP payment required response -- Reserved for future use.
                 */
                PAYMENT_REQUIRED = 402,

                /**
                 * HTTP forbidden response -- Indicates the client does not have rights to access this resource.
                 */
                FORBIDDEN = 403,

                /**
                 * HTTP not-found response -- Indicates that the requested resource could not be found.
                 */
                NOT_FOUND = 404,

                /**
                 * HTTP method-not-allowed -- Indicates that the requested method of access is not allowed by the
                 * resource.
                 */
                METHOD_NOT_ALLOWED = 405,

                /**
                 * HTTP not acceptable response -- Indicates that the server found no content that confirms to the
                 * criterial given by the client.
                 */
                NOT_ACCEPTABLE = 406,

                /**
                 * HTTP proxy-authentication required -- Indicates that authentication by a proxy is required.
                 */
                PROXY_AUTHENTICATION_REQUIRED = 407,

                /**
                 * HTTP request timeout response -- Indicates that the client took too long or wants the client to
                 * disconnect.
                 */
                REQUEST_TIMEOUT = 408,

                /**
                 * HTTP confict response -- Indicates that the request is misaligned with the current server state.
                 */
                CONFLICT = 409,

                /**
                 * HTTP gone response -- Indicates that the requested content no longer exists.
                 */
                GONE = 410,

                /**
                 * HTTP length required response -- Indicates that the content length header is required.
                 */
                LENGTH_REQUIRED = 411,

                /**
                 * HTTP precondition failed response -- Indicates that a precondition regarding the headers has not
                 * been met.
                 */
                PRECONDITION_FAILED = 412,

                /**
                 * HTTP requested entity too large response -- Indicates that the requested payload data is too large
                 * to be sent by the server.
                 */
                REQUEST_ENTITY_TOO_LARGE = 413,

                /**
                 * HTTP request URL too long response -- Indicates that the request URI was too long to handle.
                 */
                REQUEST_URI_TOO_LONG = 414,

                /**
                 * HTTP unsupported media type response -- The requested media type is not supported by the server.
                 */
                REQUEST_UNSUPPORTED_MEDIA_TYPE = 415,

                /**
                 * HTTP range not satisfiable response -- The requested range values for the request are outside of
                 * the supportable values.
                 */
                REQUESTED_RANGE_NOT_SATISFIABLE = 416,

                /**
                 * HTTP expectation failed response -- Indicates that the expection defined by the Expect request
                 * header can not be met by the server.
                 */
                EXPECTATION_FAILED = 417,

                /**
                 * HTTP internal server error response -- Indicates that an internal server error exists.
                 */
                INTERNAL_SERVER_ERROR = 500,

                /**
                 * HTTP not implemented response -- Indicates that the requested method is not supported by this
                 * server.
                 */
                NOT_IMPLEMENTED = 501,

                /**
                 * HTTP bad gateway response -- Indicates that this server, acting as a gateway, received a response
                 * that was invalid.
                 */
                BAD_GATEWAY = 502,

                /**
                 * HTTP service unavailable response -- Indicates that the server can not handle the request at this
                 * time.
                 */
                SERVICE_UNAVAILABLE = 503,

                /**
                 * HTTP gateway timeout response -- Indicates that the server, acting as a gateway, timed out waiting
                 * for a response.
                 */
                GATEWAY_TIMEOUT = 504,

                /**
                 * HTTP versionnot supported response -- Indicates that the HTTP protocol version is not supported by
                 * this server.
                 */
                HTTP_VERSION_NOT_SUPPORTED = 505
            };

            /**
             * The "Content-Type" string encoded as a QByteArray.
             */
            static const QByteArray contentTypeString;

            /**
             * The "Content-Length" string encoded as a QByteArray.
             */
            static const QByteArray contentLengthString;

            /**
             * The "Connection" string encoded as a QByteArray.
             */
            static const QByteArray connectionString;

            /**
             * The "X-Real-IP" string encoded as a QByteArray.
             */
            static const QByteArray xRealIPString;

            /**
             * The "X-Forwarded-For" string encoded as a QByteArray.
             */
            static const QByteArray xForwardedForString;

            /**
             * The connection "close" string encoded as a QByteArray.
             */
            static const QByteArray connectionCloseString;

            /**
             * The "server" string encoded as a QByteArray.
             */
            static const QByteArray serverString;

            /**
             * The "InesonicBot" string encoded as a QByteArray.
             */
            static const QByteArray inesonicBotString;

            /**
             * The "User-Agent" string encoded as a QByteArray.
             */
            static const QByteArray userAgentString;

            /**
             * The "text/plain" string encoded as a QByteArray.
             */
            static const QByteArray textPlainString;

            /**
             * The "text/html" string encoded as a QByteArray.
             */
            static const QByteArray textHtmlString;

            /**
             * The "application/json" string encoded as a QByteArray.
             */
            static const QByteArray applicationJsonString;

            /**
             * The "application/xml" string encoded as a QByteArray.
             */
            static const QByteArray applicationXmlString;

            /**
             * The "application/octet-stream" string encoded as a QByteArray
             */
            static const QByteArray applicationOctetStreamString;

            /**
             * The "x-www-form-urlencoded" string encoded as a QByteArray.
             */
            static const QByteArray xWWWFormUrlEncodedString;

            /**
             * Type used to represent the received headers.
             */
            typedef QHash<QByteArray, QByteArray> Headers;

            Handler();

            virtual ~Handler();

        protected:
            /**
             * Method you can overload to handle a session for this endpoint.  Note that this method will be called
             * from multiple threads and must therefore be fully reentrant.
             *
             * Session specific information is contained in the suppled session object.  You can use this session
             * object to send response data back to the client.  Always send the header first followed by any data.
             *
             * The session will be closed gracefully when you exit this method.
             *
             * \param[in] session A reference to the session object tied to this session.
             */
            virtual void session(Session& session) = 0;
    };

    /**
     * Hash function for the \ref Handler::Method enumerated value.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  The optional hash seed.
     *
     * \return Returns the hash calculated for the value.
     */
    inline unsigned qHash(Handler::Method value, unsigned seed = 0) {
        return ::qHash(static_cast<unsigned>(value), seed);
    }

    /**
     * Hash function for the \ref Handler::StatusCode enumerated value.
     *
     * \param[in] value The value to be hashed.
     *
     * \param[in] seed  The optional hash seed.
     *
     * \return Returns the hash calculated for the value.
     */
    inline unsigned qHash(Handler::StatusCode value, unsigned seed = 0) {
        return ::qHash(static_cast<unsigned>(value), seed);
    }
};

#endif
