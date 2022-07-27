=================
inerest_api_in_v1
=================
The inerest_api_in_v1 library provides a framework for implementing inbound
REST APIs in C++.  The library is useful for cases where you must implement
cloud services in C++.

The library is currently used by cloud based infrastructure used as part of the
**Aion** low-code algorithm development software and by the now defunct
**SpeedSentry** site monitoring system.  Both products are, or were, supported
and sold by `Inesonic, LLC <https://inesonic.com>`.

.. note::

   The format we use for our messages is technically not *REST* so using the
   term *REST* here is a misnomer.  With that said, the format is *REST* like
   in that it meets most of the major criteria for REST.

   With only minimal work, you can easily use the provided classes to implement
   your own fully REST compliant APIs.


Licensing
=========
This library is licensed under MIT license terms.


Dependencies And Building
=========================
The library is Qt based and is built using either the qmake or cmake build
tool.  You will need to build the library using a recent version of Qt 5.  The
library has also been tested against Qt 6.

The library also depends on the inecrypto library.


qmake
-----
To build inerest_api_in_v1 using qmake:

.. code-block:: bash

   cd inecrypto
   mkdir build
   cd build
   qmake ../inecrypto.pro INECRYPTO_INCLUDE=<path to inecrypto headers>
   make

If you wish to create a debug build, change the qmake line to:

.. code-block:: bash

   qmake ../inecrypto.pro CONFIG+=debug
   make

Note that the qmake build environment currently does not have an install target
defined and will alway build the library as a static library.


cmake
-----
To build inecrypto using cmake:

.. code-block:: bash

   cd inecrypto
   mkdir build
   cmake -B. -H.. -DCMAKE_INSTALL_PREFIX=/usr/local/
   make

To install, simply run

.. code-block:: bash

   make install

You can optionally also include any of the following variables on the cmake
command line.

+------------------------+----------------------------------------------------+
| Variable               | Function                                           |
+========================+====================================================+
| inerest_api_in_v1_TYPE | Set to ``SHARED`` or ``STATIC`` to specify the     |
|                        | type of library to be built.   A static library    |
|                        | will be built by default.                          |
+------------------------+----------------------------------------------------+
| INECRYPTO_INCLUDE      | You can set this variable to indicate the location |
|                        | of the inecrypto header files.  This variable only |
|                        | needs to be set on Windows or if the headers are   |
|                        | in a non-standard location.                        |
+------------------------+----------------------------------------------------+
| INECRYPTO_LIB          | You can set this variable to indicate the full     |
|                        | path to the inecrypto static or shared library.    |
|                        | This variable is only needed on Windows, if the    |
|                        | library is in a non-standard location, or if cmake |
|                        | can-not locate the library after setting the       |
|                        | ``INECRYPTO_LIBDIR`` variable.                     |
+------------------------+----------------------------------------------------+
| INECRYPTO_LIBDIR       | You can use this variable to add one or more       |
|                        | directories to the inecrypto library search path.  |
|                        | Separate paths with spaces.                        |
+------------------------+----------------------------------------------------+


Server Behind A Proxy
=====================
For the applications where we've used these libraries, we've placed our server
behind an NGINX reverse proxy.   Doing this allows the reverse proxy to filter
messages and provide TLS encryption.

A typical NGINX configuration we use would look like:

.. code-block::

   ##
   # Configuration for the NGINX reverse proxy.
   ##

   server {
       listen 443 ssl http2;
       server_name <my.domain>;

       add_header Strict-Transport-Security "max-age=31536000; includeSubdomains" always;

       location / {
           add_header X-Robots-Tag noindex,nofollow;
           root /var/www/html;
           index index.html;
       }

       location /td {
           proxy_set_header Host $host;
           proxy_set_header X-Real-IP $remote_addr;
           proxy_pass http://localhost:8080;
       }

       location ~ /(v1|customer|mapping|event|host_scheme|latency|monitor|multiple|region|server)/ {
           proxy_set_header Host $host;
           proxy_set_header X-Real-IP $remote_addr;
           proxy_pass http://localhost:8080;
       }

       ssl_certificate /etc/letsencrypt/live/<my.domain>/fullchain.pem;
       ssl_certificate_key /etc/letsencrypt/live/<my.domain>/privkey.pem;
       include /etc/letsencrypt/options-ssl-nginx.conf;
       ssl_dhparam /etc/letsencrypt/ssl-dhparams.pem;

   }

   server {
       listen 80;
       if ($host = <my.domain>) {
           return 301 https://$host$request_uri;
       }

       server_name <my.domain>;
       return 404;
   }

Be sure to replace ``<my.domain>`` with your actual server's FQDN.


Using The Library In Your Code
==============================
The entire inerest_api_in_v1 library API is contained within the
``RestApiInV1`` namespace.

To use, you'll need to instantiate an instance of ``RestApiInV1::Server`` that
will monitor a given IP address and port.  You can also specify the maximum
number of allowed simultaneous connections, a logging function, etc.  Below
is a brief example:

.. code-block:: c++

   #include <QString>
   #include <QHostAddress>

   #include <iostream>

   #include <rest_api_in_v1_server.h>

   . . .

   static void logWrite(const QString& message, bool error) {
       if (error) {
           std::cerr << "*** " << message.toLocal8Bit().data() << std::endl;
       } else {
           std::cout << "    " << message.toLocal8Bit().data() << std::endl;
       }
   }

   . . .

   void MyApplication::configureServer() {
       RestApiInV1::Server* server = new RestApiInV1::Server(
           8,   // The maximum number of simultaneous connections.
           this // The Qt parent object, derived from QObject
       );

       server->setLoggingFunction(&logWrite);

       QHostAddress hostAddres("0.0.0.0");
       unsigned short inboundPort = 8080;

       server->reconfigure(inboundHostAddress, inboundPort);
   }

Once configured, you will need to define endpoints to be monitored and serviced
by the inerest_api_in_v1 library.

Every time a new connection is made, the server starts a new connection thread
using the private ``RestApiInV1::Connection`` class.  The
``RestApiInV1::Connection`` class will parse the incoming headers, identify the
endpoint and then identify an ``RestApiInV1::Handler`` class that should serve
the connection.

You can either derive your own handler classes derived from
``RestApiInV1::Handler`` or use on of the handler classes we provide which
provide built-in authentication mechanisms and simplify sending responses.  The
base ``RestApiV1::Handler`` class accepts a ``RestApiInV1::Session`` object you
can use to send and receive data.

The provided REST API handler classes, hide all of the complexities of managing
the session.  To use, simply derive a class from one of the higher level REST
API handler classes and overload the appropriate "process request" methods.
The process methods receive JSON encoded data in a ``QJsonDocument`` object and
can return either JSON encoded data or binary data depending on the handler.
You can also send different status code responses, if needed.

Below is an example defining two endpoints using the
``RestApiInV1::InesonicRestHandler`` and
``RestApiInV1::InesonicCustomerBinaryRestHandler`` classes.

.. code-block:: c++

   #include <QObject>
   #include <QByteArray>
   #include <QJsonDocument>
   #include <QJsonObject>

   #include <rest_api_in_v1_server.h>
   #include <rest_api_in_v1_handler.h>
   #include <rest_api_in_v1_json_response.h>
   #include <rest_api_in_v1_inesonic_rest_handler.h>
   #include <rest_api_in_v1_customer_data.h>
   #include <rest_api_in_v1_binary_response.h>
   #include <rest_api_in_v1_customer_binary_rest_handler.h>

   class Endpoints:public QObject { // Deriving from QObject isn't necessary.
       Q_OBJECT

       public:
           Endpoints(
                   RestApiInV1::Server*       server,
                   const QByteArray&          secret,
                   RestApiInV1::CustomerData* customerData
                   QObject*                   parent = nullptr
               ):QObject(
                   parent
               ),getHandler(
                   secret
               ),imageHandler(
                   customerData
               ) {
               // Lines below register each handler to the server.

               server->registerHandler(
                   &getHandler,
                   RestApiInV1::Handler::Method::GET,
                   "v1/get"
               );
               server->registerHandler(
                   &imageHandler,
                   RestApiInV1::Handler::Method::POST,
                   "v1/image"
               );
           }

           ~Endpoints() override {}

       private:
           class Get:public RestApiInV1::InesonicRestHandler {
               public:
                   Get(
                           const QByteArray& secret
                       )::RestApiInV1::InesonicRestHandler(
                           secret
                       ) {}

                   ~Get() override {}

               protected:
                   RestApiInV1::JsonResponse processAuthenticatedRequest(
                           const QString&       path,
                           const QJsonDocument& request,
                           unsigned             threadId
                       ) override {
                       QJsonObject response = generateGetResponse(request);
                       response.insert("status", "OK");

                       // Line below causes a 200 response with a JSON payload.
                       // Note that the RestApiInV1::JsonResponse object has a
                       // wide range of constructors to generate different
                       // types of responses.  The class also has setters you
                       // can use.

                       return RestApiInV1::JsonResponse(response);
                   }
           };

           class Image:public RestApiInV1::InesonicCustomerBinaryRestHandler {
               public:
                   Image(
                           RestApiInV1::CustomerData* customerData,
                       ):RestApiInV1::InesonicCustomerBinaryRestHandler(
                           customerData
                       ) {}

                   ~Image() override {}

               protected:
                   RestApiInV1::BinaryResponse processAuthenticatedRequest(
                           const QString&       path,
                           unsigned long        customerId,
                           const QJsonDocument& request,
                           unsigned             threadId
                       ) override {
                       QByteArray imageData = generateImage(request);
                       return RestApiInV1::BinaryResponse(
                           QByteArray("image/png"),
                           imageData
                       );
                   }
           };

           Get getHandler;
           Image imageHandler;
   };

The ``threadId`` parameter is incorrectly named in that it doesn't identify a
unique thread.  Instead the thread ID is an integer value ranging from 0 to the
maximum number of simultaneous connections that is guaranteed unique during the
lifespan of a given connection.  You can use the ``threadId`` parameter to
index global or class scope resources that must be accessed consistently during
the lifespan of a connection.  The numbers will be reused but are guaranteed
unique during the lifespan of execution of a single received request and
subsequent response.

The "customer" REST API handlers are designed to allow you to have REST APIs
with customer unique secrets.  These classes accept a
``RestApiInV1::CustomerData`` instance that queries or generates an appropriate
customer unique secret on a per customer basis.

To prevent replay attacks against our REST API, the provided authentication
echanism is time based.  We provide a special REST API handler,
``RestApiInV1::TimeDeltaHandler`` that our REST API can use to query the time
delta between the client and serve system clocks.

If you use the **Inesonic** REST API functions, you should also instantiate
an instance of the ``RestApiInV1::TimeDeltaHandler`` and register that handler
with the server at the ``/td`` endpoint and using the POST HTTP method.  The
code snippet below shows how to do this.

.. code-block:: c++

    timeDeltaHandler = new RestApiInV1::TimeDeltaHandler;
    server->registerHandler(
        timeDeltaHandler,
        RestApiInV1::Handler::Method::POST,
        RestApiInV1::TimeDeltaHandler::defaultEndpoint
    );

Lastly, note that the ``RestApiInV1::Server`` class does not take ownership of
the handlers.  Be sure to either define the handler classes as static instances
or delete the handlers from the heap.


Inesonic REST API Message Format
================================
This section documents the **Inesonic** REST API message format.  Note that we
currently provide implementations of outbound REST APIs in Python, PHP, and C++
in the projects documented below.

+----------+------------------------------------------------------+
| Language | Project                                              |
+==========+======================================================+
| C++      | https://github.com/inesonic/inerest_api_out_v1       |
+----------+------------------------------------------------------+
| Python   | https://github.com/inesonic/speedsentry_python_api   |
+----------+------------------------------------------------------+
| PHP      | https://github.com/inesonic/speedsentry_php_api      |
+----------+------------------------------------------------------+


Goals
-----
The message format is designed to provide a reasonably light-weight, secure,
interface between servers or client and server.  More specifically, the message
format provides:

* A secure authentication mechanism to prevent spoofing or highjacking of your
  account,

* the ability to thwart replay attacks,

* safe operation with different character encodings, and

* the ability to operate across 8-bit unsafe transport mechanisms that might
  change character encodings or otherwise impact the received data.


Account Secrets
---------------
Depending on the message format used, you may need to track one or two
different secrets:

* A 56-byte long secret.
* A customer identifier or ``cid``.

The ``cid`` is only required by the customer REST API handlers.  The normal
REST API handlers work using just the 56-byte long shared secret.


HTTP Message Format
-------------------
You should include the following request headers in the messages:

* Content-Type : application/json
* Content-Length: <total length in bytes>

The message should be be JSON encoded, containing the
following three fields:

.. code-block:: json

   {
      "cid" : "<customer identifier>",
      "data" : "<base-64 encoded message>",
      "hash" : "<base-64 encoded hash>"
   }

The ``cid`` field is only included in cases where you are using the "customer"
REST APIs and should be omitted in all other cases.  The ``cid`` can contain
any identifier string that can be decoded by your ``RestApiInV1::CustomerData``
instance.

The ``data`` field should be your message, base-64 encoded so that the
provided data is 7-bit safe and not munged by any intermediate layers that
change line endings or change character encodings.  Using base-64 encoding
guarantees that the hash will be computed corrected no matter what the lower
level transport layers do.

The ``hash`` field is a base-64 encoded 32-byte long hash generated from your
raw data and secret, prior to base-64 encoding.

Below is an example message sent via POST:

.. code-block:: json

   {
       "cid": "6e6cb5cd0d2dad53",
       "data": "eyJvcmRlcl9ieSI6ICJtb25pdG9yX2lkIn0=",
       "hash": "CmORepo1AJW0mhFe6Sadn2WSIa74JhiaDddxb9RMTFc="
   }


Encoded Data
------------
Your request data should be provided as a JSON encoded string using any of the
character encodings supported by Qt's ``QJsonDocument`` class and/or your own
implementation.  At a minimum, the supported encodings are:

* 7-bit ASCII,
* Latin 1 (ISO 8851-1),
* UTF-8 (ISO 10646)
* IEC/ISO 2022
* Any other 8-bit encoding formats based on ASCII.


Hash
----
You should generate a time-based hash using your 56-byte long secret and the
JSON encoded data:

* Determine the current Unix timestamp on your system, t\ :sub:`unix`.  The
  value should represent the number of seconds that have elapsed since
  midnight, January 1, 1970 UTC.

* Calculate a time index from the Unix timestamp using the equation below.
  The t\ :sub:`d` value is used to adjust for clock error between your system
  and |company| infrastructure and will normally be 0.  We provide a REST API
  you can use to determine this correction factor.  For details, see
  :ref:`Version 1 Message Format Time Correction`.

.. math::

   t_{index} = \left \lfloor \frac{t_{unix} + t_d}{30} \right \rfloor

* Convert the time index value to an 8-byte long little-endian representation
  and append to your secret to create a 64-byte long time dependent secret.

* Calculate a 32-byte SHA-256 HMAC from your JSON encoded data using the 64-bit
  secret as your key.

* Base-64 encode both your JSON encoded message to be used as the ``data``
  field and your 32-byte long SHA-256 HMAC to be used as the ``hash`` field.

The example code below demonstrates this algorithm as implemented in Python 3:

.. code-block:: python

   import time
   import struct
   import hashlib
   import hmac
   import json
   import base64
   import requests

   # Get our Unix timestamp.

   t_unix = int(time.time())

   # Note // indicates integer division (rounding down)

   t_index = (t_unix + t_d) // 30

   # Create our 64-byte long time dependent secret.

   key = my_secret + struct.pack('<Q', t_index)

   # Calculate a SHA-256 HMAC.
   #   Note: raw_message is our JSON encoded message in one of the
   #         acceptable character sets listed above.

   raw_hash = hmac.new(
       key = key,
       msg = raw_message,
       digestmod = hashlib.sha256
   ).digest()

   # Base-64 encode both our message and our hash so they're 7-bit safe.

   encoded_message = base64.b64encode(raw_message)
   encoded_hash = base64.b64encode(raw_hash)

   payload =json.dumps(
       {
           'cid' : customer_identifier,
           'data' : encoded_message.decode('ascii'),
           'hash' : encoded_hash.decode('ascii')
       }
   )

   # Send our request and wait for a response.  The URL is the full URL to
   # the endpoint we're sending our request to.

   response = requests.post(
       url,
       data = payload,
       headers = {
           'Content-Type' : 'application/json',
           'Content-Length' : str(len(payload))
       }
   )


Time Correction
===============
If the supplied has is incorrect, the inerest_api_in_v1 library will return a
401 UNAUTHORIZED error.  Should this occur, you should use the ``/td`` endpoint
to determine the time correction value, :math:`t_d` to be applied when
calculating your message hashes.

We recommend that you only apply a correction if you receive a 401 UNAUTHORIZED
error and that you cache the correction value for future use.  In almost all
cases, if you use NTP or a similar mechanism to set your system time, clock
errors will small enough that they will not impact your system.

To request a time correction value, use the ``/td`` endpoint, supplying a JSON
encoded payload holding your current system time.

.. code-block:: json

   {
       "timestamp" : timestamp>
   }

Where ``<timestamp>`` is your current Unix timestamp, that is an integer
value holding the number of seconds since midnight, January 1, 1970, UTC.

The message requires you to define the following headers:

* Content-Type : application/json
* Content-Length : The length of the encoded JSON request message.

The returned response will be JSON encoded and will include a "status"
value indicating success ("OK") or failure.  The response will also include a
"time_delta" value holding the time delta to be applied when calculate the
message hash as described above.

An example response is shown below:

.. code-block:: json

   {
       "status" : "OK",
       "time_delta" : 43
   }
