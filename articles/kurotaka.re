= User-Agentとは何か ― その歴史・課題・そしてこれから

= はじめに

【TODO】本記事の目的・対象読者・どのような背景で書かれたか【TODO】

= User-Agentとは

== リクエストヘッダとしてのUser-Agent

User-Agentとは、HTTPの @<code>{User-Agent} リクエストヘッダーのことです。リクエストを送信したアプリケーションやオペレーティングシステム（OS）、ベンダー、バージョンなどを、サーバーやネットワーク側で識別するための特性文字列として用いられます。なお、「User agent」という語句自体は「ユーザーの代理として動作するソフトウェア」という一般的な意味を持ちますが、本記事ではHTTPリクエストヘッダーとしての @<code>{User-Agent} を中心に扱います。

主要なブラウザでは、以下のような形式でUser-Agent文字列が送信されます。

 * @<b>{Firefox}: @<code>{Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:124.0) Gecko/20100101 Firefox/124.0}
 * @<b>{Chrome}: @<code>{Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36}
 * @<b>{Safari}: @<code>{Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Safari/605.1.15}

ここで重要なのは、User-Agent文字列はあくまで@<b>{クライアント側からの自己申告}であるという点です。つまり、実際のユーザーエージェントとは異なる値を送信することも技術的には可能であり、その真正性は保証されていません。この性質は後述する課題にも深く関わってきます。

参考: @<href>{https://developer.mozilla.org/ja/docs/Web/HTTP/Reference/Headers/User-Agent, User-Agent ヘッダー - HTTP | MDN}

== User-Agentの確認方法

自分のブラウザが送信しているUser-Agent文字列を確認するには、いくつかの方法があります。

1つ目は、Webサービスを利用する方法です。「ユーザーエージェント 確認」などのキーワードで検索すると、アクセスするだけでUser-Agent文字列を表示してくれるサイトが見つかります。

//image[my-useragent][Webサービスを利用したUser-Agent確認]{
ユーザーエージェント確認ツールのスクリーンショット
//}

2つ目は、ブラウザの開発者ツール（DevTools）を利用する方法です。コンソールで @<code>{navigator.userAgent} を実行すると、現在のブラウザのUser-Agent文字列を取得できます。

//image[navigator-useragent][開発者ツールでのnavigator.userAgent確認]{
DevToolsコンソールでnavigator.userAgentを実行した結果
//}

参考: @<href>{https://developer.mozilla.org/ja/docs/Web/API/Navigator/userAgent, Navigator: userAgent プロパティ - Web API | MDN}

== User-Agentの文法

【TODO】RFCの定義を入れる or ブロックごと消す【TODO】

== 他のUser-Agent文字列の例

ブラウザ以外にも、さまざまなソフトウェアがUser-Agent文字列を送信しています。クローラ、APIクライアント、コマンドラインツールなど、その種類は多岐にわたります。

これらを網羅的にまとめたリソースとして、@<href>{https://useragentstring.com/pages/useragentstring.php, UserAgentString.com} があります。また、@<href>{https://github.com/ua-parser/uap-core, ua-parser/uap-core} は、User-Agent文字列という「カオス」を解読するために長年メンテナンスされてきた、世界で最も標準的なUser-Agent解析用の正規表現データベースです。多種多様なUser-Agentのパターンを正規表現で体系的に管理しており、多くのパーサーライブラリの基盤となっています。

== 身近な活用例

User-Agentは、私たちの日常的なWeb利用の中でもさまざまな場面で活用されています。

たとえば、スマートフォンのブラウザにある「PC版サイトを表示」という機能は、ブラウザが送信するUser-Agent文字列をモバイル向けからデスクトップ向けに切り替えることで実現されています。また、Webサービスにアクセスした際に「お使いのブラウザはサポートされていません」と表示されるケースがありますが、これもサーバーやクライアント側でUser-Agentを読み取って判定している場合があります。

= User-Agentの登場背景と変遷

== RFC 1945での登場

User-Agentヘッダーが最初に登場したのは、最初期のHTTP仕様であるRFC 1945（HTTP/1.0）です。この仕様では、User-Agentリクエストヘッダーは「そのリクエストを発生させたユーザーエージェントに関する情報を含む」ものとして定義されています。

RFC 1945では、User-Agentの利用目的として以下が挙げられていました。

 * @<b>{統計的な目的}: どのクライアントがどれだけ使われているかを収集するため
 * @<b>{プロトコル違反の追跡}: どのクライアントがRFCの仕様から逸脱しているかを判別するため
 * @<b>{特定のユーザーエージェントへの対応}: サーバーが特定のエージェント向けにレスポンスを調整するため

また、User-Agentヘッダーの送信は「必須ではないが、含めるべきである（SHOULD）」とされていました。

参考: @<href>{https://datatracker.ietf.org/doc/html/rfc1945, RFC 1945 - Hypertext Transfer Protocol -- HTTP/1.0}

== RFC 9110での現在の定義

現行のHTTPセマンティクスを定義するRFC 9110でも、User-Agentヘッダーは「リクエストを生成したユーザーエージェントソフトウェアについての情報を含むヘッダー」として引き続き定義されています。

【TODO】RFC 1945からRFC 9110に至るまでの間の変遷をもう少し詳細に、票とか入れる？【TODO】

このように、User-Agentヘッダーはもともとクライアントの識別手段として設計され、HTTP仕様の中で長年にわたり使われ続けてきました。

= User-Agentはどう使われてきたか

== 実際に挙動差を試してみる

User-Agentによってサーバーの応答がどう変わるかを、curlコマンドを使って実際に確認してみましょう。以下のコマンドでは、Google検索のトップページに対して異なるUser-Agent文字列でリクエストを送り、返されるHTMLの違いを比較しています。

//emlist{
# 不明なUser-Agentでリクエスト
curl -s -o unknown.html -H "User-Agent: Unknown" https://www.google.com

# iPhone（モバイル）のUser-Agentでリクエスト
curl -s -o mobile.html -H "User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 17_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Mobile/15E148 Safari/604.1" https://www.google.com
//}

この2つのHTMLを比較すると、モバイル向けのUser-Agentで送信した場合はモバイル最適化されたHTMLが返されるのに対し、不明なUser-Agentでは異なるレイアウトのHTMLが返されることがわかります。

//image[google-default][デフォルトUser-Agent（Unknown）でのGoogle検索]{
デフォルトUser-Agentでのレスポンス
//}

//image[google-mobile][モバイルUser-AgentでのGoogle検索]{
モバイルUser-Agentでのレスポンス
//}

また、ブラウザの開発者ツールでも同様の確認ができます。DevToolsのデバイスエミュレーション機能を使えば、User-Agentやビューポート幅を自由に変更してテストが可能です。たとえば、Google検索ページにおいて、iPhone SEのエミュレーションと、同じ画面幅でUser-Agentを @<code>{\"UnknownDevice\"} とした場合とでは、表示されるページのレイアウトが異なることを確認できます。

//image[search-iphone][iPhone SE（User-Agent: iPhone）でのGoogle検索ページ]{
iPhoneのUser-Agentでのレイアウト
//}

//image[search-unknown][UnknownDeviceユーザーエージェントでのGoogle検索ページ]{
UnknownDeviceのUser-Agentでのレイアウト
//}

== User-Agent文字列の識別方法

User-Agentは単なる文字列であるため、その識別はパターンマッチングで行うのが基本です。しかし、先述のとおりUser-Agent文字列は非常に複雑であるため、手作業でのパースは現実的ではありません。

そこで、ua-parser（前述のuap-coreを基盤とするライブラリ群）などの専用パーサーを利用するのが一般的です。これらのライブラリは、膨大なUser-Agentパターンを正規表現で管理し、ブラウザ名、バージョン、OS、デバイスタイプなどを抽出してくれます。

【TODO】具体的なライブラリの使用例を入れるか、ブロックごと消すか【TODO】

== サーバーサイドでの活用

サーバーサイドでは、User-Agentはさまざまな目的で活用されています。

1つ目は、@<b>{robots.txt によるクローラ制御}です。robots.txt ファイルでは、User-Agent名を指定してクローラごとにアクセスの許可・拒否を制御できます。たとえば @<code>{User-agent: Googlebot} のようにGooglebotに対するルールを記述するなど、検索エンジンのクローリング制御に広く使われています。

2つ目は、@<b>{アクセスログ解析での活用}です。WebサーバーのアクセスログにはUser-Agent文字列が記録されるため、これを解析することで、訪問者のブラウザやOS、デバイスの種類ごとのアクセス傾向を把握できます。

【TODO】もう少しあれば入れる【TODO】

== User-Agentは容易に変更できる

ここまで見てきたように、User-Agentはあくまでクライアントからの自己申告であり、自由に書き換えることが可能です。curlのように @<code>{-H} オプションで任意の文字列を指定できるのはもちろん、ブラウザの拡張機能やDevToolsからも簡単に変更できます。この性質は次章で取り上げる課題にも直結します。

= User-Agentを取り巻く課題

== 文字列の複雑性

初期のUser-Agent文字列はシンプルなものでしたが、ブラウザ戦争の時代を経て、互換性を維持するために文字列は肥大化の一途をたどりました。

たとえば、ChromeのUser-Agentに @<code>{Safari/537.36} や @<code>{AppleWebKit/537.36} が含まれているのは、一見不思議に思えます。これは歴史的な経緯によるもので、新しいブラウザが登場するたびに、既存のサーバーから適切なコンテンツを受け取るために、先行ブラウザの識別子を含める必要があったためです。

この歴史的経緯の詳細（Mosaic → Netscape → IE → Firefox → Chrome と続くUA文字列の「嘘の連鎖」）について、参考記事に基づく具体例を追加すると、読み物としても面白くなります。

参考: @<href>{https://humanwhocodes.com/blog/2010/01/12/history-of-the-user-agent-string/, History of the browser user-agent string}

== ブラウザ・フィンガープリント（個人特定）

User-Agent文字列に含まれる詳細な情報は、ブラウザ・フィンガープリンティングの一要素として悪用されるリスクがあります。ブラウザ・フィンガープリンティングとは、ブラウザのバージョン、OS、画面解像度、インストールされたフォントなど、複数の情報を組み合わせることで、Cookieを使わずにユーザーをサイト横断的に追跡する手法です。

詳細すぎるUser-Agent文字列は、この「指紋」の精度を高めることにつながるため、プライバシーの観点から問題視されています。

フィンガープリンティングの具体的なエントロピー（User-Agent単体でどの程度の識別力があるか）に関するデータや、EFFのPanopticlickなどの調査結果があると、読者に問題の深刻さが伝わります。

== 端末の識別困難

User-Agent文字列が複雑になりすぎた結果、開発者が解析を誤り、特定のブラウザやデバイスを不当にブロックしてしまうケースが発生しています。

【TODO】iPadでの実例【TODO】

== 容易な偽装とログの信用性

User-Agentが自由に書き換えられるという性質は、サーバーログの信用性にも影響を与えます。たとえば、サーバーへのアクセスが急増した際にログを確認しても、一般的なブラウザを装ったUser-Agent文字列が使われている場合、それがボットによるアクセスなのか正規のユーザーなのかの判別が困難になります。

@<code>{User-Agent: Bot/1.0} のように明らかにボットとわかるUser-Agentであれば原因特定は容易ですが、意図的に通常のブラウザを装うケースが多いため、User-Agentだけに頼った原因特定には限界があります。

= User-Agentに対する新たなアプローチ

== User-Agent文字列の削減

前章で述べた文字列の複雑性やプライバシーの問題を受けて、主要ブラウザではUser-Agent文字列を削減する方向に進んでいます。具体的には、UA文字列の特定のパーツが「固定値」に置き換わります。

Android版Chromeを例に見てみましょう。

 * @<b>{削減前}: @<code>{Mozilla/5.0 (Linux; Android 16; Pixel 9) ... Chrome/143.0.12.45 Mobile Safari/537.36}
 * @<b>{削減後}: @<code>{Mozilla/5.0 (Linux; Android 10; K) ... Chrome/143.0.0.0 Mobile Safari/537.36}

削減後のUser-Agentでは、Androidのバージョンが @<code>{10} に、デバイスモデルが @<code>{K} に、Chromeのマイナーバージョンが @<code>{0.0.0} にそれぞれ固定されています。これにより、フィンガープリンティングに使える情報量を減らしつつ、基本的なプラットフォーム判定は維持できるようになっています。

【TODO】削減の詳細【TODO】

== User-Agent Client Hints（UA-CH）の提唱

User-Agent文字列の削減と並行して提唱されたのが、@<b>{User-Agent Client Hints（UA-CH）} という新たな仕組みです。

【TODO】UA-CHについての補足【TODO】

== UA-CHの確認方法

ブラウザのDevToolsコンソールで @<code>{navigator.userAgentData} を実行することで、UA-CHで提供される情報を確認できます。ただし、UA-CHの対応状況はブラウザによって異なります。

【TODO】詳細な説明を入れる【TODO】


== UA-CHを実践してみる

【TODO】具体的なリクエスト例とレスポンス【TODO】

= まとめ

HTTPの登場時から存在し続けるUser-Agentは、その歴史の長さゆえにWebの基盤的な存在となっています。しかし、文字列の複雑化、プライバシーへの懸念、偽装の容易さなど、さまざまな問題点も指摘されてきました。

現在の流れとしては、User-Agent文字列に依存するような処理は避ける方向が推奨されています。代替手段であるUser-Agent Client Hints（UA-CH）は現在も議論・導入が進められている段階ですが、ブラウザ間の対応差もあり、すぐにすべての環境で恩恵を受けられるとは言いがたい状況です。

とはいえ、UA-CHが標準化されればUser-Agentが抱えてきた問題の一部は解消されることが期待されます。今すぐの完全移行は難しくとも、UA-CHへの対応準備を進めておくことは、将来を見据えた賢明な判断と言えるでしょう。

記事のまとめとして、読者に対する具体的なアクションアイテム（例：「自分のプロジェクトでUser-Agentに依存している箇所がないか確認してみましょう」「UA-CHの仕様に目を通しておきましょう」など）があると、記事としての締まりがよくなります。

= 参考リンク

 * @<href>{https://developer.mozilla.org/ja/docs/Web/HTTP/Reference/Headers/User-Agent, User-Agent ヘッダー - HTTP | MDN}
 * @<href>{https://developer.mozilla.org/ja/docs/Web/API/Navigator/userAgent, Navigator: userAgent プロパティ - Web API | MDN}
 * @<href>{https://datatracker.ietf.org/doc/html/rfc1945, RFC 1945 - Hypertext Transfer Protocol -- HTTP/1.0}
 * @<href>{https://humanwhocodes.com/blog/2010/01/12/history-of-the-user-agent-string/, History of the browser user-agent string}
 * @<href>{https://useragentstring.com/pages/useragentstring.php, UserAgentString.com}
 * @<href>{https://github.com/ua-parser/uap-core, ua-parser/uap-core - GitHub}

【TODO】UA-CH関連のリファレンスリンク（W3Cの仕様ドキュメント、Chromiumのドキュメントなど）を追加【TODO】
【TODO】参考文献のフォーマットを整える【TODO】

【TODO】LLMの例、AIによるfetchの挙動も調べてどこかに挿入する【TODO】
