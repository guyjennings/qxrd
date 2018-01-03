#include "qcepimagehistogramgraphcontrols.h"
#include "ui_qcepimagehistogramgraphcontrols.h"
#include "qcepdataobjectgraphwindow.h"

QcepImageHistogramGraphControls::QcepImageHistogramGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object) :
  QcepGraphControlsWidget(window, mode, object)
{
  setupUi(this);

  if (m_Window) {
    m_Window->prop_HistFullVis()   -> linkTo(m_HistFullVis);
    m_Window->prop_HistAVis()      -> linkTo(m_HistAVis);
    m_Window->prop_HistBVis()      -> linkTo(m_HistBVis);
    m_Window->prop_HistCVis()      -> linkTo(m_HistCVis);

    m_Window->prop_HistALeft()     -> linkTo(m_HistALeft);
    m_Window->prop_HistATop()      -> linkTo(m_HistATop);
    m_Window->prop_HistARight()    -> linkTo(m_HistARight);
    m_Window->prop_HistABottom()   -> linkTo(m_HistABottom);

    m_Window->prop_HistBLeft()     -> linkTo(m_HistBLeft);
    m_Window->prop_HistBTop()      -> linkTo(m_HistBTop);
    m_Window->prop_HistBRight()    -> linkTo(m_HistBRight);
    m_Window->prop_HistBBottom()   -> linkTo(m_HistBBottom);

    m_Window->prop_HistCLeft()     -> linkTo(m_HistCLeft);
    m_Window->prop_HistCTop()      -> linkTo(m_HistCTop);
    m_Window->prop_HistCRight()    -> linkTo(m_HistCRight);
    m_Window->prop_HistCBottom()   -> linkTo(m_HistCBottom);

    connect(m_Window->prop_HistFullVis(), &QcepBoolProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistAVis(), &QcepBoolProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistBVis(), &QcepBoolProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistCVis(), &QcepBoolProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);

    connect(m_Window->prop_HistALeft(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistATop(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistARight(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistABottom(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);

    connect(m_Window->prop_HistBLeft(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistBTop(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistBRight(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistBBottom(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);

    connect(m_Window->prop_HistCLeft(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistCTop(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistCRight(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
    connect(m_Window->prop_HistCBottom(), &QcepIntProperty::valueChanged, m_Window, &QcepDataObjectGraphWindow::updateDisplay);
  }
}

QcepImageHistogramGraphControls::~QcepImageHistogramGraphControls()
{
}

void QcepImageHistogramGraphControls::changeEvent(QEvent *e)
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
