#include "qxrdnidaqsyncdetectoroutput.h"

QxrdNIDAQSyncDetectorOutput::QxrdNIDAQSyncDetectorOutput(int relExposure, double d0, double d1)
  : inherited("detectorOutput"),
    m_RelExposure(this, "relativeExposure", relExposure, "Relative Exposure Rate"),
    m_InitialDelay(this, "initialDelay", d0, "Initial delay"),
    m_ReadoutDelay(this, "readoutDelay", d1, "Readout time")
{
}
