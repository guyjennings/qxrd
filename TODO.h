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

-------------------------------------------------------

Eliminate the duplicate script engine object from QxrdAcquisitionScripting

-------------------------------------------------------

Optimise screen redrawing to eliminate redundant replot calls - will need
a rigorous reorganisation and layering of code to ensure this.

-------------------------------------------------------

#endif // TODO_H
