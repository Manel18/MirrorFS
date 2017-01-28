# MirrorFS
The simplest filesystem one can do with FUSE. Just mirror the operations done on the mount directory to another directory.

Dependencies: fuse

Compile: make

Run (provided that the mount and loop directory exist):

mountdir = /tmp/mount (for example)
rootdir = /tmp/loop (for example)

./mirrorfs mountdir -omodules=subdir,subdir=rootdir -oallow_other

If you want to see the logs as you perform operations (different terminals), just add the -f flag:

./mirrorfs -f mountdir -omodules=subdir,subdir=rootdir -oallow_other
