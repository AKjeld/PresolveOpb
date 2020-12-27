# RoundingSatWithPresolver
PaPILO integration with pseudo boolean problems of filetype `.opb` described here: http://www.cril.univ-artois.fr/PB16/format.pdf
## Dependencies
`papilo` library. Update paths in `CMakeFiles.txt` before building. `papilo` also requires TBB, refer to https://github.com/lgottwald/PaPILO for more information.

## To build
```
mkdir build 
cd build 
cmake ..
make
```
## Binary usage
After compilation use binary as follows.
```
./PresolveOpb <file in> <file out>
```
