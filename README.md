![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)

# ARC (Adaptive Replacement Cache)

The Adaptive Replacement Cache (ARC) is a caching algorithm designed to balance
**recency** (recently accessed items) and **frequency** (frequently accessed items) adaptively
without explicitly storing frequency counters.

# How to install and build

```bash
git clone https://github.com/daniilgriga/ARC.git
cd ARC

mkdir build
cmake -S . -B build
cmake --build build

./build/ideal   # to run ideal cache
./build/arc     # to run  arc  cache
```
