img-rot
=======

Finds angle in degrees between two images where one of them is rotated around the center of the image.

Project description
-------------------

A console program that reads two PNG images and finds the angle one picture is rotated to the other. A few remarks:

  1. Both pictures must be PNG images in RGB(A) or Grayscale.
  2. Pictures should be rotated around the center of the image.
  3. Both pictures are converted to grayscale. They should have high contrast.
  4. The resolution of both pictures must be the same.

How to build
------------

The program requires two external libraries: `zlib` and `libpng` - both for reading the pictures. Makefile for MinGW is provided.

Program is tested on Visual C++ 2012 and GCC 4.7.1.

How to run the program
----------------------

    > img.exe <img-orig> <img-rot> [ <pass=3> [ <crop=0.5> ] ]

The last two parameters are optional.

For every pass of the algorithm there is printed the value of the angle (in degrees) for which the mean-square error is the lowest. The value of the error is printed in brackets. Example result:

    PS> .\out\img.exe data\chess.png data\chess-rot22.png 3 0.7
    Pass 1:
    Angle: 23 (24848)
    Pass 2:
    Angle: 22.53 (20411.9)
    Pass 3:
    Angle: 22.5262 (20411.1)

In the default settings there are 3 passes of the algorithm. It gives the result with precision of .0001 of the angle.
