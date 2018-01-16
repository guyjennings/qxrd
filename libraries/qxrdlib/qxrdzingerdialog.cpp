#include "qxrdzingerdialog.h"
#include "ui_qxrdzingerdialog.h"
#include "qxrdmaskstack.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrddataprocessor.h"

QxrdZingerDialog::QxrdZingerDialog(QxrdDataProcessorWPtr procw, QWidget *parent) :
  QDialog(parent),
  m_Processor(procw)
{
  setupUi(this);

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdMaskStackPtr masks(proc->maskStack());

    if (masks) {
      m_ZingerAlgorithm -> addItem("Default", 0);

      int nmasks = masks->maskCount();

      m_ZingerMask -> addItem("No Mask", QxrdDataProcessor::ZingerNoMask);
      m_ZingerMask -> addItem("Mask from data", QxrdDataProcessor::ZingerDataMask);

      for (int i=0; i<nmasks; i++) {
        QcepMaskDataPtr p = masks->mask(i);

        if (p) {
          QString lvl = masks->stackLevelName(i);

          m_ZingerMask        -> addItem(lvl, i);
          m_ZingerDestination -> addItem(lvl, i);
        }
      }

      m_ZingerDestination -> addItem("Top of stack", QxrdDataProcessor::ZingerTopStack);

      int algIndex = m_ZingerAlgorithm   -> findData(proc->get_ZingerAlgorithm());
      int mskIndex = m_ZingerMask        -> findData(proc->get_ZingerMask());
      int dstIndex = m_ZingerDestination -> findData(proc->get_ZingerDestination());

      m_ZingerAlgorithm   -> setCurrentIndex(algIndex);
      m_ZingerMask        -> setCurrentIndex(mskIndex);
      m_ZingerDestination -> setCurrentIndex(dstIndex);

      m_ZingerSize1       -> setValue(proc->get_ZingerSize1());
      m_ZingerSize2       -> setValue(proc->get_ZingerSize2());
      m_ZingerThreshold   -> setValue(proc->get_ZingerThreshold());
    }
  }
}

QxrdZingerDialog::~QxrdZingerDialog()
{
}

void QxrdZingerDialog::accept()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->set_ZingerAlgorithm   (m_ZingerAlgorithm   -> currentData().toInt());
    proc->set_ZingerMask        (m_ZingerMask        -> currentData().toInt());
    proc->set_ZingerDestination (m_ZingerDestination -> currentData().toInt());
    proc->set_ZingerSize1       (m_ZingerSize1       -> value());
    proc->set_ZingerSize2       (m_ZingerSize2       -> value());
    proc->set_ZingerThreshold   (m_ZingerThreshold   -> value());

    proc->findZingers();
  }

  QDialog::accept();
}
