
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ReportTemplate}[2018/12/15 (HEIA-FR) School of Engineering Fribourg - LaTeX Template (unofficial)]

% Passes all the received arguments to the article class
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}

% Ends options processing
\ProcessOptions \relax

% This class is based on the article class
\LoadClass{report}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                               REQUIRED PACKAGES                                %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Set paper size and margins
\RequirePackage[a4paper, margin=3cm, bottom=3cm, top=3cm, includefoot]{geometry}

%% For A3 pages
\usepackage[usegeometry]{typearea}

%% To include PDF
\usepackage{pdfpages}

%% For landscape pages
\usepackage{lscape}

%% Image caption
\usepackage{caption}
\usepackage{subcaption}

%% Encoding
\RequirePackage[utf8]{inputenc}
\usepackage[T1]{fontenc}

%% Font§
\RequirePackage[sc]{mathpazo}

%% Fourth level of section (with \paragraph):
% see : https://tex.stackexchange.com/a/60218
\makeatletter
\renewcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
            {-2.5ex\@plus -1ex \@minus -.25ex}%
            {1.25ex \@plus .25ex}%
            {\normalfont\normalsize\bfseries}}
\makeatother
\setcounter{secnumdepth}{4} % how many sectioning levels to assign numbers to
\setcounter{tocdepth}{4}    % how many sectioning levels to show in ToC

%% LaTeX' own graphics handling
\RequirePackage{graphicx}
    % Define graphics path
    \graphicspath{ {./img/} }

%% Header and Footer styling
\RequirePackage{fancyhdr}
\fancyhf{}
\fancyfoot[C]{\thepage}
\fancyheadoffset{0pt}% <- recalculate \headwidth

%% Links
\RequirePackage[colorlinks = true,
            linkcolor = black,
            urlcolor  = blue,
            citecolor = blue,
            anchorcolor = black]{hyperref}




%% Code
\usepackage[newfloat]{minted}
\usemintedstyle{pastie}
\setminted{
    linenos=true,
    tabsize=2,
    breaklines,
    bgcolor=white,
    frame=leftline
}

%% tables
\usepackage{longtable}

%% Titling
\RequirePackage{titling}

%% Lists
\RequirePackage{enumitem}

%% Indentation
\usepackage{parskip}

%% Quotes
\usepackage{csquotes}

%% Array
\usepackage{array}
\usepackage{multirow}


%% Glossary
\usepackage{glossaries}

%%Placing figure
\usepackage{float}

%% Biblio
\usepackage{biblatex} %Imports biblatex package
\addbibresource{biblio.bib} %Import the bibliography file

%% Todo notes
\usepackage[colorinlistoftodos]{todonotes}

\usepackage{subfiles}

\RequirePackage[skins,breakable]{tcolorbox}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                               HEADER AND FOOTER                                 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\pagestyle{fancy}
\rhead{Master MSE}
\makeatletter
\lhead{Raemy Mathis, Macherel Rémy}
\makeatother



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                    COMMANDS                                     %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand*{\captionsource}[2]{%
  \caption[{#1}]{%
    #1% 
  }%
  \caption*{
    \textbf{Source:} #2%
  }
}


\newcommand*{\maketableofcontent}{
    \cleardoublepage
    \tableofcontents
    \thispagestyle{empty}
    \cleardoublepage
    \setcounter{page}{1}
}

\makeatletter
\def\@subtitle{\empty}
\def\@version{\empty}

\newcommand*{\location}[1]{\gdef\@location{#1}}
\newcommand*{\subtitle}[1]{\gdef\@subtitle{#1}}
\newcommand*{\subsubtitle}[1]{\gdef\@subsubtitle{#1}}
\newcommand*{\version}[1]{\gdef\@version{#1}}
\newcommand*{\contact}[1]{\gdef\@contact{#1}}


\newcommand*{\maketitlepage}{
    %\AtBeginDocument{\thispagestyle{firstpage}}
    \begin{titlepage}
        \begin{center}

            \hrule
            \vspace{3em}
            {\huge{\textbf{\@title}} \par}
            \vspace{3em}
            \hrule
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            \ifx\@subtitle\empty
            \else
                \vspace{2em}
                {\Large\@subtitle \par}

            \fi
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            \ifx\@subsubtitle\empty
            \else
                \vspace{2em}
                {\Large\@subsubtitle \par}

            \fi
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            \vspace{3em}
            {\Large\@location} {\Large\@date \par}
            \vspace{13em}
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        \end{center}

        \begin{minipage}[t]{3in}
            {\large{Auteurs} \par}
            \@author\\
            \href{mailto:\@contact}{\@contact}\\
            Raemy Mathis \\
            \href{mailto:mathis.raemy@master.hes-so.ch}{mathis.raemy@master.hes-so.ch}
        \end{minipage}
        \hfill
        %\vspace{3em}
        \vfill{}
        
        \begin{flushleft}
            Version: \@version
        \end{flushleft}

    \end{titlepage}
}
\makeatother


% Minted configuration for caption:
\newenvironment{code}{\captionsetup{type=listing}}{}
\SetupFloatingEnvironment{listing}{name=Code}


% \renewtcblisting{minted}[3][]{
% enhanced,
% breakable,% Allow page breaks
% listing engine=minted,
% listing only,#1, title=#3, minted language=#2,
% colbacktitle=mintedframe,      % Title background color
% colframe=mintedframe,
% sharp corners, top=0mm, bottom=0mm,
% title code={\path[draw=mintedframe,dashed, fill=mintedbackground](title.south west)--(title.south east);},
% frame code={\path[draw=mintedframe, fill=mintedbackground](frame.south west) rectangle (frame.north east);}
% }