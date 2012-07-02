# QXRD Change Log
See also the [QXRD Git Log](http://qxrd.git.sourceforge.net/git/gitweb.cgi?p=qxrd/qxrd;a=shortlog)

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

## v0.8.4 - Feb 29 2012</h2>
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

