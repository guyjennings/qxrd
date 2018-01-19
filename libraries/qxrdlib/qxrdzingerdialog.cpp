#include "qxrdzingerdialog.h"
#include "ui_qxrdzingerdialog.h"
#include "qxrdmaskstack.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdprocessor.h"
#include "qxrdzingerfinder.h"

QxrdZingerDialog::QxrdZingerDialog(QxrdProcessorWPtr procw,
                                   QxrdZingerFinderWPtr zf,
                                   QWidget *parent) :
  QDialog(parent),
  m_Processor(procw),
  m_ZingerFinder(zf)
{
  setupUi(this);

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdMaskStackPtr masks(proc->maskStack());

    if (masks) {
      m_ZingerAlgorithm -> addItem("Default", 0);

      int nmasks = masks->maskCount();

      m_ZingerMask -> addItem("No Mask", QxrdZingerFinder::ZingerNoMask);
      m_ZingerMask -> addItem("Mask from data", QxrdZingerFinder::ZingerDataMask);

      for (int i=0; i<nmasks; i++) {
        QcepMaskDataPtr p = masks->mask(i);

        if (p) {
          QString lvl = masks->stackLevelName(i);

          m_ZingerMask        -> addItem(lvl, i);
          m_ZingerDestination -> addItem(lvl, i);
        }
      }

      m_ZingerDestination -> addItem("Top of stack", QxrdZingerFinder::ZingerTopStack);

      QxrdZingerFinderPtr z(m_ZingerFinder);

      if (z) {
        int algIndex = m_ZingerAlgorithm   -> findData(z->get_ZingerAlgorithm());
        int mskIndex = m_ZingerMask        -> findData(z->get_ZingerMask());
        int dstIndex = m_ZingerDestination -> findData(z->get_ZingerDestination());

        m_ZingerAlgorithm   -> setCurrentIndex(algIndex);
        m_ZingerMask        -> setCurrentIndex(mskIndex);
        m_ZingerDestination -> setCurrentIndex(dstIndex);

        m_ZingerSize1       -> setValue(z->get_ZingerSize1());
        m_ZingerSize2       -> setValue(z->get_ZingerSize2());
        m_ZingerThreshold   -> setValue(z->get_ZingerThreshold());
      }
    }
  }
}

QxrdZingerDialog::~QxrdZingerDialog()
{
}

void QxrdZingerDialog::accept()
{
  QxrdZingerFinderPtr zf(m_ZingerFinder);

  if (zf) {
    zf->set_ZingerAlgorithm   (m_ZingerAlgorithm   -> currentData().toInt());
    zf->set_ZingerMask        (m_ZingerMask        -> currentData().toInt());
    zf->set_ZingerDestination (m_ZingerDestination -> currentData().toInt());
    zf->set_ZingerSize1       (m_ZingerSize1       -> value());
    zf->set_ZingerSize2       (m_ZingerSize2       -> value());
    zf->set_ZingerThreshold   (m_ZingerThreshold   -> value());

    zf->findZingers();
  }

  QDialog::accept();
}
