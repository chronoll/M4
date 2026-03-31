= Axon Level ONE ― 新卒エンジニアが書く Axon Framework 入門 ―

== はじめに

私は、新卒1年目で Axon Framework による CQRS / Event Sourcing パターンのシステム開発に携わることになりました。
そもそも「Axon ってなんだ？読み方なに？」というレベルからのプロジェクト参加でした。
しかし、Axon Framework に関する入門書的な情報は少なく、CQRS / Event Sourcing パターンの概念について理解するのに大変苦労しました。

CQRS / Event Sourcing パターンは近年注目を集めているアーキテクチャパターンであり、
今後は Axon Framework に挑戦する人も増えてくるかもしれません。
これから Axon Framework を勉強する人・初学者、CQRS / Event Sourcing パターンに興味がある人に向けて、少しでも一歩を踏み出しやすいように、本書を書きました。

本書は、Axon Framework のすべてを網羅するものではありません。あくまで「最初の一歩を踏み出すための本」です。

//pagebreak

== Axon Framework とは何か

このセクションでは、Axon Framework がどのようなフレームワークで、何を解決しようとしているのかを整理します。
まずは、

 * Axon Framework は何者なのか
 * どんなシステムに向いているのか
 * 逆に、どんな場合には向いていないのか

といった全体像を整理していきます。

=== Axon Framework の概要

公式ドキュメントでは、Axon Framework を次のように説明しています。

//quote{
Axon Framework takes a messaging-centric approach to building applications.
It helps you build applications around Commands, Events, and Queries —
enabling you to build systems based on CQRS and Event Sourcing.
//}

Axon Framework とは、
@<strong>{CQRS（Command Query Responsibility Segregation）}と
@<strong>{Event Sourcing} というアーキテクチャパターンを、
Java / Spring Boot 上で実装しやすくするためのフレームワークです。

Axon Framework では、

 * Command と Event という2種類のメッセージの区別
 * Event を中心とした状態管理
 * Aggregate を整合性境界とした不変条件の保護

といった CQRS / Event Sourcing の基本構造を、フレームワークの仕組みとして提供します。

ここで、「CQRS とはなんだ？」「Event Sourcing ってなんかの祭り？」と感じた人もいるかもしれません。
安心してください。次のセクションで CQRS / Event Sourcing について噛み砕いて解説していきます。

=== Axon が向いているケース

Axon Framework は、CQRS / Event Sourcing というアーキテクチャパターンを理解しなくてはならず、一定の学習コストが必要です。
しかし、特定の条件が揃ったシステムにおいては非常に強力なフレームワークになります。
まず、Axon が向いているのは、ドメイン駆動設計（DDD）を採用しているシステムです。
例えば、

 * 状態によって実行できる操作が変わる
 * 同じ操作でも、状況によって結果が異なる
 * 「なぜその処理が行われたのか」が重要になる

といったケースでは、単純な CRUD モデルではビジネスルールを表現しきれなくなります。
これらはすべて、@<strong>{「ドメインのルールを中心に考える」}、DDD の文脈でよく現れる特徴です。

なお、DDD には Aggregate のような「戦術的パターン」のほかに、@<strong>{境界づけられたコンテキスト（Bounded Context）}や@<strong>{ユビキタス言語（Ubiquitous Language）}といった「戦略的パターン」も存在します。本書では戦術的パターンに絞って解説しますが、DDD の全体像を理解する際にはこれらも重要な概念です。

Axon Framework は、このような DDD 的な設計を前提としており、

 * Command によって「意図」を受け取り
 * Event として「事実」を記録し
 * Aggregate がドメインの不変条件を守る

という役割分担を、フレームワークの仕組みとして支えます。
そのため、ビジネスルールを中心に設計したいシステムでは、Axon Framework が特に力を発揮します。

また、状態遷移が重要なドメインも、Axon が力を発揮する分野です。
特に Axon の特徴である Event Sourcing の強みが活きます。
例えば、

 * 注文の作成・キャンセル
 * ワークフローの進行
 * 業務プロセスの段階的な遷移

といった処理では、「今どの状態にあるのか」だけでなく、「どの状態から、どの状態へ進んだのか」が重要になります。
Event Sourcing を採用することで、状態の変化そのものを Event として記録できるため、こうした要求に自然に応えることができます。

さらに、@<strong>{お金の流れが関わるシステムや、監査が重要なシステム}においても、Event Sourcing は有効です。
Event Sourcing では、「〜した」という事実（Event）が時系列で記録されるため、
システムの振る舞いを後から追いかけることができます。
この仕組みの詳細は、次のセクション「CQRS / Event Sourcing の考え方」で解説します。

//image[ginko-kouza][お金の流れを記録する]

=== Axon が向いていないケース

まず、単純な CRUD アプリケーションでは、Axon は過剰になりがちです。
例えば、

 * データを登録する
 * 一覧を表示する
 * 更新・削除する

といった処理が中心で、複雑な状態遷移や不変条件がほとんど存在しない場合、従来の MVC + ORM の構成で十分なことが多いでしょう。
このようなケースでは、Event Sourcing を導入しても、得られるメリットよりも設計や実装の複雑さの方が目立ってしまいます。

また、短期間で開発して終了する小規模なシステムにも、Axon は向いていません。
Axon Framework を使うためには、

 * CQRS
 * Event Sourcing
 * Aggregate
 * Saga

といった概念を理解する必要があります。
これらの学習コストを回収する前にシステムの役割が終わってしまう場合、Axon を導入するメリットは小さくなります。
さらに、CQRS / Event Sourcing や DDD にまだ馴染みがないチームにとっては、Axon がブラックボックスのように感じられることもあります。

Axon は多くの処理をフレームワークが肩代わりしてくれますが、「なぜこの構成になるのか」を理解していないと、トラブルシューティングが難しくなる場面もあります。
重要なのは、Axon を「流行っているから使う」のではなく、「解決したい問題に対して適切かどうか」という観点で判断することです。
Axon は強力なフレームワークですが、すべてのシステムにとっての万能な解決策ではありません。

== CQRS / Event Sourcing の考え方

このセクションでは CQRS / Event Sourcing はどういう考え方なのかを整理します。

Axon Framework は CQRS / Event Sourcing を前提としたフレームワークですが、
これらの考え方を知らずに Axon を使おうとすると、

 * なぜこんなに構成が複雑なのか
 * なぜ普通に Entity を更新しないのか
 * なぜ Event をわざわざ作るのか

といった疑問が次々に出てきます。
そこでこのセクションでは、まず「従来の設計が抱えていた問題」から話を始め、そこから CQRS / Event Sourcing がどのように生まれてきたのかを見ていきます。

=== 従来の CRUD モデルの考え方

多くの Web アプリケーションは、
CRUD（Create / Read / Update / Delete）モデルをベースに設計されています。
このモデルでは、システムの中心に「データ」があり、

 * データを作る
 * データを更新する
 * データを読む

といった操作によって、システムが成り立っています。
このアプローチは分かりやすく、小規模なシステムや単純な業務では十分に機能します。

しかし、システムが成長し、ビジネスルールが複雑になってくると、CRUD モデルは少しずつ歪みを見せ始めます。
例えば、

 * 注文を確定する
 * 注文をキャンセルする
 * 注文を発送済みにする

といった操作は、最終的には「注文の状態を更新する（Update）」として表現されます。
その結果、何が起きたのか、なぜ今の状態になったのかといった情報が、「現在の状態」の裏に隠れてしまいます。

もちろん、DDD を取り入れて、ドメインサービスや Aggregate にビジネスルールを集約することで、設計は大きく改善できます。

しかしそれでも、

 * 状態の変化そのものに焦点を当てて扱う
 * 書き込みと読み取りを構造的に分ける

といった要求には、CRUD モデルだけでは応えにくい場面が出てきます。
ここで登場するのが、CQRS / Event Sourcing という考え方です。

=== 状態管理の難しさ

CRUD モデルでは、システムの状態は「現在の値」として管理されます。
例えば、注文の状態がSHIPPEDであれば、「今は発送済みである」ということは分かります。

このように、現在の状態を正として保存する設計は、@<strong>{State Sourcing}と呼ばれます。
State Sourcing では、

 * データベースには最新の状態だけが保存される
 * 過去の状態や経緯は基本的に保持しない

という前提でシステムが設計されます。
この考え方はシンプルですが、

 * なぜその状態になったのか
 * どの操作がきっかけだったのか

といった@<strong>{状態に至るまでの経緯}は、システムの中心には残りません。
必要になった場合は、ログや履歴テーブルなどで後から補うことになります。

=== 事実（Event）という考え方

Event Sourcing では、システムの中心にあるのは「状態」ではなく、@<strong>{「何が起きたか」という事実（Event）}です。

例えば、

 * 注文が作成された
 * 注文がキャンセルされた
 * 注文が発送された

といった出来事を、Event として時系列に記録します。

現在の状態は、これらの Event を@<strong>{順番に適用した結果}として導き出されます。

この考え方の重要な点は、@<strong>{「状態は保存しなくてもよい」}ということです。
仮に、ある時点の状態が失われたとしても、過去に発行された Event をすべて集め、最初から順番に適用し直すことで、
同じ状態を再構築することができます。

つまり、Event Sourcing では、@<strong>{Event こそが唯一の正しい情報源}であり、任意の時点での状態を、Event をもとに復元できるということです。

Event Sourcing は、

 * 状態を上書きして管理するのではなく
 * 事実を積み重ね、その結果として状態を導き出す

という発想への転換です。

@<img>{state-vs-event}は、State Sourcing と Event Sourcing の違いを示したものです。
State Sourcing では現在の値だけが残り、経緯は別途調べる必要があります。
一方 Event Sourcing では、事実の積み重ねそのものがデータであり、現在の状態はそこから導き出されます。

//image[state-vs-event][State Sourcing と Event Sourcing の比較]

この考え方があるからこそ、次に紹介する CQRS や、Axon Framework における Aggregate の設計が、自然な形で成り立つようになります。

//image[ginko-kouza-2][Event に基づいた状態表現]

=== なぜ CQRS を使うのか

ここまで、Event Sourcing の考え方を見てきました。
システムの中心にあるのは「状態」ではなく、「何が起きたか」という事実（Event）でした。

ここまで読んで、次のような疑問を持った人もいるかもしれません。

「Event がすべて残っているのは分かったけど、
それって毎回すべての Event を読み込んで、
状態を復元する必要があるのでは？
結構大変じゃない？」

この疑問は、とても自然なものです。

実際、Event の数が増えてくると、

 * 毎回すべての Event を読み込むのは大変そう
 * 画面表示や検索のたびに状態を計算するのは非効率そう

と感じるのではないでしょうか。

ここで重要なのは、「Event Sourcing が悪い」という話ではない、という点です。
Event Sourcing は、「正しさ」や「履歴の完全性」を重視した設計です。
一方で、画面表示や検索といった用途には、別の工夫が必要になってきます。

そこで、
「不変条件を守る処理」と
「使いやすく表示する処理」
の役割を分けて考える必要が出てきます。

==== 書き込み処理が担う役割

まず、書き込み側の処理が担っている役割を整理してみましょう。

書き込み処理（Command 側）の関心事は、

 * この操作は実行してよいのか
 * ドメインの不変条件に違反していないか

といった「ドメインの不変条件を守ること」です。

Event Sourcing を採用している場合、
書き込み側の流れは次のようになります。

 * Command を受け取る
 * ビジネスルールをチェックする
 * 問題がなければ Event を発行する

ここでは、

 * 一覧表示のしやすさ
 * 検索の速さ

といった要件は、本質ではありません。
あくまで、「ドメインの不変条件が守られるかどうか」を保証することが目的です。

==== 読み取り処理が担う役割

一方で、読み取り側の関心事はまったく異なります。
例えば、

 * 注文一覧を表示したい
 * 条件を指定して検索したい
 * 集計結果を画面に出したい

といった処理です。

これらを毎回、Event をすべて読み込み、
最初から順番に適用して状態を計算する
という形で実現しようとすると、
システムが大きくなるにつれて現実的ではなくなってきます。

そのため、読み取り側では、
Event をもとに作られた、
表示や検索に最適化されたデータ（Query Model）を使う方が、
はるかに扱いやすくなります。
このように、

 * 不変条件を守るための処理（書き込み）
 * 使いやすく表示するための処理（読み取り）

では、求められる性質が大きく異なります。

この違いを設計として明確に分けて扱おう、という考え方が
CQRS（Command Query Responsibility Segregation）です。

この関係を図で示すと、@<img>{cqrs-overview}のようになります。

//image[cqrs-overview][CQRS の書き込み側と読み取り側の分離]

CQRS と Event Sourcing は、それぞれ独立したパターンですが、非常に相性が良く、組み合わせて使われることが多いです。
Event Sourcing で書き込み側の「履歴の完全性」を担保し、
CQRS で読み取り側の「使いやすさ」を確保するという形で、
互いの弱点を補い合います。

=== CQRS / Event Sourcing のメリットと注意点

ここまで、CQRS / Event Sourcing の考え方を見てきました。
最後に、このアーキテクチャが持つメリットと、
あらかじめ知っておくべき注意点を簡単に整理しておきましょう。

==== メリット

CQRS / Event Sourcing を採用する最大のメリットは、
@<strong>{「ドメインの不変条件の保護」と「履歴の透明性」}を高いレベルで保てる点です。
Event Sourcing によって、

 * 「何が起きたか」がすべて Event として残る
 * 状態の変化を後から正確に追いかけられる
 * 過去のある時点の状態を再現できる

といった性質が得られます。
また、CQRS によって、

 * 書き込み処理は「不変条件の保護」に集中できる
 * 読み取り処理は「使いやすさ」に最適化できる

という役割分担が明確になります。

その結果、ビジネスルールが複雑なシステムや、
状態遷移・履歴が重要なドメインにおいて、
設計の見通しを保ちやすくなります。

==== 注意点

一方で、CQRS / Event Sourcing は万能ではありません。

まず、@<strong>{設計と実装の難易度が高い}という点があります。
Command / Event / Aggregate / Projection といった概念を理解しないまま導入すると、
システム全体が分かりにくくなってしまいます。
また、1章で述べたとおり、シンプルな CRUD アプリケーションや小規模なシステムでは、
得られるメリットに対してコストが大きくなりがちです。

さらに、Event Sourcing を採用すると、
「今の状態を1行で見る」ことが難しくなる場面もあります。
状態は Event の積み重ねから導かれるため、
慣れないうちはデバッグや調査に戸惑うこともあるでしょう。

重要なのは、
CQRS / Event Sourcing を「流行っているから使う」のではなく、
@<strong>{解決したい問題に対して本当に必要かどうか}を見極めることです。

この考え方を前提として、
次のセクションでは、Axon Framework がこれらの概念を
Command、Event、Aggregate といった仕組みとして
どのようにコードで表現しているのかを見ていきます。

== Axon Framework のコア概念

このセクションでは、Axon で重要となる概念について解説します。
本書では、Command / Event / Aggregate の3つを中心に扱います。
Query Model（Projection）や Saga については、概要のみ紹介します。

=== Command と Event

Axon Framework では、システム内でやり取りされる情報を「Command」 と 「Event」 に明確に分けて考えます。

Command とは、システムに対して「〜して」という@<strong>{命令}を表します。
例えば、「ユーザを登録して」や「商品を注文して」といったものです。

一方 Event は、「〜が起きた」というドメインにおいて発生した@<strong>{事実}を表します。
例えば、「ユーザが登録された」や「商品の注文が確定された」といったものです。

Axon において、Command は失敗する可能性がありますが、Event は既に起きた事実を表すため、取り消しや失敗という概念がないということが重要です。
例えば、Command「注文をキャンセルして」が送られた場合を考えます。1回目の Command により Event「注文がキャンセルされた」が記録されると、Aggregate の状態は「キャンセル済み」になります。
その後、同じ注文に対して再度「注文をキャンセルして」という Command が送られても、Aggregate は「すでにキャンセル済みの注文は再度キャンセルできない」という不変条件に基づき、この Command を拒否します。

#@# ここに図を差し込む

Axon では、「Command を受け取り、問題がなければ Event を発行する」という流れで処理が進みます。

この流れは、4章のサンプルコードで実際に確認していきます。

=== Aggregate という考え方

Aggregate は、Axon において@<strong>{ドメインの不変条件（invariants）を守るための整合性境界}です。
少し乱暴に言うと、
「この境界の中では、ドメインのルールを絶対に破らせない」という仕組みです。

例えば「注文」であれば、

 * まだ存在しない注文を更新できない
 * すでにキャンセルされた注文を再度キャンセルできない

こうしたルールは、
Controller や Service ではなく、Aggregate が責任を持って守ります。

Axon では、Command による状態変更は必ず Aggregate を通して行われます。

これは、「ドメインの状態を変更してよい場所を一箇所にまとめる」ための設計です。

もし状態の更新を、Controller や別の Service クラスなど、
複数の場所で行ってしまうと、

 * どこでルールが守られているのか分からない
 * 修正時に意図しない不整合が起きやすい

といった問題が発生します。

そのため Axon では、Command を受け取り、Event を発行し、
その Event によって状態を更新するという一連の流れを Aggregate の中に閉じ込めます。

なお、本書では扱いませんが、
読み取り専用のデータ（Query Model）は
Aggregate とは別の場所で管理されます。

ここで重要なのは、「ドメインの不変条件を守りながらの状態更新」は Aggregate でのみ行われるという点です。

この考え方は、4章のサンプルコードで
OrderAggregate を通して実際に確認します。

=== Event Sourcing による状態管理

Axon Framework では、Aggregate の状態は Event をもとに管理されます。

Command を受け取った Aggregate は、その場で状態を直接変更するのではなく、まず Event を発行します。

その後、その Event を適用することで Aggregate の状態が更新されます。

このとき使用されるのが @<code>{@EventSourcingHandler} という仕組みです。

Axon では、Aggregate のフィールドは @<code>{@EventSourcingHandler} の中でのみ変更されるというルールがあります。

これにより、

 * 状態変更の履歴が Event として明確に残る
 * 同じ Event を適用すれば、同じ状態が再現できる

という性質が得られます。

ここで重要なのは、
@<strong>{「なぜ Event Sourcing が良いのか」}ではなく、
@<strong>{「Axon ではこうやって状態を扱う」}
という点です。

この仕組みは、4章のサンプルコードで実際に確認します。

==== 補足：Event Bus と Aggregate の復元について

ここまでの説明では、
「Command を受け取り、Event を発行し、状態が更新される」
という流れを見てきました。

実際には、Axon Framework の内部では
もう少し処理が行われています。

Axon では、Event は Event Bus と呼ばれる仕組みを通して扱われます。
Aggregate が発行した Event は Event Bus に流れ、
必要に応じて他のコンポーネントにも届けられます。

また、Command を処理する際、
Axon はいきなり Aggregate の CommandHandler を呼び出すわけではありません。

まず、過去に発行された Event をもとに、
Aggregate の状態を復元します。

この復元処理は、
@<code>{@EventSourcingHandler} を通じて行われます。

その後、復元された「最新の状態」をもとに
Command が処理され、新しい Event が発行されます。

この一連の流れにより、
常に正しい状態を前提として Command が処理される
ようになっています。

ここでは詳しい内部実装には踏み込みませんが、
「Axon は Event を基準に状態を組み立て直してから Command を処理する」
という点だけ覚えておけば十分です。

=== Query Model と Projection

ここまでの節では、
Command を受け取り、Event を発行し、
Aggregate の状態を管理する流れを見てきました。

ここで一つ、重要な前提があります。Aggregate は@<strong>{「不変条件」}を守るための存在であり、
画面表示や検索のための存在ではありません。

例えば、

 * 注文一覧を表示したい
 * 特定の条件で検索したい
 * 集計結果を表示したい

といった処理を、すべて Aggregate から行おうとすると、設計が複雑になりやすくなります。
そこで CQRS では、書き込み（Command）と読み取り（Query）を別のモデルとして考えます。

Axon では、
読み取り専用のモデルを Projection と呼ぶことが多いです。
Projection は、Event をもとに作られた「参照用のデータ」であり、ドメインの不変条件を守る責任は持ちません。
Query Model や Projection の実装については、本書の範囲を超えるため概要にとどめます。

ただし、@<strong>{「Aggregate は書き込みのための存在であり、読み取りは別のモデルで行う」}
という考え方だけは、ここで押さえておいてください。

この前提を知っておくことで、
4章のサンプルコードを
より正しく理解できるようになります。

=== Saga とは何か

ここまでの節では、1つの Aggregate の中で Command を受け取り、Event を発行し、状態を管理する流れを見てきました。
しかし、実際のシステムでは、1つの処理が 複数の Aggregate にまたがるケースが少なくありません。

例えば、

 * 注文を作成する
 * 支払いを行う
 * 配送を開始する

といった処理は、それぞれ別の Aggregate が責任を持つことが多いでしょう。
このような場合、単一の Aggregate だけでは処理全体の流れを管理することができません。

そこで登場するのが Saga です。
Saga は、複数の Aggregate にまたがる一連の処理を調整する仕組みです。
Saga は Event をきっかけに動き、必要に応じて次の Command を発行することで、一連の処理をつないでいきます。
また、途中で失敗が発生した場合に、それまでの処理を取り消す@<strong>{補償処理（compensating action）}を行う役割も担います。

重要なのは、Saga 自身がドメインの状態を直接変更するわけではない、という点です。
Saga はあくまで@<strong>{「どの順番で、どの処理を進めるか」}を管理する役割を担います。

なお、DDD の文献によっては、このような調整役を「プロセスマネージャー（Process Manager）」と呼ぶこともあります。Axon Framework ではこれらを包括して Saga として扱っています。

Axon Framework には、このような Saga を実装するための仕組みが用意されています。
4章のサンプルコードでは、Saga は登場しません。
まずは、
「単一の Aggregate で完結する世界」と
「複数の Aggregate をまたぐ世界」があることを
知っておくだけで十分です。

ここまでで、Axon Framework のコア概念を整理しました。
次の章では、このうち Command / Event / Aggregate に絞って、
最小構成のサンプルコードを実際に動かしていきます。

== 最小構成で動かす Axon Framework

この章では、Axon Framework を使った最小構成のサンプルコードを実際に動かしながら、

 * Command → Event → 状態更新
 * 書き込み（更新系）と読み取り（取得系）の分離

を「体験」として確認していきます。
本章のゴールは、Axon の仕組みを完全に理解することではありません。
それよりもまず、

 * 「Command を送ると、Event が発行される」
 * 「状態は Event を通して更新される」
 * 「更新と取得は別の入口になっている」

という流れを、手を動かしながら感じてもらうことを目的としています。
この章で使うサンプルコードは、次のリポジトリに用意しています。

サンプルコード：
@<href>{https://github.com/Koyo526/axon-sample}

サンプルでは、

 * @<code>{POST /order} … 注文を作成する（更新系 / Command）
 * @<code>{GET /orders} … 注文一覧を取得する（取得系 / Query）

という 2 つの API を用意しています。
まずは難しいことを考えず、

 1. アプリケーションを起動する
 2. API を叩いてみる
 3. 結果がどう変わるかを見る

という流れで進めていきましょう。
「なぜこうなるのか？」という疑問は、
実際に動かしたあとで、ログやコードを見ながら整理していきます。

=== クイックスタート：まずは動かしてみる

ここでは、サンプルコードを実際に起動し、
API を叩いて挙動を確認します。

この時点では、
「なぜそうなるのか」は気にしなくて構いません。
まずは@<strong>{動くこと}と@<strong>{変化すること}を体験しましょう。

==== アプリケーションの起動

リポジトリを clone し、プロジェクトのルートで
次のコマンドを実行します。

//cmd{
./gradlew bootRun
//}

アプリケーションが起動し、
@<code>{Tomcat started on port(s): 8080} と表示されれば準備完了です。

==== 初期状態を確認する（GET /orders）

まずは、注文一覧を取得してみます。

//cmd{
curl http://localhost:8080/orders
//}

起動直後は、まだ注文を作成していないため、
空の配列が返ってきます。

//emlist[][json]{
[]
//}

==== 注文を作成する（POST /order）

次に、注文を 1 件作成してみましょう。

//cmd{
curl -X POST http://localhost:8080/order \
  -H "Content-Type: application/json" \
  -d '{
    "productName": "Coffee"
  }'
//}

成功すると、次のようなレスポンスが返ってきます。

//emlist[][json]{
{
  "orderId": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
  "status": "CREATED"
}
//}

@<code>{orderId} は、サーバー側で自動的に生成された ID です。

==== 再度一覧を取得する（GET /orders）

もう一度、注文一覧を取得してみます。

//cmd{
curl http://localhost:8080/orders
//}

今度は、先ほど作成した注文が含まれているはずです。

//emlist[][json]{
[
  {
    "orderId": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
    "productName": "Coffee",
    "status": "CREATED"
  }
]
//}

==== ここまでで起きたこと

ここまでの操作で、次のことが確認できました。

 * @<code>{POST /order} を呼び出すと、注文が作成される
 * @<code>{GET /orders} の結果が、呼び出し前と後で変化する
 * 更新系（POST）と取得系（GET）が、別の API になっている

この時点では、
内部で何が起きているかを理解できていなくても問題ありません。

次の節では、
今の操作の裏側で Axon Framework が
どのように処理を進めていたのかを、
ログを使って確認していきます。

=== POST と GET は何が違ったのか

2章で説明した「書き込みと読み取りを分ける」という CQRS の考え方が、
実際の API にどう現れているかを確認してみましょう。

前の節では、実際に API を叩いて、

 * @<code>{POST /order} を呼び出すと注文が作成される
 * @<code>{GET /orders} の結果が変化する

という体験をしました。

ここで、少し立ち止まって考えてみましょう。

==== 同じ「Order」でも、やっていることは別物

今回のサンプルでは、

 * @<code>{POST /order}
 * @<code>{GET /orders}

という 2 つの API が登場しました。

どちらも「注文（Order）」を扱っていますが、
実際にやっていることはまったく異なります。

@<code>{POST /order} は、

 * 注文を作成する
 * システムの状態を変える

ための API です。

一方で @<code>{GET /orders} は、

 * すでに存在する注文を取得する
 * システムの状態を変えない

ための API です。

つまり、

 * @<strong>{POST は「状態を変える操作」}
 * @<strong>{GET は「状態を見る操作」}

という役割の違いがあります。

==== これが CQRS の基本的な考え方

CQRS（Command Query Responsibility Segregation）では、
この 2 つを明確に分けて考えます。

 * 状態を変える操作 → Command（更新系）
 * 状態を取得する操作 → Query（取得系）

今回のサンプルでは、

 * @<code>{POST /order} が Command 側の入口
 * @<code>{GET /orders} が Query 側の入口

になっています。

重要なのは、
「API が分かれていること」そのものではありません。

 * @<strong>{状態を変える責任}
 * @<strong>{状態を分かりやすく返す責任}

を、最初から別のものとして扱っている点です。

==== なぜ分けて考えるのか

この理由については、前章で CQRS / Event Sourcing の考え方として説明しました。

この章ではまず、

 * 「更新する API」と「取得する API」が分かれている
 * それぞれが違う役割を持っている

という事実だけ押さえておけば十分です。

次の節では、
@<code>{POST /order} を呼び出したとき、
Axon Framework の中で実際に何が起きていたのかを、
ログを使って確認していきます。

=== ログで見る Axon の処理の流れ

前の節では、

 * @<code>{POST /order} を呼び出すと注文が作成される
 * @<code>{GET /orders} の結果が変わる

という体験をしました。

ここでは、
その裏側で Axon Framework が
どのような処理を行っていたのかを、
ログを使って確認していきます。

==== POST /order を叩いたときのログを見る

まず、@<code>{POST /order} を呼び出した直後の
アプリケーションログを見てみましょう。

サンプルコードでは、
処理の流れが追いやすいように、
ログに番号を付けています。

（ログの内容は一部簡略化しています）

//pagebreak

//emlist{
[1] Received request to create order
[2] Sending CreateOrderCommand
[3] Handling CreateOrderCommand
[4] Applying OrderCreatedEvent
[5] Order creation completed
//}

この 5 行が、
今回の処理の「一本道」を表しています。

==== [1] Controller がリクエストを受け取る

まず最初に、

//emlist{
[1] Received request to create order
//}

というログが出力されます。

これは、@<code>{POST /order} に対して
Controller が HTTP リクエストを受け取ったことを示しています。

ここではまだ、
Axon Framework 固有の処理は始まっていません。

==== [2] Command が送信される

次に、

//emlist{
[2] Sending CreateOrderCommand
//}

というログが出力されます。

Controller は、
受け取ったリクエストをもとに
@<code>{CreateOrderCommand} を作成し、
Axon の @<code>{CommandGateway} を使って送信します。

ここで初めて、
処理が Axon Framework の世界に入ります。

==== [3] Aggregate が Command を処理する

続いて、

//emlist{
[3] Handling CreateOrderCommand
//}

というログが出力されます。

これは、
OrderAggregate の @<code>{@CommandHandler} が呼び出され、
Command が処理されていることを意味します。

この時点で Aggregate は、

 * この注文を作成してよいか
 * ドメインのルールに違反していないか

といった「不変条件」を判断します。

==== [4] Event が発行され、状態が更新される

Command の処理に問題がなければ、

//emlist{
[4] Applying OrderCreatedEvent
//}

というログが出力されます。

ここでは、

 * @<code>{OrderCreatedEvent} が発行され
 * その Event が適用されることで
 * Aggregate の状態が更新されます

重要なのは、
Aggregate のフィールドが直接変更されているのではなく、
@<strong>{Event を通して状態が更新されている}という点です。

==== [5] 処理が完了し、レスポンスが返る

最後に、

//emlist{
[5] Order creation completed
//}

というログが出力され、
Controller からクライアントへレスポンスが返されます。

これで、
@<code>{POST /order} に対する一連の処理は完了です。

==== GET /orders に反映されている理由

ここまでのログは、
@<code>{POST /order} の処理だけを追っていました。

しかし、
その結果は @<code>{GET /orders} にも反映されていました。

これは、

 * Command によって Event が発行され
 * その Event をもとに
 * 読み取り側のデータが更新されている

ためです。

この章では、
読み取り側の詳細な実装までは扱いません。

ここでは、

 * @<strong>{Event が「書き込み」と「読み取り」の橋渡しをしている}
 * @<strong>{同じ Event が、複数の役割で使われている}

という点だけ覚えておけば十分です。

==== まとめ：ログで見えた Axon の流れ

ログを通して、
Axon Framework の基本的な流れを整理すると、
次のようになります。

 1. Controller がリクエストを受け取る
 2. Command が送信される
 3. Aggregate が Command を処理する
 4. Event が発行・適用され、状態が更新される
 5. その結果が読み取り側にも反映される

この流れを図にまとめると、@<img>{axon-flow}のようになります。

//image[axon-flow][Axon Framework の処理フロー（POST /order）]

次の節では、
この流れを踏まえた上で、
実際のコードを見ながら、
それぞれの役割を確認していきます。

// pagebreak

=== コードはこの順番で読む

ここまでで、

 * API を叩いて動かす
 * ログで処理の流れを追う

という体験をしてきました。

次は、実際のコードを見ていきます。
ただし、ここでいきなりファイルを上から順に読もうとすると、
初学者はほぼ確実に迷子になります。

そこでこの節では、
@<strong>{「この順番で読めば全体像がつかめる」}
という読み方だけを紹介します。

==== Controller（入口）を見る

最初に見るのは、Controller です。

 * @<code>{POST /order}
 * @<code>{GET /orders}

という 2 つの API が、
それぞれどのクラス・メソッドで受け取られているかを確認します。

ここでは、

 * リクエストを受け取っている
 * Command を送っている / データを返している

という事実が分かれば十分です。

//emlist[OrderController.java（抜粋）][java]{
@PostMapping("/order")
public CreateOrderResponse createOrder(
        @RequestBody CreateOrderRequest request) {
    String orderId = UUID.randomUUID().toString();
    commandGateway.sendAndWait(
        CreateOrderCommand.of(
            orderId, request.productName()));
    return CreateOrderResponse.created(orderId);
}

@GetMapping("/orders")
public List<OrderSummary> getOrders() {
    return queryGateway
            .query(
                new GetOrdersQuery(),
                ResponseTypes.multipleInstancesOf(
                    OrderSummary.class))
            .join();
}
//}

ここで注目してほしいのは、@<code>{commandGateway} と @<code>{queryGateway} です。

 * @<code>{CommandGateway} は、Command を Axon Framework に送り届けるための窓口です。Controller は、ドメインの処理を直接呼び出すのではなく、この窓口を通じて Command を送信します。@<code>{sendAndWait} は、Command の処理が完了するまで同期的に待機するメソッドです。CQRS では Command を非同期に処理することも多いですが、本書ではシンプルさを優先して同期的な方法を使用しています。
 * @<code>{QueryGateway} は、読み取り側にデータを問い合わせるための窓口です。Controller は、データベースを直接参照するのではなく、この窓口を通じて Query を送信し、結果を受け取ります。

なお、@<code>{orderId} が Controller 側で生成されている点にも注目してください。CQRS では、Command の送信側が事前に ID を決めるのが一般的なパターンです。これにより、Command の処理結果を待たずに、生成した ID をそのままレスポンスとして返すことができます。

つまり、Controller 自身は「何をするか」を知りません。Command や Query を適切な窓口に渡すだけです。実際の処理は、Axon Framework が裏側で適切なハンドラに届けてくれます。

この時点で中身を深く理解しようとする必要はありません。

==== Command と Event（意図と事実）を見る

次に、Command と Event のクラスを見てみましょう。

 * @<code>{CreateOrderCommand}
 * @<code>{OrderCreatedEvent}

これらは、
今回のサンプルで最も重要な「メッセージ」です。

ここでは、

 * Command は「〜してほしい」という意図
 * Event は「〜した」という事実

を表している、という点だけ押さえてください。

//emlist[CreateOrderCommand.java][java]{
public class CreateOrderCommand {
    @TargetAggregateIdentifier
    private final String orderId;
    private final String productName;
}
//}

//emlist[OrderCreatedEvent.java][java]{
public class OrderCreatedEvent {
    private final String orderId;
    private final String productName;
}
//}

ここで、@<code>{CreateOrderCommand} に付いている @<code>{@TargetAggregateIdentifier} に注目してください。

このアノテーションは、「この Command を、どの Aggregate に届けるか」を Axon に伝えるためのものです。今回の例では、@<code>{orderId} フィールドに付いているため、Axon は「この orderId を持つ OrderAggregate に Command を届ける」と判断します。

一方、@<code>{OrderCreatedEvent} には @<code>{@TargetAggregateIdentifier} が付いていません。Event は「すでに起きた事実」を表すものであり、どこかに届ける必要がないためです。

フィールドの数や実装の細かさよりも、
@<strong>{名前と役割}を見ることが大切です。

==== Aggregate（不変条件を守る場所）を見る

次に、@<code>{OrderAggregate} を見ます。

ここは、
今回のサンプルの中で、
Axon Framework らしさが最も強く現れるクラスです。

このクラスでは、

 * Command を受け取る
 * 問題がなければ Event を発行する
 * Event によって状態を更新する

という流れが、1 つのクラスにまとまっています。

特に、

 * @<code>{@CommandHandler}
 * @<code>{AggregateLifecycle.apply(...)}
 * @<code>{@EventSourcingHandler}

の 3 点が、
「Command → Event → 状態更新」
をつないでいることを意識しながら読むとよいでしょう。

//pagebreak

//emlist[OrderAggregate.java（抜粋）][java]{
@Aggregate
public class OrderAggregate {

    @AggregateIdentifier
    private String orderId;
    private String productName;
    private OrderStatus status;

    protected OrderAggregate() {
        // Axon による Event Sourcing 復元用（引数なしコンストラクタ）
    }

    @CommandHandler
    public OrderAggregate(CreateOrderCommand command) {
        if (command.getProductName() == null
                || command.getProductName().isBlank()) {
            throw new IllegalArgumentException("商品名は必須です");
        }
        AggregateLifecycle.apply(OrderCreatedEvent.of(
                command.getOrderId(), command.getProductName()));
    }

    @EventSourcingHandler
    public void on(OrderCreatedEvent event) {
        this.orderId = event.getOrderId();
        this.productName = event.getProductName();
        this.status = OrderStatus.CREATED;
    }
}
//}

コードに登場したアノテーションと仕組みを、上から順に確認しましょう。

 * @<code>{@Aggregate} … このクラスが Axon の Aggregate であることを宣言します。これにより、Axon Framework がこのクラスを管理対象として認識し、Command のルーティングや Event Sourcing による状態復元を自動的に行います。
 * @<code>{@AggregateIdentifier} … この Aggregate を一意に識別するフィールドを示します。Axon は、この値を使って「どの Aggregate インスタンスに対する操作か」を判別します。先ほどの @<code>{@TargetAggregateIdentifier} と対になる存在です。Command 側の @<code>{@TargetAggregateIdentifier} で指定された値と、Aggregate 側の @<code>{@AggregateIdentifier} の値が一致することで、正しい Aggregate に Command が届きます。
 * @<code>{引数なしコンストラクタ} … Axon が Event Sourcing で Aggregate を復元する際に必要です。Axon はまずこの引数なしコンストラクタで空のインスタンスを作成し、その後、保存済みの Event を @<code>{@EventSourcingHandler} で順番に再生して最新の状態を復元します。@<code>{protected} にしておくことで、外部からの直接利用を防ぎつつ、Axon からのアクセスを許可します。
 * @<code>{@CommandHandler} … このメソッド（またはコンストラクタ）が Command を受け取る場所であることを示します。今回はコンストラクタに付いているため、@<code>{CreateOrderCommand} を受け取って新しい Aggregate を作成する処理になっています。Command の処理に入る前に不変条件のチェック（ここでは商品名の必須チェック）を行い、ルールに違反していれば例外を投げて Event の発行を防ぎます。
 * @<code>{AggregateLifecycle.apply(...)} … Aggregate の中から Event を発行するためのメソッドです。ここで発行された Event は、Axon によって記録され、直後に同じ Aggregate 内の @<code>{@EventSourcingHandler} に届けられます。
 * @<code>{@EventSourcingHandler} … 発行された Event を受け取り、Aggregate の状態（フィールド）を更新するメソッドです。3章で触れたとおり、Aggregate のフィールドは、このメソッドの中でのみ変更するのが Axon のルールです。ここでは @<code>{status} フィールドも @<code>{CREATED} に設定しています。

このように、1つの Aggregate クラスの中に「Command を受け取る」「Event を発行する」「Event で状態を更新する」という3つの役割がまとまっています。

この流れを図にすると、@<img>{aggregate-internal}のようになります。

//image[aggregate-internal][Aggregate 内部の処理フロー]

==== 読み取り側（Query / Projection）を見る

次に、@<code>{GET /orders} に関係するクラスを見ます。

ここでは、

 * 画面表示や一覧取得のために
 * 読み取り専用のデータが使われている

という点だけ理解できれば十分です。

この章では、

 * Projection の詳細な作り方
 * なぜこの形でデータを持っているのか

までは深掘りしません。

「書き込みとは別に、読むための形がある」
という事実を確認することが目的です。

//emlist[OrderProjection.java（抜粋）][java]{
@Component
public class OrderProjection {

    private final List<OrderSummary> orders =
        new CopyOnWriteArrayList<>();

    @EventHandler
    public void on(OrderCreatedEvent event) {
        orders.add(
            OrderSummary.create(
                event.getOrderId(),
                event.getProductName(),
                OrderStatus.CREATED));
    }

    @QueryHandler
    public List<OrderSummary> handle(GetOrdersQuery query) {
        return List.copyOf(orders);
    }
}
//}

ここで注目してほしいアノテーションが 2 つあります。

 * @<code>{@EventHandler} … Event を受け取って処理するメソッドに付けます。ここでは、@<code>{OrderCreatedEvent} を受け取り、読み取り用のデータ（@<code>{OrderSummary}）をリストに追加しています。@<code>{OrderStatus.CREATED} を渡しているのは、このイベントが「注文が作成された」事実を表しているためです。
 * @<code>{@QueryHandler} … Query を受け取って結果を返すメソッドに付けます。ここでは、@<code>{GetOrdersQuery} を受け取り、注文一覧を返しています。先ほど Controller で見た @<code>{queryGateway.query(...)} の呼び出しが、最終的にこのメソッドに届きます。

なお、このサンプルではリストの実装に @<code>{CopyOnWriteArrayList} を使用しています。@<code>{@EventHandler} と @<code>{@QueryHandler} は異なるスレッドから呼ばれる可能性があるため、スレッドセーフなデータ構造を使う必要があります。実際の運用では、データベースを読み取り用ストアとして使用するのが一般的です。

ここで、1つ疑問に思った人もいるかもしれません。Aggregate のコードには @<code>{@EventSourcingHandler} がありましたが、Projection のコードでは @<code>{@EventHandler} になっています。どちらも Event を受け取るメソッドですが、役割が異なります。

 * @<code>{@EventSourcingHandler} は、Aggregate の内部で使われます。Event を適用して Aggregate の@<strong>{状態を復元・更新}するためのものです。
 * @<code>{@EventHandler} は、Aggregate の外側で使われます。Event をきっかけに、読み取り用データの更新や外部通知など、@<strong>{Aggregate の外側で行われる処理}を担うためのものです。

つまり、同じ @<code>{OrderCreatedEvent} でも、Aggregate の中では「自分の状態を更新する」ために使われ、Projection の中では「読み取り用のデータを作る」ために使われています。

==== 設定クラス（最小構成の理由）を見る

最後に、設定クラスを確認します。

サンプルコードでは、

 * Event Store をインメモリで動かす
 * 外部サービス（Axon Server）を使わない

という、最小構成になるように設定されています。

ここでは、

 * 「まずはローカルで確実に動かす」
 * 「構成をシンプルに保つ」

ための設定である、という点だけ分かれば十分です。

//emlist[AxonConfig.java][java]{
@Configuration
public class AxonConfig {
    @Bean
    public InMemoryEventStorageEngine eventStorageEngine() {
        return new InMemoryEventStorageEngine();
    }
}
//}

==== この順番で読む理由

この順番でコードを読むことで、
Axon Framework における

 * Command
 * Event
 * Aggregate
 * Query（読み取り）

の役割が、無理なくつながって見えてくるはずです。

次の節では、
この章で体験した内容を整理し、
Axon Framework の最小構成として
押さえておくべきポイントをまとめます。

=== この章で分かったこと

この章では、Axon Framework の最小構成サンプルを使い、
実際に API を叩き、ログを確認し、コードを読むという流れで
Axon の基本的な動きを体験しました。
ここで分かったことを整理します。

 * Axon Framework では、状態を直接更新するのではなく、@<strong>{Command を受け取り、Event を発行する}という流れで処理が進む
 * Aggregate は、Command を処理し、@<strong>{ドメインの不変条件を守る整合性境界}である
 * 状態の変更は、Event を適用することで行われる
 * 書き込み（Command）と読み取り（Query）は、@<strong>{最初から別の責務として分けて考えられている}
 * 同じ Event が、書き込み処理と読み取り処理の両方をつなぐ役割を果たしている

また、

 * @<code>{POST /order} で状態が変わり
 * @<code>{GET /orders} でその結果を確認できる

という体験を通して、
CQRS / Event Sourcing の考え方が
「理屈」ではなく「動き」として確認できたはずです。

この章で扱ったのは、あくまで@<strong>{最小構成}です。
Projection の作り込みや Saga、Event Store の永続化など、
実運用では考えるべき点はまだ多くあります。
しかし、
Axon Framework を使う上で最も重要な

 * Command → Event → 状態更新
 * 更新と取得の責務分離

という土台は、この章の内容だけでも十分に掴めています。

ここまで理解できていれば、
Axon Framework のコードやドキュメントを読んだときに、
「何をやっているのか分からない」という状態からは
確実に一歩抜け出せているはずです。

== Appendix

本書の Appendix は、1〜4章を読み終えたあとに
「もう少し知りたい」「Axon を使ってみたい」という次のステップに進みたい人に向けた情報になります。

=== 次に読むとよい資料・公式ドキュメント

Axon Framework や CQRS / Event Sourcing について、
さらに理解を深めたい場合は、公式ドキュメントが最も信頼できる情報源です。

 * Axon Framework 公式ドキュメント：@<href>{https://www.axoniq.io/framework}

Axon Framework の基本概念から、
Aggregate / Saga / Projection / Event Store の設定まで、
網羅的に解説されています。
本書で扱った内容も、公式ドキュメントの中で
より詳細に説明されています。

 * Axon Framework GitHub リポジトリ：@<href>{https://github.com/AxonFramework/AxonFramework}

実装レベルで理解を深めたい場合は、
実際のコードを読むのも有効です。
Issue や Pull Request を眺めるだけでも、
Axon Framework がどのような思想で作られているかを感じ取れます。

=== 本書のサンプルコードについて

本書で使用しているサンプルコードは、
新卒 1 年目のエンジニアが、実際に Axon Framework を学びながら書いたものです。
そのため、最小構成を優先して意図的に省略している部分や、
より良い書き方・改善の余地が残っている箇所もあります。

もし、「もっときれいに書きたい」や「他の構成も試してみたい」という時は、
公式ドキュメントや実際のプロジェクト事例を参考にしながら、
ぜひ自分なりに Axon Framework を使ってみてください。

本書とサンプルコードが、
Axon Framework を学ぶ最初のきっかけになれば幸いです。
