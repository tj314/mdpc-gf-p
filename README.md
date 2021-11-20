# Implementation of p-ary QC-MDPC McEliece
This repository contains an implementation of the p-ary QC-MDPC McEliece cryptosystem as defined in the article [Improved Decoders for p-ary MDPC](https://www.lebesgue.fr/sites/default/files/proceedings_WCC/WCC_2019_paper_32.pdf). It is being developed as a [team project](https://timak.netlify.app/) at the Slovak University of Technology, Faculty of Electrical Engineering and Information Technology.

This implementation is WIP.

## How to compile
To compile our code, use cmake (version 3.16+) and a c++17 compiler, as we make heavy use of c++17 features. Navigate to the directory where you saved this project and execute:
```
mkdir -p build
cd build
cmake ..
cmake --build .
./mceliece_qcmdpc_gf_p
```

This implementation depends on NTL (developed and tested using version 11.5.1). On some Linux distribution the NTL package is build using default compilation options. This means that instead of throwing exceptions, NTL prints error message and aborts. This is undesirable behavior.

If you installed NTL using your package manager, uninstall it and then compile it from source. In order to do that follow the official instructions for [UNIX-like systems](https://libntl.org/doc/tour-unix.html) or [Windows](https://libntl.org/doc/tour-win.html) (though this implementation is only tested with Linux as of yet). Make sure to set `NTL_EXCEPTIONS=on SHARED=on`. Leave the c++ standard as default (c++11). We also recommend setting `TUNE=auto`, though this is not required.

