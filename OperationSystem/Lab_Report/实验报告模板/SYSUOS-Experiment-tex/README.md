# sysucseexp.sty
一个适用于中山大学科技类及IT类课程实验报告排版的LaTeX模板。

使用该模板时，文件组织建议按如下方式存储：
```
jobname(根目录)
│             
├── codes(报告中需要插入的代码源文件，可以有多个，根据需要增减)
│   └── ex04-01.cpp
├── figs(插图目录，根据需要增减)
│   ├── xxx.pdf
│   ├── xxx.png
│   └── xxx.jpg
├── settings(自定义命令、环境等文件、引入宏包文件，可根据需要进行调整)
│   ├── format.tex(自定义命令、环境、宏包设置等)
│   ├── packages.tex(需要引入的宏包)
│   └── terms.tex(自定义术语命令)
├── boxie.sty(盒子宏包，如果需要则置于根目录，并在settings/package.tex中引用该宏包)
├── fvextra.sty(盒子宏包需要的宏包，如果使用了boxie宏包，则必须置于根目录)
├── lstlinebgrd.sty(盒子宏包需要的宏包，如果使用了boxie宏包，则必须置于根目录)
├── main.tex(主控文件，必须存在，且置于根目录)
├── Makefile(make命令需要的脚本文件，如能执行make，可以在根目录执行make命令进行编译)
├── sysucseexp.cls(文档类文件，即模板文件，必须存在，且置于根目录)
├── pgf-umlcd.sty(UML图绘制宏包，如果需要则置于根目录，并在settings/package.tex中引用该宏包)
├── tikz-flowchart.sty(流程图绘制宏包，如果需要则置于根目录，并在settings/package.tex中引用该宏包)
├── tikz-imglabels.sty(图像标注/标记宏包，如果需要则置于根目录，并在settings/package.tex中引用该宏包)
└── .latexmkrc(latexmk命令需要的脚本文件，如能执行latexmk，可以在根目录执行latexmk命令进行编译)
```
Happy LaTeXing!~
## 使用说明
---------------------
1. 目前支持中山大学计算机学院实验报告。
2. 请使用`\documentclass{sysucseexp}`命令引入`sysucseexp`文档类。

   引入`sysucseexp`文档类后，使用`\sysucseset`命令设置报告封面信息
   ```
   % 设置封面基本信息，\linebreak 前面不要有空格，
   % 中文之间的空格无法消除
   % 另，在\nwafuset中不可以出现空行
   \sysucseset{
     college = {计算机学院},            % 学院名称
     projname = {操作系统原理实验},      % 实验课程名称
     title = {从实模式到保护模式},       % 实验题目
     stuno = {21307333},                % 学号
     author = {叶文洁},               % 姓名
     major = {计算机科学与技术},          % 专业
     adviser = {刘慈欣},                   % 任课教师姓名
     startdate = {2023年3月1日},        % 实验起始日期
     enddate = {4月1日},                % 实验结束日期
   }
   ```

3. 推荐的文档编译方式

如果采用minted宏包排版代码，则使用如下4次编译：
```
	xelatex -shell-escape main.tex
	biber main
	xelatex -shell-escape main.tex
	xelatex -shell-escape main.tex
```
如果采用listings宏包排版代码，则使用如下4次编译：
```
	xelatex main.tex
	biber main
	xelatex main.tex
	xelatex main.tex
```
或者采用minted宏包排版代码，用`latexmk`命令编译：

```
	latexmk
```

4. 发现个别英文无法正确断行，原因未找到，加了一个`\sloppy`命令进行暂时处理，这会造成当前行字距松散，请适当加入文字解决松散问题。

## 注意

1. 本文档要求 TeXLive、MacTeX、MikTeX 不低于 2020 年的发行版，并且尽可能升级到最新，强烈建议使用TeXLive2020。

2. 使用说明见：**main.pdf**。

2. **不支持** [CTeX 套装](http://www.ctex.org/CTeXDownload)。

## 主要功能
- 方便中山大学科技类或IT类实验报告排版；
- 定制摘要、关键词、各级节标题、表格、页眉页脚等样式；
- 使用自己开发的[boxie.sty宏包](https://github.com/registor/boxiesty)实现终端窗口模拟、各类代码文本框和“注意”、“重要”、“技巧”和“警告”等文本框；
- 使用自己开发的[tikz-flowchart.sty宏包](https://github.com/registor/tikz-flowchart)实现流程图的绘制；
- 使用改造于`tikz-imagelabels`宏包的`tikz-imglabels`宏包实现插图的注解；
- 使用`floatrow`宏包控制图表浮动体；

## 注意
- 由于boxie.sty宏包需要使用fvextra.sty和lstlinebgrd.sty两个宏包处理间隔显示代码行颜色，请确保当前路径下有这两个宏包存在。
