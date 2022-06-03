# Golgotha
For more info, read [Manual.doc](Documentation/Manual.doc).

Setup instructions (from cd): Just run setup.exe in the same directory
as golgotha resides. Important: If you don't install the textures, the
objects or the bitmaps, the CD must be in the drive to play. There
won't be a senseful error message if it isn't, and the program may
silently fail.

When running Golgotha for the first time, it may need to rebuild the
texture cache (depending on the capabilities of your hardware). This
takes quite long, but needs to be done only once as long as you don't
change the render settings. The g_decompressed folder should always be
user-writeable.

A message saying that some write-permissions don't exist when running
for the first time can be ignored.


## License
See [License.rtf](Documentation/License.rtf) and the "[Credits](#Credits)" section below.


## Compile

### Install Dependencies
Compiling requires 32-bit at this time due to the LISP implementation (See issue #13).

Follow the directions in the subsection for your distro to complete the following general steps:
- Resolve
- Resolve issue #16 "video__x11__mitshm.o: undefined reference to symbol 'XShmPutImage'" and "/usr/bin/ld: /lib/libXext.so.6: error adding symbols: DSO missing from command line" by installing the i686 architecture variant of the libX11 extensions development package.


#### Compile on Ubuntu/Debian/Devuan
This does not work, at least on Devuan 4 (chimera) which is based on
Debian 11 (bullseye). The reason it cannot work is that the repo is
missing the following dependencies: libglx0:i686, libgl1:i686,
libglx-mesa0:i686, libgl1-mesa-dri:i686.
```bash
sudo apt-get update
sudo apt-get install -y libglu1-mesa-dev:i686
```

#### Compile on Fedora
```bash
sudo dnf groupinstall 'Development Tools' -y
sudo dnf install -y glibc-devel.i686 mesa-libGL-devel.i686 xorg-x11-server-devel.i686 libXext-devel.i686 libXxf86vm-devel.i686
```

## Use
If the program doesn't run, see the
[issues](https://github.com/poikilos/golgotha/issues) page of the
Poikilos fork of Golgotha and the "[Project Status](#project-status)"
section below.


## Parameters
For unix type systems, the registry is replaced by a file called "golgotha.ini".
```
-no_full:	Open the game in a window on the desktop (default is fullscreen)
			This is required for debugging except if a second monitor (or
			remote- debugging) is available.
-full	        Start in fullscreen mode, even if the registry setting is windowed
-default        Use 640x480x16, regardless of registry setting. Usefull if you
			choosed some setting that doesn't work on your system
			and you therefore cannot change the settings any more.
-setup		Show the configuration dialog right on startup (use if golgotha won't start
		with the default settings)
-no_sound       Disable sound
-eout filename	Write (append) the debug log to the given file.
-max_memory n	Maximum amount of Memory to be used for i4_mem_manager
-edit		Start in edit-Mode
-eval		Command line lisp-evaluation, don't use for now.
-frame_lock     Force one frame per think() loop.
-movie		What format?, where?, when? Unimplemented
-sfxdbg		Display some debug information about sounds. (i.e. Position, Loops)
-no_demo	(default)
-cd_first	check golgotha.cd before local files. Has no effect if no
			golgotha.cd present. Unpacked files on a CD are always used last.
			(See Search-Path below)
-demo		(ignored)
-3dsound	Use DirectSound3D, defaults to true now.
-display <location> Specify the connection to the X-Server (i.e localhost:2)
                (Only for unix versions)
-ron		Enable alternative handling or keyboard reapeat events.
		    Usually not required (X server drivers only)
-no_mitshm  Disable MITSHM extension on X Windows display driver. Try this if you find
			that golgotha crashes with strange X warnings. (X server drivers only)
```


## File Search-Path
The Code behaves as follows if it looks for a file.
(see file__file.cpp)

Case A: -cd_first is not specified (default)
1. Path relative to current directory.
2. Root of first CD-Rom Drive
3. `\golgotha` on first CD-Rom Drive
4. Root of second CD-Rom Drive
5. `\golgotha` on second CD-Rom Drive
6. (more CD-Roms)
7. golgotha.cd (the first one that was found, if one)

Case B: -cd_first is specified
1. golgotha.cd
2. Path relative to current dir.
3. Same as above...

If the file is not found, the system may print a debug message to the debugger.

See file__file.cpp for debugging options.


## Data-Files
- .res:
  System-config-files (and compiled windows resources (don't mix!))
- `textures\*.jpg`, `textures\*.tga`:
  The Textures. TGA-Files may have ARGB format. Use TGA whenever
  you need alpha channels or partially transparent textures.
  TGA files must not be in compressed format!
  Be sure that there is newer a texture available in both formats.
  Internally, jpg would be prefered over tga, but that's most probably
  not what you want since tga's usually have bether quality than jpg.
- `g_compressed\.gtx`:
  temporary storage for TGA-files. _deprecated_
- `g_decompressed_(%Bitdepth)\*.gtx`:
  Unpacked Graphics-files (to different
  mip-levels)
  Only TGA textues still use this technique. JPG are run time decompressed
  _deprecated_ for all cases
- `g_decompressed\*.dat`:
  texture cache files for different rendering devices.
  - They are automatically created and updated if necessary.
- .scm: lisp AI-Scripts and in-game configs.
  - They should replace the .res - files sometime, as they are much more
    powerful.
- .wav:
  You know what that is, don't you? For Effects, format is
  16Bit 22khz, Mono. For Background music, usually 16bit 22khz, Stereo
- .mp3:
  Music, not currently used in this format.
- .gmod:
  Golgotha Model Files: Contains the objects (buildings, tanks, trees...)
- `bitmap\`:
  This directory contains files that are used directly. (Icons,
  Main-Menu GFX)
- `sfx\`:
  Here are all the sounds.
- .level:
  The maps or savegames.
- Makefile.*:
  Makefiles for different platforms and different compilers.
  - Copy the Makefile you want (i.e. 'Makefile.linux.gcc') to
    'Makefile' and type 'gmake' to build the project.
  - For MSVC, use Golgotha.dsw as workspace file.


## Some Symbols for preprocessor
- `_WINDOWS`  ->use this for windows
- `__linux`  -> and this for linux
- `__sgi`   -> and this for sgi
- `SUN4`   -> and this for SUN4

(for the above ones, check the different Makefiles in the source code distribution)

- `_DEBUG` -> Use this one instead of next
- `DEBUG`  -> Should be Replaced with MSVC's `_DEBUG`
- `WIN32` -> Also defined in windows
- `i4_NEW_CHECK` -> Leave as is.
- `i4_MEM_CHECK`
- `i4_MEM_CLEAR`
- `USE_ASM` -> This one and the next is for the assembly implementation of
  the software renderer. Check out render__software__mappers.cpp.
- `USE_AMD3D` needs some extra-files render__software__amd3d...
  - Hint: Assembly code can currently only be used with MSVC.
- `LI_TYPE_CHECK` -> Almost always set (performance degradation should be negligible)
- `[JPG_LOADER]` -> Don't touch these, as long as everything works...
- `ENTROPY_OPT_SUPPORTED`
- `C_MULTISCAN_FILES_SUPPORTED`
- `FULL_COEF_BUFFER_SUPPORTED`
- Many more in loaders__jpg__jcapistd.cpp


## Project Status
- See "Tasks" in [Documentation/development.md](Documentation/development.md).


## Credits
People that need to be mentioned in the credits:
(add yourself if you want to one or more groups)

### Coding
Jonathan Clark
Jan Jasinski
Ferret Malicious
Patrick Grawehr

### Music
- Hunting the Enemy - MP3
  Classical, VideoGame
  Year:1998 - From the Album: Four
  Ratings
  May be freely distributed without profit; may be played on radio; may not be sold
- others: Crack dot com

### Sfx
  - Crack dot com

### Levels
- test: crack dot com
- egypt: originally crack dot com, upgraded by Patrick Grawehr
- snow: originally crack dot com, upgraded by Patrick Grawehr

### Objects
Crack dot com

### Textures
- most: Crack dot com
- some free sources on the net, like 3dcafe (www.3dcafe.com)
- data becker "media pool" cd.

### Bitmaps
- Crack dot com
- Patrick Grawehr

### Beta Testing
- Edward Downling

### Web Server
- Christea Virgil Ionut

Special thanks to Jonathan Clark and his Crack Dot Com team for releasing the
source code of this great game into public.

Unfortunatelly, I was unable to find out the names of the other members of
the original Crack dot com team. If anyone knows, please add them above.

## Changes
See [changelog.md](changelog.md).
