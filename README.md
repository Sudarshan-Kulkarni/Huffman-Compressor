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

###  Performance Visualization
Given below are the screenshots showing the raw files and their compressed counterparts.

#### Alice In Wonderland
![Alice In Wonderland](https://github.com/Sudarshan-Kulkarni/Huffman-Compressor/blob/master/Screenshots/Alice%20In%20Wonderland.png?raw=true)

#### Paradise Lost by John Milton
![Paradise Lost by John Milton](https://github.com/Sudarshan-Kulkarni/Huffman-Compressor/blob/master/Screenshots/Paradise%20Lost%20by%20John%20Milton.png?raw=true)

#### Lose Yourself Lyrics
![Lose Yourself Lyrics](https://github.com/Sudarshan-Kulkarni/Huffman-Compressor/blob/master/Screenshots/Lose%20Yourself%20Lyrics.png?raw=true)
