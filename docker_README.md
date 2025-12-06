# Docker での開発環境セットアップ

## 環境の起動

### 1. Dockerコンテナのビルドと起動

```bash
docker-compose up -d
```

### 2. コンテナに入る

```bash
docker-compose exec microps /bin/bash
```

### 3. ビルド

コンテナ内で以下を実行:

```bash
make
```

### 4. テスト実行

コンテナ内で以下を実行:

```bash
./test/test.exe
```
