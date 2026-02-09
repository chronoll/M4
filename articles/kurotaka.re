= User-Agentとは何か ― その歴史・課題・そしてこれから

こんにちは、クロタカ(@chronoll)です。私からは、User-Agentについて調べたことをまとめていきます！

== はじめに

HTTPの @<code>{User-Agent} リクエストヘッダーといえば、Webに関わるエンジニアであれば一度は目にしたことがあるでしょう。しかし、その仕組みや歴史的背景まで深掘りした経験のある方は意外と少ないのではないでしょうか。

筆者自身、フロントエンドでのデバイス判定に悩んだことをきっかけに、その後はサーバーサイドのアクセスログ解析でも再びUser-Agentと向き合うことになりました。エンジニアとしてまだ1年に満たない実務の中でも、分野を問わずUser-Agentの存在を意識する場面は思いのほか多いと感じています。

この仕組みは1996年のHTTP/1.0仕様から存在し、約30年にわたってWebの基盤として使われ続けてきました。一方で、文字列の複雑化やプライバシーの問題が顕在化し、現在はUser-Agent文字列の削減やUser-Agent Client Hints（UA-CH）といった新しいアプローチが模索されています。

本記事では、User-Agentの基本・歴史・活用例・課題・新しいアプローチまでを、実際に手を動かして確認しながら解説します。最後にはおまけとして、LLMのWebクロール・検索におけるUser-Agentの例も取り扱っています。

== User-Agentとは

==== リクエストヘッダとしてのUser-Agent

「User agent」という語句自体は「ユーザーの代理として動作するソフトウェア」という一般的な意味を持ちますが、本記事ではHTTPリクエストヘッダーとしての @<code>{User-Agent}@<fn>{useragent-mdn} を取り扱います。リクエストを送信したアプリケーションやオペレーティングシステム（OS）、ベンダー、バージョンなどを、サーバーやネットワーク側で識別するための特性文字列として用いられます。

主要なブラウザでは、以下のような形式でUser-Agent文字列が送信されます。

 * @<b>{Firefox}: @<code>{Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:124.0) Gecko/20100101 Firefox/124.0}
 * @<b>{Chrome}: @<code>{Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36}
 * @<b>{Safari}: @<code>{Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Safari/605.1.15}

ここで重要なのは、User-Agent文字列はあくまで@<b>{クライアント側からの自己申告}であるという点です。つまり、実際のユーザーエージェントとは異なる値を送信することも技術的には可能であり、その真正性は保証されていません。この性質は後述する課題にも深く関わってきます。

==== User-Agentの登場背景

User-Agentヘッダーが最初に登場したのは、最初期のHTTP仕様である@<b>{RFC 1945（HTTP/1.0）}@<fn>{rfc1945} です。この仕様では、User-Agentリクエストヘッダーは「そのリクエストを発生させたユーザーエージェントに関する情報を含む」ものとして定義されています。

RFC 1945では、User-Agentの利用目的として以下が挙げられていました。

 * @<b>{統計的な目的}: どのクライアントがどれだけ使われているかを収集するため
 * @<b>{プロトコル違反の追跡}: どのクライアントがRFCの仕様から逸脱しているかを判別するため
 * @<b>{特定のユーザーエージェントへの対応}: サーバーが特定のエージェント向けにレスポンスを調整するため

また、User-Agentヘッダーの送信は「必須ではないが、含めるべきである（SHOULD）」とされていました。

現行のHTTPセマンティクスを定義するRFC 9110@<fn>{rfc9110} でも、User-Agentヘッダーは「リクエストを生成したユーザーエージェントソフトウェアについての情報を含むヘッダー」として引き続き定義されており、その立ち位置が変わることなくHTTP仕様の中で長年にわたり使われ続けてきました。

==== User-Agentの確認方法

自分のブラウザが送信しているUser-Agent文字列を確認するには、いくつかの方法があります。

1つ目は、Webサービスを利用する方法です。「ユーザーエージェント 確認」などのキーワードで検索すると、アクセスするだけでUser-Agent文字列を表示してくれるサイトが見つかります。@<fn>{testpage-ua}

//image[my-useragent][Webサービスを利用したUser-Agent確認]{
ユーザーエージェント確認ツールのスクリーンショット
//}

2つ目は、ブラウザの開発者ツール（DevTools）を利用する方法です。コンソールで @<code>{navigator.userAgent}@<fn>{navigator-ua-mdn} を実行すると、現在のブラウザのUser-Agent文字列を取得できます。

//image[navigator-useragent][開発者ツールでのnavigator.userAgent確認]{
DevToolsコンソールでnavigator.userAgentを実行した結果
//}

==== 他のUser-Agent文字列の例

ブラウザ以外にも、さまざまなソフトウェアがUser-Agent文字列を送信しています。クローラ、APIクライアント、コマンドラインツールなど、その種類は多岐にわたります。

以下のようなリソースから確認できるので、気になる方はご覧になってみてください。

 * @<href>{https://useragentstring.com/pages/useragentstring.php, UserAgentString.com}@<fn>{useragentstring}
 * @<href>{https://github.com/ua-parser/uap-core, ua-parser/uap-core}@<fn>{uap-core}
    * User-Agent 文字列という「カオス」を解読するために長年メンテナンスし続けてきた、User-Agent 解析用正規表現データベースのリポジトリ

==== 身近な活用例

User-Agentは、私たちの日常的なWeb利用の中でもさまざまな場面で活用されています。

たとえば、スマートフォンのブラウザにある「PC版サイトを表示」という機能は、ブラウザが送信するUser-Agent文字列をモバイル向けからデスクトップ向けに切り替えることで実現されています。また、Webサービスにアクセスした際に「お使いのブラウザはサポートされていません」と表示されるケースがありますが、これもサーバーやクライアント側でUser-Agentを読み取って判定している場合があります。

では、実際にどう活用されているのか、その具体例を次の章で追っていきます。

== User-Agentはどう使われてきたか

User-Agentの仕組みと歴史を押さえたところで、次に「実際にどう使われているのか」を見ていきます。この章では、具体的な活用例を押さえながら、実際にcurlやDevToolsで挙動の違いを確認してみます。

==== 活用例

サーバーサイドでは、User-Agentはさまざまな目的で活用されています。

====== robots.txtによるクローラー制御

代表的な活用が、robots.txtによるクローラーの制御です。robots.txtはWebサイトのルートディレクトリに配置するテキストファイルで、@<code>{User-agent} ディレクティブにクローラー名を指定し、クローラーごとにアクセスの許可・禁止を宣言できます。

例えば、以下の通りです。

//emlist{
User-agent: Googlebot
Disallow: /private/

User-agent: *
Allow: /
//}

これは「Googlebotには @<code>{/private/} 以下へのアクセスを禁止し、それ以外のクローラーにはすべて許可する」という意味です。Googlebotは自らのリクエストに @<code>{Googlebot/2.1} を含むUser-Agent文字列を付与しているため、サーバー側はこの名乗りをもとにルールを適用します。

実際のrobots.txtは、ブラウザで @<code>{サイトのURL/robots.txt} にアクセスするだけで誰でも確認できます。https://github.com/robots.txt のように確認してみましょう。

//emlist{
# If you would like to crawl GitHub contact us ...
# We also provide an extensive API: https://docs.github.com
User-agent: bingbot
Disallow: /ekansa/Open-Context-Data
Disallow: /ekansa/opencontext-*
Disallow: /account-login
Disallow: */tarball/
Disallow: */zipball/
Disallow: /Explodingstuff/
Disallow: /copilot/
Disallow: /copilot/c/

...
//}

====== アクセスログ解析での活用

WebサーバーのアクセスログにもUser-Agent文字列は記録されています。たとえばApacheやNginxの一般的なログ形式では、1行のアクセス記録の末尾にUser-Agent文字列が含まれます。

@<code>{203.0.113.1 - - [07/Feb/2026:10:15:30 +0900] "GET /index.html HTTP/1.1" 200 5234 "-" "Mozilla/5.0 (iPhone; CPU iPhone OS 17_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Mobile/15E148 Safari/604.1"}

この末尾の文字列から、iPhoneのSafariからのアクセスであることがわかります。Google Analyticsではこうした情報をもとに、「ユーザー環境の詳細」レポートでブラウザやOS、デバイスカテゴリごとのセッション数やコンバージョン率を確認できます。@<fn>{ga-user-env} 普段何気なく見ているこれらのレポートも、裏側ではUser-Agent文字列の解析によって成り立っているのです。

====== その他の活用例

コンテントネゴシエーション ── User-Agentを見てWebP対応ブラウザには軽量画像を、非対応ブラウザにはJPEGを返すなど、レスポンスの内容を出し分ける

Bot検知・レートリミット ── User-Agentが空やBot系の文字列だった場合にアクセスを制限したり、CAPTCHAを表示したりする

など、User-Agentによって制御されている例は多いです。

==== 実際に挙動差を試してみる

User-Agentによってサーバーの応答がどう変わるかを、curlコマンドを使って実際に確認してみましょう。以下のコマンドでは、Google検索のトップページに対して異なるUser-Agent文字列でリクエストを送り、返されるHTMLの違いを比較しています。

//emlist{
# iPhone（モバイル）のUser-Agentでリクエストして保存
curl -s -o mobile.html -A "Mozilla/5.0 (iPhone; CPU iPhone OS 17_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Mobile/15E148 Safari/604.1" https://www.google.com

# 適当なUser-Agentでリクエストして保存
curl -s -o unknown.html -A "Unknown" https://www.google.com
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

==== User-Agent文字列の識別方法

User-Agentは単なる文字列であるため、その識別はパターンマッチングで行うのが基本です。User-Agent文字列をパースして扱うライブラリも多く存在し、フレームワークに組み込まれている場合もあります。

ua-parser（前述のuap-coreを基盤とするライブラリ群）などの専用パーサが代表例で、膨大なUser-Agentパターンを正規表現で管理し、ブラウザ名、バージョン、OS、デバイスタイプなどを抽出してくれます。

==== User-Agentは容易に変更できる

ここまで見てきたように、User-Agentはあくまでクライアントからの自己申告であり、自由に書き換えることが可能です。curlのように @<code>{-A} オプションで任意の文字列を指定できるのはもちろん、ブラウザの拡張機能やDevToolsからも簡単に変更できます。この性質は次章で取り上げる課題にも直結します。

== User-Agentを取り巻く課題

前章では便利な活用例を見てきましたが、約30年にわたって使われ続けてきたこの仕組みには、いくつかの構造的な問題も積み重なっています。ここでは、文字列の複雑性、プライバシー、端末識別の困難さ、そして偽装の容易さという4つの課題を取り上げます。

==== 文字列の複雑性

初期のUser-Agent文字列はシンプルなものでしたが、ブラウザ戦争の時代を経て、互換性を維持するために文字列は肥大化の一途をたどりました。

たとえば、ChromeのUser-Agentに @<code>{Safari/537.36} や @<code>{AppleWebKit/537.36} が含まれているのは、一見不思議に思えます。これは歴史的な経緯によるもので、新しいブラウザが登場するたびに、既存のサーバーから適切なコンテンツを受け取るために、先行ブラウザの識別子を含める必要があったためです。

@<href>{https://humanwhocodes.com/blog/2010/01/12/history-of-the-user-agent-string/, History of the browser user-agent string}@<fn>{ua-history} ではこれを「嘘の連鎖」という皮肉を含めながら経緯の詳細が語られているため、ぜひご一読ください。

==== ブラウザ・フィンガープリント（個人特定）

User-Agent文字列に含まれる詳細な情報は、ブラウザ・フィンガープリンティングの一要素として悪用されるリスクがあります。ブラウザ・フィンガープリンティングとは、ブラウザのバージョン、OS、画面解像度、インストールされたフォントなど、複数の情報を組み合わせることで、Cookieを使わずにユーザーをサイト横断的に追跡する手法です。

詳細すぎるUser-Agent文字列は、この「指紋」の精度を高めることにつながるため、プライバシーの観点から問題視されています。

==== 端末の識別困難

User-Agent文字列が複雑になりすぎた結果、開発者が解析を誤り、特定のブラウザやデバイスを不当にブロックしてしまうケースが発生しています。

筆者も業務で直面したのがiPadの問題です。iPadOS 13以降、iPad（iPad miniを除く）のSafariはmacOS Safariとまったく同じUA文字列を送信するようになりました。

//emlist{
// iPadOS 12以前
Mozilla/5.0 (iPad; CPU OS 12_0 like Mac OS X) ...

// iPadOS 13以降
Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15) ...
//}

つまり、UA文字列から「iPad」という文字列が消えたのです。これにより、@<code>{/iPad/.test(userAgent)} のようなシンプルな正規表現による判定が機能しなくなりました。実際に、広く使われているライブラリの中にもこの変更に追従できていないものがあります。

==== 容易な偽装とログの信用性

User-Agentが自由に書き換えられるという性質は、サーバーログの信用性にも影響を与えます。たとえば、サーバーへのアクセスが急増した際にログを確認しても、一般的なブラウザを装ったUser-Agent文字列が使われている場合、それがボットによるアクセスなのか正規のユーザーなのかの判別が困難になります。

@<code>{User-Agent: Bot/1.0} のように明らかにボットとわかるUser-Agentであれば原因特定は容易ですが、意図的に通常のブラウザを装うケースが多いため、User-Agentだけに頼った原因特定には限界があります。@<fn>{ua-spoofing}

「User-Agentのスプーフィングは既に攻撃の基本要素になっている」と表現する記事もあるくらいです。

== User-Agentに対する新たなアプローチ

前章で見てきた課題を受けて、主要ブラウザベンダーはUser-Agentの在り方そのものを見直す動きを進めています。この章では、UA文字列の削減と、その代替として提唱されたUser-Agent Client Hints（UA-CH）という2つのアプローチを解説します。

==== User-Agent文字列の削減

前章で述べた文字列の複雑性やプライバシーの問題を受けて、主要ブラウザではUser-Agent文字列を削減する方向に進んでいます。具体的には、UA文字列の特定のパーツが「固定値」に置き換わります。

Android版Chromeを例に見てみましょう。

 * @<b>{削減前}: @<code>{Mozilla/5.0 (Linux; Android 16; Pixel 9) ... Chrome/143.0.12.45 Mobile Safari/537.36}
 * @<b>{削減後}: @<code>{Mozilla/5.0 (Linux; Android 10; K) ... Chrome/143.0.0.0 Mobile Safari/537.36}

削減後のUser-Agentでは、Androidのバージョンが @<code>{10} に、デバイスモデルが @<code>{K} に、Chromeのマイナーバージョンが @<code>{0.0.0} にそれぞれ固定されています。これにより、フィンガープリンティングに使える情報量を減らしつつ、基本的なプラットフォーム判定は維持できるようになっています。

Chromeはこの削減を2021年から7つのフェーズに分けて段階的に進め、Chrome 113@<fn>{chromium-ua-reduction}で完了しました。他のブラウザも同じ方向に動いています。Safariは実はChromeより早い2017年にUA文字列の凍結を試みたパイオニアで、@<fn>{safari-ua-freeze} 現在はmacOSバージョンやCPUアーキテクチャを固定値にしています。Firefoxも同様に、OSバージョンやマイナーバージョンの凍結を段階的に進めています。@<fn>{firefox-ua-freeze}

==== User-Agent Client Hints（UA-CH）の提唱

User-Agent文字列の削減と並行して提唱されたのが、@<b>{User-Agent Client Hints（UA-CH）}@<fn>{ua-ch-spec} という新たな仕組みです。

従来のUser-Agentは、ブラウザやOS、デバイスの情報をすべて1本の文字列にまとめて毎回送信していました。UA-CHはこの発想を転換し、サーバーが必要な情報だけを選択的にリクエストできるようにする仕組みです。ブラウザはまず最小限の情報（ブラウザ名やモバイルかどうか）だけを送信し、サーバーが @<code>{Accept-CH} ヘッダで追加情報を要求したときだけ、OSバージョンやデバイスモデルといった詳細を返します。

ただし、現時点でUA-CHを実装しているのはChromiumベースのブラウザ（ChromeやEdge）のみで、Safari等ブラウザでは未対応です。そのため開発者は、ブラウザによってUA文字列とUA-CHを使い分ける必要があり、すぐにすべての環境で恩恵を受けられる状況ではありません。

====== UA-CHの確認方法

ChromeやEdgeブラウザのDevToolsコンソールで @<code>{navigator.userAgentData}@<fn>{ua-ch-mdn} を実行することで、UA-CHで提供される情報を確認できます。

//image[ua-ch-basic][DevToolsでのnavigator.userAgentData確認（基本情報）]{
低エントロピー情報の表示
//}

ブラウザ名・メジャーバージョン、モバイルかどうか、プラットフォーム名といった基本情報が返ってきます。これらは「低エントロピー」と呼ばれる、ユーザーを特定しにくい最小限の情報だけです。もっと詳しい情報が必要な場合は、@<code>{getHighEntropyValues()} メソッドで明示的にリクエストします。

//emlist[][javascript]{
// JavaScriptでのUA-CH取得例
navigator.userAgentData.getHighEntropyValues([
  "platform",
  "platformVersion",
  "model",
  "uaFullVersion"
]).then(data ==> console.log(data));
//}

//image[ua-ch-high][DevToolsでのnavigator.userAgentData確認（高エントロピー情報）]{
高エントロピー情報の表示
//}

バージョンの詳細な情報が追加されていることがわかります。

また、単なる冗長な文字列ではなく、オブジェクトで返されるため、より端末情報等が扱いやすくなったこともわかるかと思います。

//emlist[][python]{
// 従来のAPI：すべての情報が1つの文字列に
console.log(navigator.userAgent);
// → "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 ..."

// UA-CH：構造化されたオブジェクトで必要な情報だけ
console.log(navigator.userAgentData.platform);
// → "macOS"
console.log(navigator.userAgentData.mobile);
// → false
//}

====== UA-CHと従来のUser-Agentの併用

先ほど述べた通り、これらの機能はSafari等一部ブラウザでは未対応です。試しにSafariの開発者ツールで出力を試みても、undefinedとなっています。

//image[ua-ch-unsupported][Safariでのnavigator.userAgentData確認（未対応）]{
未対応ブラウザでの出力
//}

未対応のブラウザでも動作をさせるための折衷案として、そして将来対応してくれることの期待も含めて、以下のように判定を組み合わせたコードを書くといったアプローチが最適かもしれません。

//emlist[][javascript]{
function isMobile() {
  if (navigator.userAgentData) {
    // UA-CH対応ブラウザ：構造化されたプロパティで判定
    return navigator.userAgentData.mobile;
  }
  // 非対応ブラウザ：従来のUA文字列で判定
  return /Mobile|Android/.test(navigator.userAgent);
}
//}

== おわりに

User-Agentは、HTTPの黎明期から約30年にわたってWebを支えてきた仕組みです。本記事ではその歴史と活用を振り返りつつ、複雑化・プライバシー・偽装といった課題、そしてUA文字列の削減やUA-CHという新たなアプローチを見てきました。

開発者としてまず意識しておきたいのは、User-Agent文字列に強く依存した処理は今後ますますリスクになるという点です。UA文字列の削減は主要ブラウザで既に進行しており、従来のパターンマッチングが正しく機能しなくなるケースは増えていくでしょう。可能であればFeature Detectionへの移行を検討し、UA-CHへの対応準備を進めておくことが現実的な選択肢です。

//footnote[useragent-mdn][User-Agent ヘッダー - HTTP | MDN - https://developer.mozilla.org/ja/docs/Web/HTTP/Reference/Headers/User-Agent]
//footnote[rfc1945][RFC 1945 - Hypertext Transfer Protocol -- HTTP/1.0 - https://datatracker.ietf.org/doc/html/rfc1945]
//footnote[rfc9110][RFC 9110: HTTP Semantics - https://datatracker.ietf.org/doc/html/rfc9110]
//footnote[testpage-ua][IPアドレスとユーザーエージェントの確認 - https://testpage.jp/tool/ip_user_agent.php]
//footnote[navigator-ua-mdn][Navigator: userAgent プロパティ - Web API | MDN - https://developer.mozilla.org/ja/docs/Web/API/Navigator/userAgent]
//footnote[useragentstring][UserAgentString.com - List of User Agent Strings - https://useragentstring.com/pages/useragentstring.php]
//footnote[uap-core][ua-parser/uap-core - GitHub - https://github.com/ua-parser/uap-core]
//footnote[ga-user-env][GA4 地域とデバイスに関する詳細なデータの収集 - https://support.google.com/analytics/answer/12002752?hl=ja]
//footnote[ua-history][History of the browser user-agent string - https://humanwhocodes.com/blog/2010/01/12/history-of-the-user-agent-string/]
//footnote[ua-spoofing][User-Agentを基盤とした悪意的アクセス判断の限界と対応戦略 - https://www.netassist.ne.jp/techblog/34866/]
//footnote[chromium-ua-reduction][User-Agent Reduction - https://www.chromium.org/updates/ua-reduction/]
//footnote[safari-ua-freeze][Safari による User-Agent 固定化と Web における Feature Detection - https://blog.jxck.io/entries/2018-01-17/freezing-user-agent-string.html]
//footnote[firefox-ua-freeze][Cap the User-Agent string's reported macOS version at 10.15 - https://bugzilla.mozilla.org/show_bug.cgi?id=1679929]
//footnote[ua-ch-spec][User-Agent Client Hints - https://wicg.github.io/ua-client-hints/#dom-navigatorua-useragentdata]
//footnote[ua-ch-mdn][ユーザーエージェントクライアントヒント API - Web API | MDN - https://developer.mozilla.org/ja/docs/Web/API/User-Agent_Client_Hints_API]
//footnote[perplexity-ua-evasion][Perplexity AI Is Lying about Their User Agent - https://rknight.me/blog/perplexity-ai-is-lying-about-its-user-agent/]
//footnote[llm-crawler-issues][Websites are Blocking the Wrong AI Scrapers (Because AI Companies Keep Making New Ones) - https://www.404media.co/websites-are-blocking-the-wrong-ai-scrapers-because-ai-companies-keep-making-new-ones/]

== コラム：LLMクローラーとUser-Agent ── 「名乗り」の仕組みが問い直されるとき

第3章では、robots.txtとUser-Agentの組み合わせでクローラーを制御する仕組みを紹介しました。この仕組みはGooglebotの時代から長く使われてきたものですが、近年、まったく新しいプレイヤーが同じ舞台に登場しています。LLM（大規模言語モデル）を提供する各社が、学習データ収集のためのクローラーを運用しはじめたのです。

==== 各社のクローラーとUser-Agent

主要なLLMベンダーは、それぞれ独自のUser-Agent文字列でクローラーを運用しています。

//table[llm-crawlers][各社のクローラーとUser-Agent]{
ベンダー	クローラー名	用途
-------------------------------------
OpenAI	GPTBot	モデルの学習データ収集
OpenAI	ChatGPT-User	ユーザーの指示によるWeb閲覧
OpenAI	OAI-SearchBot	ChatGPT検索機能のインデックス
Anthropic	ClaudeBot	モデルの学習データ収集
Anthropic	Claude-User	ユーザーの指示によるWeb取得
Google	Google-Extended	Geminiの学習データ収集
Meta	meta-externalagent	AI学習用データ収集
//}

たとえばGPTBotのUser-Agent文字列は次のようになっています。

//emlist{
Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko);
compatible; GPTBot/1.1; +https://openai.com/gptbot
//}

ブラウザのUser-Agentと比べるとかなりシンプルですが、構造としては同じ形式を踏襲しています。末尾の @<code>{+https://openai.com/gptbot} は、このクローラーの詳細情報を示すURLで、従来のクローラーが慣習的に含めてきた情報提供リンクです。

==== 「学習」と「引用」を分けるUser-Agent

この表で興味深いのは、OpenAIやAnthropicが@<b>{目的別に複数のUser-Agent}を使い分けている点です。

たとえばOpenAIの場合、@<code>{GPTBot} はモデルの学習用データを大量にクロールするためのボットであり、@<code>{ChatGPT-User} はユーザーがChatGPTとの会話中に「このURLの内容を要約して」と指示したときにリアルタイムでページを取得するボットです。

これにより、サイト運営者はrobots.txtで次のような細やかな制御が可能になります。

//emlist{
# 学習用のクロールはブロックする
User-agent: GPTBot
Disallow: /

# ユーザーが引用のために取得する場合は許可する
User-agent: ChatGPT-User
Allow: /
//}

「自分のコンテンツをAIの学習には使われたくないが、ユーザーが参照する分には構わない」という意思表示ができるわけです。この使い分けは、Googlebotの時代にはなかった発想です。従来の検索エンジンクローラーは「インデックスして検索結果に表示する」という単一の目的でしたが、LLMの時代には「学習に使う」と「引用・検索に使う」という複数の用途が生まれたことで、User-Agentによる識別の重要性がさらに増しています。

==== 信頼モデルの揺らぎ

ここまで読んできた読者であれば、ひとつの疑問が浮かぶはずです。第4章で見たように、User-Agentは自己申告であり偽装が容易でした。robots.txtも法的な強制力を持たない「紳士協定」です。LLMクローラーの世界でも、この構造的な問題はそのまま残っています。

実際に、2024年にはPerplexityのボットが非公開のIPアドレスや偽装したUser-Agentを使ってrobots.txtによるブロックを迂回していたことが報じられました。@<fn>{perplexity-ua-evasion} また、AnthropicのClaudeBotもクローラー名の変更により多くのサイトのブロックをすり抜けてしまっていたほか、過剰なリクエスト頻度がサイト運営者から問題視されました。@<fn>{llm-crawler-issues}

User-Agent文字列を正直に名乗っていても、robots.txtのルールを遵守するかどうかは各ベンダーの実装次第です。@<href>{https://platform.openai.com/docs/bots, OpenAI}、@<href>{https://support.claude.com/en/articles/8896518-does-anthropic-crawl-data-from-the-web-and-how-can-site-owners-block-the-crawler, Anthropic}、@<href>{https://developers.google.com/crawling?hl=ja, Google}、@<href>{https://developers.facebook.com/docs/sharing/webmasters/web-crawlers, Meta} などが各々のクローラー情報を公開しています。

検証の面では、GoogleがGooglebotの逆引きDNSを公開しているのと同様に、OpenAIも @<code>{openai.com/gptbot.json} でIPアドレスの一覧を公開しています。robots.txtによるルール＋IPアドレスによる検証という二段構えの仕組みは、従来の検索エンジンクローラーと同じ考え方を受け継いでいます。

User-Agentという「名乗り」の仕組みは、ブラウザの時代から検索エンジンの時代を経て、いまAIの時代にも引き継がれています。その信頼モデルの限界も含めて引き継がれているという事実は、本書を通じて見てきたUser-Agentの物語の、ある意味で最も現代的な続きと言えるでしょう。
