v2.1.1a
Here's the change log:

-- Fixed sync game selected between cover modes
-- Added image for Devices
-- Fixed ui freeze (press LT+RT to get back to function properly before)
-- Added Swizzy rom loading data support from aurora now with gameprofile support
-- Re-Added psx.iso autoboot for freestyle dash/ aurora cover mode, now with gameprofile support. You have to put psx.iso in emulator root folder, and psx.iso.ini in the gameprofile folder and set AutobootIso = 1 in pcsx.ini on the emulator root
-- Fixed covermode initial value [cover modes freeze]
-- Fixed profile menu now working with profile setup for each individual game as it was initialy supposed to
-- Emulator assets are now packed into PsxSkin.xzp
-- Fixed LoadGameProfile at current selection so you dont have to press Y button to load gamesettings for booting game profile correctly
-- Fixed game profile loading after exit a game to select new game to play
-- Fixed shaders to re-add it into gameprofile settings instead of another .ini file in gameshaders since it doesnt make much sense
-- Added psedo new OsdMenu
-- Some code cleanups
-- New osdmenu ingame combo LB + RB + ABXY, now you dont need to change freestyle dash screenshot combo anymore
   Whats not included in this release yet for the lack of time/ not finished:
-- Html Game Guide
-- Games sorting by first letter
-- 4 player multitap support
   known issues:
   XUI Issue on osdmenu: if you autoboot psx.iso or load a rom from aurora, it kinda works.. but it will open main scene first then you have to select a game and it will open osdmenu and resume the supposed action. after that you can use it normaly

v2.1.0
* FEATURES

- Just one build. No more 2 builds(old and new cdr plugin). Cdda
games are working fine and the compatibility was maintained (dead or
alive and others games), in a single default.xex

- Added option to exit to dash on main menu.(right thumb click button)

- Added linear filter options(again)

- Fixed the weird characters that appears in the end of the text when you load a game guide. Byte order mark issues. Still, remember to
set any gameguide text on notepad to unicode big endian format ( File->save as, then change from ansi to unicode big endian, then save the gameguide.txt).

- Back("B") button is really a back button now, like it should be, in the main menu. On previous version, this button was used to load the
profile settings,sorry for my stupidity :P

- Added cpu bias option. Turning this option "on" will make the emulator run faster,
setting the bias in the core to "3", but it
will underclock the psx cpu. It's good for some
games that dont use the full psx power, most 2d games or some
3d games with less cpu demand like brigandine. Heavy cpu games (tekken 2,3) will suffer from a lot of slowdowns.

- Added Front mission 3 fix on main menu


* Bug regression-freeze fixes

- Soul Calibur(when you finish the game)
- Tomb Raider 3 (when entering on ingame menu and try to return to the game again)
- Brave Fencer Musashi (voices)
- Valkirie Profile (BGM)


* Freeze Fix

- Front Mission 3 (random freezes in battles when your wanzer or the enemy wanzer has the legs smashed)

--------------------------------------------------------------------------------------------------------------------------------------------

Important notes about the fixes and other stuff.

Some people are getting trouble to make the emulator work. It seems a compatibility issue between the new versions of freestyle dash and the emulator. For now, the only workaround i know is use a old version of freestyle. I'm using freestyle 3.0 revision 483 and it works fine smile1.gif

These fixes was not made by me. It was made thanks to efforts of lots of devs(shalma,edgbla,notaz,ckain and a lot others from the pscsx,
pcsx-df and reloaded teams. Big thanks to these devs, and of course, to ced2911, that brought to us the first stable 360 release smile1.gif

--------------------------------------------------------------------------------------------------------------------------------------------

source code included with this version

cheers !!!

cmkn1983

 

-fixed default.xex (crash-team-racing-fix)

v2.0.9
- Spu fixes(overal games are working with a little better sound effects. Noticiable in games like Ridge Race type 4, megamen x series, and maybe others.

- Fix some gameguide problems.

- Remove virtual memory.(VM broke some games).

- Some fixes on ppc dynarec found on pcsx reloaded source code.(lode runner now runs on dynarec).

- New shaders by retroarch added(new hls folder).

cheers!!!

cmkn1983

v2.0.8
Since some people still don't know what is this, here's a brief explanation ^^

Pcsxr-360 is a emulator that allows you to play psx games(iso, bin/cue, img format) on a rgh/hacked xbox360. You can launch the emulator using the freestyle dash. If you have some troubles to use Pcsxr-360 with more recent versions of freestyle, try use a older version.
The emulator works very well for me using freestyle 3.0B Revision 483.

The emulator was ported by ced2911 that made the first stable release using the last pcsx reloaded soure code. A first unnoficial version was made by dreamboy. Later versions was made by cmkn1983.

So here's the most recent version guys.

* Pcsxr-360-2.0.8 features:

- Reverted back to the old spu source code in the new cdr build. Overall emulation stability is better than sound effects :P

- Fixed some thread gpu problems in games that have some fmv/cg errors when the thread gpu is on( tested Alladin, Vagrant Story, Chrono Cross) as well some games with glitches( Moto Racer).

- Auto Region detection is always set to on. This will fix most sound problems on PAL games.

- Added ced2911 virtual memory for better speed on games that don't work with dynarec( lode runner, tekken2, brigandine, syphon filter 1). Treasure of the deep is the only game i tested that doesn't work with the virtual memory, so i made a option on game profile options to disable this feature.

- Added ced code for small/standart TVs.

- Added a better gameguide with full screen text. Finally.^^

* HOW TO MAKE A GAME GUIDE:

- Copy any text on notepad. Before save the file, remember to set the encoding to "Unicode Big Endian" format.

* HOW THE FILES MUST BE RENAMED TO MAKE THE GAME GUIDE AND GAME COVERS WORK:

- EX: "Alundra.bin" on "games" folder, "Alundra.bin.txt" on "gameguides" folder and "Alundra.bin.jpg on "covers" folder.

* Issues / Bugs:

- There some games that have slowdows when the threaded gpu is on. Try disable this option and load the game again.

- Sometimes, a kind of japanese letters with glitches, will appear in the end of the game guide text , and i don't know why :( but most of time it will not happen, and even if happen you can read all the game with no problems.

- Some games keeps playing the sound in the background even if you go to the osd menu. I know. This bug is pretty annoying :( I tried fix this problem sometime ago but don't worked.

- Cdda games have problems to load states when the virtual memory is on. Use "Disable Virtual Memory" to play cdda games.


v2.0.7
-Old build is working at right speed.

-Support for large .txt files on game guides.

-Change the name "Parasite eve fix" to "Parasite eve 2" fix in the game profile settings, since parasite eve 1 doesn't need this option enabled to work. Actually the game will freeze at the beginning if this option is set to on.

-The new cdr build is the default.xex now,since it have better sound/compatibility.

***************************************
HOW USE .TXT FILES ON GAME GUIDE:

-Copy any text on notepad. Before save the file, remember to set the encoding to "Unicode Big Endian" format. If you not do this, the text will be saved as ansi format and you'll have to read the gameguide in japanese

MANAGING SAVES ON MEMORY CARD:

-Go to the old cdr build. Press 'B" button on the 1_Memory_Card.bin file to load the right profile settings. Launch the file. The bios intro will load and you will be able to manage the files from memory card.

***************************************

v2.0.6.a
Features:

- Better sound. Thanks to ced2911.
- Better Gui with game cover and profile settings. Thanks to dreamboy.
-Two game view modes in game seletion both with covers(horizontal mode and vertical mode). Thanks to dreamboy
-Games will be recognized with the extension .bin, .iso, .img, .mdf so all the other extensions will be ignored (no more duplicated roms because of the .cue files, etc). Thanks to dreamboy
- Ability to load and save profile settings, including shaders. Thanks to cmkn1983.
Todo:
- Add a game guide reader. I'm still working on this :P

Instructions:

- Replace all files.
-Two little homebrew psx games are included in the release to show how the cover(back or front cover) files must be renamed and how the settings work.
-Put all your games(bin,cue, cdd, img,etc) files on "games" folder. Don't use a separated folder for each game inside the "games" folder.
- Make sure to make a game profile before launch a game, pressing Y button.
- Make sure to load a game profile before launch the game pressing B button(the game must be highlighted). A message showing profile loaded and game name will appear.
- Press “Back” Button to change the cdr plugin.
- Deleting files from memory card: Just load the psx bios(.bin file) on bios folder. you'll see a black screen. Press “start “and “Back “ buttons to go to OSD menu. Reset the game(bios) and the psx bios will be loaded.