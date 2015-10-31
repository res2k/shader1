#!/usr/bin/env python
import os
import shutil
import sys

if os.name == "nt":
    # Windows: symlinking requires admin privileges, so don't bother
    def os_symlink(src, dst):
        shutil.copy2(src, dst)
else:
    def os_symlink(src, dst):
        os.symlink(src, dst)

# based on http://docs.python.org/2/library/shutil.html#copytree-example
def treelink(src, dst):
    if not os.path.isdir(src):
        os_symlink(src, dst)
        return
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
                os_symlink(srcname, dstname)
            # Subdirectories are currently not desired for doc generation,
            # so skip them
        except (IOError, os.error) as why:
            errors.append((srcname, dstname, str(why)))
        # catch the Error from the recursive copytree so that we can
        # continue with other files
        except Exception as err:
            errors.append(err.args[0])
    if errors:
        raise Exception(errors)

if len(sys.argv) < 2:
    print("Syntax: [src] [dest]")
    sys.exit(1)

src = sys.argv[1]
dst = sys.argv[2]
treelink (src, dst)
