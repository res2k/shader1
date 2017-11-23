#!/usr/bin/env python

import shutil
import sys

for fn in sys.argv:
    with open(fn, 'rb') as f:
        shutil.copyfileobj(f, sys.stdout.buffer)
