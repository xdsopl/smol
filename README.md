# Compression of Short English Text

This tool compresses short English text messages (between a hundred and a thousand letters) for efficient storage and transmission.

## Features

* Hybrid Compression Algorithm: This tool employs a hybrid compression approach combining several techniques for efficient encoding of short English text.
* Block-wise Processing: The input text is processed in blocks of 2^block_power bytes (where block_power can be specified, defaulting to 8 for a block size of 256).
* Burrows-Wheeler Transform (BWT):  A full rotation sorting BWT is applied to each block to improve compressibility by grouping similar characters.
* Move-to-Front (MTF) Encoding: The output of the BWT is then processed using Move-to-Front encoding, which further reduces redundancy by assigning smaller codes to frequently occurring characters.
* Run-Length Encoding (RLE):  Run-length encoding is used to compress sequences of identical characters.
* Adaptive Rice Coding: The MTF values and RLE counts are encoded using an adaptive Rice coder with two contexts: one for the MTF value and one for the count.  This allows the coder to adapt to the statistical properties of the data for better compression.
* Lossless Compression: The compression is lossless, ensuring that the original text can be perfectly reconstructed upon decompression.
* Significant Compression: Achieves significant compression ratios, especially for text with repetitions.
* Fast Encoding and Decoding: Designed for efficient encoding and decoding of short text messages.

## Installation

This tool requires only a C compiler (like GCC or Clang) and `make`.  No other dependencies are needed.

```
make
```

This will compile the `smol` executable.

## Usage

This tool is a command-line utility.

**Encoding:**

```
./smol e [block_power] < input.txt > encoded.dat
```

* block_power: Optional. Specifies the block power for encoding. The block size is calculated as 2^block_power. Must be 8, 9, 10, or 11. If omitted, the default block power is 8 (resulting in a block size of 256).
* input.txt: The input text file to be encoded.
* encoded.dat: The output file containing the compressed data.

**Decoding:**

```
./smol d < encoded.dat > original.txt
```

* encoded.dat: The input file containing the compressed data.
* original.txt: The output file containing the decompressed text.

## Example

**Short Message:**

```
echo -n "This is a short example message to demonstrate the compression tool." | ./smol e | ./smol d

./smol: encoded 68 to 50 bytes -26.47%
./smol: decoded 50 to 68 bytes +36.00%
This is a short example message to demonstrate the compression tool.
```
**Message with Repetitions (rick.txt):**

```
make  # Compiles the code (if not already compiled)
./smol e < rick.txt | ./smol d | diff -q -s - rick.txt

./smol: encoded 177 to 94 bytes -46.89%
Never gonna give you 🆙
Never gonna let you ⬇️
Never gonna 🏃 around and 🏜️ you
Never gonna make you 😭
Never gonna say 👋
Never gonna tell a 🤥 and 🤕 you
./smol: decoded 94 to 177 bytes +88.30%
Files - and rick.txt are identical
```

**Explanation:**

The first example shows the compression achieved on a short, relatively diverse message.  The second example, using `rick.txt` (included in the repository), demonstrates the higher compression ratio achieved on text with more repetitions.  As you can see, the 177-byte `rick.txt` file is compressed to 94 bytes (a 46.89% reduction). The `diff` command confirms that the decompressed file is identical to the original. This highlights the effectiveness of the algorithm for data with repetitive patterns.  The `make` command is included to show how to compile and run the example.


## Limitations

* Currently only supports English text.
* Performance may degrade with messages shorter than 50 characters.

## Dependencies

* A C compiler (e.g., GCC, Clang)
* `make`

## Contributing

Contributions are welcome!  Please open an issue or submit a pull request.

## License

0BSD

## Author

Ahmet Inan

## References
* Run-length encodings  
by Solomon W. Golomb - 1966
* A Block-sorting Lossless Data Compression Algorithm  
by M. Burrows and D.J. Wheeler - 1994
