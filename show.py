#!/usr/bin/python
# -*- coding: utf-8 -*-
import os.path
import sys
import numpy as np
import matplotlib.pyplot as plt
import array
fname = sys.argv[1]
with open(fname,"rb") as f:
  x = np.fromfile(f, dtype=np.int16)
  print x.shape
  fac = 1
  if x.shape[0] >= 307200:
    fac = 2
  x = x.reshape(fac*320,fac*240)
#  plt.imshow(x, interpolation='nearest', cmap='RdBu')
  plt.imshow(x, interpolation='none')
#  plt.imshow(x)
  plt.show()

