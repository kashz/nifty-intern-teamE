# Exciting Now
nifty IoT intern 2016

## About
イベントなどで参加者からリアルタイムで心拍数を測定
盛り上がり度としてリアルタイムに可視化

**どこ**
が
**どれだけ**
盛り上がっているかが分かるWebサービス

イベントの参加者→行く場所の参考になる

イベントの主催者→リアルタイムにフィードバックが得られる

## System configuration
心拍センサー

↓  (心拍数, SSID識別のためのID)

@nifty IoT デバイスハブ

↓

@nifty RDBサーバ

↓

@nifty クラウド サーバ
(Node.js)

↓ (Soket.io, Riot.js)

ユーザ
