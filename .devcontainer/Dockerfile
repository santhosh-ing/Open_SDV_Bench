# Development container for Open SDV Bench:
# docs-as-code tools, CAN tooling and Python SDKs.
# Firmware flashing (ST-LINK over USB) runs on the host, not in this container.
FROM mcr.microsoft.com/devcontainers/python:3.12

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y --no-install-recommends \
        can-utils \
        graphviz \
        default-jre-headless \
        plantuml \
        build-essential \
        cmake \
        gcc-arm-none-eabi \
        asciidoctor \
    && rm -rf /var/lib/apt/lists/*

RUN pip install --no-cache-dir \
        cantools \
        pyyaml \
        wireviz \
        kuksa-client \
        python-can \
        udsoncan \
        can-isotp
