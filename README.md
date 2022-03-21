# The Portable Sockets Library.

This is a library aiming to provide a reasonable, reliable and cross-platform
sockets interface for C++.

As you may know, networking is kind of a pain as most code using raw
sockets can be horribly difficult to port to other systems. This library
provides a simple abstraction over the raw sockets so that you can compile
the same code for different operating systems with ease.

# Building


## Windows

When building for windows, you need to have mingw installed, as the build
system relies heavily on it, and you may have to set the compiler variable
inside the makefile to set a different mingw executable if your installation
isn't the same as that of debian.
