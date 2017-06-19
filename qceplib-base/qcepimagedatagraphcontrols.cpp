#include "qcepimagedatagraphcontrols.h"
#include "ui_qcepimagedatagraphcontrols.h"
#include "qcepdataobjectgraphwindow.h"

QcepImageDataGraphControls::QcepImageDataGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object) :
  QcepGraphControlsWidget(window, mode, object)
{
  setupUi(this);

  if (m_Window) {
    m_Window->prop_ColorMap()    -> linkTo(m_ColorMap);
    m_Window->prop_ScalingMode() -> linkTo(m_ScalingMode);
    m_Window->prop_MinimumPct()  -> linkTo(m_MinimumPct);
    m_Window->prop_MaximumPct()  -> linkTo(m_MaximumPct);
    m_Window->prop_MinimumPctle()  -> linkTo(m_MinimumPctle);
    m_Window->prop_MaximumPctle()  -> linkTo(m_MaximumPctle);
    m_Window->prop_MinimumVal()  -> linkTo(m_MinimumVal);
    m_Window->prop_MaximumVal()  -> linkTo(m_MaximumVal);

    connect(m_ScalingMode, SIGNAL(currentIndexChanged(int)), m_ParmsStack, SLOT(setCurrentIndex(int)));

    connect(m_Window->prop_ColorMap(),      SIGNAL(valueChanged(int,int)),    m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_ScalingMode(),   SIGNAL(valueChanged(int,int)),    m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_MinimumPct(),    SIGNAL(valueChanged(double,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_MaximumPct(),    SIGNAL(valueChanged(double,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_MinimumPctle(),  SIGNAL(valueChanged(double,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_MaximumPctle(),  SIGNAL(valueChanged(double,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_MinimumVal(),    SIGNAL(valueChanged(double,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_MaximumVal(),    SIGNAL(valueChanged(double,int)), m_Window, SLOT(updateDisplay()));
  }
}

QcepImageDataGraphControls::~QcepImageDataGraphControls()
{
}

void QcepImageDataGraphControls::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
