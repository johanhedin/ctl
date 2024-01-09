ctl
====
[![Build](https://github.com/johanhedin/ctl/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/johanhedin/ctl/actions/workflows/c-cpp.yml)

`ctl` is a small example how to build a C library with cmake. It support
building both static and shared versions as well as creation of pkg-config
configurations. The example is submodule friendly if included in another
(git) repo. The cmake configuration works with cmake from 2.8.12 all the way up
to 3.28.

Download and build
----
Clone `ctl` from GitHub:

    $ git clone https://github.com/johanhedin/ctl.git
    $ cd ctl

and then build with cmake:

    $ mkdir build
    $ cd build
    $ cmake .. -DCTL_LINK_TYPE=Shared
    $ make

`ctl` accepts the following build options on the command line (set with -D):

 * `CTL_LINK_TYPE={Intree,Static,Shared}` Defaults to `Intree` if not set
 * `CMAKE_BUILD_TYPE={Debug,Release,RelWithDebInfo}` Defaults to `Debug` if not set
 * `CMAKE_INSTALL_PREFIX=<path>` Defaults to `/usr/local` if not set

Keep up to date with changes
----
To keep up to date with changes and updates to `ctl`, simply run:

    $ cd ctl
    $ git pull --ff-only
    $ cd build
    $ cmake ..
    $ make
