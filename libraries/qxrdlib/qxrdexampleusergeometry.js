/*
  This file gives an example user geometry function.

  The value of 'centering.enableUserGeometry' determines hoe the geometry
  function will be called:

  0:  No user geometry function
  1:  Called with detector pixel coordinates as arguments

  The function should return a radial distance value which represents
  the bin number that the pixel should map into,  If you want a pixel to
  be ignored you can return a NaN value.
  */

function userGeometry(i, j) {
  /* this example implements an untilted detector, with hte
    beam center at (1234,2076)
    */

  return Math.sqrt(Math.pow(i-1234,2) + Math.pow(j-2076,2));
}
