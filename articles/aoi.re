= AG Gridを使ってみよう 〜セルカスタマイズ編〜

=={intro} はじめに

==={intro-about} この章について
本章では、グリッドライブラリの1つであるAG Gridについて、基本的なテーブル表示から、@<b>{セルのカスタマイズ}にフォーカスして解説していきます。
セルにバッジやプログレスバーを表示したり、その場で編集できるようにしたり、
条件に応じて色を変えたりと、AG Gridでどんなことができるのかを
サンプルコードとともに紹介していきます。

==={intro-aggrid} AG Gridとは
AG Gridは、React/Angular/Vue/JavaScriptに対応している、
多機能でカスタマイズ性の高いグリッドライブラリです。

ソート、フィルタ、ページング、セル編集、行のドラッグ&ドロップ、
Excel/CSVエクスポートなど、テーブルUIに求められる機能が幅広く揃っています。

AG GridにはCommunity版（無料）と、さらに高度な機能を持つEnterprise版（有料）があります。本章ではCommunity版の範囲で解説します。

//pagebreak
==={intro-goal} 本章のゴール
この章を読み終えた後、以下のことができるようになります：

 * AG Gridでテーブルを表示し、基本的なカスタマイズができる
 * セルにステータスバッジやプログレスバーなどを表示できる
 * セルを編集可能にし、カスタムエディタを作れる
 * 条件に応じてセルのスタイルを動的に変えられる

==={intro-target} 対象読者
本章は以下のような方を対象としています：

 * React + TypeScriptの基本的な書き方が分かる
 * 多機能でカスタマイズ性のあるグリッドライブラリを探している
 * AG Gridを触ったことがある、またはこれから使う予定
 * AG Gridでセルの見た目や編集機能をカスタマイズしたい

==={intro-versions} 使うバージョン

 * ag-grid-community 35.0.1
 * ag-grid-react 35.0.1
 * React 19.2.0
 * TypeScript 5.9.3

//pagebreak
==={intro-setup} AG Gridのインストール
以下のコマンドでAG Grid（Community版）をインストールします。

//cmd{
npm install ag-grid-community ag-grid-react
//}

=={basic} 表を表示してみる


==={basic-intro} まずは動くテーブルを作る

まずは最小構成でテーブルを表示してみましょう。
AG Gridの最小構成は、データ配列とカラム定義の2つを渡すだけで動作します。

本章では、TODO管理をテーマとしたコードを例に解説します。

==={basic-code} コード例

//listnum[app-basic][最小構成のテーブル]{
import { useState } from "react";
import { AgGridReact } from "ag-grid-react";
import type { ColDef } from "ag-grid-community";
import { ModuleRegistry, AllCommunityModule } from "ag-grid-community";

// AG Grid v33以降ではModuleの明示的な登録が必要
ModuleRegistry.registerModules([AllCommunityModule]);

//行データの型定義
interface Todo {
  id: number;
  title: string;
  status: "todo" | "doing" | "done";
  progress: number; // 0-100
}

// サンプルデータ
const initialData: Todo[] = [
  { id: 1, title: "デザイン作成", status: "done", progress: 100 },
  { id: 2, title: "API設計", status: "doing", progress: 60 },
  { id: 3, title: "テスト作成", status: "doing", progress: 30 },
  { id: 4, title: "環境構築", status: "todo", progress: 0 },
  { id: 5, title: "レビュー実施", status: "todo", progress: 0 },
];

function App() {
  const [rowData] = useState<Todo[]>(initialData);

//カラム定義
  const [colDefs] = useState<ColDef<Todo>[]>([
    { field: "id", headerName: "ID", width: 80 },
    { field: "title", headerName: "タスク", flex: 1 },
    { field: "status", headerName: "ステータス", width: 120 },
    { field: "progress", headerName: "進捗", width: 120 },
  ]);

  return (
    <div style={{ height: 300, width: 700 }}>
      <AgGridReact<Todo> rowData={rowData} columnDefs={colDefs} />
    </div>
  );
}
export default App;
//}

//image[aoi-basic-table][最小構成のテーブル表示]

これだけのコードで、ヘッダーのクリックによるソートや、ドラッグによるカラム幅のリサイズが最初から動作します。

==={basic-points} ポイント解説
@<code>{ModuleRegistry.registerModules}は、AG Grid v33で導入され、モジュールの明示的な登録が必要になりました。
@<code>{AllCommunityModule}を登録すればCommunity版の全機能が使えます。


=={cellrenderer} セルの見た目をカスタマイズする - cellRenderer

==={cellrenderer-intro} cellRendererとは
先ほどのテーブルでは、ステータスや進捗が文字列として表示されていました。
「ステータスを色付きのバッジにしたい」「進捗をプログレスバーで表示したい」といったカスタマイズには@<code>{cellRenderer} を使います。

@<code>{cellRenderer}は、セルの中身を自由なJSXで描画できる機能です。
カラム定義に関数やReactコンポーネントを指定するだけで使えます。

==={cellrenderer-badge} ステータスバッジ

まずは「ステータス」カラムを色付きバッジで表示してみます。

//list[cellrenderer-badge-code][ステータスバッジ]{
import type { CustomCellRendererProps } from "ag-grid-react";

const statusStyles: Record<Todo["status"], { bg: string; color: string }> = {
  "todo":  { bg: "#e0e0e0", color: "#616161" },
  "doing": { bg: "#bbdefb", color: "#1565c0" },
  "done":  { bg: "#c8e6c9", color: "#2e7d32" },
};

// カラム定義
{
  field: "status",
  headerName: "ステータス",
  cellRenderer: (params: CustomCellRendererProps<Todo>) => {
    const style = statusStyles[params.value as Todo["status"]];
    const label = 
      params.value === "todo" ? "未着手" :
      params.value === "doing" ? "進行中" :
      "完了";

    return (
      <span
        style={{
          padding: "2px 8px",
          borderRadius: "4px",
          backgroundColor: style.bg,
          color: style.color,
          fontWeight: "bold",
          fontSize: "0.85em",
        }}
      >
        {label}
      </span>
    );
  },
}
//}

//image[aoi-badge][ステータスバッジの表示例]

@<code>{cellRenderer}に関数を渡し、
@<code>{params.value}でそのセルの値を受け取ってJSXを返しています。

==={cellrenderer-progress} プログレスバー

次に「進捗」カラムをプログレスバーで表示してみます。

//list[cellrenderer-progress-code][プログレスバー]{
const ProgressRenderer = (params: CustomCellRendererProps<Todo>) => {
  const value = params.value ?? 0;

  const barColor =
    value >= 80 ? "#4caf50" :
    value >= 50 ? "#ff9800" :
    "#f44336";

  return (
    <div style={{
      display: "flex",
      alignItems: "center",
      width: "100%",
      height: "100%",
    }}>
      <div style={{
        flex: 1,
        backgroundColor: "#e0e0e0",
        borderRadius: "4px",
        height: "16px",
        overflow: "hidden",
      }}>
        <div style={{
          width: `${value}%`,
          backgroundColor: barColor,
          height: "100%",
          borderRadius: "4px",
          transition: "width 0.3s ease",
        }} />
      </div>
      <span style={{
        marginLeft: "8px",
        minWidth: "36px",
        textAlign: "right",
        fontSize: "0.85em",
      }}>
        {value}%
      </span>
    </div>
  );
};

// カラム定義
{
  field: "progress",
  headerName: "進捗",
  width: 180,
  cellRenderer: ProgressRenderer,
}
//}

//image[aoi-progress][プログレスバーの表示例]


==={cellrenderer-icon} アイコン付きテキスト

アイコンライブラリと組み合わせると、さらに表現の幅が広がります。
ここでは@<code>{lucide-react}を使いますが、@<code>{react-icons}など他のライブラリでも同様です。

//cmd{
npm install lucide-react
//}

//list[cellrenderer-icon-code][アイコン付きステータス]{
import { CheckCircle, Clock, CircleDashed } from "lucide-react";

const statusIcons: Record<
  Todo["status"],
  { icon: React.ReactNode; color: string }
> = {
  todo: { icon: <CircleDashed size={14} />, color: "#616161" },
  doing: { icon: <Clock size={14} />, color: "#1565c0" },
  done: { icon: <CheckCircle size={14} />, color: "#2e7d32" },
};

// カラム定義
{
  field: "status",
  headerName: "ステータス",
  cellRenderer: (params: CustomCellRendererProps<Todo>) => {
    const item = statusIcons[params.value as Todo["status"]];
    const label = 
      params.value === "todo" ? "未着手" :
      params.value === "doing" ? "進行中" :
      "完了";

    return (
      <div style={{
        display: "flex",
        alignItems: "center",
        gap: "4px",
        color: item.color,
      }}>
        {item.icon}
        <span>{label}</span>
      </div>
    );
  },
}
//}

//image[aoi-icon][アイコン付きステータスの表示例]

==={cellrenderer-params} 操作ボタンの配置（cellRendererParams）

セルにボタンを配置して、クリック時に何かアクションを実行することもできます。
レンダラーに追加のパラメータを渡したい場合は@<code>{cellRendererParams}を使います。

//list[cellrenderer-params-code][操作ボタン付きセル]{
const ActionRenderer = (
  props: CustomCellRendererProps<Todo> & {
    onDelete: (id: number) => void;
  }
) => {
  return (
    <button
      onClick={() => props.onDelete(props.data!.id)}
      style={{
        padding: "2px 8px",
        fontSize: "0.85em",
        cursor: "pointer",
      }}
    >
      削除
    </button>
  );
};

// カラム定義
{
  headerName: "操作",
  width: 100,
  cellRenderer: ActionRenderer,
  cellRendererParams: {
    onDelete: (id: number) => {
      console.log("削除:", id);
    },
  },
}
//}
//image[aoi-delete][操作ボタンの表示例]

@<code>{cellRendererParams}で渡した値はレンダラーのpropsにマージされるため、
コールバック関数を外から注入できます。
レンダラーの中でデータの更新やAPI呼び出しの処理を直接書かずに済むので、
責務の分離という点でも有用です。


=={celleditor} セルを編集可能にする - cellEditor


==={celleditor-intro} cellEditorとは

ここまではセルの「表示」をカスタマイズしてきましたが、AG Gridではセルの「編集」も簡単に実現できます。

@<code>{cellEditor}は、セルをクリック（またはダブルクリック）したときに
表示される編集UIを制御する機能です。
AG Gridにはテキスト入力やセレクトボックスなどの組み込みエディタが用意されており、
独自のカスタムエディタを作ることもできます。

==={celleditor-text} テキスト編集

最もシンプルなパターンです。
カラム定義に@<code>{editable: true}を追加するだけで、
セルをクリックするとテキスト入力欄が表示されるようになります。

//list[celleditor-text-code][テキスト編集]{
{
  field: "title",
  headerName: "タスク",
  editable: true,
}
//}

//image[aoi-edit-task][テキスト編集例]

たった1行の追加でセル編集が有効になります。デフォルトのエディタは@<code>{agTextCellEditor}で、特にエディタを指定しなければテキスト入力になります。

//pagebreak
==={celleditor-select} セレクトボックス

ステータスのように決まった選択肢から選ばせたい場合は、
組み込みの@<code>{agSelectCellEditor}を指定します。

//list[celleditor-select-code][セレクトボックス]{
{
  field: "status",
  headerName: "ステータス",
  editable: true,
  cellEditor: "agSelectCellEditor",
  cellEditorParams: {
    values: ["未着手", "進行中", "完了"],
  },
}
//}

//image[aoi-edit-status][セレクトボックス例]

==={celleditor-custom} カスタムエディタ

組み込みエディタでは表現しきれない場合、
Reactコンポーネントとして独自のエディタを作ることもできます。

カスタムエディタでは@<code>{useImperativeHandle}を使って
@<code>{getValue()}メソッドを公開する必要があります。
AG Gridは編集完了時にこのメソッドを呼び出して、新しい値を取得します。

//list[celleditor-custom-code][カスタムセレクトエディタ]{
import {
  forwardRef,
  useEffect,
  useImperativeHandle,
  useRef,
  useState,
} from "react";
import type { CustomCellEditorProps } from "ag-grid-react";

interface Props extends CustomCellEditorProps<Todo> {
  options: { value: string; label: string }[];
}

export const SelectEditor = forwardRef((props: Props, ref) => {
  const [value, setValue] = useState(props.value);
  const selectRef = useRef<HTMLSelectElement>(null);

  useEffect(() => {
    selectRef.current?.focus();
  }, []);

  useImperativeHandle(ref, () => ({
    getValue() {
      return value;
    },
  }));

  return (
    <select
      ref={selectRef}
      value={value}
      onChange={(e) => setValue(e.target.value)}
      style={{ width: "100%", height: "100%" }}
    >
      {props.options.map((opt) => (
        <option key={opt.value} value={opt.value}>
          {opt.label}
        </option>
      ))}
    </select>
  );
});

//カラム定義
{
  field: "status",
  headerName: "ステータス",
  editable: true,
  cellEditor: SelectEditor,
  cellEditorParams: {
    options: [
      { value: "todo", label: "🔘 未着手" },
      { value: "doing", label: "🔄 進行中" },
      { value: "done", label: "✅ 完了" },
    ],
  },
}
//}

//image[aoi-celleditor-custom][カスタムエディタ例]

組み込みの@<code>{agSelectCellEditor}との違いは、
選択肢の表示を自由にカスタマイズできる点です。
絵文字を付けたり、@<code>{optgroup}でグループ分けしたりといった表現ができます。

==={celleditor-validation} バリデーション

編集された値を保存する前にチェックしたい場合は、@<code>{valueSetter}を使います。

//list[celleditor-validation-code][valueSetterでバリデーション]{
{
  field: "progress",
  headerName: "進捗",
  editable: true,
  valueSetter: (params) => {
    const newValue = Number(params.newValue);

    if (isNaN(newValue) || newValue < 0 || newValue > 100) {
      // false を返すと編集がキャンセルされる
      return false;
    }

    params.data.progress = newValue;
    return true;
  },
}
//}

@<code>{valueSetter}が@<code>{false}を返すと、セルの値は変更されません。
ここではシンプルに編集をキャンセルするだけですが、
次に紹介する@<code>{cellStyle}と組み合わせれば、
不正な値のセルを赤くハイライトするといった表現もできます。

=={cellstyle} セルのスタイルを条件で変える — cellStyle / cellClassRules

==={cellstyle-intro} cellRendererを使わないスタイリング

前の章ではcellRendererでセルの描画ごと置き換えましたが、
「背景色を変えたい」「フォントを太くしたい」程度であれば、
@<code>{cellStyle}や@<code>{cellClassRules}の方がシンプルです。

セルの描画はそのままに、スタイルだけを条件に応じて変更できます。

==={cellstyle-basic} cellStyleで色を変える

@<code>{cellStyle}に関数を渡すと、セルの値に応じたインラインスタイルを返せます。

//list[cellstyle-basic-code][cellStyleで条件付きスタイル]{
import type { CellStyle } from "ag-grid-community";

{
  field: "progress",
  headerName: "進捗",
  cellStyle: (params): CellStyle => {
    const value = params.value ?? 0;
    if (value === 100) {
      return { backgroundColor: "#c8e6c9", fontWeight: "bold" };
    }
    if (value === 0) {
      return { backgroundColor: "#ffcdd2" };
    }
    return {};
  },
}
//}

//image[aoi-cellstyle][条件付きスタイルの表示例]

1つ注意点があります。
条件に該当しない場合は空のオブジェクト@<code>{{}}を返すようにしてください。
@<code>{undefined}や@<code>{null}を返すと、
以前に適用されたスタイルがリセットされないことがあります。

==={cellstyle-classrules} cellClassRulesでCSSクラスを切り替える

@<code>{cellClassRules}を使うと、条件に応じてCSSクラスを動的に付け外しできます。

//list[cellclass-basic-code][cellClassRulesでクラス切り替え]{
{
  field: "status",
  headerName: "ステータス",
  cellClassRules: {
    "cell-todo":  (params) => params.value === "todo",
    "cell-doing": (params) => params.value === "doing",
    "cell-done":  (params) => params.value === "done",
  },
}
//}

//pagebreak

対応するCSSを用意します。

//list[cellclass-css][CSS定義]{
.cell-todo {
  background-color: #e0e0e0;
  color: #616161;
}
.cell-doing {
  background-color: #bbdefb;
  color: #1565c0;
}
.cell-done {
  background-color: #c8e6c9;
  color: #2e7d32;
}
//}


//image[aoi-cellClassRules][cellClassRulesでのスタイル切り替え例]

==={cellstyle-heatmap} ヒートマップ風の色分け

応用として、0〜100の数値をHSLの色相を使って
赤→黄→緑のグラデーションで表現する例を紹介します。

//list[cellstyle-heatmap-code][ヒートマップ風スタイリング]{
{
  field: "progress",
  headerName: "進捗",
  cellStyle: (params) => {
    const value = params.value ?? 0;
    // 0→赤(0°) 、50→黄(60°)、100→緑(120°)
    const hue = (value / 100) * 120;
    return {
      backgroundColor: `hsl(${hue}, 70%, 85%)`,
      fontWeight: "bold",
      textAlign: "center",
    };
  },
}
//}

//image[aoi-heatmap][ヒートマップ風の表示例]

HSLの色相（Hue）を0〜120の範囲で動かしています。
0が赤、60が黄、120が緑に対応するため、
数値をそのままパーセンテージとして色にマッピングできます。
スコアや達成率の一覧で使うと、全体の傾向が一目で把握しやすくなります。

=={outro} おわりに

本章では、AG Gridの基本的な使い方と、セルカスタマイズの3つの機能を紹介しました。

 * @<code>{cellRenderer} — セルの描画をJSXで自由に置き換える
 * @<code>{cellEditor} — セルの編集UIをカスタマイズする
 * @<code>{cellStyle} / @<code>{cellClassRules} — 条件に応じてスタイルを変える

AG Gridにはこの他にも、行のグルーピング、ピン留め、
マスター/ディテール表示、サーバーサイドのデータ取得など、
多くの機能があります。
本章ではセルのカスタマイズに絞りましたが、
AG Gridの柔軟さの一端が伝わっていれば幸いです。

公式ドキュメント@<fn>{aggrid-docs}は英語ですが、
インタラクティブなサンプルが豊富に掲載されています。
本章で基本的な使い方を掴んだ上で読むと、かなりスムーズに進められるはずです。

//footnote[aggrid-docs][@<href>{https://www.ag-grid.com/react-data-grid/getting-started/}]