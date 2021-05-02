# Fake SID 2

A C64 tracker for Android.
This will be WIP for a long time before any of this becomes usable.

The goal is to build a new version of Fake SID that allows for
import and export of GoatTracker song files (GTS5).

The play routine is copy paste from GoatTracker by Lasse Öörni.


# Next Steps

+ understand the mechanisms of songinit
+ simplify playroutine?

+ write `fs::Song`

+ write `fs::Player`
  + add functions to convert fs::Song <-> gt::Song
  + run both routines and compare register content

+ Do we really need a global wave table and pulse table?
  I want one per instrument.



## Pattern Row

```
            C-3 A A
            |   | |
      note--+   | |
instrument------+ |
   command--------+
```

+ 63 instruments
+ 63 commands
+ I need 63 glyphs
