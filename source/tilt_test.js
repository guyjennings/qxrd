testImage.alpha = 20;
testImage.beta  = 0;
testImage.gamma = 0;

testImage.setDistance(1000,100,100);
testImage.setChiRange(45,135);

testImage.clearRings();
testImage.appendRing(5.0, 100, 0.1);
testImage.appendRing(7.5, 100, 0.1);

testImage.generateImage();

