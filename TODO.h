#ifndef TODO_H
#define TODO_H

//-------------------------------------------------------
//
//Check that settings are not being saved twice (in QxrdWindow destructor):
//
-------------------------------------------------------

Check thread correctness of acquisition
Adopt the same threading model as qavrg

//-------------------------------------------------------
//
//Run script engine in separate thread
//
//-------------------------------------------------------
//
//Correct signal/slot corrections to use new properties
//
//-------------------------------------------------------
//
//Cut down the number of warnings - currently 96!
//Reduced to 7, 6 of those within qwt code
//
-------------------------------------------------------

Add a plausible simulation mode

//-------------------------------------------------------
//
//Add window position/state saving.
//Load and save settings for everything...
//
//-------------------------------------------------------
//
//Eliminate the duplicate script engine object from QxrdAcquisitionScripting
//
-------------------------------------------------------

Optimise screen redrawing to eliminate redundant replot calls - will need
a rigorous reorganisation and layering of code to ensure this.

//-------------------------------------------------------
//
//BLOCKER
//Ensure that program will not overwrite files without warning.  Use modified
//file names where necessary.   Only do this for programmatic saves, saves
//after a dialog box will already have been given a confirmation dialog

Done, but there is a bug on linux which stops the file overwrite confirmation
dialog from being shown.

//-------------------------------------------------------
//
//BLOCKER
//Automatically reload dark, bad pixel and gain images when program restarts
//Load/save mask and dark images when program starts/exits, make default mask
//all visible.
//(Will need to load/save more metadata before this will work well).
//
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

//-------------------------------------------------------
//
//BLOCKER
//Make it possible to interrupt spec more gracefully - add some
//kind of cancel operation.
//
//-------------------------------------------------------
//
//Allow cancelling to happen sooner - especially during long exposures
//
-------------------------------------------------------

Zooming on graphs does not work properly when program first starts - graphs need
an autoscale operation before they start zooming properly

//-------------------------------------------------------
//
//Add menu commands to show/hide toolbox windows
//
-------------------------------------------------------

Automatically add integrated curves to output panel.

-------------------------------------------------------

Display range choosing doesn't work very well

//-------------------------------------------------------
//
//BLOCKER
//Dark subtraction doesn't seem to be working at the moment
//
// caused by typing 'if (m_DarkFrame = NULL) {'
// instead of       'if (m_DarkFrame == NULL) {'
// spotted by following up compiler warnings
-------------------------------------------------------

BLOCKER
Simplify the interactions between the various processing options and
perform a simple sanity check before starting acquisition

-------------------------------------------------------

BLOCKER
Improve GUI performance when doing rapid acquisition sequences -
don't queue processed output images for display - have a current and alternate image
and swap as appropriate.

-------------------------------------------------------

BLOCKER
Optimize processing routines

//-------------------------------------------------------
//
//BLOCKER
//Make processing single-threaded
//
-------------------------------------------------------

BLOCKER
Flag dropped frames more obviously.

-------------------------------------------------------

BLOCKER
Add timestamps and other metadata to saved files
e.g.  TIFFTAG_ORIENTATION, TIFFTAG_DOCUMENTNAME
TIFFTAG_IMAGEDESCRIPTION, TIFFTAG_DATETIME
TIFFTAG_FRAMECOUNT?, TIFFTAG_UNIQUECAMERAMODEL?

-------------------------------------------------------

BLOCKER
Provide facility to save integrated scans

-------------------------------------------------------

BLOCKER
Leak check...

//-------------------------------------------------------
//
//Coalesce controls from the 'output' panel into the 'acquire' panel
//
-------------------------------------------------------

BLOCKER
Do camera model number check...
Check more hardware settings when entering program...
Support binning modes...
Is not calling Acquisition_Init a problem?

-------------------------------------------------------

BLOCKER
Provide a user interface to the camera gain setting

-------------------------------------------------------

BLOCKER
Allow finer-grained control of the processing chain

-------------------------------------------------------

Remove the g_Acquisition global and replace it with
acquisition handle passed via Acquire_SetAcqData

-------------------------------------------------------

Rearrange the QxrdAcquisition hierarchy so that
QxrdAcquisition becomes a base class and
QxrdAcquisitionPerkinElmer the 'leaf' class that
is instantiated.  Will simplify working with multiple
detector types.

//-------------------------------------------------------
//
//BLOCKER
//Check that the load/save data, dark and mask routines
//actually work!
//The problem was that the file conversion factories for raw and mask data
//were not initialized.
//
-------------------------------------------------------

BLOCKER
Check handling of image dimensions for mask, acquisition
etc.

-------------------------------------------------------

Try LZW encoding images to see how it affects performance

-------------------------------------------------------

BLOCKER
Don't allow program to quit or start a new acquisition
until outstanding processing has completed.

//-------------------------------------------------------
//
//BLOCKER
//Combine the tails of QxrdDataProcessor::processAcquiredInt{16,32}Image to eliminate
//duplicated code.
//
-------------------------------------------------------

BLOCKER
Provide a means to remove dark image, gain map, mask etc.
(and put them back again!)

-------------------------------------------------------

BLOCKER
Add frame skip option in acquisition

-------------------------------------------------------

BLOCKER
The script:
for (i=0; i<512; i++) {
   print("Binning mode = ",i);
   acquisition.binningMode = i;
   acquire("h:/junk-test/testing",0.1,1,1,0)
   while (status(0.5) != 1) {}
}
gets deadlocked

-------------------------------------------------------

If acquisition is triggered before sufficient pre-trigger
images have been taken, then post-triggered acquisition
will continue until a total of pre- plus post- images
have been taken - i.e. extra post-trigger images will be
taken.

-------------------------------------------------------

BLOCKER
Add error return checking wherever appropriate -
particularly in TIFF, Acquire_ and file i/o routines
Check output directory exists before starting acquisition sequence.

-------------------------------------------------------

#endif // TODO_H
