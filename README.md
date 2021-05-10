# ControlCast Firmware

This firmware for the ESP32-S2 has an HTTP server with SSL encryption using OpenSSL.

## Certificates

You will need to approve a security exception in your browser. This is because a self signed
certificate is used; this will be always the case, unless you preload the CA root into your browser/system
as trusted.

The reason for using a self signed certificate is due to the setup where the ConsoleCast device
is not connected to the Internet and no way to renew or validate the certificate on-line.

REVISE!!

If required you can generate a new certificate using the OpenSSL command line tool:

```
openssl req -newkey rsa:2048 -nodes -keyout prvtkey.pem -sha256 -extfile v3.ext -x509 -days 3650 -out cacert.pem -subj "/CN=ConsoleCast"

v3.ext file:

authorityKeyIdentifier=keyid,issuer
basicConstraints=CA:FALSE
keyUsage = digitalSignature, nonRepudiation, keyEncipherment, dataEncipherment
subjectAltName = @alt_names

[alt_names]
DNS.1 = ConsoleCast
```

Expiry time and metadata fields can be adjusted in the invocation.

Please see the openssl man pages (man openssl-req) for more details.

It is **strongly recommended** to not reuse the certificate included in this source code, in your application;
it is included only for demonstration.
