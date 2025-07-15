# fcmp

Utility for byte-by-byte file comparison

## Usage

Compare two files: `path\to\file1.ext` and `path\to\file2.ext`

```
fcmp.exe "path\to\file1.ext" "path\to\file2.ext"
```

If all bytes of two files match, the result will be the statement that the files match. Otherwise, the differences will be output (no more than `DIFFS_MAX` = 10) in format:

```
[0000000000000038]: 7470733A2F2F6368 != 6C2D442065786974
[0000000000000040]: 726973616E743939 != 7320636D642E6578
[0000000000000048]: 362E676974687562 != 65207768656E2069
```

If the files are of different sizes, then the comparison is made by the length of the smaller file. And if within the length of the smallest file the contents of the two files are the same, then the result will be the statement that the files are the same, despite the fact that they are obviously different.
