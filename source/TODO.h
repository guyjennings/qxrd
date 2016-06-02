#ifndef TODO_H
#define TODO_H

DONE("2009-08-27", "2009-08-27",
     "Check that settings are not being saved twice (in QxrdWindow destructor):");

DONE("2009-08-27", "2009-08-27",
     "Check thread correctness of acquisition\n"
     "Adopt the same threading model as qavrg"
     );

DONE("2009-07-08", "2009-07-08",
     "Run script engine in separate thread");

DONE("2009-09-13", "2009-09-13",
     "Correct signal/slot corrections to use new properties");

DONE("2009-06-28", "2009-06-28",
     "Cut down the number of warnings - currently 96!\n"
     "Reduced to 7, 6 of those within qwt code");

DONE("2010-09-13", "2010-09-13",
     "Add a plausible simulation mode - this should accurately mimic "
     "the real acquisition.");

DONE("2009-08-27", "2009-08-27",
     "Add window position/state saving.\n"
     "Load and save settings for everything...");

DONE("2009-08-27", "2009-08-27",
     "Eliminate the duplicate script engine object from QxrdAcquisitionScripting");

TODO("2009-07-20", "",
     "Optimise screen redrawing to eliminate redundant replot calls - will need "
     "a rigorous reorganisation and layering of code to ensure this.");

DONE("2009-09-13", "2009-09-13",
     "Ensure that program will not overwrite files without warning.  Use modified "
     "file names where necessary.   Only do this for programmatic saves, saves "
     "after a dialog box will already have been given a confirmation dialog\n\n"
     "Done, but there is a bug on linux which stops the file overwrite confirmation "
     "dialog from being shown.");

DONE("2009-09-14", "2009-09-14",
     "Automatically reload dark, bad pixel and gain images when program restarts\n"
     "Load/save mask and dark images when program starts/exits, make default mask "
     "all visible.\n"
     "(Will need to load/save more metadata before this will work well).");

DONE("2009-08-27", "2015-09-21",
     "Investigate why the vertical window size increases each time program is run "
     "(Only happens on linux...)\n"
     "Doesn't happen any more!");

TODO("2009-08-09", "",
     "Implement generalized image slicing routine - integrate over a rectangular "
     "region, rather than along a line.");

TODO("2009-08-09", "",
     "Re-implement center finder plot to use routine above.");

TODO("2009-08-14", "",
     "Add some kind of beam presence detection threshold");

DONE("2009-09-15", "2009-09-15",
     "Make it possible to interrupt spec more gracefully - add some "
     "kind of cancel operation.");

DONE("2009-08-27", "2009-08-27",
     "Allow cancelling to happen sooner - especially during long exposures");

DONE("2009-11-13", "2009-11-13",
     "Zooming on graphs does not work properly when program first starts - "
     "graphs need an autoscale operation before they start zooming properly");

DONE("2009-09-14", "2009-09-14",
     "Add menu commands to show/hide toolbox windows");

DONE("2009-09-30", "2009-09-30",
     "Automatically add integrated curves to output panel.");

TODO("2009-08-24", "",
     "Display range choosing doesn't work very well");

DONE("2009-09-04", "2009-09-04",
     "Dark subtraction doesn't seem to be working at the moment\n"
     "caused by typing 'if (m_DarkFrame = NULL) {'\n"
     "instead of       'if (m_DarkFrame == NULL) {'\n"
     "spotted by following up compiler warnings");

DONE("2009-08-27", "2015-09-21",
     "Simplify the interactions between the various processing options and "
     "perform a simple sanity check before starting acquisition\n"
     "Pre-acquisition sanity check added");

DONE("2009-09-28", "2009-09-28",
     "Improve GUI performance when doing rapid acquisition sequences -\n"
     "don't queue processed output images for display - have a current "
     "and alternate image and swap as appropriate.");

DONE("2010-09-13", "2010-09-13",
     "Optimize processing routines");

DONE("2009-09-11", "2009-09-11",
     "Make processing single-threaded");

DONE("2010-09-13", "2010-09-13",
     "Flag dropped frames more obviously.");

DONE("2009-09-28", "2009-09-28",
     "Add timestamps and other metadata to saved files\n"
     "e.g.  TIFFTAG_ORIENTATION, TIFFTAG_DOCUMENTNAME\n"
     "TIFFTAG_IMAGEDESCRIPTION, TIFFTAG_DATETIME\n"
     "TIFFTAG_FRAMECOUNT?, TIFFTAG_UNIQUECAMERAMODEL?");

DONE("2010-09-13", "2010-09-13",
     "Provide facility to save integrated scans");

TODO("2009-08-27", "",
     "Leak check...");

DONE("2009-09-11", "2009-09-11",
     "Coalesce controls from the 'output' panel into the 'acquire' panel");

DONE("2009-09-28", "2009-09-28",
     "Do camera model number check...\n"
     "Check more hardware settings when entering program...\n"
     "Support binning modes...\n"
     "Is not calling Acquisition_Init a problem?");

DONE("2009-09-28", "2009-09-28",
     "Provide a user interface to the camera gain setting");

DONE("2009-09-28", "2009-09-28",
     "Allow finer-grained control of the processing chain");

DONE("2009-09-28", "2015-09-17",
     "Remove the g_Acquisition global and replace it with "
     "acquisition handles passed via Acquire_SetAcqData");

DONE("2010-09-13", "2010-09-13",
     "Rearrange the QxrdAcquisition hierarchy so that "
     "QxrdAcquisition becomes a base class and "
     "QxrdAcquisitionPerkinElmer the 'leaf' class that "
     "is instantiated.  Will simplify working with multiple "
     "detector types.");

DONE("2009-09-11", "2009-09-11",
     "Check that the load/save data, dark and mask routines "
     "actually work!\n"
     "The problem was that the file conversion factories for raw and mask data "
     "were not initialized.");

TODO("2009-08-31", "",
     "Check handling of image dimensions for mask, acquisition etc.");

DONE("2010-09-13", "2010-09-13",
     "Try LZW encoding images to see how it affects performance");

TODO("2009-09-04", "",
     "Don't allow program to quit or start a new acquisition "
     "until outstanding processing has completed.");

DONE("2009-09-11", "2009-09-11",
     "Combine the tails of QxrdDataProcessor::processAcquiredInt{16,32}Image "
     "to eliminate duplicated code.");

DONE("2009-09-08", "2009-09-08",
     "Provide a means to remove dark image, gain map, mask etc.\n"
     "(and put them back again!)");

DONE("2010-09-13", "2010-09-13",
     "Add frame skip option in acquisition");

DONE("2009-09-30", "2009-09-30",
     "The script:\n"
     "for (i=0; i<6; i++) {\n"
     "   print(\"Camera Gain = \",i);\n"
     "   acquisition.cameraGain = i;\n"
     "   acquire(\"h:/junk-test/testing\",0.1,1,1,0);\n"
     "   while (status(0.5) != 1) {\n"
     "     print(\"Waiting\");\n"
     "   }\n"
     "}\n"
     "gets deadlocked");

TODO("2009-09-08", "",
     "If acquisition is triggered before sufficient pre-trigger "
     "images have been taken, then post-triggered acquisition "
     "will continue until a total of pre- plus post- images "
     "have been taken - i.e. extra post-trigger images will be "
     "taken.");


DONE("2009-09-28", "2009-09-28",
     "Add error return checking wherever appropriate -\n"
     "particularly in TIFF, Acquire_ and file i/o routines\n"
     "Check output directory exists before starting acquisition sequence.");

DONE("2009-09-30", "2009-09-30",
     "Add acquireStatus and processStatus functions and "
     "processor.status() and acquisition.status() methods "
     "which test status of acquisition and processing operations");

DONE("2009-09-20", "2009-09-20",
     "Separate printMessage, warnMessage and errorMessage signals\n"
     "Color code in output.");

TODO("2009-09-20", "",
     "Binning mode not working");

DONE("2009-09-21", "2009-09-21",
     "Added user interface to log file choosing");

DONE("2009-09-23", "2009-09-23",
     "Copy acquisition dynamic properties into acquired images\n"
     "Copy processor dynamic properties into processed images\n"
     "Doesn't work at present because setProperty is not thread-safe");

DONE("2009-09-22", "2009-09-22",
     "Provide a means of saving raw data files in separate sub-directories");

DONE("2009-09-28", "2009-09-28",
     "the ProcessedCount variable should not depend on the display progress, only "
     "on the Processor progress.");

DONE("2009-09-25", "2009-09-25",
     "Something wrong with acquired data summation (16-32 bit problem?)\n"
     "Actually caused by forgetting to clear image before summation.");

DONE("2009-09-28", "2009-09-28",
     "Masking operations don't seem to work.\n"
     "copyMask operations were other way round");

DONE("2009-11-13", "2009-11-13",
     "Image doesn't update when display parameters are changed.");

DONE("2009-10-02", "2009-10-02",
     "There's some kind of deadlock in the acquisition process");

DONE("2009-09-30", "2009-09-30",
     "Can't interrupt acquisition from GUI if it was started from script\n"
     "The GUI cancel calls should call the acquisition object directly, rather "
     "than via invokeMethod");

DONE("2009-10-02", "2009-10-02",
     "Need to limit the number of curves that can be added to the az avg window\n"
     "Need to implement saving the az avg data");

DONE("2010-09-13", "2010-09-13",
     "Allow for acquired but not yet processed images when calculating number of "
     "buffered images.");

DONE("2010-09-13", "2010-09-13",
     "Investigate whether the intensities would be more stable if the acquisition "
     "was running continuously rather than being started and stopped during each "
     "acquisition sequence. - could also include some kind of continuously updating "
     "display.");

DONE("2009-09-30", "2015-01-01",
     "Provide commands for printing graphs.");

DONE("2009-11-13", "2009-11-13",
     "The first average after the program has been started is always zero");

DONE("2009-10-21", "2009-10-21",
     "Need to re-enable adding sliced data to az avg graph");

TODO("2009-10-02", "",
     "Implement processor.processDataSequence");

DONE("2009-10-05", "2009-10-05",
     "Manual integration should update graph and/or save integrated data");

DONE("2009-10-21", "2009-10-21",
     "Memory handling is still poor, at least in simulation mode. "
     "If you do a 1000 frame acquisition the memory usage may reach "
     "~10GB, regardless of the memory usage limit\n"
     "Simulation mode allocates images via new, the PE driver does not, "
     "so the memory handling is only a problem for simulation mode.");

DONE("2009-10-22", "2009-10-22",
     "Initial choice of 'home' directory under windows is the application directory "
     "- a poor place to save data!\n"
     "Added code (in windows only) to set the current directory "
     "to QDir::homePath() at startup.");

DONE("2010-09-13", "2010-09-13",
     "File browser should automatically switch to the output data "
     "directory when this is changed.");

DONE("2010-09-13", "2010-09-13",
     "Check that the image buffer memory allocation works before trying "
     "to use the allocated buffers.  (Exception handler needed?)");

DONE("2009-11-17", "2009-11-17",
     "Investigate a situation which can arise (twice now) where the GUI "
     "becomes unresponsive even though acquisition and processing continues "
     "to run.  Presumably some kind of race condition?\n"
     "This is caused by poor performance of file browser window.  "
     "I have disabled the file browser for the moment.");

TODO("2009-11-13", "",
     "Display some progress indication when angular integration is taking place.");

DONE("2010-09-13", "2010-09-13",
     "Complete implementation of file browser panel");

DONE("2009-11-13", "2009-11-13",
     "Implement an image statistics panel");

DONE("2009-11-13", "2009-11-13",
     "Implement an image slice panel");

DONE("2009-11-13", "2009-11-13",
     "Implement an image histogram panel ?");

TODO("2009-11-13", "",
     "Implement an image calculator panel ?");

DONE("2010-09-13", "2010-09-13",
     "Additional acquisition features requested:\n"
     "Allow skipping frames at start of acquisition,\n"
     "Allow skipping frames between frames of acquisition,\n"
     "Allow saving of intermediate frames as an insurance during long acquisitions "
     " - alternatively save an 'accumulated' image at the end of an acquisition.\n"
     "Provide some kind of beam intensity monitoring facility and pause acquisition "
     "when beam is not present.\n"
     "Skip at start doesn't work yet");

TODO("2009-11-17", "",
     "If qxrd crashes the TCP/IP server socket gets left in CLOSE_WAIT state.  "
     "If you then restart qxrd it will automatically use the next socket "
     "in sequence - unfortunately spec will still try to use the original socket "
     "when searching for host:qxrd and will fail to connect.\n"
     "Could try to use SO_REUSEADDR, perhaps.");


DONE("2010-09-13", "2010-09-13",
     "Errors during acquisition (e.g. file save errors) are handled extremely poorly!");

DONE("2010-01-26", "2010-01-26",
     "Implement some kind of control of the maximum size of the text "
     "in the messages window\n"
     "Limited by preferences parameter");

DONE("2009-12-03", "2015-09-21",
     "Rename 'Correction' panel to 'Processing' ?\n"
     "No longer relevant since correction panel is replaced by "
     "separate detector control windows");

DONE("2010-09-13", "2010-09-13",
     "Provide control of whether dark images are saved - "
     "probably via the Save 'raw' images option.");

DONE("2010-09-13", "2010-09-13",
     "Better handling of cancellation would be desirable");

TODO("2010-02-04", "",
     "Try to find cause of a problem where acquisition stalls during long sequences.  "
     "Seen twice now, application was still responsive afterwards but acquisition "
     "stopped.  On attempting to cancel there were two different results - \n"
     "1: when acquiring dark frame, cancel button did not operate\n"
     "2: when acquiring data, cancel button did operate, but application "
     "crashed imediately.");

DONE("2010-01-26", "2010-01-26",
     "Modify handling of file index numbers during acquisition "
     "so that dark measurements do not increment the file index.\n"
     "This is to simplify subsequent processing of image sequences "
     "with embedded dark measurements.\n"
     "Done - dark acquisition does not increment index number");

DONE("2010-09-13", "2010-09-13",
     "Program doesn't remember output directory  (possibly only on OSX?)\n"
     "Also doesn't restore dark image - in fact it doesn't save any preferences at all\n"
     "QxrdDataProcessor properties were not being saved because the writeSettings method "
     "was in a super class - QxrdDataProcessorBase while the properties were in QxrdDataProcessor");

DONE("2010-11-04", "2010-11-04",
     "Modify preferences handling so that updated prefs are saved as "
     "they are modified (or perhaps after a few seconds delay)");

DONE("2010-09-13", "2010-09-13",
     "Saved raw images appear to be full of zeroes.  No - they are OK");

DONE("2010-09-13", "2010-09-13",
     "Add a separate file io thread for image saving.");

DONE("2010-09-13", "2010-09-13",
     "Image display isn't working any more - perhaps due to initialization "
     "of m_NewData and/or m_NewMask to NULL in qxrdwindow -\n"
     "Had failed to copy m_Range in QxrdRasterData::copy");

DONE("2010-09-13", "2010-09-13",
     "Doesn't auto-load dark data on startup - \n"
     "caused by failure to save QxrdDataProcessor preferences");

DONE("2010-09-13", "2010-09-13",
     "Need progress info during acquisition/processing");

DONE("2010-09-13", "2010-09-13",
     "Make the average signal level widget allow for the background, "
     "calculate in data processor thread");

DONE("2010-09-13", "2010-09-13",
     "Live image update");

DONE("2010-09-13", "2010-09-13",
     "Image statistics/histogram display");

DONE("2010-09-13", "2010-09-13",
     "Cancel PE acquisition -> crash\n"
     "Fixed");

DONE("2010-09-13", "2010-09-13",
     "Problem with fast readout - the queueing between the PE "
     "callback thread and the acquisition thread builds up so that the "
     "buffer numbers are no longer valid");

DONE("2010-09-13", "2010-09-13",
     "There's a skipped readout frame after each readout image is finished"
     " - this makes image sequences take longer than they should "
     " - e.g. 0.5 sec exposure, single frame produces a new frame every 1 second.");

DONE("2010-09-13", "2010-09-13",
     "The integration routines are not properly threaded...");

TODO("2010-09-13", "",
     "Need to make the allocator algorithm more optimal.");

DONE("2010-09-13", "2010-09-13",
     "Only display dropped frames during acquisition");

DONE("2010-09-13", "2010-09-13",
     "Add detector tilt, distance and rotation parameters to center finder window");

DONE("2010-09-17", "2010-09-17",
     "Update center finder slicing and integration routines to use "
     "tilt parameters where appropriate\n"
     "not sure what to do for polygon slicing - will ignore for now");

DONE("2010-09-13", "2010-09-13",
     "Disable ring fitting dialog");

DONE("2010-09-13", "2010-09-13",
     "Add percentile display mode");

DONE("2010-11-04", "2010-11-04",
     "Add display of bad and over exposed pixels");

DONE("2010-09-13", "2010-09-13",
     "Improve user interface for setting debug mode");

TODO("2010-09-13", "",
     "Update help documentation to reflect changes");

DONE("2010-09-13", "2010-09-13",
     "Remove single-threaded and disable cuda processor options");

DONE("2010-09-13", "2010-09-13",
     "Reduce idle-time CPU load by reducing number of calls to "
     "idle image routine");

TODO("2010-09-13", "",
     "Add AB image acquisition mode");

DONE("2010-11-04", "2010-11-04",
     "Test 64 bit readout mode\n"
     "Works.");

TODO("2010-09-23", "",
     "Improve behaviour when plugins do not load");

TODO("2010-09-23", "",
     "Track integrated graph update time to suppress updates during "
     "rapid acquisition");

TODO("2010-09-28", "",
     "Find threading problem when quitting debug build - QNativeSocketEngine");

DONE("2010-09-28", "2010-09-28",
     "Make proxy script objects for GUI script so that they can be called "
     "from different threads. - or maybe look at putting the script engine "
     "in the main thread.");

DONE("2010-09-28", "2010-09-28",
     "Be more aggressive about memory preallocation on 64 bit systems - "
     "or possibly have another go at recycling images.");

DONE("2010-11-03", "2010-11-03",
     "Investigate file naming problems when acquiring - sometimes "
     "data seem to be saved in wrong file names, also metadata not "
     "always saved.");

DONE("2010-11-04", "2010-11-04",
     "'trigger()' script command doesnt work - blocks, while "
     "'acquisition.triggered=1' does work.\n"
     "Does work, but is unintuitive - 'trigger()' returns "
     "the trigger state, 'trigger(1)' sets the trigger.\n"
     "Changed so that 'trigger()' does a trigger");

DONE("2010-11-04", "2010-11-04",
     "Make 'preTriggerFiles' default to zero if not given explicitly.\n"
     "If 'acquire(...)' is given 4 or fewer arguments, set "
     "'preTriggerFiles' to zero.");

DONE("2010-11-09", "2010-11-09",
     "Metadata is not saved for QcepDoubleImageData, because "
     "the metadata saving stuff doesn't know about inheritance\n"
     "Changed QcepImageData::writeSettings to use staticMetaObject "
     "rather than metaObject()");

TODO("2012-01-09", "",
     "Gain setting menu missing for PE acquisition");

TODO("2012-01-09", "",
     "Crashes on exit from PE acquisition");

DONE("2010-11-04", "2014-03-05",
     "Start thinking about how to synchronise acquisition with "
     "external processes - possible support for running a "
     "National Instruments MIO card in sync with acquisition");

TODO("2010-11-04", "",
     "Add a status function which waits until sufficient "
     "pre-trigger files have been acquired.");

TODO("2011-02-24", "",
     "Load/Save mask stack state on program entry/exit");

TODO("2011-02-24", "",
     "Overall mask should be made to be a combination of the various "
     "mask stack levels.");

TODO("2011-01-17", "",
     "Make the PE acquisition code work for a wider variety "
     "of PE detectors, ideally for all of them.   Need to check "
     "model number before trying to e.g. set gain, or allow "
     "failures when setting gain to still permit acquisition.");

TODO("2011-01-17", "",
     "PE Acquisition fails in debug builds - sending messages "
     "to object in different thread");

DONE("2012-12-05", "2012-12-05",
     "May crash on exit if live view active\n"
     "Fixed by improving memory allocation tracking by using more QSharedPointers");

TODO("2014-03-05", "",
     "Mouse click behavior for pickers needs to be reviewed/modified - esp. polygon pickers");

TODO("2014-03-05", "",
     "Investigate display update behaviour when scaling parameters changed");

DONE("2015-09-18", "2015-10-14",
     "Fix problem where the saved dark image path is incorrect if the "
     "dark image acquired was a duplicate and was renamed.  Also for masks, potentially");

DONE("2015-09-20", "2015-10-14",
     "Do we really need QxrdAcquisitionParameterPack and QxrdDarkAcquisitionParameterPack - yes");

TODO("2015-09-20", "",
     "Fix problems when copying dynamic properties between objects in different threads - get "
     "errors in debug builds because dynamic property changed events get sent.");

DONE("2015-09-20", "2015-10-14",
     "Enable sorting in TODO list view");

DONE("2015-09-20", "2015-10-14",
     "TODO List items should resize to contents, with word wrap");

DONE("2015-09-20", "2015-10-14",
     "Implement separate processing chain for acquired data, attached to each detector");

TODO("2015-09-20", "",
     "Implement multi-threaded tiled integration routines to reduce integration latency");

TODO("2015-09-20", "",
     "Implement ROI intensity calculations");

DONE("2015-09-20", "2015-10-14",
     "Implement UI to set up and visualize ROIs");

DONE("2015-09-20", "2015-10-14",
     "Implement routines to pass ROI values to spec as macro counter channels");

TODO("2015-09-20", "",
     "Implement Pilatus Detector readout");

DONE("2015-09-20", "2015-09-21",
     "Implement detector enable/disable\n"
     "Now working");

DONE("2015-09-20", "2015-10-14",
     "Implement multi-detector readout");

TODO("2015-09-20", "",
     "Improve NI-DAQ extra inputs readout - support detector enable");

DONE("2015-09-20", "2015-10-23",
     "Detector enable script commands -\n"
     "detector(n).enabled=<true|false>");

TODO("2015-09-20", "",
     "Improve NI-DAQ output synchronization");

TODO("2015-09-22", "",
     "Redo acquireStatus function");

TODO("2015-09-29", "",
     "Updates to saved settings not stored as properties do not trigger re-saves");

DONE("2015-10-05", "2015-10-13",
     "Implement masking in ROI calculations");

DONE("2015-10-05", "2015-10-14",
     "Reimplement mask/dark/gain loading in detector initialization");

DONE("2015-10-06", "2015-10-14",
     "Make file index, phase number available as scaler channels"
     " - global channels though, since they are identical for each detector");

TODO("2015-10-07", "",
     "Need to make sure detector image processing steps are not too slow");

DONE("2015-10-12", "2015-10-14",
     "Implement 'Live View at Idle' flags for multi-detector acquisition");

TODO("2015-10-14", "",
     "Implement different output file format options");

DONE("2015-10-21", "2015-10-23",
     "Finish implementing remaining ROI calculations");

TODO("2015-10-21", "",
     "Find if/why mask range commands don't seem to work");

TODO("2015-10-21", "",
     "Fix mask display issues in detector control windows");

TODO("2016-03-07", "",
     "Investigate problems with last column of polar transforms");

DONE("2016-03-07", "2016-03-15",
     "Reinstate self normalize option for polar normalized integration");

TODO("2016-03-07", "",
     "Auto plotting of polar normalized integration results");

TODO("2016-03-09", "",
     "Make peak fitting and ring tracing routines honour masks");

DONE("2016-03-09", "2016-03-15",
     "Make peak fitting and ring tracing save results as data objects");

TODO("2016-03-09", "",
     "Rework masking operations to make them a bit easier to use");

TODO("2016-03-09", "",
     "Add undo facility to mask operations");

TODO("2016-03-09", "",
     "Make sure that detector data processing options and parameters are easy to set up"
     " - possibly support copying parameters from the main processor to the detector processors");

TODO("2016-03-09", "",
     "Implement QxrdCalibrant duplication, deletion and property dialog");

TODO("2016-03-11", "",
     "Powder point renormalization should update data objects");

DONE("2016-03-11", "2016-03-15",
     "Center Finder should use calibrant library, instead of its own internal vars");

TODO("2016-03-15", "",
     "Check integration paramaters for excessive memory use before execution");

DONE("2016-03-24", "2016-03-28",
     "Consider removing the separate Title property of images and scans - use Name instead");

DONE("2016-03-29", "2016-03-30",
     "More work on drag and drop data items");

DONE("2016-03-29", "2016-04-06",
     "Allow creating new data items at top level");

DONE("2016-04-06", "2016-04-08",
     "Set initial selection of imported data");

TODO("2016-04-08", "",
     "Make scatter plot windows work with columns in data groups as well as column scans");

DONE("2016-04-08", "2016-05-29",
     "Rationalize and simplify the interfaces to QcepAllocator");

TODO("2016-04-08", "",
     "Consider a variant-typed QcepDataValue type");

TODO("2016-04-12", "",
     "Re-implement QcepDataArray to use chunked format");

DONE("2016-05-28", "2016-06-01",
     "Reinstate QxrdExperimentThread");

TODO("2016-05-29", "",
     "Reuse existing data object paths when inserting data objects");

TODO("2016-05-29", "",
     "Run ring tracing code in background thread");

TODO("2016-05-29", "",
     "Renumber detectors when appropriate");

TODO("2016-06-01", "",
     "Reinstate experiment autosaver");

TODO("2016-06-02", "",
     "Allow rotation of ROIs, allow polygonal ROIs");

#endif // TODO_H
