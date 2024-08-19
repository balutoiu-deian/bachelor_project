This project aims to see the performance differences between sorting algorithms written in Assmebly x86-64, C and Rust.

Measurements are done using the "RDTSCP" instruction. This instruction reads the current processor count. It is native in Assembly, and implemented using inline Assembly for C and Rust.
