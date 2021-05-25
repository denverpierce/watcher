# Soil Watcher

This is a monorepo that contains the software to monitor the soil moisture in my backyard remotely.  I produced this code over the weekend, and did only the bare minimum required to read values from an ESP8266 sensor and post them to a server.  I don't particularly claim the code to be particularly good, but it *does* run.

## Overview

### Sensors

I nested these directories because I envision adding more platforms to the sensor cluster, and wanted the directory structure to reflect that.

#### Soil Sensor

This is the directory for the soil moisture sensor, the launch sensor in the project.

##### Software

Arduino C++ that reads the value from A0 and posts it to my remote flask server, then sleeps for 15 minutes.

##### Hardware

The sensor I'm currently using is a [Capacitive Soil Moisture](https://smile.amazon.com/Gikfun-Capacitive-Corrosion-Resistant-Detection/dp/B07H3P1NRM/).  The current configuration is an NodeMCU ESP8266 with the sensor connected A0 and 3.3V + GND.  The whole contraption is powered by an external cell-phone battery sitting inside an old peanut jar.  With deep sleep, it runs for about a week before needing a recharge.

### Sensor Server

This is a simple flask server that just gives me an endpoint to receive the values from.  I don't currently hold any state of the returned values, just check them periodically to see if I need to water my plants.
