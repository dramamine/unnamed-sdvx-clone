## Crazy shit I did to get things to build

- had to also install rtmidi
- In Properties -> C/C++ -> Preprocessor, add __WINDOWS_MM__ 
- Also add winmm.lib :#pragma comment(lib, "winmm.lib") to stdafx.h
- In Properties -> Linker -> Input, add rtmidi.lib as a dependency
  - This is sorta broken actually - only including the rtmidi.cpp file directly seems to be working.

