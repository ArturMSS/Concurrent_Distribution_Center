FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        libncurses-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /trabalho

COPY . .

RUN make clean && make

ENTRYPOINT ["./idp_sim"]
CMD ["1"]
