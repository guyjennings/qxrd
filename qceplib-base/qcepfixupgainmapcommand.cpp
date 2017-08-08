#include "qcepfixupgainmapcommand.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"

QcepFixupGainMapCommand::QcepFixupGainMapCommand()
  : QcepSerializableObject("fixupGainMapCommand"),
    m_AverageSubTiles(this, "averageSubTiles", false, "Pre-smooth image"),
    m_AverageWidth(this, "averageWidth", 10, "Pre-smooth width"),
    m_AverageHeight(this, "averageHeight", 10, "Pre-smooth height"),
    m_GenerateAveragedImage(this, "generateAveragedImage", false, "Generate pre-smooth image"),
    m_GenerateAveragedDiff(this, "generateAveragedDiff", false, "Generate diff pre-smooth and orig data"),
    m_FitParaboloid(this, "fitParaboloid", false, "Fit elliptical paraboloid"),
    m_GenerateParaboloid(this, "generateParaboloid", false, "Generate fitted paraboloid image"),
    m_GenerateParaboloidDiff(this, "generateParaboloidDiff", false, "Generate diff paraboloid and orig data"),
    m_FittedOffset(this, "fittedOffset", 0.0, "Fit Offset"),
    m_FittedCenterX(this, "fittedCenterX", 0.0, "Fit Center X"),
    m_FittedCenterY(this, "fittedCenterY", 0.0, "Fit Center Y"),
    m_FittedWidth(this, "fittedWidth", 0.0, "Fit Width"),
    m_FittedHeight(this, "fittedHeight", 0.0, "Fit Height")
{
}

void QcepFixupGainMapCommand::onImageAvailable(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res = exec(img);

  emit processedImageAvailable(res);
}

QcepDoubleImageDataPtr QcepFixupGainMapCommand::exec(QcepDoubleImageDataPtr img)
{
  clearIntermediateResults();

  QcepDoubleImageDataPtr res;

  if (img) {
    int imageHeight = img->get_Height();

    m_NSteps=0;
    m_Step=0;

    if (get_AverageSubTiles()) {
      m_NSteps += imageHeight;

      if (get_GenerateAveragedImage()) {
        m_NSteps += imageHeight;

        if (get_GenerateAveragedDiff())  m_NSteps += imageHeight;
      }
    }

    if (get_FitParaboloid()) {
      m_NSteps += imageHeight;

      if (get_GenerateParaboloid())     m_NSteps += imageHeight;
      if (get_GenerateParaboloidDiff()) m_NSteps += imageHeight;
    }

    if (get_AverageSubTiles()) {
      QcepDoubleImageDataPtr avg = averageSubTiles(img, get_AverageWidth(), get_AverageHeight());

      appendIntermediateResult(avg);

      if (get_GenerateAveragedImage()) {
        QcepDoubleImageDataPtr avg2 = generateAveragedImage(avg, img);

        appendIntermediateResult(avg2);

        if (get_GenerateAveragedDiff()) {
          QcepDoubleImageDataPtr avg3 = generateAveragedDiff(avg2, img);

          appendIntermediateResult(avg3);
        }
      }

      res = avg;
    } else {
      res = img;
    }

    if (get_FitParaboloid()) {
      fitParaboloid(res);

      if (get_GenerateParaboloid()) {
        res = generateParaboloid(img);

        appendIntermediateResult(res);

        if (get_GenerateParaboloidDiff()) {
          res = generateParaboloidDiff(res, img);
        }
      }
    }
  }

  return res;
}

int QcepFixupGainMapCommand::intermediateResultCount()
{
  return m_IntermediateResults.count();
}

QcepDataObjectPtr QcepFixupGainMapCommand::intermediateResult(int i)
{
  return m_IntermediateResults.value(i);
}

void QcepFixupGainMapCommand::clearIntermediateResults()
{
  m_IntermediateResults.clear();
}

void QcepFixupGainMapCommand::appendIntermediateResult(QcepDataObjectPtr obj)
{
  m_IntermediateResults.append(obj);
}

void QcepFixupGainMapCommand::performWork(int nSteps)
{
  m_Step += nSteps;

  emit progressMade(int(100.0*m_Step/m_NSteps));
}

QcepDoubleImageDataPtr QcepFixupGainMapCommand::averageSubTiles(QcepDoubleImageDataPtr img, double tileWidth, double tileHeight)
{
  QcepDoubleImageDataPtr res;

  if (img) {
    int w = img->get_Width();
    int h = img->get_Height();

    int tw = tileWidth;
    int th = tileHeight;
    int nw = w/tw;
    int nh = h/th;

    res = QcepAllocator::newDoubleImage("avg", nw, nh, QcepAllocator::AlwaysAllocate);

    res -> copyPropertiesFrom(img);

    for (int tr=0; tr<nh; tr++) {
      performWork(th);

      for (int tc=0; tc<nw; tc++) {
        double sum = 0, npts = 0;

        for (int r=0; r<th; r++) {
          for (int c=0; c<tw; c++) {
            double val = img->getImageData(tc*tw+c, tr*th+r);

            if (val==val) {
              sum  += val;
              npts += 1;
            }
          }
        }

        if (npts > 0) {
          res->setImageData(tc, tr, sum/npts);
        } else {
          res->setImageData(tc, tr, qQNaN());
        }
      }
    }

    res->set_Name(tr("%1.tavg").arg(img->get_Name()));
  }

  return res;
}

QcepDoubleImageDataPtr QcepFixupGainMapCommand::generateAveragedImage(QcepDoubleImageDataPtr avg, QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (img && avg) {
    int w = img->get_Width();
    int h = img->get_Height();

    int tw = get_AverageWidth();
    int th = get_AverageHeight();

    res = QcepAllocator::newDoubleImage("avg", w, h, QcepAllocator::AlwaysAllocate);

    res -> copyPropertiesFrom(img);

    for (int r=0; r<h; r++) {
      performWork(1);

      double y = double(r)/double(th) - 0.5;
      for (int c=0; c<w; c++) {
        double x = double(c)/double(tw) - 0.5;

        res->setImageData(c,r, avg->value(x,y));
      }
    }

    res -> set_Name(tr("%1.avg").arg(img->get_Name()));
  }

  return res;
}

QcepDoubleImageDataPtr QcepFixupGainMapCommand::generateAveragedDiff(QcepDoubleImageDataPtr avg, QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (img && avg) {
    int w = img->get_Width();
    int h = img->get_Height();

    res = QcepAllocator::newDoubleImage("diff", w, h, QcepAllocator::AlwaysAllocate);

    performWork(h);

    res -> copyFrom(img);
    res -> subtract(avg);

    res->set_Name(tr("%1.tdif").arg(img->get_Name()));
  }

  return res;
}

void QcepFixupGainMapCommand::fitParaboloid(QcepDoubleImageDataPtr img)
{
}

QcepDoubleImageDataPtr QcepFixupGainMapCommand::generateParaboloid(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (img) {
    int w = img->get_Width();
    int h = img->get_Height();

    double offset  = get_FittedOffset();
    double centerX = get_FittedCenterX();
    double centerY = get_FittedCenterY();
    double width   = get_FittedWidth();
    double height  = get_FittedHeight();

    res = QcepAllocator::newDoubleImage("fit", w, h, QcepAllocator::AlwaysAllocate);

    res -> copyPropertiesFrom(img);

    for (int r=0; r<w; r++) {
      performWork(1);

      double t2 = (r - centerY)/height;

      for (int c=0; c<h; c++) {
        double t1 = (c - centerX)/width;

        double v = offset + t1*t1 + t2*t2;

        res->setImageData(c,r, v);
      }
    }

    res -> set_Name(tr("%1.fit").arg(img->get_Name()));
  }

  return res;
}

QcepDoubleImageDataPtr QcepFixupGainMapCommand::generateParaboloidDiff(QcepDoubleImageDataPtr par, QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (par && img) {
    int w = img->get_Width();
    int h = img->get_Height();

    res = QcepAllocator::newDoubleImage("diff", w, h, QcepAllocator::AlwaysAllocate);

    performWork(h);

    res -> copyFrom(img);
    res -> subtract(par);

    res->set_Name(tr("%1.fdif").arg(img->get_Name()));
  }

  return res;
}
