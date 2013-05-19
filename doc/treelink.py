#!/usr/bin/env python
import os
import shutil
import sys

# based on http://docs.python.org/2/library/shutil.html#copytree-example
def treelink(src, dst):
    names = os.listdir(src)
    if os.path.exists(dst): shutil.rmtree(dst)
    os.makedirs(dst)
    errors = []
    for name in names:
        if name.endswith("~"): continue # Ignore backup files
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)
        try:
            if not os.path.isdir(srcname):
                os.symlink(srcname, dstname)
            # Subdirectories are currently not desired for doc generation,
            # so skip them
        except (IOError, os.error) as why:
            errors.append((srcname, dstname, str(why)))
        # catch the Error from the recursive copytree so that we can
        # continue with other files
        except Error as err:
            errors.extend(err.args[0])
    if errors:
        raise Error(errors)

if len(sys.argv) < 2:
    print "Syntax: [src] [dest]"
    sys.exit(1)

src = sys.argv[1]
dst = sys.argv[2]
treelink (src, dst)
