with (testImage) {
   alpha = 0;
   beta = 0;
   gamma = 0;
   var w = 0.1;
   setDistance(300,100,100);
   clearRings();
   appendRing(0.0, 500, w);
   appendRing(5.0, 800, w);
   appendRing(7.7, 300, w);
   appendRing(10.0, 500, w);
   appendRing(15.0, 400, w);
   appendRing(17.7, 300, w);

   generateImage()
}
