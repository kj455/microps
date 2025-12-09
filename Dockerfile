FROM ubuntu:22.04

# 必要なパッケージをインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    iproute2 \
    iputils-ping \
    netcat \
    libpcap-dev \
    net-tools \
    vim \
    git \
    sudo \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリを設定
WORKDIR /workspace

# Bashプロンプトをカスタマイズ
RUN echo 'export PS1="\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\n\$ "' >> /root/.bashrc && \
    echo 'export PROMPT_COMMAND="echo"' >> /root/.bashrc

# デフォルトコマンド
CMD ["/bin/bash"]
