#ifndef TODO_H
#define TODO_H

-------------------------------------------------------

Check that settings are not being saved twice (in QxrdWindow destructor):

-------------------------------------------------------

Check thread correctness of acquisition
Adopt the same threading model as qavrg

//-------------------------------------------------------
//
//Run script engine in separate thread
//
-------------------------------------------------------

Correct signal/slot corrections to use new properties

//-------------------------------------------------------
//
//Cut down the number of warnings - currently 96!
//Reduced to 7, 6 of those within qwt code
//
-------------------------------------------------------

Add a plausible simulation mode

-------------------------------------------------------

Add window position/state saving.
Load and save settings for everything...

-------------------------------------------------------

Automatically reload dark, bad pixel and gain images when program restarts

//-------------------------------------------------------
//
//Eliminate the duplicate script engine object from QxrdAcquisitionScripting
//
-------------------------------------------------------

Optimise screen redrawing to eliminate redundant replot calls - will need
a rigorous reorganisation and layering of code to ensure this.

-------------------------------------------------------

Ensure that program will not overwrite files without warning.  Use modified
file names where necessary.

-------------------------------------------------------

Load/save mask and dark images when program starts/exits, make default mask
all visible.

-------------------------------------------------------

Investigate why the vertical window size increases each time program is run
(Only happens on linux...)

-------------------------------------------------------

Implement generalized image slicing routine - integrate over a rectangular
region, rather than along a line.

-------------------------------------------------------

Re-implement center finder plot to use routine above.

-------------------------------------------------------

Add some kind of beam presence detection threshold

-------------------------------------------------------

Make it possible to interrupt spec more gracefully - add some
kind of cancel operation.

//-------------------------------------------------------
//
//Allow cancelling to happen sooner - especially during long exposures
//
-------------------------------------------------------

Zooming on graphs does not work properly when program first starts - graphs need
an autoscale operation before they start zooming properly

-------------------------------------------------------

Add menu commands to show/hide toolbox windows

-------------------------------------------------------

Automatically add integrated curves to output panel.

-------------------------------------------------------

Display range choosing doesn't work very well

-------------------------------------------------------

Provide a user interface to the camera gain setting

-------------------------------------------------------

Dark subtraction doesn't seem to be working at the moment

-------------------------------------------------------

Simplify the interactions between the various processing options and
perform a simple sanity check before starting acquisition

-------------------------------------------------------

Improve GUI performance when doing rapid acquisition sequences -
don't queue processed output images for display - have a current and alternate image
and swap as appropriate.

-------------------------------------------------------

Optimize processing routines

-------------------------------------------------------

Make processing single-threaded

-------------------------------------------------------

Flag dropped frames more obviously.

-------------------------------------------------------

Add timestamps and other metadata to saved files

-------------------------------------------------------

Provide facility to save integrated scans

-------------------------------------------------------

Leak check...

-------------------------------------------------------

Coalesce controls from the 'output' panel into the 'acquire' panel

-------------------------------------------------------

Do camera model number check...
Check more hardware settings when entering program...
Support binning modes...
Is not calling Acquisition_Init a problem?

-------------------------------------------------------

#endif // TODO_H
