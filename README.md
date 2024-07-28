# ByteShrink: Huffman Coding Compression Tool

## Introduction

ByteShrink is a C++ implementation of the Huffman coding algorithm for file compression and decompression. This tool allows users to compress files to save storage space and decompress them when needed. Huffman coding is a widely used algorithm for lossless data compression, which assigns variable-length codes to characters based on their frequency of occurrence.

## Features

- File compression using Huffman coding
- File decompression
- Command-line interface for easy use
- Efficient encoding and decoding processes
- Support for various file types

## Prerequisites

To compile and run ByteShrink, you'll need:

- C++ compiler with C++11 support (e.g., GCC, Clang)
- CMake (optional, for easier building)
- Basic knowledge of command-line operations

## Installation

1. Clone the repository: git clone https://github.com/your-username/ByteShrink-Huffman-Coding-Tool.git
cd ByteShrink-Huffman-Coding-Tool

2. Compile the source code: g++ -std=c++11 ByteShrink.cpp -o ByteShrink

## Usage

ByteShrink can be used to compress or decompress files using the following command-line syntax:

Where:
- `[mode]` is either `compress` or `decompress`
- `[input_file]` is the path to the file you want to process
- `[output_file]` is the path where the processed file will be saved

### Examples

1. To compress a file: ./ByteShrink compress myfile.txt myfile.compressed
2. To decompress a file: ./ByteShrink decompress myfile.compressed myfile_decompressed.txt

## How It Works

ByteShrink uses the Huffman coding algorithm to compress files:

1. It reads the input file and builds a frequency map of characters.
2. A priority queue is used to construct the Huffman tree.
3. The Huffman tree is traversed to generate unique binary codes for each character.
4. The original file is encoded using these binary codes.
5. The encoded data, along with the frequency map, is written to the output file.

For decompression:

1. The tool reads the frequency map and reconstructs the Huffman tree.
2. It then uses the tree to decode the compressed data back to its original form.

## Performance

The compression ratio achieved by ByteShrink depends on the content of the input file. Files with more repetitive content tend to achieve better compression ratios.

## Limitations

- The current implementation loads the entire file into memory, which may not be suitable for extremely large files.
- The compression efficiency may vary depending on the input file's characteristics.

## Acknowledgments

- This project was inspired by the Huffman coding algorithm, developed by David A. Huffman.
