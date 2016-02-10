# Getting Started with The Things UNO

Super simple code samples to get you started with The Things UNO and The Things Network.

## Instructions

 * [Install Arduino IDE](https://www.arduino.cc/en/Main/Software) and connect your The Things UNO to your computer.
 * Ensure that in the IDE 'Tools -> Board' is set to 'Arduino Leonardo' (otherwise compilation will fail, as it will not recognize Serial1)
 * Clone this repository:

        $ git clone https://github.com/TheThingsNetwork/examples.git

 * Open the Arduino IDE and then open the [hello-world sketch](hello-world/hello-world.ino).
 * Open the `LoRa.cpp` file and assign a new device address to your device on the line `Serial1.write("mac set devaddr 02010101\r\n");`. Please use and register the address space in the [wiki](http://thethingsnetwork.org/wiki/AddressSpace) <sup>(1)</sup>.
 * Compile and upload the code.
 * Head over to [the API and see your packets coming in](http://thethingsnetwork.org/api/v0/nodes/02010101/).
 * Rejoice and hack something cool!

<sup>(1)</sup> TTN doesn't support over-the-air activation yet, so this is only temporary. In a later stage, it won't be needed to manually assign a device address, but for now, please be a good boy/girl and respect the address space.

## Credits
Based on https://github.com/thingkit-ee/things_uno