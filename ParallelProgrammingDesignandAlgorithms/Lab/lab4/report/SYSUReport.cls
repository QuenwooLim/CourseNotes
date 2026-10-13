\ProvidesClass{SYSUReport}
\LoadClass[12pt,hyperref,a4paper,UTF8]{ctexart}
%%页边距
\RequirePackage[left=2.50cm, right=2.50cm, top=2.50cm, bottom=2.50cm]{geometry}


%%一些必要的宏包
\RequirePackage[unicode=true,colorlinks,urlcolor=blue,linkcolor=blue,bookmarksnumbered=blue]{hyperref}

\RequirePackage{latexsym,amssymb,amsmath,amsbsy,amsopn,amstext,amsthm,amsxtra,color,bm,calc,ifpdf}
\RequirePackage{graphicx}
\RequirePackage{enumerate}
\RequirePackage{fancyhdr}
\RequirePackage{listings}
\RequirePackage{multirow}
\RequirePackage{makeidx}
\RequirePackage{xcolor}
\RequirePackage{fontspec}
\RequirePackage{subfigure}
\PassOptionsToPackage{hyphens}{url}
\RequirePackage{hyperref}
\RequirePackage{booktabs}
\RequirePackage{tcolorbox}

\definecolor{myteal}{cmyk}{1,0,1,0.60} % 主题色改为中大绿
%% 更改摘要二字的样式
\renewcommand{\abstractname}{\textbf{\large {摘\quad 要}}}

%可固定下划线长度
\makeatletter
\newcommand\dlmu[2][4cm]{\hskip1pt\underline{\hb@xt@ #1{\hss#2\hss}}\hskip3pt}
\makeatother

% %更改主题
\hypersetup{
    colorlinks=true,
    linkcolor=black,
    filecolor=blue,
    urlcolor=magenta,
    citecolor=orange,
    linktoc=all,
}

%%对一些autoref的中文引用名作修改
\def\equationautorefname{式}
\def\footnoteautorefname{脚注}
\def\itemautorefname{项}
\def\figureautorefname{图}
\def\tableautorefname{表}
\def\partautorefname{篇}
\def\appendixautorefname{附录}
\def\chapterautorefname{章}
\def\sectionautorefname{节}
\def\subsectionautorefname{小小节}
\def\subsubsectionautorefname{subsubsection}
\def\paragraphautorefname{段落}
\def\subparagraphautorefname{子段落}
\def\FancyVerbLineautorefname{行}
\def\theoremautorefname{定理}

\newtheorem{Theorem}{定理}[section]
\newtheorem{Lemma}[Theorem]{引理}
\newtheorem{Corollary}[Theorem]{推论}
\newtheorem{Proposition}[Theorem]{命题}
\newtheorem{Definition}[Theorem]{定义}
\newtheorem{Example}[Theorem]{例}

%%文本框设置
\newcommand{\tbox}[1]{
    \begin{center}
        \begin{tcolorbox}[colback=gray!10,%gray background
                colframe=black,% black frame colour
                width=8cm,% Use 8cm total width,
                arc=1mm, auto outer arc,
                boxrule=0.5pt,
            ]
            {#1}
        \end{tcolorbox}
    \end{center}
}

\newcommand\headl[1]{\def\headl{#1}}
\newcommand\headc[1]{\def\headc{#1}}
\newcommand\headr[1]{\def\headr{#1}}
\newcommand\lessonTitle[1]{\def\lessonTitle{#1}}
\newcommand\reportTitle[1]{\def\reportTitle{#1}}
\newcommand\stuname[1]{\def\stuname{#1}}
\newcommand\stuid[1]{\def\stuid{#1}}
\newcommand\inst[1]{\def\inst{#1}}
\newcommand\major[1]{\def\major{#1}}
\renewcommand\date[1]{\def\date{#1}}

%%页眉设置
\pagestyle{fancy}
\fancyhead[L]{\fangsong {\headl}}
\fancyhead[C]{\fangsong {\headc}}
\fancyhead[R]{\fangsong {\headr}}
%% 去掉页眉横线
% \renewcommand{\headrulewidth}{0pt}

%% 常用图片文件夹路径
\graphicspath{{figures/}{figure/}{pictures/}{picture/}{pic/}{pics/}{image/}{images/}}

%%在这里可更改封面logo
\newcommand{\cover}{
    %%封面校名logo，图片都在figures文件夹里
    \begin{center}
        \begin{figure}
            \centering
            \vspace{2cm}
            \includegraphics[width=0.6\textwidth]{sysu.jpg}
        \end{figure}

        \vspace*{\fill}

        \heiti \Huge \textbf{{\lessonTitle}} \par
        \vspace{1cm}
        \heiti \Large {\underline{\reportTitle}} \par
        \vspace{3cm}

        \vspace{0.5cm}
        \kaishu\Large 姓\ 名\ \dlmu[9cm]{\stuname} \qquad \\ %姓名 
        \vspace{0.5cm}
        \kaishu\Large 学\ 号\ \dlmu[9cm]{\stuid} \qquad  \\ %学号
        \vspace{0.5cm}
        \kaishu\Large 学\ 院\ \dlmu[9cm]{\inst} \qquad \\ %院所
        \vspace{0.5cm}
        \kaishu\Large 专\ 业\ \dlmu[9cm]{\major} %专业


        \vspace*{\fill}

        \large \date
    \end{center}
}

