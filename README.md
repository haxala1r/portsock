# The Portable Sockets Library.

This is a library aiming to provide a reasonable, reliable and cross-platform
sockets interface for C++.

As you may know, networking is kind of a pain as most code using raw
sockets can be horribly difficult to port to other systems. This library
provides a simple abstraction over the raw sockets so that you can compile
the same code for different operating systems with ease.

This library is simple enough that it isn't really as much of a PIT to
use as some other, more complex ones, but is still useful enough IMO.

# Building

## Windows

When building *FOR* windows, you need to have mingw installed, as the build
system relies heavily on it, and you may have to set the COMPILER environment
variable if your mingw executable has a different name. 

Things get just slightly more complicated when building *ON* windows. For 
one, you *need* a linux-style environment, either through WSL or Cygwin
(or whatever other eldritch horror you decide to make a contract with)
and you need to use mingw (or something compatible) as a compiler.

# Using

## Linux

Things are pretty standard on linux; just include the 
`socket.hpp` header file in any code you want to use the library in (either 
locally, or by installing the headers to a system directory) and then link 
with the library (again either locally or by installing) when compiling
your code. Everything should just work.

## Windows

On windows, on top of the steps for linux, you also have to link your code
with winsock2. (for mingw, this is done by adding the -lws2_32 flag)

Otherwise, the steps are plenty similar to Linux.
