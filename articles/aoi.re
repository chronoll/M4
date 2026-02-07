= AG Gridを使ってみよう 〜セルカスタマイズ編〜

React + TypeScript でAG Gridを使ってセルをカスタマイズしてみる

== 目次と内容メモ(仮)

==== 1. はじめに
 * AG Gridとは
 * 対象読者
 * 使用バージョン
 * パッケージインストール

==== 2. 表を表示してみる
 * サンプルコードとスクショ

==== 3. セルの見た目をカスタマイズ — cellRenderer編
 * cellRendererの基本
 * 実例1：ステータスバッジ（色付きラベル）
 * 実例2：プログレスバー
 * 実例3：アイコン + テキスト
 * Tips: cellRendererParamsで外から値を渡す

==== 4. セルを編集可能にする — cellEditor編
 * 組み込みエディタ（テキスト、セレクト）
 * カスタムエディタの作り方
 * 実例：セレクトボックスエディタ
 * Enter/Escのハンドリング
 * 編集時のバリデーション

==== 5. 条件付きスタイリング
 * cellStyleで色分け
 * cellClassRulesでクラス切り替え
 * 実例：ヒートマップ風の数値色分け

==== 6. おわりに
 * まとめ

=={intro} はじめに


==={intro-about} この章について
AG Gridは、ReactでテーブルUIを作るときの最強ライブラリです。
ソート、フィルタ、ページング、セル編集、エクスポートなど、テーブルに必要な機能がすべて揃っています。

この章では、@<b>{セルのカスタマイズ}に絞って、サンプルコードとともに解説していきます。

==={intro-aggrid} AG Gridとは
AG Gridは、React/Angular/Vue/JavaScriptに対応している、
多機能でカスタマイズ性の高いグリッドライブラリです。

AG GridにはCommunity版(無料)と、さらに高度な機能を持つEnterprise版(有料)があります。本書ではCommunity版を使用します。

==={intro-goal} 本章のゴール

この章を読み終えた後、あなたはこんなことができるようになります：

 * AG Gridでテーブルを表示し、基本的なカスタマイズができる
 * セルにステータスバッジやプログレスバーなどを表示できる
 * セルを編集可能にし、カスタムエディタを作れる
 * 条件に応じてセルの色を動的に変えられる


==={intro-target} 対象読者
 * React + TypeScriptの基本的な書き方が分かる
 * 多機能でカスタマイズ性のあるグリッドライブラリを探している
 * AG Gridを触ったことがある or これから使う予定
 * AG Gridでセルの見た目や編集機能をカスタマイズしたい

==={intro-versions} 使うバージョン

 * ag-grid-community 35.0.1
 * ag-grid-react 35.0.1
 * React 19.2.0
 * TypeScript 5.9.3

==={intro-setup} AG Gridのインストール
下記のコマンドでAG Grid(Community版)をインストールすることができます。

//cmd{
npm install ag-grid-community ag-grid-react
//}

=={basic} 表を表示してみる


==={basic-intro} まずは動くテーブルを作る

まずは「動くテーブル」を作りましょう。
AG Gridの最小構成は、データ配列とカラム定義さえあれば動きます。

==={basic-code} コード例

//listnum[app-basic][ ]{
import { useState } from "react";
import { AgGridReact } from "ag-grid-react";
import type { ColDef } from "ag-grid-community";
import { ModuleRegistry, AllCommunityModule } from "ag-grid-community";

ModuleRegistry.registerModules([AllCommunityModule]);

interface User {
  id: number;
  name: string;
  email: string;
}

function App() {
  const [rowData] = useState<User[]>([
    { id: 1, name: "山田太郎", email: "yamada@example.com" },
    { id: 2, name: "佐藤花子", email: "sato@example.com" },
    { id: 3, name: "鈴木一郎", email: "suzuki@example.com" },
  ]);

  const [colDefs] = useState<ColDef<User>[]>([
    { field: "id", headerName: "ID", width: 100 },
    { field: "name", headerName: "名前" },
    { field: "email", headerName: "メールアドレス" },
  ]);

  return (
    <div style={{ height: 200, width: 500 }}>
      <AgGridReact<User> rowData={rowData} columnDefs={colDefs} />
    </div>
  );
}

export default App;
//}


