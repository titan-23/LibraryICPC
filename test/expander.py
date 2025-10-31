import re
import sys
from pathlib import Path

def expand_cpp_includes():
    # 1. コマンドライン引数をチェック
    if len(sys.argv) < 2:
        print(f"[エラー] 処理対象のC++ファイル名を指定してください。")
        print(f"使い方: python3 {sys.argv[0]} <入力ファイル.cpp>")
        sys.exit(1)

    try:
        # 2. このスクリプト (expander.py) があるディレクトリを取得
        script_dir = Path(__file__).parent.resolve()

        # 3. 各パスを定義
        input_cpp_path = Path(sys.argv[1]).resolve()
        
        # (修正) 入力ファイルの「親ディレクトリ」を取得
        # (例: /.../test/algorithm/mo.cpp の場合、/.../test/algorithm/ を取得)
        input_file_dir = input_cpp_path.parent 
        
        # 出力先はスクリプトと同じ階層
        output_cpp_path = script_dir / "combined.cpp" 

        # #include "..." を見つけるための正規表現
        include_regex = re.compile(r'^\s*#include\s*"([^"]+)"\s*$')

        processed_lines = []

        # 4. 引数で指定された入力ファイルを読み込む
        with open(input_cpp_path, 'r', encoding='utf-8') as f_main:
            print(f"'{input_cpp_path.name}' を読み込んでいます...")
            
            for line in f_main:
                match = include_regex.match(line)
                
                if match:
                    header_name = match.group(1) # 例: ../../akakoi_lib/template.cpp
                    
                    # (修正) ヘッダパスを「入力ファイルの場所」を基準に解決
                    # (input_file_dir / header_name) でパスを結合
                    # .resolve() で '..' などを解釈して絶対パスにする
                    header_path = (input_file_dir / header_name).resolve()

                    try:
                        with open(header_path, 'r', encoding='utf-8') as f_header:
                            print(f"  -> '{header_name}' ( {header_path.name} ) を展開しています...")
                            header_content = f_header.read()
                            
                            processed_lines.append(f"// --- Start of include: {header_name} ---\n")
                            processed_lines.append(header_content)
                            processed_lines.append(f"\n// --- End of include: {header_name} ---\n")
                            
                    except FileNotFoundError:
                        # (修正) 警告メッセージで、解決後のフルパスを表示
                        print(f"  [警告] ヘッダファイルが見つかりません: {header_path}")
                        processed_lines.append(line)
                else:
                    processed_lines.append(line)

        # 5. スクリプトと同じ階層に combined.cpp を書き出す
        with open(output_cpp_path, 'w', encoding='utf-8') as f_out:
            f_out.writelines(processed_lines)
            
        print(f"\n展開が完了しました！ -> {output_cpp_path}")

    except FileNotFoundError:
        print(f"[エラー] 入力ファイルが見つかりません: {input_cpp_path}")
    except Exception as e:
        print(f"[エラー] 予期せぬエラーが発生しました: {e}")

if __name__ == "__main__":
    expand_cpp_includes()