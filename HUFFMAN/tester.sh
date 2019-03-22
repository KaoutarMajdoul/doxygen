#!/bin/bash
chmod +x hufREP.py 

echo "gcc -o huf huf.c -std=c99"
gcc -o huf huf.c -std=c99
echo "gcc -o dehuf dehuf.c  -std=c99"
gcc -o dehuf dehuf.c -std=c99

echo "COMPRESSION : ./huf fichiersource fichierdestination "
echo "COMPRESSION REPERTOIRE: ./hufREP.py repertoire  "


echo "DECOMPRESSION ./dehuf fichiercompressé fichierdecompressé "

