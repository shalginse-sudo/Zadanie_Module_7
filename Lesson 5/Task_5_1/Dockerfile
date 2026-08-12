FROM ubuntu:16.04

RUN apt-get update \
    && apt-get install -y \
        gcc \
        g++ \
        nano

WORKDIR /app

COPY main.cpp .

RUN g++ main.cpp -o hello

CMD ["./hello"]