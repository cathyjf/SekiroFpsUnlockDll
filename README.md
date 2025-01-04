# cathyjf/SekiroFpsUnlockDll
This project builds a DLL that automatically unlocks the framerate in
[Sekiro](https://www.sekirothegame.com/), without the need for any manual
action when launching the game in the future. This DLL works with the latest
version of the game available on Steam (1.06).

For instructions on how to install, please refer to the download page for
the [latest release](https://github.com/cathyjf/SekiroFpsUnlockDll/releases/latest).

## Configuration
After the DLL for this project is loaded into Sekiro for the first time, a
configuration file named `SekiroFpsUnlockAndMore.xml` will be created in the
same directory as the DLL. This is the same configuration file format used by
the `uberhalit/SekiroFpsUnlockAndMore` project. You can view the default
configuration [here](https://github.com/cathyjf/SekiroFpsUnlockDll/blob/main/src/config/SekiroFpsUnlockAndMore.xml).

By default, the framerate is set to 144, but you can easily change this by
substituting a different number in this part of the configuration file:
```xml
<tbFramelock>144</tbFramelock>
```

There are various other options in the configuration file, corresponding to
other features of `uberhalit/SekiroFpsUnlockAndMore`. By default, nothing is
changed about the game other than the framerate.

## Building from source
If you want to build this project from source, clone this
repository with the `--recurse` argument:
```
git clone --recurse https://github.com/cathyjf/SekiroFpsUnlockDll
```

Open the `SekiroFpsUnlockDll.sln` file in [Visual Studio 2022](https://visualstudio.microsoft.com/vs/)
to build the project. Once the build process is complete, you will find the
binary at `x64\Release\SekiroFpsUnlockDll.dll`. You should build in `Release`
mode. The `Debug` build configuration has not been tested.

If you just want to check out how the program works, most of the code related
to the implementation of the DLL is found in [`src\dllmain.cpp`](https://github.com/cathyjf/SekiroFpsUnlockDll/blob/main/src/dllmain.cpp).

## Licensing

### License for cathyjf/SekiroFpsUnlockDll
This SekiroFpsUnlockDll program was written by [Cathy J. Fitzpatrick](https://github.com/cathyjf).
Copyright 2022 Cathy J. Fitzpatrick.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program (see `LICENSE.txt`). If not, see <https://www.gnu.org/licenses/>.

### License for uberhalit/SekiroFpsUnlockAndMore
This SekiroFpsUnlockDll program incorporates the `uberhalit/SekiroFpsUnlockAndMore`
project. The `uberhalit/SekiroFpsUnlockAndMore` project was released by its
author ([uberhalit](https://github.com/uberhalit)) under the following license:

> MIT License
>
> Copyright (c) 2019
>
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.
