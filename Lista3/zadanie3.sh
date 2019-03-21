#!/bin/bash

Json=$(curl -s https://api.thecatapi.com/v1/images/search?mime_types=jpg&format=json&order=RANDOM&limit=1)
Json=${Json#'['}
Json=${Json%']'}
chuckNorrisText=$(curl -s http://api.icndb.com/jokes/random | jq '.value' | jq '.joke' )



catUrl=$(echo ${Json} | jq -r  '.url')
wget --quiet -N -O kot.jpg $catUrl


img2txt -x 1 -y 1 kot.jpg
echo $chuckNorrisText
