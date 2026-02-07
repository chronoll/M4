= User-Agentとは

== リクエストヘッダとしてのUser-Agent

 * HTTP の @<code>{User-Agent} リクエストヘッダーを指す
    * User agent の語義（ソフトウェアとしてのエージェント）とは異なる
 * リクエストを送信したアプリケーション、オペレーティングシステム（OS）、ベンダー、またはバージョンを、サーバーやネットワーク側で識別するための特性文字列です。
 * 主要ブラウザの形式例
    * @<b>{Firefox}: @<code>{Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:124.0) Gecko/20100101 Firefox/124.0}
    * @<b>{Chrome (Blink)}: @<code>{Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36}
    * @<b>{Safari}: @<code>{Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Safari/605.1.15}
 * User-Agentヘッダーの値はあくまでクライアント側からの自己申告なので、実際のユーザーエージェントと異なる値を送信することもできる。（詳細は後述）
    * UA文字列は@<b>{自己申告情報であり、真正性は保証されない}
 * @<href>{https://developer.mozilla.org/ja/docs/Web/HTTP/Reference/Headers/User-Agent, MDN - User-Agent}

== User-Agentの確認方法

 * 「ユーザーエージェント 確認」などで検索するとサイトが出てくる
    * @<href>{https://testpage.jp/tool/ip_user_agent.php}

//image[screenshot1][User-Agent確認サイトのスクリーンショット]{
スクリーンショット 2026-02-07 14.26.06
//}

 * @<code>{navigator.userAgent} を開発者ツールで確認
    * @<href>{https://developer.mozilla.org/ja/docs/Web/API/Navigator/userAgent, MDN - Navigator.userAgent}

//image[screenshot2][開発者ツールでのnavigator.userAgent確認]{
image
//}

== User-Agentの文法

== 他のUser-Agent文字列の例

 * @<href>{https://useragentstring.com/pages/useragentstring.php}
 * @<href>{https://github.com/ua-parser/uap-core, ua-parser/uap-core}
    * User-Agent 文字列という「カオス」を解読するために長年メンテナンスし続けてきた、@<b>{世界で最も標準的な User-Agent 解析用正規表現データベース}です。

== 身近な活用例

身近な例を挙げると、スマホのブラウザにある「PC版サイトを表示」という機能は、ブラウザが送信するUser-Agent文字列をモバイル向けからデスクトップ向けに切り替えることで実現されています。また、Webサービスにアクセスした際に「お使いのブラウザはサポートされていません」と表示されるのも、サーバーやクライアント側でUser-Agentを見て判定しているケースがあります。

= User-Agentの登場背景と変遷

== RFC1945とHTTP標準仕様
 * 最初にUser-Agentが登場したのはRFC1945（最初期の公式HTTP仕様）
    * User-Agent リクエストヘッダーは、@<b>{そのリクエストを発生させたユーザーエージェントに関する情報を含む}ものである。
    * 複数の @<b>{product tokens}（製品名/バージョン）
    * これは以下のような目的で利用されうる：
       * 統計的な目的（どのクライアントがどれだけ使われているかの収集）
       * プロトコル違反の追跡（どのクライアントが RFC から逸脱しているかを判別）
       * 特定のユーザーエージェントの制限を回避するための自動的な認識（サーバが特定のエージェント向けにレスポンスを調整する）
    * @<b>{必須ではないが}、ユーザーエージェントは @<b>{このヘッダーをリクエストに含めるべきである}
    * （@<href>{https://datatracker.ietf.org/doc/html/rfc1945, datatracker.ietf.org}）

== RFC9110
 * RFC9110
    * @<b>{User-Agent ヘッダーは、リクエストを生成した「ユーザーエージェント」ソフトウェアについての情報を含むヘッダー}

このように識別の手段として用いられていた

= User-Agentはどう使われてきたか

== 実際に挙動差を試してみる

curlでヘッダを変更して返されるHTMLの違いを確認する

//emlist{
curl -s -o unknown.html -H "User-Agent: Unknown" https://www.google.com

curl -s -o mobile.html -H "User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 17_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Mobile/15E148 Safari/604.1" https://www.google.com
//}

//image[screenshot3][curlコマンド実行結果]{
スクリーンショット 2026-02-07 16.06.06
//}

//image[screenshot4][User-Agent変更時のスクリーンショット]{
スクリーンショット 2026-02-07 16.05.50
//}

 * 開発者ツールでは、User-Agent等の設定や幅を変えて端末のエミュレートが可能
    * 例えばgoogleの検索ページでは、iPhoneSEのエミュレートと、同じ幅だがUser-Agentを"UnknownDevice"としたときのエミュレートではこのように異なる

//image[screenshot5][iPhoneSEエミュレートのスクリーンショット]{
スクリーンショット 2026-02-07 12.45.37
//}

//image[screenshot6][UnknownDeviceエミュレートのスクリーンショット]{
スクリーンショット 2026-02-07 12.45.49
//}

== 識別する方法

 * User-Agentは文字列のため、その内容の検証になる
 * パースするライブラリも存在する

== サーバーサイドでの使われ方

 * robots.txtによるクローラ制御
 * アクセスログ解析での活用

== User-Agentは容易に変更できる

上記で確認したように、User-Agentはあくまでユーザーから申告するものであり、自由に書き換えが可能

= User-Agentを取り巻く課題

== 文字列の複雑性

初期は単純だった UA 文字列が、互換性維持のために肥大化

@<href>{https://humanwhocodes.com/blog/2010/01/12/history-of-the-user-agent-string, humanwhocodes.com}

=== ブラウザ・フィンガープリント（個人特定）

詳細すぎる情報は、ユーザーをサイト間で追跡するための「指紋」として悪用されます。

=== 端末の識別が困難

 * 文字列が複雑すぎて、開発者が解析を誤り、特定のブラウザを不当にブロックしてしまう
    * iPad問題

== 容易に偽装が可能

ログの信用性の低下

 * アクセス過多のログを見ても、例えばBotなどの文字列が入った明らかに違うUser-Agentならそのせいだと理解できるが、意図的に普通のブラウザを装った例が多いため原因の特定がより困難に

= User-Agentに対する新たなアプローチ

== 削減の流れ

前章「文字列の複雑性」による問題の解消を目的として削減の方向に

削減が行われたブラウザでは、UA 文字列の特定のパーツが「固定値」に置き換わります。

例：Android 版 Chrome の比較

 * @<b>{以前}: @<code>{Mozilla/5.0 (Linux; Android 16; Pixel 9) ... Chrome/143.0.12.45 Mobile Safari/537.36}
 * @<b>{削減後}: @<code>{Mozilla/5.0 (Linux; Android 10; K) ... Chrome/143.0.0.0 Mobile Safari/537.36}

== 新たな概念の提唱

User-Agent Client Hints

== 確認方法

@<code>{navigator.userAgentData}（ UA-CH ）

※ UA-CH の利用はブラウザ差あり

//image[screenshot7][navigatorUserAgentDataの確認1]{
スクリーンショット 2026-02-07 11.55.59
//}

//image[screenshot8][navigatorUserAgentDataの確認2]{
スクリーンショット 2026-02-07 11.56.46
//}

== 実践してみる

= まとめ

 * HTTP登場時から存在し続けるUser-Agentの存在は大きく、問題点が指摘されつつも現在も標準となり続けている
 * ただ、その問題点や仕様の変更により、User-Agent文字列に依存するような処理は避けた方が良い
 * User-Agent Client Hintsは現在も議論され導入が進められている話題であるが、浸透の遅さからすぐ恩恵を受けるとは言いづらい
 * とはいえ、これが標準化すればUser-Agentの問題点も一部解消できるため、対応の準備くらいはしておいても良さそう
