#include "qxrdimagehistogramgraphcontrols.h"
#include "ui_qxrdimagehistogramgraphcontrols.h"
#include "qxrddataobjectgraphwindow.h"

QxrdImageHistogramGraphControls::QxrdImageHistogramGraphControls(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object) :
  QxrdGraphControlsWidget(window, mode, object)
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

    connect(m_Window->prop_HistFullVis(), SIGNAL(valueChanged(bool,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistAVis(), SIGNAL(valueChanged(bool,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistBVis(), SIGNAL(valueChanged(bool,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistCVis(), SIGNAL(valueChanged(bool,int)), m_Window, SLOT(updateDisplay()));

    connect(m_Window->prop_HistALeft(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistATop(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistARight(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistABottom(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));

    connect(m_Window->prop_HistBLeft(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistBTop(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistBRight(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistBBottom(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));

    connect(m_Window->prop_HistCLeft(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistCTop(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistCRight(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    connect(m_Window->prop_HistCBottom(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
  }
}

QxrdImageHistogramGraphControls::~QxrdImageHistogramGraphControls()
{
}

void QxrdImageHistogramGraphControls::changeEvent(QEvent *e)
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
