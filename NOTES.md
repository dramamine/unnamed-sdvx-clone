## Crazy shit I did to get things to build

- had to also install rtmidi
- In Properties -> C/C++ -> Preprocessor, add __WINDOWS_MM__ 
- Also add winmm.lib :#pragma comment(lib, "winmm.lib") to stdafx.h
- In Properties -> Linker -> Input, add rtmidi.lib as a dependency
  - This is sorta broken actually - only including the rtmidi.cpp file directly seems to be working.

## Tips

- Change debugging active song by right clicking ALL_BUILD and going to Debugging -> Command line properties
-  -debug -autoskip -autoplay
"bin\songs\nautica\Aliquam\aliquam_exh.ksh"
"bin\songs\FANCY\Seethrough\141 - electrobro - we see through (ft. romaine).ksh"
- F2 calls SelectRandom() in SongSelect.cpp to choose a random song

## Changes

- Sending tempo changes (converting 60-314 to 0-127 to send over the wire)
