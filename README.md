# Huffman-Compressor
## Introduction
Huffman code is a particular type of optimal prefix code that is commonly used for lossless data compression. The method of compression involves constructing a frequency table of all characters, and based on this, the characters that occur very frequently in the file will have shorter codes, whereas the opposite is true for the less frequent occurring characters.

This is an implementation in C++ which will compress and decompress any text file. Necessary comments are provided to aid the user should he/she peer into the source code.

## Usage
### Compilation of the code
```
g++ huffman.cpp main.cpp -o app
```

### Compression of a text file

```
./app.exe -c <filepath.txt>
```

### Decompression of a huf file

```
./app.exe -d <filepath.huf>
```
