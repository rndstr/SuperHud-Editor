http://plrf.org/superhudeditor

# SuperHud Editor v0.3.0

SuperHud Editor gives you a graphical interface to edit your HUD 
(Head-up display) for [CPMA][1] (a Quake3 modification) and possibly other 
first-person shooter games in the future.

_Was initially hosted on sf.net but moved to GitHub due to sf's downfall._


## Using SuperHud Editor

You start with a default HUD supplied by CPMA. Other HUDs are placed in 
<quake3dir>/cpma/hud/
Make sure you save it to that directory. Otherwise you cannot use
them ingame.

To tell cpma which hudfile you want to use set the cvar ch_file to
the name of the hud (without the extension .cfg).
For example if you want to use hud3.cfg, put in <quake3dir>/cpma/autoexec.cfg 

```
seta ch_file "hud3"
```

The GUI should be more or less self explanatory, check the ChangeLog for
a few features to look out for.


## Config file

The application's config file will be automatically created at `~/.superhudeditor/superhudeditor.conf`
(or `C:\Documents and Settings\<username>\Application Data\q3cpmahudeditor\superhudeditor.conf` on Windows)

Some preferences that cannot be accessed in the app but still might be of 
interest (name=default value):


`view_updatewhiledragging=true`
  Set to false if drag&drop is laggy

`q3_homedirname=.q3a`
`q4_homedirname=.quake4`
  Only available on non-win32. If your user specific settings/installations 
  are in a different directory in your homedir, change this.
  Although it should probably be preferred to create a symlink. But the 
  default above is where we additionally look for paks&stuff.

`q3_pakfiles=baseq3/pak*.pk3;baseq3/map_cpm*.pk3;cpma/z-cpma-pak*.pk3`
`q4_pakfiles=q4base/pak*.pk4;zz-q4max-*.pk4`
  In which pak files we look for gamefiles (images, defaulthud, fonts, etc)
  Do not change unless you are sure what you are doing.


## Images/fonts from pk3 files

In hud elements you may also specify which image to draw as background.
Those are taken from the paks the game provides which are normally not
subject to change but restricted which ones you can use. That restriction
depends on the server you are on, you can only pick images from paks the
server has.

Hence you should (normally) stick to those images that are in paks which are
on _all_ servers. The pak browser only presents you those available files.
A missing image (or from a non-standard pak) will be displayed as big red 
cross.

## SuperHud

A (mostly) complete documentation about SuperHud files can be found
at the [cpma wiki][2].

## Bugs

Known bugs can be found in the source distribution in file KNOWNBUGS
Limitations of SuperHud itself can be read at the [promode wiki][3].


## Updating
By default, at each start the app checks in the background if a new version 
is available and prompts you what to do if it found an update. 
SuperHud Editor now features a Web Updater which takes care of updating, no
need to download and install the new version. At least on windows. For linux
this feature is disabled as it probably requires superuser access with the 
default installation routine. A notifcation about new versions is displayed 
nevertheless.


[1]: http://www.promode.org
[2]: http://www.promode.org/wiki/index.php/Custom_HUDs
[3]: http://www.promode.org/wiki/index.php/Custom_HUDs#Known_Issues

