#! /bin/sh

rm -rf slides
mkdir -p slides
convert -density 400 lp20.pdf slides/lp.png

cat > slides.lst <<EOF
file slides/lp-0.png
duration 14s

file slides/lp-1.png
duration 14s

file slides/lp-2.png
duration 12s

file slides/lp-3.png
duration 7s

file slides/lp-4.png
duration 9s

file slides/lp-5.png
duration 19s

file slides/lp-6.png
duration 10s

file slides/lp-7.png
duration 18s

file slides/lp-8.png
duration 11s

file slides/lp-9.png
duration 9s

file slides/lp-10.png
duration 5s

file slides/lp-11.png
duration 8s

file slides/lp-12.png
duration 10s

file slides/lp-13.png
duration 4s

file slides/lp-14.png
duration 5s

file slides/lp-15.png
duration 4s

file slides/lp-16.png
duration 4s

file slides/lp-17.png
duration 9s

file slides/lp-18.png
duration 7s

file slides/lp-19.png
duration 8s

file slides/lp-20.png
duration 10s

file slides/lp-21.png
duration 9s

file slides/lp-22.png
duration 13s

file slides/lp-23.png
duration 11s

file slides/lp-24.png
duration 11s

file slides/lp-25.png
duration 22s

file slides/lp-26.png
duration 11s

file slides/lp-27.png
duration 17s
EOF

echo ffmpeg -y \
       -f concat \
       -vsync cfr \
       -i slides.lst \
       -i lp20.ogg \
       -c:a libmp3lame \
       -c:v libx264 -pix_fmt yuv420p \
       -vf pad='width=ceil(iw/2)*2:height=ceil(ih/2)*2' \
       -threads 4 \
       -r 15 \
       -metadata title="GNU Mes -- Bootstrapping GNU" \
       -metadata year="2020" \
       -metadata author="janneke@gnu.org" \
       -metadata comment="LibrePlanet 2020 Lightning talk" \
       lp20.avi

echo ffmpeg -y \
       -vsync vfr \
       -f concat \
       -i slides.lst \
       -i lp20.ogg \
       -c:a libvorbis -b:a 112K \
       -c:v libtheora -crf 31 -b:v 0 -pix_fmt yuv420p \
       -vf pad='width=ceil(iw/2)*2:height=ceil(ih/2)*2' \
       -threads 4 \
       -r 15 \
       -metadata title="GNU Mes -- Bootstrapping GNU" \
       -metadata year="2020" \
       -metadata author="janneke@gnu.org" \
       -metadata comment="LibrePlanet 2020 Lightning talk" \
       lp20.ogv

ffmpeg -y \
       -vsync vfr \
       -f concat \
       -i slides.lst \
       -i lp20.ogg \
       -c:a libopus -b:a 112K \
       -c:v libvpx-vp9 -crf 31 -b:v 0 -pix_fmt yuv420p \
       -vf pad='width=ceil(iw/2)*2:height=ceil(ih/2)*2' \
       -threads 4 \
       -r 15 \
       -metadata title="GNU Mes -- Bootstrapping GNU" \
       -metadata year="2020" \
       -metadata author="janneke@gnu.org" \
       -metadata comment="LibrePlanet 2020 Lightning talk" \
       lp20.webm
