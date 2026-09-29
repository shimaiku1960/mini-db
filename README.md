# mini-db

C言語で一から作る、Redis風のキーバリュー型データベースです（学習用）。

## 動かし方

```
cc -Wall -o db db.c
./db
```

## 使えるコマンド

```
db> set name taro
OK
db> get name
taro
db> del name
OK
db> get name
(見つかりません)
db> exit
```

## 進め方

1. REPL
2. コマンド分解
3. メモリに保存（set / get / del）
4. ファイルに保存
5. 追記ログ
6. ハッシュ表
7. HTTPサーバーとつなぐ
