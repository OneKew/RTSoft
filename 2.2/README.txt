1) docker build -t ss2019 

2) docker build -t example .
   xhost +
   docker run -e DISPLAY=unix$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix example
   xhost -
