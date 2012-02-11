#include "qxrdpowderfitrefinepage.h"
#include <QDoubleSpinBox>
#include <QCheckBox>

QxrdPowderFitRefinePage::QxrdPowderFitRefinePage(QxrdSettingsSaverWPtr saver, QxrdDataProcessorWPtr proc, QWidget *parent) :
    QxrdPowderFitIdentifyPage(saver, proc, parent),
    m_RingSet(NULL),
    m_NStatic(0),
    m_NRings(0)
{
  QxrdDataProcessorPtr dp(proc);

  if (dp) {
    m_RingSet = dp -> refinedRingSetFitParameters();
  }

  if (m_RingSet) {
    appendStaticFitParameter(m_RingSet->prop_CenterX());
    appendStaticFitParameter(m_RingSet->prop_CenterY());
    appendStaticFitParameter(m_RingSet->prop_Distance());
    appendStaticFitParameter(m_RingSet->prop_Energy());

    for (int i=0; i < m_RingSet->count(); i++) {
      appendRingFitParameter(i, m_RingSet->ring(i)->prop_TwoTheta());
    }
  }

  connect(m_AddRingButton, SIGNAL(clicked()), this, SLOT(appendRingFit()));
  connect(m_DeleteRingButton, SIGNAL(clicked()), this, SLOT(removeRingFit()));
}

void QxrdPowderFitRefinePage::selectRefinePage()
{
  m_PowderFitSteps->setCurrentIndex(1);

  removeGraphMarkers();
}

void QxrdPowderFitRefinePage::appendFitParameter(int n, QxrdFitParameter *parm)
{
  QTableWidget   *table = m_FitParameters;

  QLabel         *nameWidget = new QLabel(parm -> name());

  QDoubleSpinBox *valueWidget = new QDoubleSpinBox();
  valueWidget -> setMinimum(-HUGE_VAL);
  valueWidget -> setMaximum(HUGE_VAL);
  valueWidget -> setMinimumWidth(80);
  valueWidget -> setValue(parm -> value());

  QCheckBox      *isFittedWidget = new QCheckBox();
  isFittedWidget -> setChecked(parm -> isFitted());

  QCheckBox      *isLimitedWidget = new QCheckBox();
  isLimitedWidget -> setChecked(parm -> isLimited());

  QDoubleSpinBox *lowerLimitWidget = new QDoubleSpinBox();
  lowerLimitWidget -> setMinimum(-HUGE_VAL);
  lowerLimitWidget -> setMaximum(HUGE_VAL);
  lowerLimitWidget -> setMinimumWidth(80);
  lowerLimitWidget -> setValue(parm -> lowerLimit());

  QDoubleSpinBox *upperLimitWidget = new QDoubleSpinBox();
  upperLimitWidget -> setMinimum(-HUGE_VAL);
  upperLimitWidget -> setMaximum(HUGE_VAL);
  upperLimitWidget -> setMinimumWidth(80);
  upperLimitWidget -> setValue(parm -> upperLimit());

  table -> insertRow(n);
  table -> setCellWidget (n, 0, nameWidget);
  table -> setCellWidget (n, 1, valueWidget);
  table -> setCellWidget (n, 2, isFittedWidget);
  table -> setCellWidget (n, 3, isLimitedWidget);
  table -> setCellWidget (n, 4, lowerLimitWidget);
  table -> setCellWidget (n, 5, upperLimitWidget);

  m_FitParameters -> resizeColumnsToContents();
  m_FitParameters -> resizeRowsToContents();
}

void QxrdPowderFitRefinePage::appendStaticFitParameter(QxrdFitParameter *parm)
{
//  appendOutputString(tr("Append static fit parameter (m_NStatic=%1, m_NRings=%2)").arg(m_NStatic).arg(m_NRings));

  appendFitParameter(m_NStatic, parm);

  m_FitParameters->setVerticalHeaderItem(m_NStatic,
                                             new QTableWidgetItem(""));

  m_NStatic++;
}

void QxrdPowderFitRefinePage::appendRingFitParameter(int n, QxrdFitParameter *parm)
{
//  appendOutputString(tr("Append fit ring parameter (n=%1, m_NStatic=%2, m_NRings=%3)").arg(n).arg(m_NStatic).arg(m_NRings));

  appendFitParameter(m_NStatic+n, parm);

  m_FitParameters->setVerticalHeaderItem(m_NStatic+n,
                                             new QTableWidgetItem(tr("%1").arg(n)));

  m_NRings++;
}

void QxrdPowderFitRefinePage::removeRingFitParameter(int n)
{
//  appendOutputString(tr("Remove fit ring parameter (n=%1, m_NStatic=%2, m_NRings=%3)").arg(n).arg(m_NStatic).arg(m_NRings));

  if (m_NRings) {
    QTableWidget *table = m_FitParameters;

    table -> removeRow(m_NStatic+n);

    m_NRings--;
  }
}

void QxrdPowderFitRefinePage::appendRingFit()
{
//  appendOutputString(tr("Append fit ring (m_NRings=%1)").arg(m_NRings));

  m_RingSet -> append(12.5);

  int n = m_RingSet->count()-1;

  appendRingFitParameter(n, m_RingSet->ring(n)->prop_TwoTheta());

  m_DeleteRingButton -> setEnabled(true);
}

void QxrdPowderFitRefinePage::removeRingFit()
{
//  appendOutputString(tr("Remove fit ring (m_NRings=%1)").arg(m_NRings));

  if (m_NRings) {
    m_RingSet->remove(m_NRings-1);

    removeRingFitParameter(m_NRings-1);
  }

  m_DeleteRingButton -> setEnabled(m_NRings);
}
