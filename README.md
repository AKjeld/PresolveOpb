# RoundingSatWithPresolver
PaPILO integration with pseudo boolean problems of filetype `.opb`. File format is described here: http://www.cril.univ-artois.fr/PB16/format.pdf
## Dependencies
`papilo` library. Update paths in `CMakeFiles.txt` before building. `papilo` also requires TBB, refer to https://github.com/lgottwald/PaPILO for more information.<br>
Standard location for papilo is `<project_root>/papilo`. If your papilo is located in another folder, change location in `CMakeLists.txt`
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
