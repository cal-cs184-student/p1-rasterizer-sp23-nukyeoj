#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/joeyhou0804/Desktop/cs184/p1-rasterizer-sp23-nukyeoj-master/xcode
  make -f /Users/joeyhou0804/Desktop/cs184/p1-rasterizer-sp23-nukyeoj-master/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/joeyhou0804/Desktop/cs184/p1-rasterizer-sp23-nukyeoj-master/xcode
  make -f /Users/joeyhou0804/Desktop/cs184/p1-rasterizer-sp23-nukyeoj-master/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/joeyhou0804/Desktop/cs184/p1-rasterizer-sp23-nukyeoj-master/xcode
  make -f /Users/joeyhou0804/Desktop/cs184/p1-rasterizer-sp23-nukyeoj-master/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/joeyhou0804/Desktop/cs184/p1-rasterizer-sp23-nukyeoj-master/xcode
  make -f /Users/joeyhou0804/Desktop/cs184/p1-rasterizer-sp23-nukyeoj-master/xcode/CMakeScripts/ReRunCMake.make
fi

