# Implementation of p-ary QC-MDPC McEliece
This repository contains an implementation of the p-ary QC-MDPC McEliece cryptosystem as defined in the article [Improved Decoders for p-ary MDPC](https://www.lebesgue.fr/sites/default/files/proceedings_WCC/WCC_2019_paper_32.pdf). It is being developed as a [team project](https://timak.netlify.app/) at the Slovak University of Technology, Faculty of Electrical Engineering and Information Technology.

This implementation is currently WIP.

## How to compile
You can compile this project using the provided Makefile. 
You need the g++ compiler. The code is compiled with the c++17 standard and makes heavy use of c++17 features. From the project's root directory simply call:
```shell
$ make
```

The resulting binary will be located in the `build` directory.

Note, that this implementation depends on [FLINT](https://flintlib.org/). You can install FLINT using your distribution's package manager. For instance, on Fedora 35 you may run this command:
```shell
$ sudo dnf install flint flint-devel flint-static
```

The provided Makefile assumes FLINT's header files are available in the `/usr/include/flint` directory. Should you distribution place them elsewhere, you must modify the Makefile. You can find the location of these files running these commands:
```shell
$ sudo updatedb
$ locate flint
```

## Note about previous implementation
The master branch contains the current implementation. There is also the old-master branch. This contains the previous work on this project largely based on [NTL](https://libntl.org/). This implementation is not in a usable state and was kept only due to archival reasons.

We decided to switch from NTL to FLINT because NTL's featureset didn't meet our requirements. During the implementation, it became obvious that we will have to work extensively with rational numbers. FLINT implements these, NTL does not. We may create our own implementation and keep using NTL, but that is not the focus of this project. We therefore concluded that a switch to FLINT is necessary.