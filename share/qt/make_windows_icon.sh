#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/RootCoin.ico

convert ../../src/qt/res/icons/RootCoin-16.png ../../src/qt/res/icons/RootCoin-32.png ../../src/qt/res/icons/RootCoin-48.png ${ICON_DST}
