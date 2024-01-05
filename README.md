ctl
====

`ctl` is a simple template for how to build a C library with cmake. It support
building both static and shared versions as well as creation of pkg-config
configurations.

Download and build
----
Clone `ctl` from GitHub:

    $ git clone https://github.com/johanhedin/ctl.git
    $ cd cte

and then build with cmake:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

`ctl` accepts the following cmake options on the command line (set with -D):

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
