# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).


## [1.0.9.2]
### Added
(common)
- It's now possible to control every single unit by hand.

### Fixed
(common)
- Subversion on qbic is operating fully now. See the changelogs
  there for details.
- Improved the border frame window.
- Fixed some old and anoying bugs.


## [1.0.8.4]
### Added
(common)
- Made the golgotha source code including the version control
  available on qbic.vipnet.ro.

### Fixed
(common)
- Lots of cleanups.


## [1.0.8.3]
### Added
(FreeBSD)
- Golgotha compiles and runs under freebsd. Checked with gcc-3.3.
  - Does *not* compile with gcc-2.95 (the default on bsd) because
    that one can't properly handle CR/LF linefeeds.

(common)
- The maxtool can now create and render an octree from an object.
  - Saving and loading is not yet implemented.


## [1.0.8.2]
### Added
(common)
- Lisp symbols can now be attached properties. The commands are
  (put 'symbol 'name 'value) and (get 'symbol 'name)

### Fixed
(common)
- Fixed several minor bugs all over the place.
- To avoid problems in networking, all object types are now sorted
  by name before instantiating the objects themselves.

(linux)
- Fixed some minor compiler-related inconsistencies to get the
  code to compile under linux again.
- The network code is running under linux now. Windows and linux
  version talk to each other (with the limitation that the network
  protocol of golg is incomplete and has bugs)


## [1.0.8.1]
### Added
(common)
- Added a feature to change the cloud texture. (should do more on the sky)

### Fixed
(common)
- Redesigned the lisp memory management code: All global memory
  limitations are gone. (Except for the 4GB mark, of course)
- Fixed a minor bug in the lisp code that could lead to inconsistent
  data handling.
- Lisp dialogs now properly display the name of boxed classes.
- Fixed some other bugs in the lisp code and the garbage collector.


## [1.0.8.0]
### Added
(windows)
- Golgotha builds again under VC6, including the DirectX9 support.

(common)
- New feature added: Zooming. Can only be used manually now. Press
  Alt-F7 to see the camera window. Change the scale_x and scale_y
  parameters to zoom. Larger values bring you nearer to the target.
  Values smaller than one currently result in missing polygons
  and on values larger than one, the detail level is not increased
  for far objects (that should become visible if zooming).
  Using non-square values (scale_x and scale_y differ) will yield
  a very funny picture...

### Fixed
(windows)
- The DX9 Mouse is almost functional. Since dx9 uses page flipping
  again, some minor glitches might occur.

(common)
- Fixed a small bug that made some dialog windows a bit dangerous to use:
  The yes and no buttons had the same action.


## [1.0.7.0]
### Added
(windows)
- Added a new rendering device driver for DirectX9. This one is now the default
  and will always be used if available. The usage of DirectX9 avoids
  some known bugs of DX5 and might increase performance on newer systems.
- A setup script has been generated and can be used now.

### Changed
(windows)
- I'm now using Visual Studio.NET, so exspect additional workspace files.
- Golgotha also builds on VS.NET 2003, after some minor modifications.

### Fixed
(windows)
- The Mouse cursor is for dx9 is not yet functional, gotta do this soon.

(common)
- The Cobra Tank lisp implementation is functional.
- Fixed a few flaws of the level editor.


## [1.0.6.1]
### Added
(unix)
- Added new Makefiles for other plattforms/compilers.
- The unix builds now also have a useful error-window.

### Fixed
(common)
- The software renderer now uses true alpha scanline drawing as default.
- Improved lisp object creation functions. (needs further work)

(unix)
- The Settings are now correctly saved and loaded from golgotha.ini.
- Software rendering with an X11 type display now works.
  (add a line "display=X Windows" in golgotha.ini)

### Removed
(unix)
- Removed all STL code from app_cdatafile.


## [1.0.6.0]
### Added
- Golgotha compiles and runs on SUN Sparc Solaris and Silicon Graphics SGI with OpenGL support.
- Golgotha now has the software renderer builtin again. Works under windows
  in either directx mode or plain gdi mode. Unix not tested yet.

(unix only)
- The settings are now saved to a file called golgotha.ini.

### Fixed
(common)
- Further improved collision handling.
- Improved the enemy stank ai a tiny bit, such that he doesn't get stuck so quickly.
- Fixed a few bugs in the texture management.
- Cleanup in arch.h.
- Perhaps the sky is now finally looking kinda like it should?
- Objects can now be added using plain lisp code only.
- Implemented a kind of 3d-logo programming to learn about vector translations and
-   other matrix-vector operations in 3d space.
- Fixed a bug in the A* map solver that caused stanks to take bad paths. Unfortunatelly,
-   A* doesn't seem to be really much faster that breath first search in most cases,
-   perhaps we need some improvement here.

### Fixed
(unix only)
- Fixed a bug that caused the ai to fail on big endian machines.


## [1.0.5.6]
### Fixed
(common)
- Improved collision handling (still not fast and precise enough)
- The statistics window now correctly displays the number of
  elapsed frames and the number of calculated ai-steps.
- Fixed a bug that prevented the placement of some objects at most
  locations in the editor.
- The radar can now be correctly hid in the ai-window. An enemy stank
  no more uncovers it.


## [1.0.5.5]
### Fixed
(Windows only)
- Continued working on the network code.
- Fixed the "Win2000/XP show no sensefull error messages" bug.

### Fixed
(common)
- Added support for special unit commands. The models for the buttons still need
  to be written and the button placement is bad.


## [1.0.5.4]
(Windows only)
### Added
- Partial network support added. Still need to implement all the load() methods.
- Did a few tests with stereo-vision support.

### Fixed
- Fixed a few strange bugs in the math library.


## [1.0.5.3]
### Fixed
(common)
- Huge cleanups in the texture management. Now all textures load directly
  from the textures folder. No more g_compressed folders needed.
- Lots of bugs fixed that became visible with the above change.
- The tex_chache.dat file now also contains lowmips for 32 bit texture modes.
- I hope I haven't broken anything that worked before. (Did some tests,
  but there might still be unhandled cases)
- Hint: After updating to this version, you can delete all g_compressed and
  g_decompressed* folders. They will be rebuilt if required.


## [1.0.5.2]
### Fixed
(Windows only)
- Optimized the asynchronous jpg texture loader.
- Golgotha now supports 32 Bit texture modes. This requires a screen resolution
  with 24 or 32 bit color depth. Hint 1: Some older graphics cards might not
  support hardware acceleration in these modes, so the performance can drop
  significantly. Hint 2: There's no texture cache for 32 Bit textures yet,
  so loading can take quite long.
- You find the new settings on the graphics tab in the options.
- Better support for different software renderer types.

(Linux only)
- The higher texture levels now load correctly.
- File load debug statements added.
- Fixed a strange bug with multiple render windows in the editor.
- Fixed a shutdown error.
- The button images now display correct collors.
- Fixed the render-window-overlaps-menus-and-windows bug.

(common)
- Added set and get texture methods to texture manager.
- The async readers now use 2 different queues depending on priorities of
  the request.
- Image32, Image16, Image24: Improved copy() method.
- Image8, i4_pal_man: Fixed a nasty but that caused palettes to get screwed up
  esp. under linux.
- The stank now has a different model for the top if viewed from outside.
- The sky and the clouds now look _much_ better. Choosing the sky now works.
  Need still changing that model.
- Fixed a memory leak in maxtool.
- Added fn to update the lod - texture (not yet working properly under linux)
- Fixed some bugs in the transportation simulation.
- Minor performance improvements.


## [1.0.5.1]
### Added
- Golgotha is compiling, linking and running again under linux!!!
- Just use the included "Makefile" to get it done. You need to have
  the "mesa" and "mesa-devel" (or similar) opengl support package installed.
- I'm using gcc V2.95 on mandrake 8.1.

### Fixed
- Further improved the transportation simulation.
- Fixed some minor bugs.


## [1.0.5.0]
### Added
- I'm using VSS now. The Complete what's new feature is now included in a file
  called ssreport.txt.

### Changed
- Improved the selecting and comanding of units.
- Units can now be pointed at to show their remaining health.
- Implementing a transportation simulation on golg, for my studies.
- Increased the flexibility of most data structures to support a larger
  amount of objects on the map. (must still be improved)


## [1.0.4.1]
### Added
- New Objects: Lawfirm, armor (no visible effect yet)
- Supergun accepts user targets.
- Most units can now be selected and manually commanded.
- In action mode, you can now move the cursor to the edge of the
  screen to turn faster.

### Fixed
- Fixed another bunch of memory leaks in the object management.


## [1.0.4.0]

### Changed
- Default position for the statistics window (FPS, Polys) is now top
  right corner of screen instead of center.

### Fixed
- Got the following objects to work: Bomber, Rocket tank, tank buster,
  Jet, Bridger and the supergun.
- Fixed a bug in the texture management, that caused some textures to
  get screwed up on DDERR_SURFACELOST.
- Hope to have fixed a bug to avoid a fatal error loading lowest
  miplevels.
- Remark: Release build is about 5 times faster than debug build.
  I reach about 20 to 27 FPS on my Celeron-433.


## [1.0.3.6]
### Added
- Added new editor mode to find out infos about already placed tiles.
- Tile picker now shows the name of a texture.
- Partial implementation of lisp bignums working. (addition, subtraction,
  multiplication). Use a "b" after the operation to use the bignum-version.
  I.e (+b 999999999999 88888888888888)
  Try (! 100) *g*
- Added some other lisp system functions.

### Fixed
- Fixed a bunch of memory leaks.


## [1.0.3.5]
### Added
- Implemented possiblity for playback of avi files.
- Updated AI-Window with a few new modes.
  Now three different path-solving algorithms are available:
  1. Breadth_first_map_solver: Finds the shortest path for a given
     grade (uses blocking indications on the map)
  2. Astar_map_solver: Finds shortest path using some info
     on the map (I think this one is usable only for supertanks
     or airplanes)
  3. Breadth_first_graph_solver: Uses a precompiled graph to find
     a path (requires that the map contains one, test.level doesn't)
- Checkboxes and Radiobuttons are now available as new controls.
  - A group box for radiobuttons that ensures exactly one is choosen
    is also available.

### Fixed
- Fixed a bug introduced with the changes to the sound-engine.

### Removed
- Removed main__win32_self_modify from project. (Not needed)


## [1.0.3.4]
### Added
- Implemented the possiblity to use modal dialog boxes. Create them using
  style->create_modal_window(). Any mouse-down events outside the
  modal window are ignored. The key-manager is inactive, too.
  Modal windows immediatelly get the fucus when opened and cannot loose it,
  except if another modal window is opened on top of them.
  Still missing: Some feedback to the user when clicking outside
  of modal windows. Not doing this now, as the soundman needs work
  anyway. Should do that in one part.
- Extended capablilities of singal-objects.

### Fixed
- All error or warning messages are now broadcasted through the kernel.
- Found a problem with the map. If we cannot load some of the texture files
  ("Texture %s not found in texture cache...") the index of the map
  texture doesn't get recalced and therefore is invalid (reverts to
  white).
  Workaround (not fix): Check that the scm-file doesn't contain
  inexistent textures.
- Found the Bug that causes reloading of textures to fail: memory_images
  (the map) are not conserved upon reopen() and load_textures().
  This bug has been fixed.
- The sky still needs fixing. Not doing this now, as it will be entirelly
  rewritten.
- There seems to be a bug in the sound engine when no sound is requested
  or forced (i.e Winamp running). We run out of mem?!

### Removed
- Removed os-dependent code from golg__sound_man.cpp. Now uses
  i4_signal_object for synchronisation.
- Removed obsolete reference to a3d.lib and a3d.dll.


## [1.0.3.3]
### Changed
- The radar no more uses G1_RADAR_INTERLACED.

### Fixed
- Restoring all the surfaces and textures after DDERR_SURFACELOST seems
  to work.
- Reloading the textures still has trouble (max_view_distance becomes
  important then, this needs fixing)
- The Target of the enemy supertank is no more shown in action mode.
- Resizing the main window in windowed mode works for action and strategy modes
- Added new error-dialogue.
- The border frame now displays the active ammo types. (needs change
  though, as sometimes, supertank updates are on the spot, other
  methods don't change until the next rebuild)
- Other updates I don't remember.


## [1.0.3.2]
### Added
- Added many editing options to Maxtool
- Added old ai code

### Changed
- Combined res/ and resource/ directories in resource/ directory.

### Fixed
- Fixed a bunch of bugs, i.e. Texture loading, Object Picker class
- Enemy is using his SuperTank again
- Fixed draw_3d_line() and Maxtool to prevent overwritting of windows.


## [1.0.3.1]
### Added
(Implementing the missing CLISP-functions is just a matter of time.)
- The lisp-engine has been updated. Most features are implemented, including:
  - Lambda functions
  - defun operators
  - mapcar and other functions that take functions as operands
  - many system functions such as arithmetics, stacks, internals
  - backquote corrected
- Started to update the maxtool to become a fully usable 3D-Object editor.
- Implemented a command line utility to enter lisp commands.
  - Press F11 to see the window. Press ALT-F11 to open the debug window
    which takes the replies. This will change soon.

### Changed
- Small change to the initialisation sequence.

### Fixed
- Fixed full-screen-mode (no more flickering, but a bit slower)
- Options fixed.


## [1.0.2.2]
### Added
- Included the Maxtool in the Golgotha Main Executable. This simplifies compilling
  and debugging. Press F10 to get there.
- Some additions to the Option-Settings. Rendering Quality can be choosen.
- A rendering device can now have multiple texture managers attached to it.
- The Texture manager can now reload it's textures without the need to
  reregister them. This helps in situations, where the tman needs to reload the
  textures (i.e Maxtool) but the main textures shan't change.
- An intro-image is now displayed.

### Fixed
- Fixed a bunch of memory-leaks.
- Alpha mode fully working.
- Fixed a bug in the texture manager that caused an overload of the disk cache and
  used up far to much memory.
- Out of Memory conditions are now reported to the user before the process quits.
- Implemented i4_char_send_event_class: Should be used to send ascii data to windows  where translations are welcome. (The textinput-window is currently the
  only one to use it)
- Fixed a bug that caused the kernel to loose events when a WM_KEYUP message
  corresponding to some WM_KEYDOWN event got lost due to the window losing focus.


## [1.0.2.1]
### Added
- Implemented reverse-lookup of texture ids
- Added some system-functions from abuse to the lisp-subsystem, needs
  much more to get it completed
- Landscape textures are preloaded during map-rendering.

### Fixed
- Reimplemented Extras/AI Menu for old Maps
- JPG-Textures are now run-time decompressed
  (Software Renderer has not been adapted yet, so doesn't work right now)
- 32Bit-Color Depth Mode partially works (no map, textures still 16 bit)
- Improved Options, Graphics Settings are now usefull
- More Progress bars visible
- Fixed some of the textures with color key. Alpha still needs some work.
- Implemented i4_temp_file_class
- Some minor changes to the memory-subsystem. This mainly has the purpose
  of using the default c++ new operator instead of I4_MALLOC to get
  more memory.


## [1.0.0.1]
### Fixed
- Fixed Unit doesn't see first waypoint bug
- Implemented searching the CD for files
- Reactivated the menu.
- Implemented Options to set Resolution and Sound
- Fixed Critical Sections
- Fixed Soundman-Poller (now Real-Time-Capable)
- Fixed the scrolling in-game text window.


## [1.0.0.0]
(Original Version as published on www.jonathanclark.com/crack.com/golgotha)
