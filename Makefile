COMPILER=gcc
FILESYSTEM_FILES=mirrorfs.c

# gcc -Wall passthrough.c `pkg-config fuse3 --cflags --libs` -o passthrough

build: $(FILESYSTEM_FILES)
		$(COMPILER) -g -Wall $(FILESYSTEM_FILES) `pkg-config fuse3 --cflags --libs` -o mirrorfs 
		echo 'To Mount: ./mirrorfs -f [mount point]'

clean:
		rm mirrorfs
