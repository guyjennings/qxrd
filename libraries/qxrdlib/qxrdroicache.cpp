#include "qxrdroicache.h"

QxrdROICache::QxrdROICache() :
  inherited("cache")
{
}

void QxrdROICache::redimension(QRect bounds)
{
  m_Bounds = bounds;

  m_Cache.resize(bounds.width()*bounds.height());
  m_Cache.fill(0);
}

void QxrdROICache::setPoint(int c, int r, int inner, int outer)
{
  int offset = offsetOf(c,r);

  if (offset >= 0) {
    int val = 0;
    if (inner) val |= InnerMask;
    if (outer) val |= OuterMask;

    m_Cache[offset] = val;
  }
}

int QxrdROICache::getPoint(int c, int r)
{
  int offset = offsetOf(c, r);

  return m_Cache.value(offset);
}

bool QxrdROICache::innerPoint(int c, int r)
{
  return innerPoint(getPoint(c,r));
}

bool QxrdROICache::outerPoint(int c, int r)
{
  return outerPoint(getPoint(c,r));
}

bool QxrdROICache::innerPoint(int v)
{
  return v & InnerMask;
}

bool QxrdROICache::outerPoint(int v)
{
  return v & OuterMask;
}

int QxrdROICache::offsetOf(int c, int r)
{
  int dx = c - m_Bounds.left();
  int dy = r - m_Bounds.top();
  int wd = m_Bounds.width();

  return dy*wd + dx;
}
