EASTL ライブラリをビルドします。

Tools/Bash.cmd で窓を開いておいて、以下のように作業します。

```
$ cd ../EASTL/build_twelite
$ make TWELITE=BLUE all deplopy
...
$ make TWELITE=RED all deplopy
...
```

ライブラリは `../../../lib` にコピーされます。