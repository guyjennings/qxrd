/*
  This file contains some example user absorption functions.

  The value of 'centering.enableUserAbsorption' determines how the absorption
  function will be called:

  0:   No user defined absorption
  1:   Called with detector pixel coordinate as arguments (ignores tilt correction)
  2:   Called with x,y displacement from beam center at detector plane (uses tilt correction)
  3:   Called with r,chi as arguments (uses tilt correction)
  4:   Called with q,chi as arguments (uses tilt correction)

  The value returned by the function will be used to multiply the image intensity


  */

function userAbsorb1(i,j) {
  /* As a function of pixel coordinates */
  /* this example implements a 500 pixel radius circle, centered at (1234,2076) */

  var rad = Math.sqrt(Math.pow(i-1234,2) + Math.pow(j-2076,2));

  if (rad > 500) {
    return 0;
  } else {
    return 1;
  }
}

function userAbsorb2(x,y) {
  /* As a function of x,y displacement from detector center */
  /* this example implements a 50mm radius circle */

  var rad = Math.sqrt(Math.pow(x,2) + Math.pow(y,2));

  if (rad > 50) {
    return 0;
  } else {
    return 1;
  }
}

function userAbsorb3(r,chi) {
  /* as a function of r,chi - again a 50mm circle */

  if (r > 50) {
    return 0;
  } else {
    return 1;
  }
}

function userAbsorb4(q, chi) {
  /* as a function of q,chi */

  if (q > 0.1) {
    return 0;
  } else {
    return 1;
  }
}
