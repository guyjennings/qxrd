#include "qcepsetcentercommand.h"
#include <QToolButton>
#include "qcepcenterfinderpicker.h"
#include "qcepimageplot.h"
#include "qcepcenterfinder.h"

QcepSetCenterCommand::QcepSetCenterCommand(QcepPlotWidget            *plot,
                                           QcepPlotWidgetSettingsWPtr set,
                                           QcepCenterFinderWPtr       cf)
  : QcepPlotButtonCommand("setCenterCommand", "Set Center", plot, set, ":/images/ringfinder.png", true),
    m_CenterFinder(cf)
{  
  m_Picker = new QcepCenterFinderPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  QcepCenterFinderPtr f(m_CenterFinder);

  if (f) {
    connect(m_Picker, (void (QwtPlotPicker::*)(const QPointF &)) &QwtPlotPicker::selected,
            f.data(), &QcepCenterFinder::onCenterChanged);
  } else {
    printMessage("QcepSetCenterCommand::QcepSetCenterCommand no center finder");
  }
}

////TODO: implement
//void QcepSetCenterCommand::selected(const QPointF &pt)
//{
//  printMessage(tr("QcepSetCenterCommand::selected(x:%1,y:%2)")
//               .arg(pt.x()).arg(pt.y()));
//}
