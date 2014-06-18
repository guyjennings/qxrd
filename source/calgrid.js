/*
  A js script to generate an array of plot markers from a calibration grid image
  */

var grid;

function initGrid()
{
//  if (centering.countPowderPoints() >= 3) {
//    x0 = centering.getPowderPointX(0);
//    y0 = centering.getPowderPointY(0);

//    dxx = centering.getPowderPointX(1)-x0;
//    dxy = centering.getPowderPointY(1)-y0;

//    dyx = centering.getPowderPointX(2)-x0;
//    dyy = centering.getPowderPointY(2)-y0;

//    distortion.p0 = [x0, y0];
//    distortion.p1 = [x0+dxx, y0+dxy];
//    distortion.p2 = [x0+dyx, y0+dyy];
    x0 = distortion.p0[0];
    y0 = distortion.p0[1];
    dxx = distortion.p1[0]-x0;
    dxy = distortion.p1[1]-y0;
    dyx = distortion.p2[0]-x0;
    dyy = distortion.p2[1]-y0;

    print("x0 = ",  x0,  " y0 = ", y0);
    print("dxx = ", dxx, " dxy = ", dxy);
    print("dyx = ", dyx, " dyy = ", dyy);
//  }
}

function goodPeak(x,y)
{
  var dx = Math.abs(centering.peakCenterX - x);
  var dy = Math.abs(centering.peakCenterY - y);
  var ht = centering.peakHeight;
  var rt = ht/centering.peakBackground;
  var wd = centering.peakWidth;

  if ((wd > distortion.wMin) &&
      (wd < distortion.wMax) &&
      (ht > distortion.hgtMin) &&
      (rt > distortion.ratMin) &&
      (dx < distortion.distMax[0]) &&
      (dy < distortion.distMax[1])) {
    return true;
  } else {
    return false;
  }
}

function refineAxes()
{
  j = 0;
  nx = distortion.n1;
  maxx = distortion.distMax[0];
  maxy = distortion.distMax[1];
  hgtm = distortion.hgtMin;
  ratm = distortion.ratMin;

  for (i=2; i<=nx; i++) {
    x = x0 + i*dxx + j*dyx;
    y = y0 + i*dxy + j*dyy;
    centering.peakRadius = distortion.wNom;

    if (centering.fitPeakNear(x,y)) {
      if (goodPeak(x,y)) {
        xx = centering.peakCenterX;
        yy = centering.peakCenterY;
        centering.appendPowderPoint(xx,yy);
      }
    }
  }

  i = 0;
  ny = distortion.n2;
  for (j=2; j<=ny; j++) {
    x = x0 + i*dxx + j*dyx;
    y = y0 + i*dxy + j*dyy;
    centering.peakRadius = distortion.wNom;

    if (centering.fitPeakNear(x,y)) {
      if (goodPeak(x,y)) {
        xx = centering.peakCenterX;
        yy = centering.peakCenterY;
        centering.appendPowderPoint(xx,yy);
      }
    }
  }
}

function calGrid()
{
  grid = [];
  nx = distortion.n1;
  ny = distortion.n2;

  centering.deletePowderPoints();
  distortion.clearGridPoints();

  for (j=0; j<=ny; j++) {
    for (i=0; i<=nx; i++) {
      x = x0 + i*dxx + j*dyx;
      y = y0 + i*dxy + j*dyy;

      centering.peakRadius = distortion.wNom;
      centering.peakBackground = 1500;
      centering.peakBackgroundX = 0;
      centering.peakBackgroundY = 0;

      if (centering.fitPeakNear(x,y)) {
        if (goodPeak(x,y)) {
          xx = centering.peakCenterX;
          yy = centering.peakCenterY;
          centering.appendPowderPoint(xx,yy);
          grid.push([i,j,xx,yy]);
          distortion.appendGridPoint(i,j,xx,yy);
        }
      }
    }
  }
}

function dumpGrid()
{
  for(p in grid) {
    pt = grid[p]
    i = pt[0];
    j = pt[1];
    x = pt[2];
    y = pt[3];
    xx = x0 + i*dxx + j*dyx;
    yy = y0 + i*dxy + j*dyy;
    printMessage(i,j,xx,yy,x,y,x-xx,y-yy);
  }
}
