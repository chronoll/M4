= Axon Level ONE ― 新卒エンジニアが書く Axon Framework 入門 ―

== はじめに

私は、新卒1年目でAxon FrameworkによるCQRS+Event Sourcingパターンのシステム開発に携わることになりました。
そもそも「Axonってなんだ？読み方なに？」というレベルからのプロジェクト参加でした。
しかし、Axon Frameworkに関する入門書的な情報は少なく、CQRS+Event Sourcingパターンの概念について理解するのに大変苦労しました。

CQRS / Event Sourcingパターンは近年注目を集めているアーキテクチャパターンであり、
今後はAxon Frameworkに挑戦する人も増えてくるかもしれません。
これからAxon Frameworkを勉強する人・初学者、CQRS+Event Sourcingパターンに興味がある人に向けて、少しでも一歩を踏み出しやすいように、本書を書きました。

== Axon Framework とは何か

=== Axon Framework の概要

=== Axon は何を解決しようとしているのか

=== Axon が向いているシステム

=== Axon が向いていないケース

=== Axon を学ぶ前に知っておいてほしいこと

== なぜ CQRS / Event Sourcing なのか

=== 従来の CRUD モデルの考え方

=== 状態管理の難しさ

=== 「事実（Event）」という考え方

=== 書き込みと読み取りを分ける理由（CQRS）

=== CQRS / Event Sourcing のメリットと注意点

=== なぜ Axon Framework が登場するのか

== Axon のコア概念

=== Command と Event

Axon Framework では、システム内でやり取りされる情報を「Command」 と 「Event」 に明確に分けて考えます。

Commandとは、システムに対して「〜して」という@<strong>{命令}を表します。
例えば、「ユーザを登録して」や「商品を注文して」といったものです。

一方Eventは、「〜した」というシステムがおこなた@<strong>{事実}を表します。
例えば、「ユーザの登録をした」や「商品の注文を完了した」といったものです。

Axonにおいて、Commandは失敗する可能性があるが、Eventは事実に基づいてただ1つしかあり得ない（失敗しない）ということが重要です。
下図のように、同じCommand「xx商品の注文して」が2度送られた時に、1回目のCommandからEvent「xx商品を注文した」が作られた場合、
2回目のCommandはすでにEvent「xx商品を注文した」が存在するため、2つ目の失敗します。

## ここに図を差し込む

Axon では、「Command を受け取り、問題がなければ Event を発行する」という流れで処理が進みます。

この流れは4章のサンプルコードで実際の動きを見ていきましょう。





=== Aggregate という考え方

Aggregateは、Axon において@<strong>{ドメインの整合性を守るための中心的な存}です。
少し乱暴に言うと、
「この中ではルールを破らせない」という境界のようなものです。

例えば「注文」であれば、
* まだ存在しない注文を更新できない
* すでにキャンセルされた注文を再度キャンセルできない

こうしたルールは、
Controller や Service ではなく、Aggregate が責任を持って守ります。

Axonでは、Commandによる状態変更は必ず Aggregate を通して行われます。

これは、「ドメインの状態を変更してよい場所を一箇所に集約する」ための設計です。

もし状態の更新を、Controller や別のServiceClassなど、
複数の場所で行ってしまうと、
* どこでルールが守られているのか分からない
* 修正時に意図しない不整合が起きやすい

といった問題が発生します。

そのため Axon では、Commandを受け取り、Eventを発行し、
そのEventによって状態を更新するという一連の流れをAggregateの中に閉じ込めます。

なお、本書では扱いませんが、
読み取り専用のデータ（Query Model）は
Aggregateとは別の場所で管理されます。

ここで重要なのは、「ドメインの正しさを守る状態更新」はAggregateでのみ行われるという点です。

この考え方は、Chapter 4 のサンプルコードで
OrderAggregateを通して実際に確認します。


=== Event Sourcing による状態管理

=== Query Model と Projection（概要）

=== Saga とは何か

本書では深掘りしません。

== 最小構成で動かす Axon

=== この章でやること

サンプルコードを通して、Command → Event → 状態更新の流れを体験します。

サンプルコード：@<href>{https://github.com/Koyo526/axon-sample}

=== まずは動かしてみる（クイックスタート）

=== ログで見る Axon の処理の流れ

=== サンプル全体像（登場人物）

=== REST API から Command を送る

=== Command と Event（意図と事実）

=== OrderAggregate の中身

=== なぜこの構成なのか

=== この章で分かったこと

== Appendix

=== 次に読むとよい資料・公式ドキュメント

=== 本書のサンプルコードについて

本サンプルコードは、新卒1年目の視点で書かれています。
そのため、より良い書き方や改善点が存在する可能性があります。
