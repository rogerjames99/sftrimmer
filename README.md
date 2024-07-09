# Soundfont Trimmer
## Introduction

Soundfont trimmer (sftrimmer) is a small program that is designed to shrink the size of soundfont files so that they occupy less memory space when loaded. The is especially important for embedded devices with restricted ram storage. It does this by removing redundant sample points that are not used to play the the sample loop.

## The ideal sample loop layout

1. 8 sample points before loop start sample point, the last 4 sample points of this section
identical to the 4 sample points before the loop end sample.
2. loop start sample point, this should be identical to the loop end sample point.
3. loop sample points.
4. loop end sample point.
5. 8 sample points after loop end sample point, the first 4 sample points of this section 
should be identical to the 4 sample points after the loop start sample point.
6. 46 zero sample points.

This implementation looks at the 9 samples around the loop start and loop end points, It overwrites these points with the mean of the the two corresponding samples. Thus ensuring that these points are identical.

## Usage

The program takes two parameters the first being the the full pathname to the soundfont file. The second being the full pathname of the new file to be created.

