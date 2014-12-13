ld31 - Cold Remembrance
=======================

Repository for my Ludum Dare #31 entry.

On this README, you can find instructions on how to compile this game for
Windows (using MinGW or something alike) and Linux. Note, however, that I
only built it on Ubuntu, and can't assure that it will work anywhere else... It
should work on any platform that supports SDL.
Also, note that I never tested this game on Linux 32bits and Windows 64bits.

How to compile
==============

First of all, get SDL2:
```
$ sudo apt-get install libsdl2-dev
```
On Windows, you'll probably have to compile it manually... That's what I did, at
least. The exact SDL2's version I'm using is 2.0.3

Then, check that the framework was also downloaded into libs. It's required to
successfully compiling the game. If you can't find it, run the following
commands, if you cloned from git:
```
  $ git submodule init
  $ git submodule update
```
Or, if you downloaded the project's zip, manually download the framework (you
can find the correct revision by browsing to ```libs/GFraMe```) and unzip it
on ```ld31/libs/GFraMe/```.

If you are on a 32 bits machine, you'll have to manually modify the Makefile and
change the ```-m64```, on line 13, to ```-m32```.

Now, if you want to compile with sounds, run
```
  $ make RELEASE=yes
```
Otherwise, simply run
```
  $ make
```
Note that to change from a version to the other you must recompile the game,
i.e. run
```
  $ make clean
```
At this point, you'll be ready to run the game without sounds. But if you
compiled the release version, you'll have to manually compile and add the song
to the assets folder.
If you are on Windows, you'll have to take step, afterward.

Rendering the song
==================

To render the song you can use any software that renders Macro Media Language
into WAVE files. The one I used both to write the song and to render was
VMML, by  @benjamin_soule. It can be found at:
http://benjaminsoule.fr/tools/vmml/.
The song can be found at ld31/assets/vmml/song.mml. After rendering it, put it
into ```ld31/assets``` with the name ```song.wav```.

Adding an extension to the sfxs
===============================

Because of some mistake while coding the framework, the song binary files on
Windows requires the extension '.dat', while they work as-is on Linux.
So, if you are compiling on Windows, be sure to append '.dat' to every file on
```assets/``` that has no extension.

Running the game
================

Make sure that the directory ```assets/``` is on the same directory the binary is at.
That's the default disposition, if you downloaded or cloned this repository.
Other than that, simply run
```
  $ ./game
```
on Linux or double-click the executable on Windows.
