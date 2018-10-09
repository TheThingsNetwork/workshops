---
title: The Things Conference Pisa
description: Getting Started Workshop
background: /assets/img/background.jpg
---

# Prerequisites

You need:

- Some Programming Experience
- A laptop with WiFi and at least one USB port
- To [install the Arduino IDE and the `TheThingsNetwork` library](/resources.md).
- An [account on The Things Network](https://account.thethingsnetwork.org/); please send your username to `htdvisser` on Slack, Telegram or email (@thethingsnetwork.org).
  - It saves a lot of time if you do this **before the workshop**

We'll provide:

- The Things Uno
- USB Cable
- Potentiometer
- Connection wires M/F

# Introduction

In this workshop we'll build a sensor system that is connected to the Internet through The Things Network.
Many of these systems work in a similar way. They usually start with an analog sensor that changes its electrical resistance based on some environmental factor such as temperature, light or gravity. By connecting those sensors to a microcontroller, we can measure the voltage and build sensor systems.

Today we'll use a potentiometer that changes its resistance with rotation, but you can just replace it with any other analog sensor for your specific use case, without having to change the code.

# Connecting the Potentiometer

- Connect the outer pins of the potentiometer to `5V` and `GND`.
- Connect the inner pin of the potentiometer to Analog port `A0`.

![Hardware Setup](/potmeter-workshop/hardware.svg)

# Sketch for Reading the Sensor

Now it's time to start the Arduino IDE. If you didn't do so yet, **set up your Arduino IDE** and **install the libraries** as explained on the [Workshop Resources](/resources.md) page.

We'll start with the following Arduino sketch ([download](/potmeter-workshop/PotMeterRead/PotMeterRead.ino)):

```cpp
int sensorPin = A0;
int sensorValue = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  delay(250);
}
```

In this piece of code we do the following things:

- In the `setup` function, we initialize the sensor pin (`A0`) as an input pin and start serial communication on the USB port.
- In the `loop` function, we read the analog value of the sensor pin, and print it as a line to the serial port.

Now **connect The Things Uno** with the USB cable to your computer and **upload the sketch** (the "right arrow" button in the menu bar). You can view the serial output by opening the **serial monitor** (the "magnifying glass" button in the menu bar). What happens when you turn the knob of the potentiometer?

**If this works as expected, we can continue.**

# Register Your Device on The Things Network

ℹ **[The Things Network Console](https://console.thethingsnetwork.org/) is where all applications and devices are managed.**

[**Go to the `eitdigital` Application**](https://console.thethingsnetwork.org/applications/eitdigital) and [**register a new Device**](https://console.thethingsnetwork.org/applications/eitdigital/devices/register); mine is called `htdvisser` (my username), but feel free to come up with a nice name. The **Device EUI** can be generated, we won't need it in the workshop.

⚠ _If the console can't find the application `eitdigital`, make sure that you sent us your username (see prerequisites)._

Instead of the default (and recommended) device activation method "OTAA", we're going to use "ABP" in this workshop. To change this, **go to the Device you created** and then to **Settings**. Change the **Activation Method** to **ABP**, **disable Frame Counter Checks** and **Save**. 

⚠ _This is pretty bad for security, but it will make development faster during today's workshop._

# Sketch for Reading the Sensor and Transmitting to TTN

Considering the short time we have for this workshop, we'll immediately skip to the [finished sketch](/potmeter-workshop/PotMeterWorkshop/PotMeterWorkshop.ino). **download it** and **open it in the Arduino IDE**.

You only need to change a couple of lines to make this sketch work. [Go to your device](https://console.thethingsnetwork.org/applications/eitdigital/devices) and **copy the example code** on the bottom of the page. In the Arduino IDE, **replace the placeholder lines** with the lines you just copied. Now you can **upload** the new sketch and see if it works.

![Code to replace](/potmeter-workshop/PotMeterWorkshop/replace.png)

ℹ The code contains comments that explain what's going on. Let us know if you have any questions.

<script src="//cdnjs.cloudflare.com/ajax/libs/highlight.js/9.12.0/highlight.min.js"></script>
<script>hljs.initHighlightingOnLoad();</script>

# Transmissions <small>live</small>

<iframe src="https://ttn.htdvisser.nl/grafana/d-solo/QnsGpOZmz/workshop?refresh=1m&orgId=1&panelId=8&var-ns=eitdigital&theme=light" width="100%" height="400" frameborder="0"></iframe>

# Sensor Values <small>live</small>

<iframe src="https://ttn.htdvisser.nl/grafana/d-solo/QnsGpOZmz/workshop?refresh=1m&orgId=1&panelId=2&var-ns=eitdigital&theme=light" width="100%" height="400" frameborder="0"></iframe>
