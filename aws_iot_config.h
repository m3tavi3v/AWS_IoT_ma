/*
 * Copyright 2019 mayato.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef config_usr_h
#define config_usr_h

// Copy and paste your configuration into this file
//===============================================================
#define AWS_IOT_MQTT_HOST "<Objekt Endpoint>"   // your endpoint
#define AWS_IOT_MQTT_PORT 8883                  // your port
#define AWS_IOT_CLIENT_ID "<Client ID"           // your client ID
#define AWS_IOT_MY_THING_NAME "<MY Thing Name>"           // your thing name
#define AWS_IOT_ROOT_CA_FILENAME "AmazonRootCA1.crt"           // your root-CA filename
#define AWS_IOT_CERTIFICATE_FILENAME "<hash-certificate.pem>"                 // your certificate filename
#define AWS_IOT_PRIVATE_KEY_FILENAME "<hash-private.pem>"              // your private key filename
//===============================================================
// SDK config, DO NOT modify it
#define AWS_IOT_PATH_PREFIX "../certs/"
#define AWS_IOT_ROOT_CA_PATH AWS_IOT_PATH_PREFIX AWS_IOT_ROOT_CA_FILENAME			// use this in config call
#define AWS_IOT_CERTIFICATE_PATH AWS_IOT_PATH_PREFIX AWS_IOT_CERTIFICATE_FILENAME	// use this in config call
#define AWS_IOT_PRIVATE_KEY_PATH AWS_IOT_PATH_PREFIX AWS_IOT_PRIVATE_KEY_FILENAME	// use this in config call

#endif
