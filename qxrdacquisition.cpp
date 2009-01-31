#include "qxrdacquisition.h"

#include "qxrdapplication.h"

QxrdAcquisition::QxrdAcquisition(QxrdApplication *app)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_Application(app)
{
  printf("Enter QxrdAcquisition::QxrdAcquisition\n");
}

QxrdAcquisition::~QxrdAcquisition()
{
}

void QxrdAcquisition::acquire(double integ, int nreps)
{
  {
    QMutexLocker lock(&m_Mutex);

    printf("QxrdAcquisition::acquire(%g,%d)\n", integ, nreps);

//     m_Acquisition -> setNAverages((int)(integ*1000));
//     m_Acquisition -> setNRepetitions(nreps);
  }

//   configureAverager();

//   for (int i=0; i<nreps; i++) {
//     startAcquisition(i);

//     waitAcquisition(i);
//     readoutAcquisition(i);
//   }
}

void QxrdAcquisition::resultsAvailable(int chan)
{
  emit resultsChanged();
}

void QxrdAcquisition::savingComplete(int chan)
{
}

