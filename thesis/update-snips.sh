#!/bin/sh
rm -rf snips
mkdir snips
python snipextract.py ../include ../lib
