voronoify: vor.c
	gcc -o vor vor.c `MagickWand-config --cflags --cppflags` `MagickWand-config --ldflags --libs` libkdtree.a
