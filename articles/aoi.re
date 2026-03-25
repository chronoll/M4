= AG Gridを使ってみよう 〜セルカスタマイズ編〜
こんにちは、aoi（@4o1sobblue）です。本章では、グリッドライブラリのAG Gridを取り上げ、React + TypeScript環境におけるセルカスタマイズの方法を紹介します。

=={intro} はじめに

==={intro-about} この章について
本章では、グリッドライブラリの1つである@<b>{AG Grid}について、基本的なテーブル表示から、@<b>{セルのカスタマイズ}にフォーカスして解説していきます。
セルにバッジやプログレスバーを表示したり、その場で編集できるようにしたり、
条件に応じて色を変えたりと、AG Gridでどんなことができるのかを
React + TypeScriptを使ったサンプルコードとともに紹介していきます。

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
 * セルを編集可能にし、カスタムエディタを作ることができる
 * 条件に応じてセルのスタイルを動的に変えることができる

==={intro-target} 対象読者
本章は以下のような方を対象としています：

 * React + TypeScriptの基本的な書き方が分かる
 * 多機能でカスタマイズ性のあるグリッドライブラリを探している
 * AG Gridを触ったことがある、またはこれから使う予定
 * AG Gridでセルの見た目や編集機能をカスタマイズしたい

==={intro-versions} 使うバージョン

 * ag-grid-community 35.1.0
 * ag-grid-react 35.1.0
 * React 19.2.0
 * TypeScript 5.9.3

//pagebreak
=={basic} 表を表示してみる

まずは最小構成でテーブルを表示してみましょう。
AG Gridの最小構成は、データ配列とカラム定義の2つを渡すだけで動作します。

本章では、TODO管理をテーマとしたコードを例に解説します。

==={basic-setup} AG Gridのインストール
以下のコマンドでAG Grid（Community版）をインストールします。

//cmd{
npm install ag-grid-react
//}

==={basic-code} コード例

//listnum[app-basic][最小構成のテーブル]{
import { useState } from "react";
import { AgGridReact, AgGridProvider } from "ag-grid-react";
import type { ColDef } from "ag-grid-community";
import { AllCommunityModule } from "ag-grid-community";

// AG Grid v33以降ではModuleの明示的な指定が必要
const modules = [AllCommunityModule];

// 行データの型定義
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
  const [rowData, setRowData] = useState<Todo[]>(initialData);

  // カラム定義
  const [colDefs] = useState<ColDef<Todo>[]>([
    { field: "id", headerName: "ID", width: 80 },
    { field: "title", headerName: "タスク", flex: 1 },
    { field: "status", headerName: "ステータス", width: 120 },
    { field: "progress", headerName: "進捗", width: 120 },
  ]);

  return (
    <AgGridProvider modules={modules}>
      <div style={{ height: 300, width: 700 }}>
        <AgGridReact<Todo> rowData={rowData} columnDefs={colDefs} />
      </div>
    </AgGridProvider>
  );
}
export default App;
//}

//image[aoi-basic-table][最小構成のテーブル表示例]

これだけのコードで、ヘッダーのクリックによるソートや、ドラッグによるカラム幅のリサイズが最初から動作します。

AG Grid v33以降は、利用するモジュールを明示する必要があります。
本章では@<code>{AgGridProvider}の@<code>{modules}に@<code>{AllCommunityModule}を渡し、Community版の機能を利用可能にしています@<fn>{fn-1}。

//footnote[fn-1][本章では便宜上@<code>{AllCommunityModule}を指定していますが、バンドルサイズ削減のため必要なモジュールのみ登録することを推奨します。]

本章ではこれ以降、@<b>{上記のコードを土台とし、変更箇所のみを抜粋}して示します。

=={cellrenderer} セルの見た目をカスタマイズする - cellRenderer

==={cellrenderer-intro} cellRendererとは
先ほどのテーブルでは、ステータスや進捗が文字列として表示されていました。
「ステータスを色付きのバッジにしたい」「進捗をプログレスバーで表示したい」といったカスタマイズには@<code>{cellRenderer} を使います。

@<code>{cellRenderer}は、セルの中身を自由なJSXで描画できる機能です。
カラム定義に関数やReactコンポーネントを指定するだけで使えます。

==={cellrenderer-badge} ステータスバッジ

まずは「ステータス」カラムを色付きバッジで表示してみます。

@<code>{CustomCellRendererProps}のimportと@<code>{statusStyles}を追加し、@<code>{colDefs}のステータス列（@<code>{field:"status"}）を、次の定義に差し替えます。

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

ステータスごとに色とラベルが付いたことで、一覧を流し見したときにも状態の違いがすぐ分かるようになりました。

==={cellrenderer-progress} プログレスバー

次に「進捗」カラムをプログレスバーで表示してみます。

@<code>{ProgressRenderer}を定義し、@<code>{colDefs}の進捗列（@<code>{field:"progress"}）を次のように差し替えます。

//pagebreak
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

進捗をバーで表すことで、数値を読み込まなくても「どのタスクがどの程度進んでいるか」を直感的に把握できるようになりました。

==={cellrenderer-icon} アイコン付きテキスト

アイコンライブラリと組み合わせると、さらに表現の幅が広がります。
ここでは@<code>{lucide-react}を使いますが、@<code>{react-icons}など他のライブラリでも同様です。

以下のコマンドで@<code>{lucide-react}をインストールします。
//cmd{
npm install lucide-react
//}

@<code>{statusIcons}を定義し、ステータス列を次の定義に置き換えます。
//pagebreak
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

アイコンを併用すると、色だけに頼らず状態を識別できるため、視認性を上げつつ表現の幅も広げられます。

ここまでは表示の工夫でしたが、次はセルにボタンを置き、行ごとの操作を親コンポーネントから渡して実行する方法を見ていきます。

==={cellrenderer-params} 操作ボタンの配置（cellRendererParams）

セルにボタンを配置して、クリック時に何かアクションを実行することもできます。
レンダラーに追加のパラメータを渡したい場合は@<code>{cellRendererParams}を使います。

@<code>{ActionRenderer}を定義し、@<code>{colDefs}に操作列として次の列を追加します。
削除処理はレンダラー内に直接書かず、親コンポーネント側で@<code>{setRowData}を使って実行する関数を@<code>{cellRendererParams}経由で渡します。

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
      setRowData((prev) => prev.filter((row) => row.id !== id));
    },
  },
}
//}
//image[aoi-delete][操作ボタンの表示例]

@<code>{cellRendererParams}で渡した値はレンダラーのpropsにマージされるため、コールバック関数を外から注入できます。
レンダラーの中でデータの更新やAPI呼び出しの処理を直接書かずに済むので、責務の分離という点でも有用です。


=={celleditor} セルを編集可能にする - cellEditor


==={celleditor-intro} cellEditorとは

ここまではセルの「表示」をカスタマイズしてきましたが、AG Gridではセルの「編集」も簡単に実現できます。

@<code>{cellEditor}は、編集を開始したあとにセル内に表示されるUIを制御する機能です。編集の開始は、デフォルトでは@<b>{セルのダブルクリック}です（ほかに Enter や F2 などでも開始できます）@<fn>{single-click}。
//footnote[single-click][シングルクリックで編集に入りたい場合は、列定義で@<code>{singleClickEdit:true}を指定します。]
AG Gridにはテキスト入力やセレクトボックスなどの組み込みエディタが用意されており、
独自のカスタムエディタを作ることもできます。

==={celleditor-text} テキスト編集

最もシンプルなパターンです。
カラム定義に@<code>{editable:true}を追加するだけで、セルが編集可能になります。

@<code>{colDefs}のタスク列（@<code>{field:"title"}）に@<code>{editable:true}を指定します。

//list[celleditor-text-code][テキスト編集]{
{
  field: "title",
  headerName: "タスク",
  editable: true,
}
//}

//image[aoi-edit-task][テキスト編集例]

@<code>{editable:true}でセル編集が有効になります。デフォルトのエディタは@<code>{agTextCellEditor}で、別途指定しなければテキスト入力です。

候補が決まっている列では、自由入力ではなくプルダウンにした方が表示（バッジやアイコン）と値の整合がとりやすくなります。次にセレクトボックスの例を示します。

#@# //pagebreak
==={celleditor-select} セレクトボックス

ステータスのように決まった選択肢から選ばせたい場合は、
組み込みの@<code>{agSelectCellEditor}を指定します。

@<code>{colDefs}のステータス列を、編集可能かつセレクトエディタを用いる定義に差し替えます。
プルダウンに限定すれば、先ほどのバッジやアイコン表示と矛盾しない値だけが入ります。@<code>{agSelectCellEditor}と@<code>{cellEditorParams.values}は、次のように書きます。

//list[celleditor-select-code][セレクトボックス]{
{
  field: "status",
  headerName: "ステータス",
  editable: true,
  cellEditor: "agSelectCellEditor",
  cellEditorParams: {
    values: ["todo", "doing", "done"],
  },
}
//}

//image[aoi-edit-status][セレクトボックス例]

ここではデータの内部値を@<code>{todo}/@<code>{doing}/@<code>{done}で統一したいため、@<code>{values}も英語キーを使っています。
編集時の選択肢まで日本語にしたい場合は、次に示すカスタムエディタで@<code>{value}と@<code>{label}を分けるのが分かりやすいです。

==={celleditor-custom} カスタムエディタ

組み込みエディタでは表現しきれない場合、Reactコンポーネントとして独自のエディタを作ることもできます。

カスタムエディタでは@<code>{useImperativeHandle}を使って@<code>{getValue()}メソッドを公開する必要があります。
AG Gridは編集完了時にこのメソッドを呼び出して、新しい値を取得します。

@<code>{SelectEditor}コンポーネントを新たに定義し、ステータス列を次のとおり差し替えます。

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

// カラム定義
{
  field: "status",
  headerName: "ステータス",
  editable: true,
  cellEditor: SelectEditor,
  cellEditorParams: {
    options: [
      { value: "todo", label: "未着手 (todo)" },
      { value: "doing", label: "進行中 (doing)" },
      { value: "done", label: "完了 (done)" },
    ],
  },
}
//}

//image[aoi-celleditor-custom][カスタムエディタ例]

組み込みの@<code>{agSelectCellEditor}との違いは、選択肢の表示を自由にカスタマイズできる点です。
@<code>{optgroup}でグループ分けしたり、表示専用の接頭辞や記号を付けたりといった表現ができます。

==={celleditor-validation} バリデーション

編集された値を保存する前にチェックしたい場合は、@<code>{valueSetter}を使います。

@<code>{colDefs}の進捗列に@<code>{editable:true}および@<code>{valueSetter}を指定します。
@<code>{valueSetter}は、新しい値を受け取り、採用するかどうかを返します。範囲外なら@<code>{false}で編集を巻き戻し、通ったときだけ@<code>{params.data}を更新します。

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

@<code>{valueSetter}が@<code>{false}を返すと、セルの値は変更されません。ここではシンプルに編集をキャンセルするだけですが、
次に紹介する@<code>{cellStyle}と組み合わせれば、不正な値のセルを赤くハイライトするといった表現もできます。

=={cellstyle} セルのスタイルを条件で変える — cellStyle / cellClassRules

==={cellstyle-intro} cellRendererを使わないスタイリング

先ほどはcellRendererでセルの描画ごと置き換えましたが、「背景色を変えたい」「フォントを太くしたい」程度であれば、
@<code>{cellStyle}や@<code>{cellClassRules}の方がシンプルです。

セルの描画はそのままに、スタイルだけを条件に応じて変更できます。

==={cellstyle-basic} cellStyleで色を変える

@<code>{cellStyle}に関数を渡すと、セルの値に応じたインラインスタイルを返せます。

@<code>{colDefs}の進捗列に@<code>{cellStyle}を指定します。@<code>{CellStyle}型は@<code>{ag-grid-community}からimportします。
セル全体を@<code>{cellRenderer}で描き直さなくても、値に応じた背景色や太字だけを足せます。条件に合わないときは空のオブジェクトを返し、スタイルを上書きしないようにします。

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


==={cellstyle-classrules} cellClassRulesでCSSクラスを切り替える

@<code>{cellClassRules}を使うと、条件に応じてCSSクラスを動的に付け外しできます。

@<code>{colDefs}のステータス列に@<code>{cellClassRules}を指定します。

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

#@# //pagebreak

続いて、列定義で付けたクラス名に対応するスタイルを定義します。


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


//image[aoi-cellclassrules][cellClassRulesでのスタイル切り替え例]

==={cellstyle-heatmap} ヒートマップ風の色分け

応用として、0〜100の数値をHSLの色相を使って赤→黄→緑のグラデーションで表現する例を紹介します。

@<code>{colDefs}の進捗列における@<code>{cellStyle}を、次の定義に差し替えます。

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
    };
  },
}
//}

//image[aoi-heatmap][ヒートマップ風の表示例]

HSLの色相（Hue）を0〜120の範囲で動かしています。0が赤、60が黄、120が緑に対応するため、
数値をそのままパーセンテージとして色にマッピングできます。スコアや達成率の一覧で使うと、全体の傾向が一目で把握しやすくなります。

#@# //pagebreak
=={outro} おわりに

本章では、AG Gridの基本的な使い方と、セルカスタマイズの主な機能を紹介しました。

 * @<code>{cellRenderer} — セルの描画をJSXで自由に置き換える
 * @<code>{cellEditor} — セルの編集UIをカスタマイズする
 * @<code>{cellStyle} / @<code>{cellClassRules} — 条件に応じてスタイルを変える
 * @<code>{valueSetter} — 編集値の検証・反映

AG Gridにはこの他にも、行のグルーピング、ピン留め、マスター/ディテール表示、サーバーサイドのデータ取得など、多くの機能があります。
本章ではセルのカスタマイズに絞りましたが、AG Gridの柔軟さの一端が伝わっていれば幸いです。

公式ドキュメント@<fn>{aggrid-docs}は英語ですが、インタラクティブなサンプルが豊富に掲載されています。
本章で基本的な使い方を掴んだ上で読むと、かなりスムーズに進められるはずです。

//footnote[aggrid-docs][@<href>{https://www.ag-grid.com/react-data-grid/getting-started/}]