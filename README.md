# NeuralLib

A small deep learning library.


## Installation

Only Linux (Ubuntu) is supported at this time.

* Installing the graphical library SDL2, and some FreeType fonts. This is only needed for the MNIST animation:

```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libfreetype6-dev
```

* Installing OpenBLAS, the high performance library for fast matrix products on CPUs. Optional, but highly recommended for fast learning.

Official instructions and precompiled binaries can be found [here.](https://www.openblas.net/) To build from source, just run the script below (note that this step may take some time):

```
install_OpenBLAS.sh
```

Once this is done, make sure the line ``` HIGH_PERF_LIB = OPENBLAS ``` is uncommented in ``` global_settings.mk ```. Futhermore, if OpenBLAS install directory has been changed, edit here the variable ``` HIGH_PERF_PATH ``` accordingly.


## Compilation

- For compiling all projects in the right order, type:

```
sh cleanAndBuild.sh
```

- For building each project separately, do in each directory:

```
make clean
make
```

- For cleaning and compressing the whole project to an archive placed in the same directory than the project is, type:

```
sh compress.sh
```

Note that either way may result in an error message in case object files
are missing. This is totally benign. It may also be useful to emphasize here
that doing a 'make clean' call can be necessary (before the 'make' one) in case
only a header file has been modified, in a given project.


## Runtime:

Go in the desired project directory, find the executable name, and type
(by replacing 'executable_name' by the correct one):

``` ./executable_name ```


## For a Windows port

Potential incompatibilities:

- Saved .txt files may have missing carriage returns (\r\n vs \n).
- createDirectory()
- getFileSize()
- byte_swap_... functions


## Saved nets

The lib is shipped with pre-trained neural nets for the MNIST database. Note that those nets have been prevented to be indexed by git anymore, using a command such as:

```
git update-index --skip-worktree MNIST_learning/saves/MNIST_learned/*
```

To index new instances, use the same command with ``` --no-skip ```


## Future improvements

- Produce a share library (.so) along the static one (.a).
- Use a more generic backprop function.
- Bring back old 1-input SGD, it may be useful for reinforcement learning.
- Add an early stopping mechanism, at least using quick estimations.
- Support printing the loss function dynamically.
- Support printing a basic confusion matrix. Bonus point: full stats per classes.
- Update saved nets formats: infos.txt as .json, add endianness check and Number size data in binary files. Bonus point for adding checksums, compression even!
- Try the OpenCL lib for GPU computations, try CNN and RBM nets, and reinforcement learning. The sky the limit.
