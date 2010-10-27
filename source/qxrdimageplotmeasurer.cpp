/******************************************************************
*
*  $Id: qxrdimageplotmeasurer.cpp,v 1.3 2010/09/17 16:21:51 jennings Exp $
*
*******************************************************************/

#include "qxrdimageplotmeasurer.h"

#include "qxrdplotmeasurer.h"
#include "qxrdrasterdata.h"
#include "qxrdimageplot.h"
#include "qxrdcenterfinder.h"

QxrdImagePlotMeasurer::QxrdImagePlotMeasurer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot)
  : QxrdPlotMeasurer(canvas, plot),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdimageplotmeasurer.cpp,v 1.3 2010/09/17 16:21:51 jennings Exp $")
{
  qRegisterMetaType< QwtArray<QwtDoublePoint> >("QwtArray<QwtDoublePoint>");

  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PolygonSelection);
  setRubberBand(QwtPicker::PolygonRubberBand);
}


QwtText QxrdImagePlotMeasurer::trackerText(const QwtDoublePoint &pos) const
{
  QxrdRasterData *raster = m_Plot->raster();
  QxrdMaskRasterData *mask = m_Plot->maskRaster();

  QxrdDataProcessorPtr processor = m_Plot->processor();
  QxrdCenterFinderPtr centerFinder = NULL;

  if (processor) {
    centerFinder = processor->centerFinder();
  }

  QString res = tr("%1, %2").arg(pos.x()).arg(pos.y());

  if (raster) {
    res += tr(", %1").arg(raster->value(pos.x(),pos.y()));
  }

  if (mask) {
    res += tr(", %1").arg(mask->value(pos.x(),pos.y()));
  }

  if (centerFinder) {
    res += tr(", TTH %1").arg(centerFinder->getTTH(pos));
  }

  return res;
}

/******************************************************************
*
*  $Log: qxrdimageplotmeasurer.cpp,v $
*  Revision 1.3  2010/09/17 16:21:51  jennings
*  Rationalised the trackerText implementations
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/09/10 21:09:52  jennings
*  Tilt and powder fitting modifications
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1  2009/08/09 15:38:29  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*
*******************************************************************/
