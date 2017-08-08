#include "qcepsetrangeofimagecommand.h"
#include "qcepallocator.h"
#include "qcepimagedata.h"

QcepSetRangeOfImageCommand::QcepSetRangeOfImageCommand()
  : QcepSerializableObject("setRangeOfImageCommand"),
    m_RangeChoice(this, "rangeChoice", 0, "Range Choices\n"
                                          "0=outside both\n"
                                          "1=inside both\n"
                                          "2=above max\n"
                                          "3=below min"),
    m_OutputChoice(this, "outputChoice", 0, "Output Choice\n"
                                            "0=User Defined Value\n"
                                            "1=NaN\n"
                                            "2=Inf\n"
                                            "3=-Inf"),
    m_Minimum(this, "minimum", -qInf(), "Range Minimum"),
    m_Maximum(this, "maximum", qInf(),  "Range Maximum"),
    m_NewValue(this, "newValue", 0, "New Value")
{
}

void QcepSetRangeOfImageCommand::onImageAvailable(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res = exec(img);

  emit processedImageAvailable(res);
}

QcepDoubleImageDataPtr QcepSetRangeOfImageCommand::exec(QcepDoubleImageDataPtr img)
{
  QcepDoubleImageDataPtr res;

  if (img) {
    emit progressMade(0);

    res = QcepAllocator::newDoubleImage("dup",
                                        img->get_Width(), img->get_Height(), QcepAllocator::AlwaysAllocate);

    emit progressMade(10);

    res->copyFrom(img);

    emit progressMade(20);

    int rangeChoice = get_RangeChoice();

    double newValue;

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

    double min = get_Minimum();
    double max = get_Maximum();

    int nRows = res->get_Height();
    int nCols = res->get_Width();

    for (int r=0; r<nRows; r++) {
      int pct = r*80/nRows;

      emit progressMade(10+int(pct));

      for (int c=0; c<nCols; c++) {
        double v = res->getImageData(c, r);

        switch (rangeChoice) {
        case 0:
          if (v < min || v > max) {
            res->setImageData(c, r, newValue);
          }
          break;

        case 1:
          if (v >= min && v <= max) {
            res->setImageData(c, r, newValue);
          }
          break;

        case 2:
          if (v > max) {
            res->setImageData(c, r, newValue);
          }
          break;

        case 3:
          if (v < min) {
            res->setImageData(c, r, newValue);
          }
          break;
        }
      }
    }

    res->set_Name(tr("%1.seti").arg(img->get_Name()));

    emit progressMade(100);
  }

  return res;
}
