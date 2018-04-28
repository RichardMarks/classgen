# classgen

Are you sick of typing C++ `#ifndef` include guards?

Are you tired of having to create directories and cpp/hpp file pairs?

Well I am. So I made classgen for myself and if you like it, you can use it too! 😁

## Requirements
For now classgen is built for POSIX style operating systems. Developed on, and tested on OSX.
*Windows users beware. This will not work for you out the box.*

## Install
For now there is no automatic installation process, so here is what to do.
+ install clang (if you are on OSX, install XCode commandline tools)
+ clone this repo
+ run `make`
+ copy `bin/classgen` somewhere on your `PATH`

## Usage
Run classgen with no parameters and you will get the help screen which tells you all about how to use it.

```
$ classgen
```

## License
MIT Licence (C) 2018, Richard Marks
