# mlaf-arduino

## PlatformIO
This project uses [PlatformIO](http://platformio.org) for dependency management and builds. Be sure to install the plugin for your favourite editor.

## Cloning
This project uses git submodules. To clone the project you should:

```
$ git clone --recurse-submodules <url>
```


To initialize the submodule system after cloning without `--recurse-submodules`, you should: 
```
$ git submodule init
$ git submodule update
```


## Third-party libraries
This project uses the TinyXML-2 library by Lee Thomason. This library is provide under the following license:

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