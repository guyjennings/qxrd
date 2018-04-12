#include "qceproicache.h"

QcepROICache::QcepROICache() :
  inherited("cache")
{
}

void QcepROICache::redimension(QRect bounds)
{
  m_Bounds = bounds;

  m_Cache.resize(bounds.width()*bounds.height());
  m_Cache.fill(0);
}

void QcepROICache::setPoint(int c, int r, int inner, int outer)
{
  int offset = offsetOf(c,r);

  if (offset >= 0) {
    int val = 0;
    if (inner) val |= InnerMask;
    if (outer) val |= OuterMask;

    m_Cache[offset] = val;
  }
}

int QcepROICache::getPoint(int c, int r)
{
  int offset = offsetOf(c, r);

  return m_Cache.value(offset);
}

bool QcepROICache::innerPoint(int c, int r)
{
  return innerPoint(getPoint(c,r));
}

bool QcepROICache::outerPoint(int c, int r)
{
  return outerPoint(getPoint(c,r));
}

bool QcepROICache::innerPoint(int v)
{
  return v & InnerMask;
}

bool QcepROICache::outerPoint(int v)
{
  return v & OuterMask;
}

int QcepROICache::offsetOf(int c, int r)
{
  int dx = c - m_Bounds.left();
  int dy = r - m_Bounds.top();
  int wd = m_Bounds.width();

  return dy*wd + dx;
}
