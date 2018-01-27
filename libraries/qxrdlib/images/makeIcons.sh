#!/bin/bash

for res in 16 32 48 64 128 256 512 ; do
    convert -background none -gravity center qxrdviewer-icon.svg \
          -resize ${res}x${res} -extent ${res}x${res} qxrdviewer-icon-${res}x${res}.png

    convert -background none -gravity center qxrd-icon.svg \
          -resize ${res}x${res} -extent ${res}x${res} qxrd-icon-${res}x${res}.png
done

convert -background none -gravity center qxrdviewer-icon.svg \
    -resize 64x64 -extent 64x64 qxrdviewer-icon.ico

convert -background none -gravity center qxrd-icon.svg \
    -resize 64x64 -extent 64x64 qxrd-icon.ico

png2icns qxrdviewer-icon.icns \
    qxrdviewer-icon-16x16.png \
    qxrdviewer-icon-32x32.png \
    qxrdviewer-icon-48x48.png \
    qxrdviewer-icon-128x128.png \
    qxrdviewer-icon-256x256.png \
    qxrdviewer-icon-512x512.png

png2icns qxrd-icon.icns \
    qxrd-icon-16x16.png \
    qxrd-icon-32x32.png \
    qxrd-icon-48x48.png \
    qxrd-icon-128x128.png \
    qxrd-icon-256x256.png \
    qxrd-icon-512x512.png
