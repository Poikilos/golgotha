# Development

## Contributing
### i4_win32_startup_options
If some parts of your code access i4_win32_startup_options, be aware
that the options can change at runtime!

```C++
Initalization (in init_init.cpp: void i4_init(void))
0=I4_INIT_TYPE_MEMORY_MANAGER,     // main i4 memory manager
1=I4_INIT_TYPE_PRIORITY,	   // things that need to be initialized before anything else
2=I4_INIT_TYPE_THREADS,            // initialized thread info
3=I4_INIT_TYPE_LISP_MEMORY,        // for lisp object allocations - uses i4 memory manager,
4=I4_INIT_TYPE_LISP_BASE_TYPES,    // adds lisp types into the system (li_int.. etc)
5=I4_INIT_TYPE_LISP_FUNCTIONS,     // adds lisp functions (li_load & any user li_automatic..)
6=I4_INIT_TYPE_STRING_MANAGER,
7=I4_INIT_TYPE_FILE_MANAGER,
8=I4_INIT_TYPE_DLLS,
9=I4_INIT_TYPE_BEFORE_OTHER,       // init just before the big mess starts
10=I4_INIT_TYPE_OTHER,
11=I4_INIT_TYPE_AFTER_ALL	  // For objects that rely on some that get inited on OTHER.
```

## Tasks
- [x] Critical Sections don't really work. (This program is multi-threaded!)
  This Bug has been fixed.

- [x] objs__map_piece.cpp(938): i4_warning("i can't get there!"); [Illegal Waypoint cause for malfunction of Unit creation?]
  This Bug has been fixed.

- [x] fix 3D-Sound.
  This has been fixed
  - [x] Removed obsolete references to a3d.dll

- [ ] Allow use of 24 or 32Bit color-depth.
  - Seems to partially work. Needs more testing and some additional coding to get
    some parts (i.e. the radar) working in these modes.

- [x] The in-game-messages are corrupted
  This bug has been fixed by partially reimplementing the Text-Window.

- [ ] Decode and Play MP3
  Not first priority, as playing MP3 needs more CPU-Power than WAV.

- [ ] golg__map_lod.cpp(302):
  The setting below should be user-definable as it has a big influence on
  quality and rendering speed.

```C++
i4_float fx = pos.x - mx, fy = pos.y - my, fz = pos.z - g1_get_vertex(mx,my)->get_height();
		i4_float d1 = fx*fx+fy*fy+fz*fz;
		i4_float d2 = p->metric*200; //here!!!
```

- [x] render__tmanager(504):
  `desired_width = max_texture_dimention;` reducing max_texture_dimention here
  will reduce texture quality but needs less memory. (must be multiple of 2)
  setting it to 8 will use the preloaded lowest miplevel.
  - These settings can now be controlled in the options dialog.
  - [x] Known problem: The textures are now scaled to the choosen
    size even if they are much smaller. (Waste of memory)
    - This has been fixed. The setting allows to define the highest miplevel for all textures.
    - This includes foreground ones (like the target-crosshairs), will fix that sometime.

- [ ] Improve the LISP-ENGINE to the level of Abuse
  must implement most system functions, namely the arithmetic functions and
  the "defun" operator
  - new types: LI_USER_FUNCTION, LI_VECTOR (one dimensional array of arbitrary size)
  - Need to get the emacs-lisp-examples to run.
  - Most of this has been implemented. flame.scm (the emacs flamer) works!

- [x] Fix the mapcar-code
  - fixed.

- [x] Fix Mouse in FS, don't reallocate the mouse-backbuf every time
  - fixed.

- [x] Fix Maxtool in FS
  - fixed. (currently, if double-buffering, we copy the frontbuffer back every frame)

- [x] Save Detail level in registry
  - done.

- [x] Improve opening-screen
  - done. Might be even better. Need to add possibility to play intro-movie.
  - This is possible now. Use (play_video FILENAME) for it.

- [x] Allow exclusive focus on the keyboard for textual input (no side-effects desired)
  Implementation done.
  - Even modal windows are possible now.

- [x] Allow direct access to options and lisp engine on command-line
  not yet implemented.

- [ ] Fix the sky and the sky-select window
  - still buggy.

### Known Issues
- [ ] User Functions get their arguments once evaluated (li_bind_params evaluates args), while
  System functions get their parameters unevaluated.
  - This is an inherent problem of the language, that cannot be changed, CLISP
    has same problem.

- [x] Reseting the Texture Manager when a map is loaded is impossible
  - Cause all Texture handles get lost. Need to implement a "reopen()"
    method to free memory but not unload the tman. Then it should be
    possible to create a new instance of the texture manager.
  - Creating a new instance of the tman and then reverting to the default
    one must restore the global pointers to the instances.
  - Done, works mainly. Some trouble with the sky. Need to fix that anyway.
  - Did many fixes again. Restoring the textures after ALT+TAB seems to work now.

- [x] g1_saver_classes get leaks references.
  - Fixed.

- [x] verybiggun needs fixes.
  Done, seems to work.
  - [ ] Fires very inaccuratelly.

- [x] field_camera occupy_location() must be changed.
  - Done. References to field_cameras seem to get leaked.

- [x] bird needs fixes.
  - Done. Looks fine.

- [x] cobra_tank needs proper think() method.

- [ ] changing the height of objects in the editor needs fixing.

- [x] where comes the window for ambient sound-editing from?
  - Found.

- [x] object display window in editor (on the pick-objects list) needs fixing.
  - Looks acceptable.

- [ ] what can be done with sprites?

- [x] Selecting objects during game doesn't work. Must look at g1_render.current_selectable_list.
  - golg__controller Line 415.
  - Fixed, works very fine.

- [x] Must add a function to change the players ai.
  - Done. The ai of each player can now be set in the player params dialogue in the editor
  - [ ] The different ais need fixing, though.

- [x] Moving the mouse to the edge of the controller - window should look around
  in strategic and action modes.
  - Implemented in a very simple way.
  - [x] Doesn't work yet in strategy mode.


## i4-class-hirarchy

### Window-Management

i4_style_manager_class (mit its static instance i4_style_man) handles
all window styles. The styles are of type i4_graphical_style_class
(An almost pure virtual abstract class).
That one is a child of i4_init_class.
Currently, i4_mwm_style_class is the only implementation of
i4_graphical_style_class and therefore it defines the look of all
windows.

The window-manager itself is a child of i4_event_handler.

- i4_event_handler_class
  - ...
  - i4_window_class
    - i4_mwm_....
    - i4_parent_window_class
      - ...
      - i4_window_manager_class

### Object-Handling
Ableitungen von g1_object_class
