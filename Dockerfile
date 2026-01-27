FROM ubuntu:22.04

RUN apt update && apt install -y \
    build-essential \
    procps \
    manpages \
    strace \
    git \
    vim \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work
