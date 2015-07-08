#include "qxrdimageslicegraphcontrols.h"
#include "ui_qxrdimageslicegraphcontrols.h"
#include "qxrddataobjectgraphwindow.h"

QxrdImageSliceGraphControls::QxrdImageSliceGraphControls(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object) :
  QxrdGraphControlsWidget(window, mode, object)
{
  setupUi(this);

  if (m_Window) {
    if (m_Mode == QxrdDataObjectGraphWindow::HorizontalSlice) {
      m_Window->prop_SliceHStart()   -> linkTo(m_SliceStart);
      m_Window->prop_SliceHSummed()  -> linkTo(m_SliceSummed);
      m_Window->prop_SliceHSkipped() -> linkTo(m_SliceSkipped);
      m_Window->prop_SliceHRepeats() -> linkTo(m_SliceRepeats);

      connect(m_Window->prop_SliceHStart(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
      connect(m_Window->prop_SliceHSummed(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
      connect(m_Window->prop_SliceHSkipped(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
      connect(m_Window->prop_SliceHRepeats(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    } else if (m_Mode == QxrdDataObjectGraphWindow::VerticalSlice) {
      m_Window->prop_SliceVStart()   -> linkTo(m_SliceStart);
      m_Window->prop_SliceVSummed()  -> linkTo(m_SliceSummed);
      m_Window->prop_SliceVSkipped() -> linkTo(m_SliceSkipped);
      m_Window->prop_SliceVRepeats() -> linkTo(m_SliceRepeats);

      connect(m_Window->prop_SliceVStart(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
      connect(m_Window->prop_SliceVSummed(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
      connect(m_Window->prop_SliceVSkipped(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
      connect(m_Window->prop_SliceVRepeats(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    }
  }
}

QxrdImageSliceGraphControls::~QxrdImageSliceGraphControls()
{
}

void QxrdImageSliceGraphControls::changeEvent(QEvent *e)
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
