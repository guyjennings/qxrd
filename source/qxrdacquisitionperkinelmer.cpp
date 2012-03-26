//#ifdef HAVE_PERKIN_ELMER

#include "qxrdacquisitionperkinelmer.h"
//#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdimagedata.h"
#include "qxrdwindow.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdallocator.h"
#include "qxrdexperiment.h"

#include <string.h>

//#ifdef Q_OS_UNIX
//#include "AcqLinuxTypes.h"
//#endif

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include <math.h>

#include "Acq.h"
#include <QThread>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QComboBox>
//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
#include <QMetaObject>
#include <QMetaProperty>
//#include "qxrdxislinterface.h"
#include "qxrdperkinelmerplugininterface.h"

QxrdAcquisitionPerkinElmer::QxrdAcquisitionPerkinElmer(QxrdSettingsSaverPtr saver,
                                                       QxrdExperimentWPtr doc,
                                                       QxrdDataProcessorWPtr proc,
                                                       QxrdAllocatorWPtr allocator)
  : QxrdAcquisition(saver, doc, proc, allocator)
{
  initialize();
}

QxrdAcquisitionPerkinElmer::~QxrdAcquisitionPerkinElmer()
{
}

//#endif // HAVE_PERKIN_ELMER
