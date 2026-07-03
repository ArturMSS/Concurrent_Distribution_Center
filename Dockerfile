FROM ubuntu:24.04

# Raylib requer X11/OpenGL. O raylib NAO existe como pacote apt no Ubuntu,
# entao instalamos as dependencias e compilamos o raylib do fonte mais abaixo.
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        git \
        cmake \
        ca-certificates \
        libx11-dev \
        libxrandr-dev \
        libxi-dev \
        libxcursor-dev \
        libxinerama-dev \
        libgl1-mesa-dev && \
    rm -rf /var/lib/apt/lists/*

# Compila e instala o raylib (5.5) do codigo-fonte em /usr/local
# (headers em /usr/local/include, libs em /usr/local/lib — caminhos padrao do gcc)
RUN git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git /tmp/raylib && \
    cd /tmp/raylib && \
    cmake -B build -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --target install && \
    ldconfig && \
    rm -rf /tmp/raylib

WORKDIR /trabalho

COPY . .

RUN make clean && make

# NOTA: a interface grafica (Raylib) precisa de um servidor X11 ou Wayland.
# Para executar com display:
#   podman run --rm -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix idp_sim 1
ENTRYPOINT ["./idp_sim"]
CMD ["1"]
