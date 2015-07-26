
Flir hackathon 24 Jul 2015

https://www.eventbrite.com/e/flirs-bring-the-heat-hacker-maker-challenge-london-tickets-7620167125?aff=es2

App enhances infrared images captured from a FlirONE attached to an iPhone

The FlirONE infrared sensor contains 160 by 120 pixels, relatively low resolution.

By merging two pictures of the same scene it is possible to improve the image resolution (subpixel interpolation).

The iPhone motion detector (3D gyroscope) is used to detect changes in camera orientation between successive images.  This orientation information is used to calculate which pixels from the twoimages correspond to the same point in space.  The intensity from the adjacent pixels is interpolated to double (in our case) the resolution.

The workflow is currently rather involved.  Things were done this way to remove single points of failure during the hack.

Images from the iPhone are sent to a server and the raw data extracted from the mine format sent by the phone.

A C program (for high performance) calculates the pixel overlap and merges the two images.

The 'raw' data returned by the Flir API has been upsamples to produce a 320 by 240 image.

Dependencies:  an iPhone with a gyroscope sensor supporting better than 0.1 degree resolution (needed because of field of view of one pixel is around 0.25 degree).


