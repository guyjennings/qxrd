#include "qcepsetdatavaluerangecommand.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"

QcepSetDataValueRangeCommand::QcepSetDataValueRangeCommand() :
  QcepSerializableObject("setDataValueRangeCommand"),
  m_RegionType(this, "regionType", 0, "Region Type\n"
                                      "0 = Inside Rectangle\n"
                                      "1 = Outside Rectangle\n"
                                      "2 = Inside Ellipse\n"
                                      "3 = Outside Ellipse"),
  m_RegionCoordinateStyle(this, "regionCoordinateStyle", 0, "Region Coordinate Style\n"
                                                            "0 = Left,Top,Right,Bottom\n"
                                                            "1 = CentreX,CentreY,Width,Height\n"
                                                            "2 = BorderLeft,BorderTop,BorderRight,BorderBottom\n"),
  m_RegionX1(this, "regionX1", 0, "Region X1 Coord"),
  m_RegionY1(this, "regionY1", 0, "Region Y1 Coord"),
  m_RegionX2(this, "regionX2", 0, "Region X2 Coord"),
  m_RegionY2(this, "regionY2", 0, "Region Y2 Coord"),
  m_OutputChoice(this, "outputChoice", 0, "Output Choice\n"
                                          "0=User Defined Value\n"
                                          "1=NaN\n"
                                          "2=Inf\n"
                                          "3=-Inf"),
  m_NewValue(this, "newValue", 0, "New Value")
{

}

void QcepSetDataValueRangeCommand::onImageAvailable(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res = exec(img);

  emit processedImageAvailable(res);
}

QcepDoubleImageDataPtr QcepSetDataValueRangeCommand::exec(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (img) {
    emit progressMade(0);

    res = QcepAllocator::newDoubleImage("dup",
                                        img->get_Width(), img->get_Height(), QcepAllocator::AlwaysAllocate);

    emit progressMade(10);

    res->copyFrom(img);

    emit progressMade(20);

    double newValue;

    int nRows = res->get_Height();
    int nCols = res->get_Width();

    switch (get_OutputChoice()) {
    case 0:
      newValue = get_NewValue();
      break;

    case 1:
      newValue = qQNaN();
      break;

    case 2:
      newValue = qInf();
      break;

    case 3:
      newValue = -qInf();
      break;
    }

    double x1 = get_RegionX1();
    double y1 = get_RegionY1();
    double x2 = get_RegionX2();
    double y2 = get_RegionY2();

    double lf, tp, rt, bt;

    switch (get_RegionCoordinateStyle()) {
    case 0:  //LTRB
      lf = qMin(x1,x2);
      rt = qMax(x1,x2);
      tp = qMin(y1,y2);
      bt = qMax(y1,y2);
      break;

    case 1:  //CxCyWdHt
      lf = x1-qAbs(x2);
      rt = x1+qAbs(x2);
      tp = y1-qAbs(y2);
      bt = y1+qAbs(y2);
      break;

    case 2:  //BlBtBrBb
      lf = x1;
      rt = nCols-x2;
      tp = y2;
      bt = nRows-y2;
      break;
    }

    double cx = (lf + rt)/2.0;
    double cy = (tp + bt)/2.0;
    double a  = (rt - lf)/2.0;
    double b  = (bt - tp)/2.0;

    for (int r=0; r<nRows; r++) {
      int pct = r*80/nRows;

      emit progressMade(pct);

      switch (get_RegionType()) {
      case 0: // Inside Rectangle
        for (int c=0; c<nCols; c++) {
          if (r >= tp && r <= bt && c >= lf && c <= rt) {
            res->setImageData(r,c,newValue);
          }
        }
        break;

      case 1: // Outside Rectangle
        for (int c=0; c<nCols; c++) {
          if (!(r >= tp && r <= bt && c >= lf && c <= rt)) {
            res->setImageData(r,c,newValue);
          }
        }
        break;

      case 2: // Inside Ellipse
        {
          double y = r - cy;
          double xx = a*sqrt(1 - pow(y/b,2));

          for (int c=0; c<nCols; c++) {
            if (xx == xx && c >= cx-xx && c <= cx+xx) {
              res->setImageData(r,c,newValue);
            }
          }
        }
        break;

      case 3: // Outside Ellipse
        {
          double y = r - cy;
          double xx = a*sqrt(1 - pow(y/b,2));

          for (int c=0; c<nCols; c++) {
            if (xx != xx || c < cx-xx || c > cx+xx) {
              res->setImageData(r,c,newValue);
            }
          }
        }
        break;
      }
    }

    res->set_Name(tr("%1.setr").arg(img->get_Name()));
  }

  return res;
}
