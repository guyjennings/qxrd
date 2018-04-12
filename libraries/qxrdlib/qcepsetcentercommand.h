#ifndef QCEPSETCENTERCOMMAND_H
#define QCEPSETCENTERCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qcepcenterfinderpicker-ptr.h"
#include "qcepcenterfinder-ptr.h"

class QCEP_EXPORT QcepSetCenterCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepSetCenterCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set,
                       QcepCenterFinderWPtr cf);

private:
//  void selected(const QPointF &pt);

private:
  QcepCenterFinderWPtr    m_CenterFinder;
  QcepCenterFinderPicker *m_Picker;
};

#endif // QCEPSETCENTERCOMMAND_H
