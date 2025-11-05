import pathlib
import re
root = pathlib.Path(r"./")
for path in root.rglob("*"):
    if path.suffix in (".cpp", ".hpp", ".h", ".cc"):
        text = path.read_text(encoding="utf-8", errors="ignore")
        # 各行の先頭のスペースを4→2に再計算
        lines = []
        for line in text.splitlines():
            leading = len(line) - len(line.lstrip(' '))
            new_leading = leading * 2 // 4  # 4スペース → 2スペース
            lines.append(' ' * new_leading + line.lstrip(' '))
        path.write_text("\n".join(lines), encoding="utf-8")
        print(f"Converted: {path}")
