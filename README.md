Plugin for OpenCPN to make .kap charts on the fly from tile
-----------------------------------------------------------
This plugin only runs on Windows Systems ....
the reason is, there are used executables which are not available on other systems.

https://github.com/Markavian/tile-merger

https://github.com/nohal/imgkap

and these programs are not available as a "libray"  for example in the "opencpn_lib"

There is a Installer that can be used to install without compile for OpenCPN 5.8.4
TileChart_pi-1.0.0.0-Windows_Installer_OpenCPN_8.6.4.exe
to "c:\users\[Username]\Appdata\local\opencpn"

or import the tarball TileChart_pi-1.0.0.0-msvc-x86_64-10.0.19045-MSVC.tar.gz in OpenCPN.

How it works
------------
- Start the Plugin
- Select a directory for tmp tile and the charts in the preferences
- Make other setting in preferences ....
- klick "Start" on the dialog and select an area on the OpenCPN canvas.
- When ready, Klick "gererate chart"
- after a while, the chart is on the screen.

At the moment there can be downloaded "Satallite charts" from google-maps and charts from opensteetview
In the preferences there is a feature to design a User-URL for all other servers, which use the 
x,y,z  tile structure in the URL.

Example ...

![1](https://github.com/BerndCirotzki/TileChart_pi/assets/32094108/121e8567-121b-4612-bd7a-fe39ed191846)


Select the area ....

![2](https://github.com/BerndCirotzki/TileChart_pi/assets/32094108/01f563b8-d9df-4984-8248-4769b36bac7e)

Generate the chart and ready after ... 40 Seconds ... select the new chart.

![3](https://github.com/BerndCirotzki/TileChart_pi/assets/32094108/8998ddc2-fbd8-42a8-8e18-5c11d9e96038)


