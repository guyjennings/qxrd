#ifndef TODO_H
#define TODO_H

//-------------------------------------------------------
//
//Check that settings are not being saved twice (in QxrdWindow destructor):
//
//-------------------------------------------------------
//
//Check thread correctness of acquisition
//Adopt the same threading model as qavrg
//
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
//-------------------------------------------------------
//
//Add a plausible simulation mode - this should accurately mimic
//the real acquisition.
//
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
//-------------------------------------------------------
//
//Zooming on graphs does not work properly when program first starts - graphs need
//an autoscale operation before they start zooming properly
//
//-------------------------------------------------------
//
//Add menu commands to show/hide toolbox windows
//
//-------------------------------------------------------
//
//Automatically add integrated curves to output panel.
//
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

//-------------------------------------------------------
//
//BLOCKER
//Improve GUI performance when doing rapid acquisition sequences -
//don't queue processed output images for display - have a current and alternate image
//and swap as appropriate.
//
//-------------------------------------------------------
//
//BLOCKER
//Optimize processing routines
//
//-------------------------------------------------------
//
//BLOCKER
//Make processing single-threaded
//
//-------------------------------------------------------
//
//BLOCKER
//Flag dropped frames more obviously.
//
//-------------------------------------------------------
//
//BLOCKER
//Add timestamps and other metadata to saved files
//e.g.  TIFFTAG_ORIENTATION, TIFFTAG_DOCUMENTNAME
//TIFFTAG_IMAGEDESCRIPTION, TIFFTAG_DATETIME
//TIFFTAG_FRAMECOUNT?, TIFFTAG_UNIQUECAMERAMODEL?
//
//-------------------------------------------------------
//
//BLOCKER
//Provide facility to save integrated scans
//
-------------------------------------------------------

BLOCKER
Leak check...

//-------------------------------------------------------
//
//Coalesce controls from the 'output' panel into the 'acquire' panel
//
//-------------------------------------------------------
//
//BLOCKER
//Do camera model number check...
//Check more hardware settings when entering program...
//Support binning modes...
//Is not calling Acquisition_Init a problem?
//
//-------------------------------------------------------
//
//BLOCKER
//Provide a user interface to the camera gain setting
//
//-------------------------------------------------------
//
//BLOCKER
//Allow finer-grained control of the processing chain
//
-------------------------------------------------------

Remove the g_Acquisition global and replace it with
acquisition handle passed via Acquire_SetAcqData

//-------------------------------------------------------
//
//Rearrange the QxrdAcquisition hierarchy so that
//QxrdAcquisition becomes a base class and
//QxrdAcquisitionPerkinElmer the 'leaf' class that
//is instantiated.  Will simplify working with multiple
//detector types.
//
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

//-------------------------------------------------------
//
//Try LZW encoding images to see how it affects performance
//
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

//-------------------------------------------------------
//
//BLOCKER
//Add frame skip option in acquisition
//
//-------------------------------------------------------
//
//BLOCKER
//The script:
//for (i=0; i<6; i++) {
//   print("Camera Gain = ",i);
//   acquisition.cameraGain = i;
//   acquire("h:/junk-test/testing",0.1,1,1,0)
//   while (status(0.5) != 1) {
//     print("Waiting")
//   }
//}
//gets deadlocked

-------------------------------------------------------

If acquisition is triggered before sufficient pre-trigger
images have been taken, then post-triggered acquisition
will continue until a total of pre- plus post- images
have been taken - i.e. extra post-trigger images will be
taken.

//-------------------------------------------------------
//
//BLOCKER
//Add error return checking wherever appropriate -
//particularly in TIFF, Acquire_ and file i/o routines
//Check output directory exists before starting acquisition sequence.
//
//-------------------------------------------------------
//
//BLOCKER
//Add acquireStatus and processStatus functions and
//processor.status() and acquisition.status() methods
//which test status of acquisition and processing operations
//
-------------------------------------------------------

BLOCKER
Separate printMessage, warnMessage and errorMessage signals
Color code in output.

//-------------------------------------------------------
//
//BLOCKER
//Binning mode not working
//Disabled
//
//-------------------------------------------------------
//
//Added user interface to log file choosing
//
//-------------------------------------------------------
//
//BLOCKER - DEFERRED
//Copy acquisition dynamic properties into acquired images
//Copy processor dynamic properties into processed images
//Doesn't work at present because setProperty is not thread-safe
//
-------------------------------------------------------

BLOCKER
Provide a means of saving raw data files in separate
sub-directories

//-------------------------------------------------------
//
//BLOCKER
//the ProcessedCount variable should not depend on the display progress, only
//on the Processor progress.
//
//-------------------------------------------------------
//
//BLOCKER
//Something wrong with acquired data summation (16-32 bit problem?)
//Actually caused by forgetting to clear image before summation.
//
//-------------------------------------------------------
//
//BLOCKER
//Masking operations don't seem to work.
//copyMask operations were other way round
//
//-------------------------------------------------------
//
//BLOCKER
//Image doesn't update when display parameters are changed.
//
//-------------------------------------------------------
//
//BLOCKER
//There's some kind of deadlock in the acquisition process
//
//-------------------------------------------------------
//
//BLOCKER
//Can't interrupt acquisition from GUI if it was started from script
//The GUI cancel calls should call the acquisition object directly, rather
//than via invokeMethod
//
//-------------------------------------------------------
//
//BLOCKER
//Need to limit the number of curves that can be added to the az avg window
//Need to implement saving the az avg data
//
//-------------------------------------------------------
//
//BLOCKER
//Allow for acquired but not yet processed images when calculating number of
//buffered images.
//
//-------------------------------------------------------
//
//Investigate whether the intensities would be more stable if the acquisition
//was running continuously rather than being started and stopped during each
//acquisition sequence. - could also include some kind of continuously updating
//display.
//
-------------------------------------------------------

Provide commands for printing graphs.

//-------------------------------------------------------
//
//The first average after the program has been started, is always zero
//
//-------------------------------------------------------
//
//BLOCKER
//Need to re-enable adding sliced data to az avg graph
//
-------------------------------------------------------

Implement processor.processDataSequence

//-------------------------------------------------------
//
//BLOCKER
//Manual integration should update graph and/or save integrated data
//
//-------------------------------------------------------
//
//Memory handling is still poor, at least in simulation mode.  If you do a 1000 frame
//acquisition the memory usage may reach ~10GB, regardless of the memory usage limit
//
//Simulation mode allocates images via new, the PE driver does not, so the memory
//handling is only a problem for simulation mode.

//-------------------------------------------------------
//
//Initial choice of 'home' directory under windows is the application directory - a poor place
//to save data!
//
//Added code (in windows only) to set the current directory to QDir::homePath() at startup.
//

//-------------------------------------------------------
//
//File browser should automatically switch to the output data directory when this is changed.
//
//-------------------------------------------------------
//
//Check that the image buffer memory allocation works before trying
//to use the allocated buffers.  (Exception handler needed?)
//
//-------------------------------------------------------
//
//Investigate a situation which can arise (twice now) where the GUI becomes unresponsive even
//though acquisition and processing continues to run.  Presumably some kind of race condition?
//This is caused by poor performance of file browser window.  I have disabled the file browser
//for the moment.
//
-------------------------------------------------------

Display some progress indication when angular integration is taking place.

//-------------------------------------------------------
//
//Complete implementation of file browser panel
//
-------------------------------------------------------

Implement an image statistics panel

-------------------------------------------------------

Implement an image slice panel ?

-------------------------------------------------------

Implement an image histogram panel ?

-------------------------------------------------------

Implement an image calculator panel ?

//-------------------------------------------------------
//
//Additional acquisition features requested:
//Allow skipping frames at start of acquisition,
//Allow skipping frames between frames of acquisition,
//Allow saving of intermediate frames as an insurance during long acquisitions - alternatively save an 'accumulated' image
//at the end of an acquisition.
//Provide some kind of beam intensity monitoring facility and pause acquisition when beam is not present.
//
//
//Skip at start doesn't work yet
//
-------------------------------------------------------

If qxrd crashes the TCP/IP server socket gets left in CLOSE_WAIT state.  If you then restart qxrd it will
automatically use the next socket in sequence - unfortunately spec will still try to use the original socket
when searching for host:qxrd and will fail to connect.
Could try to use SO_REUSEADDR, perhaps.

//-------------------------------------------------------
//
//Errors during acquisition (e.g. file save errors) are handled extremely poorly!
//
//-------------------------------------------------------
//
//Implement some kind of control of the maximum size of the text in the messages window
//
//Limited to 20000 lines
//
-------------------------------------------------------

Rename 'Correction' panel to 'Processing' ?

//-------------------------------------------------------
//
//Provide control of whether dark images are saved - probably via the Save 'raw' images option.
//
//-------------------------------------------------------
//
//Better handling of cancellation would be desirable
//
-------------------------------------------------------

Try to find cause of a problem where acquisition stalls during long sequence.  Seen twice
now, application was still responsive afterwards but acquisition stopped.  On attempting
to cancel there were two different results - 1: when acquiring dark frame, cancel button
did not operate, 2: when acquiring data, cancel button did operate, but application
crashed imediately.

//-------------------------------------------------------
//
//Modify handling of file index numbers during acquisition so that dark measurements do not increment the file index.
//This is to simplify subsequent processing of image sequences with embedded dark measurements.
// Done
//
//-------------------------------------------------------
//
//Program doesn't remember output directory  (possibly only on OSX?)
//Also doesn't restore dark image - in fact it doesn't save any preferences at all
//
//QxrdDataProcessor properties were no being saved because the writeSettings method
//was in a super class - QxrdDataProcessorBase while the properties were in QxrdDataProcessor
//
//-------------------------------------------------------
//
//Modify preferences handling so that updated prefs are saved as they are modified (or perhaps after a few seconds delay)
//
//-------------------------------------------------------
//
//Saved raw images appear to be full of zeroes.  No - they are OK
//
//-------------------------------------------------------
//
//Add a separate file io thread for image saving.
//
//-------------------------------------------------------
//
//Image display isn't working any more - perhaps due to initialization
//of m_NewData and/or m_NewMask to NULL in qxrdwindow -
//Had failed to copy m_Range in QxrdRasterData::copy
//
//-------------------------------------------------------
//
//Doesn't auto-load dark data on startup - caused by failure
// to save QxrdDataProcessor preferences
//
-------------------------------------------------------

Need progress info during acquisition/processing

//-------------------------------------------------------
//
//Make the average signal level widget allow for the background, calculate in data processor thread
//
-------------------------------------------------------

Live image update?

-------------------------------------------------------

Image statistics/histogram display?

//-------------------------------------------------------
//
//Cancel PE acquisition -> crash
//
//-------------------------------------------------------
//
//Problem with fast readout - the queueing between the PE callback thread and
//the acquisition thread builds up so that the buffer numbers are no longer valid
//
//-------------------------------------------------------
//
//There's a skipped readout frame after each readout image is finished - this
//makes image sequences take longer than they should - e.g. 0.5 sec exposure,
//single frame produces a new frame every 1 second.
//
//-------------------------------------------------------
//
//The integration routines are not properly threaded...
//
-------------------------------------------------------

Need to make the allocator algorithm more optimal.

//-------------------------------------------------------
//
//Only display dropped frames during acquisition
//
//-------------------------------------------------------
//
//Add detector tilt, distance and rotation parameters to center finder window
//
//-------------------------------------------------------
//
//Update center finder slicing and integration routines to use tilt parameters where appropriate
//
// not sure what to do for polygon slicing - will ignore for now
//-------------------------------------------------------
//
//Disable ring fitting dialog
//
//-------------------------------------------------------
//
//Add percentile display mode
//
//-------------------------------------------------------
//
//Add display of bad and over exposed pixels
//
-------------------------------------------------------

Improve user interface for setting debug mode

-------------------------------------------------------

Update help documentation to reflect changes

//-------------------------------------------------------
//
//Remove single-threaded and disable cuda processor options
//
//-------------------------------------------------------
//
//Reduce idle-time CPU load by reducing number of calls to
//idle image routine
//
-------------------------------------------------------

Add AB image acquisition mode

//-------------------------------------------------------
//
//Test 64 bit readout mode
//
//Works.
//
-------------------------------------------------------

Improve behaviour when plugins do not load

-------------------------------------------------------

Track integrated graph update time to suppress updates during
rapid acquisition

-------------------------------------------------------

Find threading problem when quitting debug build - QNativeSocketEngine

-------------------------------------------------------

Make proxy script objects for GUI script so that they can be called
from different threads. - or maybe look at putting the script engine
in the main thread.

-------------------------------------------------------

Be more aggressive about memory preallocation on 64 bit systems -
or possibly have another go at recycling images.

-------------------------------------------------------

Investigate file naming problems when acquiring - sometimes
data seem to be saved in wrong file names, also metadata not
always saved.

//-------------------------------------------------------
//
//'trigger()' script command doesnt work - blocks, while
//'acquisition.triggered=1' does work.
//
//Does work, but is unintuitive - 'trigger()' returns
//the trigger state, 'trigger(1)' sets the trigger.
//
//Changed so that 'trigger()' does a trigger
//
//-------------------------------------------------------
//
//Make 'preTriggerFiles' default to zero if not given
//explicitly.
//
//If 'acquire(...)' is given 4 or fewer arguments, set
//'preTriggerFiles' to zero.
//
-------------------------------------------------------

Add a status function which waits until sufficient
pre-trigger files have been acquired.

-------------------------------------------------------

Start thinking about how to synchronise acquisition with
external processes - possible support for running a
National Instruments MIO card in sync with acquisition

-------------------------------------------------------

#endif // TODO_H
