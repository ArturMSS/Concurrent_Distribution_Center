FROM ubuntu:24.04

# Raylib requer X11/OpenGL; instale as dependencias e o proprio libraylib-dev
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        libraylib-dev \
        libx11-dev \
        libxrandr-dev \
        libxi-dev \
        libxcursor-dev \
        libxinerama-dev \
        libgl1-mesa-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /trabalho

COPY . .

RUN make clean && make

# NOTA: a interface grafica (Raylib) precisa de um servidor X11 ou Wayland.
# Para executar com display:
#   podman run --rm -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix idp_sim 1
ENTRYPOINT ["./idp_sim"]
CMD ["1"]
