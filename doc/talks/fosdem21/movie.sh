#! /bin/sh

rm -rf slides
mkdir -p slides
convert -density 400 fosdem21.pdf -resize 1280x720 slides/frame.png

cat > slides.lst <<EOF
file slides/frame-0.png
duration 15.8s

file slides/frame-1.png
duration 6.5s

file slides/frame-2.png
duration 36.7s

file slides/frame-3.png
duration 19.7s

file slides/frame-4.png
duration 16s

file slides/frame-5.png
duration 17.9s

file slides/frame-6.png
duration 9.9s

file slides/frame-7.png
duration 9s

file slides/frame-8.png
duration 15.5s

file slides/frame-9.png
duration 21.2s

file slides/frame-10.png
duration 13.8s

file slides/frame-11.png
duration 16.9s

file slides/frame-12.png
duration 10.2s

file slides/frame-13.png
duration 6.6s

file slides/frame-14.png
duration 4.5s

file slides/frame-15.png
duration 5.2s

file slides/frame-16.png
duration 10.2s

file slides/frame-17.png
duration 5.5s

file slides/frame-18.png
duration 3.1s

file slides/frame-19.png
duration 4.1s

file slides/frame-20.png
duration 5.4s

file slides/frame-21.png
duration 5.8s

file slides/frame-22.png
duration 7.5s

file slides/frame-23.png
duration 8.7s

file slides/frame-24.png
duration 8.2s

file slides/frame-25.png
duration 41.4s

file slides/frame-26.png
duration 19s

file slides/frame-27.png
duration 34.7s

file slides/frame-28.png
duration 9.6s

file slides/frame-29.png
duration 5s

file slides/frame-30.png
duration 37.3s

file slides/frame-31.png
duration 21.7s

file slides/frame-32.png
duration 9.5s

file slides/frame-33.png
duration 3.7s

file slides/frame-34.png
duration 43.8s

file slides/frame-35.png
duration 9s

file slides/frame-36.png
duration 7.9s

file slides/frame-37.png
duration 18.6s

file slides/frame-38.png
duration 37.2s

file slides/frame-39.png
duration 19.2s

file slides/frame-40.png
duration 24.3s

file slides/frame-41.png
duration 34.2s

file slides/frame-42.png
duration 8.3s

file slides/frame-43.png
duration 20s

EOF

echo ffmpeg -y \
       -f concat \
       -vsync cfr \
       -i slides.lst \
       -i fosdem21.ogg \
       -c:a libmp3lame \
       -c:v libx264 -pix_fmt yuv420p \
       -vf pad='width=ceil(iw/2)*2:height=ceil(ih/2)*2' \
       -threads 4 \
       -r 15 \
       -metadata title="GNU Mes -- the Full Source Bootstrap" \
       -metadata year="2021" \
       -metadata author="janneke@gnu.org" \
       -metadata comment="FOSDEM'21 Lightning talk" \
       fosdem21.avi

echo ffmpeg -y \
       -vsync vfr \
       -f concat \
       -i slides.lst \
       -i fosdem21.ogg \
       -c:a libvorbis -b:a 112K \
       -c:v libtheora -crf 31 -b:v 0 -pix_fmt yuv420p \
       -vf pad='width=ceil(iw/2)*2:height=ceil(ih/2)*2' \
       -threads 4 \
       -r 15 \
       -metadata title="GNU Mes -- the Full Source Bootstrap" \
       -metadata year="2021" \
       -metadata author="janneke@gnu.org" \
       -metadata comment="FOSDEM'21 Lightning talk" \
       fosdem21.ogv

echo ffmpeg -y \
       -vsync vfr \
       -f concat \
       -i slides.lst \
       -i fosdem21.ogg \
       -c:a libopus -b:a 112K \
       -c:v libvpx-vp9 -crf 31 -b:v 0 -pix_fmt yuv420p \
       -vf pad='width=ceil(iw/2)*2:height=ceil(ih/2)*2' \
       -threads 4 \
       -r 15 \
       -metadata title="GNU Mes -- the Full Source Bootstrap" \
       -metadata year="2021" \
       -metadata author="janneke@gnu.org" \
       -metadata comment="FOSDEM'21 Lightning talk" \
       fosdem21.webm

ffmpeg -y \
       -vsync vfr \
       -f concat \
       -i slides.lst \
       -i fosdem21.ogg \
       -c:a aac -b:a 128k -ar 48000 \
       -c:v libx264 -crf 31 -b:v 0 -pix_fmt yuv420p \
       -vf pad='width=ceil(iw/2)*2:height=ceil(ih/2)*2' \
       -threads 4 \
       -r 25 \
       -metadata title="GNU Mes -- the Full Source Bootstrap" \
       -metadata year="2021" \
       -metadata author="janneke@gnu.org" \
       -metadata comment="FOSDEM'21 Lightning talk" \
       fosdem21.mp4
