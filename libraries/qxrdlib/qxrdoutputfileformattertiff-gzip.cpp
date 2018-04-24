#include "qxrdoutputfileformattertiff.h"
#include "qcepimagedata.h"
#include "qxrdoutputfileformattersettings.h"
#include "zlib.h"

void QxrdOutputFileFormatterTIFF::compressOutputDataGzip()
{
  gzFile gzf = gzopen(qPrintable(m_FileName), "w");

  gzwrite(gzf, (void*) m_OutputBuffer, m_OutputCount);

  gzclose(gzf);
}
