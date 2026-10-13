%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% sysucse_Experiment_paper - 中山大学实验报告模板
%% 2023年3月
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{sysucseexp}[March, 2023 LaTeX cls for SYSU Project Report]
\RequirePackage{kvoptions}
\DeclareDefaultOption{\PassOptionsToClass{\CurrentOption}{ctexart}}
\ProcessKeyvalOptions*
\PassOptionsToClass{zihao=-4}{ctexart}% 指定\normalsize为小四号字体
\PassOptionsToClass{scheme=chinese,space=auto}{ctexart}%a4paper,
% 载入ctexart宏包
\LoadClass{ctexart}

\RequirePackage{ulem}% 下划线
%\RequirePackage{array}
% 内部命令
% 封面条目标签
\newcommand\sysucse@label@sysu{中山大学}
\newcommand\sysucse@label@title{题目}
\newcommand\sysucse@label@stuno{学号}
\newcommand\sysucse@label@author{姓名}
\newcommand\sysucse@label@major{专业名称}
\newcommand\sysucse@label@adviser{任课教师}
\newcommand\sysucse@label@date{实验时间}
\newcommand\sysucse@label@labroom{实验地点}
% 封面条目内容变量
\SetupKeyvalOptions{
  family=sysucse,
  prefix=sysucse@,
  setkeys=\kvsetkeys
}
\def\sysucseset{\kvsetkeys{sysucse@value}}
\def\sysucse@define #1{
  \define@key{sysucse}{#1}{\csname #1\endcsname{##1}}
  \expandafter\gdef\csname #1\endcsname##1{
    \expandafter\gdef\csname sysucse@#1\endcsname{##1}}
  \csname #1\endcsname{}
}
\sysucse@define{value@college}
\sysucse@define{value@projname}
\sysucse@define{value@title}
\sysucse@define{value@stuno}
\sysucse@define{value@author}
\sysucse@define{value@major}
\sysucse@define{value@adviser}
\sysucse@define{value@date}
\sysucse@define{value@labroom}
\sysucse@define{value@startdate}
\sysucse@define{value@enddate}

% 排版封面命令
\def\makecover{
    \hypersetup{
      pdftitle = {\sysucse@value@title},
      pdfauthor = {\sysucse@value@author}
    }
  \pagestyle{empty}
  \cleardoublepage
  \sysucse@make@cover@sysucseexp
}

% 定义表格列格式
%\newcolumntype{L}[1]{>{\raggedright\let\newline\\\arraybackslash\hspace{0pt}}m{#1}}
%\newcolumntype{C}[1]{>{\centering\let\newline\\\arraybackslash\hspace{0pt}}p{#1}}
%\newcolumntype{R}[1]{>{\raggedleft\let\newline\\\arraybackslash\hspace{0pt}}m{#1}}

% 封面内容排版
\newcommand\sysucse@make@cover@sysucseexp{
  \cleardoublepage
  \newgeometry{top=1.0in, bottom=1.0in, left=1.25in, right=1.25in}
  \vfill
  \begin{center}
    \vspace{\stretch{1.5}}
    \begin{figure}[!htp]
          \includegraphics[width=0.5\textwidth]{figs/SYSULogo.png}
    \end{figure}
    \linespread{1.5}\kaishu\bfseries\Huge
    
    本科生\uline{操作系统原理实验}报告
    
    \vspace{\stretch{1.0}}
  \end{center}

  \begin{center}
    \linespread{1.212}%\heiti%\Huge%\fontsize{22bp}{26.4bp}\selectfont%\zihaoer%
    \begin{tabular}{>{\heiti\huge}c>{\heiti\huge}l}    
    {\makebox[3\ccwd][s]{\sysucse@label@title}：} &    
    {\expandafter\uline\expandafter{\sysucse@value@title}}
    \end{tabular}
    \vspace{\stretch{1.5}}
    
    \linespread{3.0}\bfseries\songti\zihao{3}% 目的是计算字符占有宽度
    \newlength{\lablen}
    \setlength{\lablen}{\widthof{\sysucse@label@adviser}}
    \begin{tabular} {>{\bfseries\songti\zihao{3}}c>{\bfseries\songti\zihao{3}}c}%
      \makebox[\the\lablen][s]{\sysucse@label@stuno} & \sysucse@value@stuno \\ \cline{2-2}%
      \makebox[\the\lablen][s]{\sysucse@label@author} & \sysucse@value@author \\ \cline{2-2}
      \makebox[\the\lablen][s]{\sysucse@label@major} & \sysucse@value@major \\ \cline{2-2}      
      \makebox[\the\lablen][s]{\sysucse@label@adviser} & \sysucse@value@adviser \\ \cline{2-2}
      \makebox[\the\lablen][s]{\sysucse@label@labroom} & \sysucse@value@labroom \\ \cline{2-2}
      \makebox[\the\lablen][s]{\sysucse@label@date} & {\sysucse@value@startdate---\sysucse@value@enddate} \\ \cline{2-2}
    \end{tabular}
  \end{center}
   \vfill
  \restoregeometry
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 其它需要的宏包
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{etoolbox}% 为命令和环境打补丁的宏包
\RequirePackage{enumitem}% 调整列表格式
\RequirePackage{amsmath,mathrsfs,amsfonts} % 数学相关
\RequirePackage{booktabs}% 三线表格
\RequirePackage{longtable}% 跨页长表格
\RequirePackage{multirow,makecell}% 表格行合并，单元格处理
\RequirePackage{zhlineskip}% 中文行距调整
\RequirePackage[backend=biber,
                style=gb7714-2015,
                maxbibnames=99,% 著录所有作者
                maxcitenames=2,% 引用标注中最多显示2个作者
                mincitenames=1,% 3个及3个以上的作者截断为1个作者
                gbpub=false,
                gbnamefmt=familyahead,
                url=false,
                doi=false,
                isbn=false,
                gbfieldtype=true, % 输出学位论文标识
                ]{biblatex} % 参考文献
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 标题设置
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{caption}
\def\fnum@figure{\figurename\thefigure}% 删除“图”与编号间的空格
\def\fnum@table{\tablename\thetable}% 删除“表”与编号间的空格
\DeclareCaptionLabelSeparator{labsep}{\hspace{0.5em}}% 图表与标题间的分隔符
\DeclareCaptionFont{hei}{\small\heiti}% 字体
\captionsetup[table]{labelsep=labsep, belowskip=0pt,aboveskip=5pt,labelfont=hei,textfont=hei}
\captionsetup[figure]{labelsep=labsep, belowskip=0pt,aboveskip=5pt,format=hang,labelfont=hei,textfont=hei}

\RequirePackage{circledsteps}% 带圈数字

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 将列表项缩进方式改为首行缩进的中文格式
%% 需要enumitem宏包
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\setlist[description]{
    topsep=0pt,
    itemsep=0pt,
    partopsep=0pt,
    parsep=\parskip,
    labelindent=\parindent,
    labelwidth=1.3em,
    labelsep = .3em,
    itemindent=3.6em,
    leftmargin=!,
}

\setlist[enumerate]{
    topsep=0pt,
    itemsep=0pt,
    partopsep=0pt,
    parsep=\parskip,
    label={\arabic*)},
    labelindent=\parindent,
    labelwidth=1.3em,
    labelsep = .3em,
    itemindent=3.6em,
    leftmargin=!,
}
\setlist[itemize]{
    topsep=0pt,
    itemsep=0pt,
    partopsep=0pt,
    parsep=\parskip,
    labelindent=1.4em,
    labelwidth=1.3em,
    labelsep = .3em,
    itemindent=3em,
    leftmargin=!,
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% hyperref settings
%% 超链接设定
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{hyperref}
\hypersetup{
  bookmarksnumbered,
  colorlinks,
  linkcolor={black},
  %citecolor={black},
  %urlcolor={black}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 交叉引用增强宏包(需在hyperref后引入)
%% \autoref命令
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ===========设置引用标记名称============
\def\equationautorefname~#1\null{公式(#1)\null}%
\def\footnoteautorefname{脚注}%
\def\itemautorefname~#1\null{第#1项\null}%
%\def\figureautorefname{图}%
\def\figureautorefname~#1\null{图#1\null}%
%\def\tableautorefname{表}%
\def\tableautorefname~#1\null{表#1\null}%
\def\sectionautorefname~#1\null{第#1节\null}%
\def\subsectionautorefname~#1\null{第#1小节\null}%
\def\subsubsectionautorefname~#1\null{第#1小小节\null}%
\def\theoremautorefname{定理}%
\def\pageautorefname~#1\null{第~#1~页\null}%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 图片搜索路径设定
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{graphicx}
\graphicspath{{./figs/}{./figure/}{./figures/}{./image/}{./images/}{./graphics/}{./graphic/}{./pictures/}{./picture/}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% geometry settings
%% 页面设定
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{geometry}
\geometry{
  paperwidth=210mm,
  paperheight=297mm,
  left=28mm,
  right=22mm,
  top=25mm,
  bottom=25mm,
  headheight=20mm,
  headsep=3mm,
  footskip=10mm,
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 禁止段末孤字成行
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\xeCJKsetup{CJKecglue={\hskip 0.10em plus 0.05em minus 0.05em},CheckSingle=true}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 标题格式设定
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\ctexset{
  %tocdepth=1,
  section = {
    format+ = \zihao{4} \heiti \raggedright,
    name = {,、},
    number = \chinese{section},
    beforeskip = 1.0ex plus 0.2ex minus .2ex,
    afterskip = 1.0ex plus 0.2ex minus .2ex,
    aftername = \hspace{0.2\ccwd}
  },
  subsection = {
    format+ = \zihao{-4} \heiti \raggedright,
    % name={\thesubsection、},
    name = {(,)},
    number = \arabic{subsection},
    beforeskip = 1.0ex plus 0.2ex minus .2ex,
    afterskip = 1.0ex plus 0.2ex minus .2ex,
    aftername = \hspace{0.3\ccwd},
    %indent=2\ccwd,%
  },
  subsubsection = {
    %name={,},%
    number=\Circled{\arabic{subsubsection}},%
    format+ = \zihao{-4} \songti \raggedright,
    beforeskip = 1.0ex plus 0.2ex minus .2ex,
    afterskip = 1.0ex plus 0.2ex minus .2ex,
    aftername = \hspace{0.3\ccwd},
    %indent=2\ccwd,%
  }
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 目录设置
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\setcounter{tocdepth}{1}
\RequirePackage{titletoc}   
\renewcommand{\contentsname}{\begin{center} \heiti\zihao{3}目\hspace{\ccwd}录\end{center}\vspace*{2ex}}    
\titlecontents{section}[0em]{\songti \zihao{-4}}
    {\thecontentslabel{}}{}
    {\hspace{.5em}\titlerule*[5pt]{$\cdot$}\contentspage}
    
% 调整目录行距
\pretocmd{\section}{\addtocontents{toc}{\protect\addvspace{3ex}}}{}{}    

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 页眉页脚
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{fancyhdr}

% 定义正文页码样式(规范无页眉)
\fancypagestyle{main}{
  % 重置页码计数器 
  \setcounter{page}{1}
  % 页眉置为空
  \fancyhead{}
  % 页脚置为空
  \fancyfoot{}
  % 页脚居中显示页码
  \fancyfoot[C]{\songti\small{-\ \thepage\ -}}
  % 取消页眉线
  \renewcommand{\headrulewidth}{0bp}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 定义实习日志环境
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newenvironment{logentry}[2][]% date, heading
{\par{\bfseries\heiti{}#2：#1}\par}{\par}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 重定义强调字体的代码，
%% 解决默认强调字体是italic，中文用楷体代替，
%% 在此设置为加粗，注意需要使用etoolbox宏包
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\makeatletter
\let\origemph\emph
\newcommand*\emphfont{\normalfont\bfseries}
\DeclareTextFontCommand\@textemph{\emphfont}
\newcommand\textem[1]{%
  \ifdefstrequal{\f@series}{\bfdefault}
    {\@textemph{\CTEXunderline{#1}}}
    {\@textemph{#1}}%
}
\RenewDocumentCommand\emph{s o m}{%
  \IfBooleanTF{#1}
    {\textem{#3}}
    {\IfNoValueTF{#2}
      {\textem{#3}\index{#3}}
      {\textem{#3}\index{#2}}%
     }%
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% URL排版设置
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\urlstyle{same}
\def\UrlBreaks{%
  \do\/%
  \do\a\do\b\do\c\do\d\do\e\do\f\do\g\do\h\do\i\do\j\do\k\do\l%
     \do\m\do\n\do\o\do\p\do\q\do\r\do\s\do\t\do\u\do\v\do\w\do\x\do\y\do\z%
  \do\A\do\B\do\C\do\D\do\E\do\F\do\G\do\H\do\I\do\J\do\K\do\L%
     \do\M\do\N\do\O\do\P\do\Q\do\R\do\S\do\T\do\U\do\V\do\W\do\X\do\Y\do\Z%
  \do0\do1\do2\do3\do4\do5\do6\do7\do8\do9\do=\do/\do.\do:%
  \do\*\do\-\do\~\do\'\do\"\do\-}
\Urlmuskip=0mu plus 0.1mu

\endinput
%% End of file `nwafucoursepaper.cls'.
