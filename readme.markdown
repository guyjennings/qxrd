# QXRD Change Log
See also the [QXRD Git Log](http://qxrd.git.sourceforge.net/git/gitweb.cgi?p=qxrd/qxrd;a=shortlog)

## v0.11.7 - Oct 10 2014
Added user interface to display shrinking routines

Fixed problems displaying pilatus images

## v0.11.6 - Sep 2 2014
Added simple calibrant diffraction angle calculation routines

Plot of fitted ring TTH and calibrant TTH vs azimuth

Added file access scripting commands

Preferred qt version is now qt5 for all platforms

## v0.11.5 - Aug 20 2014
Improved memory handling during acquisition - should drop fewer frames

Added optional automatic retry mode for acquisition - dropped frames will be repeated automatically
during acquisition.   Note that will affect acquisition timing.

Added a saver backlog counter to go with the processing and integration backlog counters already provided. The
backlog counters should now count down to zero properly as well.

Tidied up powder ring fitting routines and removed some redundant duplications.

## v0.11.4 - Aug 11 2014
More work on fitting - ellipse fitting added

MINGW builds work again

Manual control of output voltage in sync acq dialog

## v0.11.3 - Jul 31 2014
Implemented histogram window

Extensive rework of powder ring and peak fitting code

Some work on a distortion calibration from grid mask images

Script Editor window added

## v0.11.1 - May 21 2014
First release in 0.11.x series ...

Major changes to use current versions of qceplib 0.1.4, qwt 6.1 and qt 5.x

Still problems with qt 5.x builds so still include qt 4.8.5 build

Minor updates:

Various changes related to saving integrated data

## v0.9.17 - Nov 8 2013
Fixed problems with saving user defined properties in acquired images

Added a timeStamp property to acquired images - number of seconds since unix epoch,
with msec resolution if supported by OS

Can change the detector type for an experiment from the preferences dialog

## v0.9.16 - Oct 4 2013
The application will now always re-open the most recent experiment when restarted

Made proper distinction between the experiment directory - where experiment and log files are stored,
and the data directory - the root directory where data files are stored

Removed some confusing commands related to new experiment creation

Rearranged order of widgets in acquisition panel and experiment preferences dialog to better
reflect hierarchy of directories used.

## v0.9.15 - Sep 12 2013
Added an experiment.detectorNumber parameter to permit working with multiple detectors on the same machine.

Added detector number control widget in the acquisition dialog.

## v0.9.14 - Aug 12 2013
Fixed race problem with acquire and acquireDark - there was a race condition where
the actual start of acquisition could be delayed till after acquire returned, meaning
that a call to status would erroneously indicate that acquisition had succeeded.

Added more information in the log file when an experiment starts up - show version
information, host name and type, experiment name and path

Fixed problems with conversion of array and list values to strings in scripts

## v0.9.13 - Jun 21 2013
Added more routines to access powder points

Improved display of script values: arrays and objects are now printed
out property by property.

## v0.9.12 - Jun 13 2013
Added the ability to set user-defined properties on the 'acquisition' object - such
properties are copied into acquired image files.

Fix for crash when moving image center parameter during acquisition

Fix for initial detector gain problem after restart

## v0.9.11 - Jun 4 2013
Added script functions to access and modify the marked points in images

## v0.9.10 - Mar 18 2013
Fixed problems with integration corrections

Added commands to save cached integration arrays for inspection

Made the centering dialog more compact

## v0.9.9 - Mar 13 2013
Added support for user-defined calibration functions in integration

Modified build system to use git submodules - need to do 'git submodule init'
and 'git submodule update' when checking out source tree.

## v0.9.8 - Jan 23 2013
Fixed problem with remote command execution

Updated Qt to 4.8.4 on Mac and Windows

## v0.9.7 - Dec 5 2012
Primarily a bugfix release - address memory leaks at shutdown

Added pulldown menus for NIDAQ device and channel names

## v0.9.6 - Jul 16 2012
Moved triggering options into extra inputs objects

Added liveData() script function to access liveView data from scripts

## v0.9.5 - Jul 9 2012
Added liveView readout option

## v0.9.4 - Jun 26 2012
Fixed problems with mingw installer

Added 'initialize' button to extra inputs dialog

## v0.9.3 - Apr 23 2012
Added beam center fitting from user-entered points on a powder ring

## v0.9.2 - Apr 19 2012
Added automatic scripting function help generation

## v0.9.1 - Apr 3 2012
First release in 0.9.x series - many many changes!

Multiple 'experiments' may be open simultaneously - each has its own settings file

A welcome dialog allows choosing from recent experiments

Settings are saved automatically, whenever they are changed

A NI-DAQ card can be used to measure auxiliary signals during acquisition (e.g. ion chamber readings)

Logarithmic Intensity option for image plots

File browsers highlight recently changed files

Shrink/grow commands for masks

## v0.8.4 - Feb 29 2012
Backport logarithmic intensity option from v0.9.1

## v0.8.3 - Nov 14 2011
Added newOutputScan, appendToOutputScan, plotOutputScan and saveOutputScan script commands to 'processor'

## v0.8.2 - Nov 11 2011
Added a loadScript routine to read and execute a script from a file.

Added some device query routines into the NIDAQ plugin

Added integrateRectangle(x0,y0,x1,y1) routine to processor

## v0.8.1 - Oct 27 2011
Display error dialog if another copy of qxrd is already running

Allow specifying number of integration steps as alternative to integration step size

## v0.7.16 - Oct 7 2011
Set initial camera gain on program startup

Automatic insertion of skipped exposures when skippedExposuresAtStart==0

## v0.7.15 - Sep 16 2011
Added caching scheme to speed up integration.  Looks to give about an order of
magnitude speed boost

## v0.7.14 - Sep 15 2011
Improved implementation of remaining property types - should, (I hope) eliminate
the update loop problem once and for all...

## v0.7.13 - Sep 9 2011
Improved implementation of integer properties - should eliminate the occasional
update loops that we get from time to time

## v0.7.12 - Sep 7 2011
Add user parameters to control numeric field widths in generated file names

Improve handling of memory exhaustion during acquisition

## v0.7.11 - Sep 1 2011
Accelerate readout by executing more of the processing logic in subsidiary threads

Added options for more timing messages during acquisition

Added options to control number of lines in messages window

## v0.7.10 - Aug 9 2011
Added phase shift option to sync acquisition waveform generation

Added menu commands duplicating commands available from toolbox windows

Merged experimental branch back into master

## v0.7.9 - Aug 4 2011
Changes to Windows installer script - QXRD appears in Add/Remove Programs

Improved responsiveness to acquisition cancel

More debugging messages

## v0.7.8 - Jun 29 2011
Fixed crash when clearing dark/gainmap/badpixels after already cleared

Added thread names to log file output

Added debug output for server operations

## v0.7.7 - Jun 27 2011
Allow integration vs Q and vs R in addition to vs TTH

Improved handling of different image and mask dimensions

File browser windows use polling so that they work on net mounted directories

## v0.7.6 - Jun 22 2011
Implement gain /flat field correction during acquisition

## v0.7.5 - Jun 6 2011
Improved responsiveness for file browser windows when displaying many files

## v0.7.4 - Jun 6 2011
Delayed initialization option for acquisition

## v0.7.3 - Jun 6 2011
Improved debugging messages.

## v0.7.2 - May 29 2011
Mainly internal code reorganization

## v0.5.24 - Apr 19 2011
Changed PE Acquisition - call Acquisition_CloseAll at QXRD exit, fixed bug in
setting lower bound exposure time

## v0.5.23 - Apr 11 2011
Documentation upgrade. Added release notes to embedded documentation

Changed PE acquisition - added lower bound to exposure times based on values
returned by Acquire_GetIntTimes call to XISL library

Allow setting exposure time from fresh start options dialog

## v0.5.22 - Apr 7 2011
Changes in PE acquisition initialization to support more PE detector types

## v0.5.21 - Apr 6 2011
Pre-triggered acquisition should be working again.

Changes to file browser window, including a refresh button. File browser
windows do not auto-update you must click the refresh button.

Show/Hide Rng... buttons now work in masking dialog

More info saved in acquired files - Qxrd and Qt version numbers - also more
shown in info dialog

## v0.5.20 - Apr 4 2011
fixed problems with image accumulation.

an initial implementation of a file browser that is more responsive when there
are many files in a directory.

load and save preferences commands which transfer application preferences to/
from files.

## v0.5.19 - Mar 30 2011
allows setting some preferences before the application starts. To activate this
pass the command line argument '-fresh' when starting QXRD. On windows there's
an extra entry in the start menu to do this automatically.

## v0.5.18 - Mar 25 2011
contains fixes for an acquisition problem caused by not resetting exposure
counts

## v0.5.17 - Mar 22 2011
contains fixes and changes to the 'synchronized acquisition' mode

Added a 'bipolar triangle' waveform. Added nidaq.getAnalogInput and
nidaq.setAnalogOutput script commands. Added display of syncronization waveform
to UI

## v0.5.16 - Mar 14 2011
mainly involves a major re-write of the acquisition code to support
'synchronized' acquisition.

This involves taking 'groups' of images while varying an external programming
voltage (using a supported National Instruments NI-DAQ card). You specify the
number of images in a group and then QXRD adds together multiple cycles of
acquired images in phase with the output waveform.

The acquisition code does not currently support the pre-trigger acquisition
mode of earlier versions. I was hoping to remove this, but it seems that people
are actually using it so I had better put it back in. The functions and
parameters are there, but they don't do anything at the moment.

