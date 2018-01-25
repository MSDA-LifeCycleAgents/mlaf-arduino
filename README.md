# mlaf-arduino
This project is a microprocessor extension made for the JADE based [MLAF](https://github.com/MSDA-LifeCycleAgents/mlaf-java) project. For extensive usage of the framework, please carefully read the documentation provided in the MLAF repository. Additional instructions for the usage of this library and API examples are included in this file.

## Quickstart
* Set up a JADE based MLAF hub following the instructions in the [MLAF repository](https://github.com/MSDA-LifeCycleAgents/mlaf-java)
* Install the Arduino enviroment and required libraries (see [Environment](##Environment) for more elaborate instructions)
* Clone the master branch of this repository to a local directory
* Open Configuration.h and edit WiFi_ssid and WiFi_pass
* Deploy MLAF_boot.ino to your microprocessor

You now have an external sensor agent connected to the HUB. The HUB logging feed should tell you if the registration went successful.

## Configuration

## Examples

## Environment

### Arduino IDE
This project uses [Arduino IDE](https://www.arduino.cc/en/Main/Software) for dependency management and builds. Be sure to install the IDE, following the instructions. 

#### Arduino libraries
This project has dependencies on a number of Arduino libraries, listed below. Be sure to install them using the Arduino library manager.
* ESP8266
* ESP8266WiFi
* NtpClientLib
* ESP8266mDNS

### Third-party libraries
This project uses the TinyXML-2 library by Lee Thomason (source code included in the project). This library is provide under the following license:

```
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
```
