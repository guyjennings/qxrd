#include "qcepimageslicegraphcontrols.h"
#include "ui_qcepimageslicegraphcontrols.h"
#include "qcepdataobjectgraphwindow.h"
#include "qcepimagedata.h"

QcepImageSliceGraphControls::QcepImageSliceGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object) :
  QcepGraphControlsWidget(window, mode, object)
{
  setupUi(this);

  if (m_Window) {
    QcepImageDataBase *img = qobject_cast<QcepImageDataBase*>(object.data());

    int nRows = 0, nCols = 0;

    if (img) {
      nRows = img->get_Height();
      nCols = img->get_Width();
    }

    if (m_Mode == QcepDataObjectGraphWindow::HorizontalSlice) {
      m_SliceStart   -> setMaximum(10000 /*nRows*/);
      m_SliceSummed  -> setMaximum(10000 /*nRows*/);
      m_SliceSkipped -> setMaximum(10000 /*nRows*/);
      m_SliceRepeats -> setMaximum(10000 /*nRows*/);

      m_Window->prop_SliceHStart()   -> linkTo(m_SliceStart);
      m_Window->prop_SliceHSummed()  -> linkTo(m_SliceSummed);
      m_Window->prop_SliceHSkipped() -> linkTo(m_SliceSkipped);
      m_Window->prop_SliceHRepeats() -> linkTo(m_SliceRepeats);

//      connect(m_Window->prop_SliceHStart(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
//      connect(m_Window->prop_SliceHSummed(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
//      connect(m_Window->prop_SliceHSkipped(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
//      connect(m_Window->prop_SliceHRepeats(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    } else if (m_Mode == QcepDataObjectGraphWindow::VerticalSlice) {
      m_SliceStart   -> setMaximum(10000 /*nCols*/);
      m_SliceSummed  -> setMaximum(10000 /*nCols*/);
      m_SliceSkipped -> setMaximum(10000 /*nCols*/);
      m_SliceRepeats -> setMaximum(10000 /*nCols*/);

      m_Window->prop_SliceVStart()   -> linkTo(m_SliceStart);
      m_Window->prop_SliceVSummed()  -> linkTo(m_SliceSummed);
      m_Window->prop_SliceVSkipped() -> linkTo(m_SliceSkipped);
      m_Window->prop_SliceVRepeats() -> linkTo(m_SliceRepeats);

//      connect(m_Window->prop_SliceVStart(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
//      connect(m_Window->prop_SliceVSummed(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
//      connect(m_Window->prop_SliceVSkipped(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
//      connect(m_Window->prop_SliceVRepeats(), SIGNAL(valueChanged(int,int)), m_Window, SLOT(updateDisplay()));
    }

    connect(m_ReplotButton, SIGNAL(clicked(bool)), m_Window, SLOT(updateDisplay()));
  }
}

QcepImageSliceGraphControls::~QcepImageSliceGraphControls()
{
}

void QcepImageSliceGraphControls::changeEvent(QEvent *e)
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
