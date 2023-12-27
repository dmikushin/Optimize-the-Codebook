# Codebook optimizer

Find an optimal codebook with K-Means and LBG algorithms.

## Building

```
mkdir build
cd build
cmake ..
make
```

## Testing

* **K-means**: Generate the codebook, compress, decompress and compare the result against the reference:

```
./lossy_compress_kmeans ../universe.csv >codebook.csv
./lossy_compress_compressor ../universe.csv codebook.csv
./lossy_compress_decompressor compressed.bin codebook.csv
head ../universe.csv
head decompressed.txt
```

* **LBG**: Generate the codebook, compress, decompress and compare the result against the reference:
 
```
./lossy_compress_lbg ../universe.csv >codebook.csv
./lossy_compress_compressor ../universe.csv codebook.csv
./lossy_compress_decompressor compressed.bin codebook.csv
head ../universe.csv
head decompressed.txt
```
