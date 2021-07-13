# MNIST learning


## Compiling

Compile with ``` make ```.


## Usage

To learn the standard MNIST dataset, run:

```
./learning.exe
```

It takes approximately 8 seconds on an Intel i5-7300HQ (2.50GHz) with OpenBLAS installed, to run 10 epochs. The resulting validation accuracy should be around 98.5 %.

Alternatively, MNIST symbols and their mirrors can also be learned, by running:

```
./learning.exe 1
```

Symbols are also shifted in this mode, to try to make the recognition more robust. This takes ~ 10 seconds, and ends up at 91 % validation accuracy.
