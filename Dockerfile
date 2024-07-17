FROM ubuntu
COPY . /app
WORKDIR /app
RUN apt update && apt upgrade -y && \
    apt install make build-essential cmake g++ libpng-dev libjpeg-dev qtmultimedia5-dev qtcreator -y && \
    mkdir -p dist && \
    cd tools/mdif_viewer_linux && ./build.sh && cd ../.. && \
    cd examples/generate_gradient && ./build.sh && cd ../..
CMD ./dist/generate_gradient && ./tools/mdif_viewer_linux/build/mdif_viewer image.mdif
