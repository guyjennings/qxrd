#include "qxrdoutputfileformattertiff.h"
#include "qcepimagedata.h"
#include "qxrdoutputfileformattersettings.h"
#include "qcepbzlib.h"

void QxrdOutputFileFormatterTIFF::compressOutputDataBzip2()
{
  int bzerror;
  unsigned int nbytes_in, nbytes_out;

  FILE*   f   = fopen(qPrintable(m_FileName), "w");
  BZFILE* bzf = qcepBZ2_bzWriteOpen(&bzerror, f, m_CompressionLevel, 0, 0);

  qcepBZ2_bzWrite(&bzerror, bzf, (void*) m_OutputBuffer, m_OutputCount);
  qcepBZ2_bzWriteClose(&bzerror, bzf, 0, &nbytes_in, &nbytes_out);

  fclose(f);
}

