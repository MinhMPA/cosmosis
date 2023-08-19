- 1/ Load the modules or restore an existing module environment
```
module load [module_name]
```
or
```
module restore cosmosis-gnu
```
For example, when you do
```
module list
```
you should find the following modules loaded:
```
Currently Loaded Modules:

  1) gcc/10.3.0    3) gsl/2.7      5) hdf5/1.10.8     7) lapack/3.10.1

  2) fftw/3.3.10   4) szip/2.1.1   6) impi/2021.5.1   8) python3.10-anaconda/2023.03
```

- 2/ Create or activate ``conda`` environment
```
conda create -n cosmosis-gnu -c conda-forge -y python==3.9 gcc mpi4py gsl numpy lapack fitsio fftw camb astropy cython scikit-learn fast-pt
```
then
```
conda activate cosmosis-gnu
```

- 3/ Check and/or set the following executables and environment variables

```
python
pip
gcc
g++
gfortran
mpicc
mpicxx
mpifort
```
and
```
- `COSMOSIS_SRC_DIR` = <path_to_cosmosis_repo>/cosmosis
- `GSL_INC` = $GSL_INCLUDE
- `GSL_LIB` = $GSL_LIB
- `CFITSIO_INC` = $CFITSIO_INCLUDE
- `CFITSIO_LIB` = $CFITSIO_LIBRARY
- `FFTW_INCLUDE_DIR` = $FFTW_INCLUDE
- `FFTW_LIBRARY` = $FFTW_LIBRARY
- `LAPACK_LINK` = -L/<lapack_lib> -llapack -lblas
- `CXX` = g++
- `CC` = gcc
- `FC` = gfortran
- `MPIFC` = mpifort
- `COSMOSIS_ALT_COMPILERS` = 1
```
4/ Install `cosmosis` and the standard library `cosmosis-standard-library`
 - Cloning the repos if they're not already there
 ```
git clone <cosmosis_repo_address>
cd cosmosis
git clone <cosmosis-standard-library_repo_address>
cd ..
```
- Installing `cosmosis`
```
pip install -e .
```
- Installing `comosis-standard-library`
```
cd cosmosis-standard-library
```
  add this line
```
export COSMOSIS_SRC_DIR=<local_path_to_comosis_dir>/cosmosis
```
  into `Makefile`, `shear/Makefile` and `shear/limber/Makefile`. Then
```
make
```

5/ Run `cosmosis`
- Copy `.ini` files to `cosmosis-standard-library/examples/`
- Make sure to be inside `cosmosis-standard-library`
```
cd cosmosis-standard-library
```
- Execute
```
cosmosis examples/<ini_filename>
```
or, if the sampler supports parallel execution through `mpi`,
```
mpirun -n <nr_mpi_processes> cosmosis examples/<ini_filename>
```