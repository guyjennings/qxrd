v0.5.18 contains fixes for an acquisition problem caused by not resetting exposure counts

v0.5.17 contains fixes and changes to the 'synchronized acquisition' mode

Added a 'bipolar triangle' waveform.  Added nidaq.getAnalogInput and nidaq.setAnalogOutput
script commands.  Added display of syncronization waveform to UI

v0.5.16 mainly involves a major re-write of the acquisition code to support 'synchronized' acquisition.

This involves taking 'groups' of images while varying an external programming voltage (using a
supported National Instruments NI-DAQ card).  You specify the number of images in a group and
then QXRD adds together multiple cycles of acquired images in phase with the output waveform.

The acquisition code does not currently support the pre-trigger acquisition mode of earlier versions.
I was hoping to remove this, but it seems that people are actually using it so I had better put it
back in.  The functions and parameters are there, but they don't do anything at the moment.
