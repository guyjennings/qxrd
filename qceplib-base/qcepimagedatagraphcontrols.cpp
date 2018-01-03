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

    connect(m_ScalingMode, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged,
            m_ParmsStack,  &QStackedWidget::setCurrentIndex);

    connect(m_Window->prop_ColorMap(),      &QcepIntProperty::valueChanged,    m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_ScalingMode(),   &QcepIntProperty::valueChanged,    m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_MinimumPct(),    &QcepDoubleProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_MaximumPct(),    &QcepDoubleProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_MinimumPctle(),  &QcepDoubleProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_MaximumPctle(),  &QcepDoubleProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_MinimumVal(),    &QcepDoubleProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_MaximumVal(),    &QcepDoubleProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
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
