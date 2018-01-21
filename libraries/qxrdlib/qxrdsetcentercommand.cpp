#include "qxrdsetcentercommand.h"
#include <QToolButton>
#include "qxrdcenterfinderpicker.h"
#include "qxrdimageplot.h"
#include "qxrdcenterfinder.h"

QxrdSetCenterCommand::QxrdSetCenterCommand(QString                    name,
                                           QxrdPlotWidget            *plot,
                                           QxrdPlotWidgetSettingsWPtr set,
                                           QxrdCenterFinderWPtr       cf)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/ringfinder.png", "Set Center", true),
    m_CenterFinder(cf)
{  
  m_Picker = new QxrdCenterFinderPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  QxrdCenterFinderPtr f(m_CenterFinder);

  if (f) {
    connect(m_Picker, (void (QwtPlotPicker::*)(const QPointF &)) &QwtPlotPicker::selected,
            f.data(), &QxrdCenterFinder::onCenterChanged);
  } else {
    printMessage("QxrdSetCenterCommand::QxrdSetCenterCommand no center finder");
  }
}

////TODO: implement
//void QxrdSetCenterCommand::selected(const QPointF &pt)
//{
//  printMessage(tr("QxrdSetCenterCommand::selected(x:%1,y:%2)")
//               .arg(pt.x()).arg(pt.y()));
//}
