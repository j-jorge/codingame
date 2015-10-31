#!/bin/sh

for f in music/in/*.txt
do
    KEY=$(basename $f .txt)
    
    echo ">> $f"
    ./a.out < $f
    
    if [ -f clean.data ]
    then
        convert -size $(identify music/in/${KEY}.png | cut -d' ' -f3) \
                -depth 8 \
                gray:clean.data \
                music/out/${KEY}-clean.png
    fi
    
    if [ -f final.data ]
    then
        convert -size $(identify music/in/${KEY}.png | cut -d' ' -f3) \
                -depth 8 \
                gray:final.data \
                music/out/${KEY}-final.png
    fi
done
