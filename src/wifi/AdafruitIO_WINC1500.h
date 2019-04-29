//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Copyright (c) 2015-2016 Adafruit Industries
// Authors: Tony DiCola, Todd Treece
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.
//
#ifndef ADAFRUITIO_WINC1500_H
#define ADAFRUITIO_WINC1500_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD) && !defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) && !defined(ADAFRUIT_PYPORTAL)

#include "Arduino.h"
#include "AdafruitIO.h"
#include "SPI.h"
#include "WiFi101.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// feather wifi m0
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2

class AdafruitIO_WINC1500 : public AdafruitIO {

  public:
    AdafruitIO_WINC1500(const char *user, const char *key, const char *ssid, const char *pass) : AdafruitIO(user, key)
    {
      _ssid = ssid;
      _pass = pass;
      _mqtt_client = new WiFiSSLClient;
      _mqtt = new Adafruit_MQTT_Client(_mqtt_client, _host, _mqtt_port);
      _http_client = new WiFiSSLClient;
      _http = new HttpClient(*_http_client, _host, _http_port);
    }

    ~AdafruitIO_WINC1500()
    {
      if (_mqtt_client)
        delete _http_client;
      if (_http_client)
        delete _mqtt_client;
      if (_mqtt)
        delete _mqtt;
      if (_http)
        delete _http;
    }

    aio_status_t networkStatus()
    {

      switch (WiFi.status())
      {
      case WL_CONNECTED:
        return AIO_NET_CONNECTED;
      case WL_CONNECT_FAILED:
        return AIO_NET_CONNECT_FAILED;
      case WL_IDLE_STATUS:
        return AIO_IDLE;
      default:
        return AIO_NET_DISCONNECTED;
      }
    }
    const char *connectionType()
    {
      return "winc1500";
    }

  protected:
    const char *_ssid;
    const char *_pass;

    WiFiSSLClient *_http_client;
    WiFiSSLClient *_mqtt_client;

    void _connect()
    {

      WiFi.setPins(WINC_CS, WINC_IRQ, WINC_RST, WINC_EN);

      // no shield? bail
      if (WiFi.status() == WL_NO_SHIELD)
        return;

      WiFi.begin(_ssid, _pass);
      _status = AIO_NET_DISCONNECTED;
    }
};

#endif // ARDUINO_ARCH_SAMD

#endif // ADAFRUITIO_WINC1500_H
