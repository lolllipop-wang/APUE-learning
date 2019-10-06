#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <map>
#include <regex>
#include <string>
#include <vector>

#define MAXLINE 4096
#define MAXKEYWORDLEN 50

struct Token {
  std::string str;
  int type;
};

static struct Token GlobalToken;

static char *KeywordsFile = nullptr; /* 记录关键字的文件 */
static char *SourceFile = nullptr;   /* 待处理的源文件 */
static char *TargetFile = nullptr;   /* 输出的目标文件 */

static FILE *TempFileFp = nullptr;

static char CstrToken[MAXLINE];
static std::vector<Token> Words;         /* 词法分析产生的结果 */
static std::map<std::string, int> Table; /* 储存关键字及其对应的种别码 */
static const std::regex Pattern("^[+-]?[\\d]+([.][\\d]*)?"
                                "([Ee][+-]?[\\d]+)?$");

void Initialize();   /* 初始化-读入关键词表 */
void MainProcess();  /* 主循环 */
void Preprocessed(); /* 预处理-去除注释 */
bool GetToken();     /* 从文件中读取一个标识符 */
int Classify();      /* 对标识符进行分类，映射成种别码 */
bool IsNumber(std::string &text); /* 判断当前是否为数字 */
bool MaybeDoublyCharacterOperator(
    char c); /* 可能是包含两个字符的运算符 ++, --... */

void (*NextState)(void) = Initialize;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr,
            "usage: %s  <sources file> "
            "<target file> <keywords file>\n",
            argv[0]);
    exit(1);
  }

  /**
   *初始化三个文件的名称，便于记忆
   */
  SourceFile = argv[1];
  TargetFile = argv[2];
  KeywordsFile = argv[3];

  while (NextState != nullptr)
    NextState();

  exit(0);
}

void Initialize() {
  printf("Begin Initialize\n");
  FILE *fp = nullptr;
  if ((fp = fopen(KeywordsFile, "r")) == nullptr) {
    fprintf(stderr, "Failed to open %s\n", KeywordsFile);
    exit(1);
  }

  char line[MAXLINE];
  char keyword[MAXKEYWORDLEN];
  int num;
  while (fgets(line, MAXLINE, fp) != nullptr) {
    sscanf(line, "%s%d", keyword, &num);
    Table[keyword] = num;
  }
  NextState = Preprocessed;
  printf("Initialize Complete!\n");
}

void Preprocessed() {
  printf("Begin Preprocessed\n");
  int i;
  int nu;          /* 源文件中的行号 */
  int length;      /* 每行的长度 */
  int count;       /* 注释的层数 */
  int line_ch;     /* 每行已输出的字符数量 */
  int flag, cando; /* 标记是否在注释中 */
  FILE *ifp, *ofp; /* 源文件和输出文件的文件指针 */

  char line[MAXLINE];

  if ((ifp = fopen(SourceFile, "r")) == nullptr) {
    fprintf(stderr, "can't open source file\n");
    exit(1);
  }

  if ((ofp = fopen("temp", "w")) == nullptr) {
    fprintf(stderr, "can't open output file\n");
    exit(1);
  }

  nu = 0;
  count = 0;
  flag = 0;
  cando = 1;

newline:
  while (fgets(line, MAXLINE, ifp) != nullptr) {
    ++nu;

    length = strlen(line);

    if (line[length - 1] == '\n') {
      line[length - 1] = '\0';
      length--;
    }

    i = 0;
    while (i < length &&
           (line[i] == ' ' || line[i] == '\n')) /* 跳过每一行的空白字符 */
      i++;

    line_ch = 0;
    for (; i < length; ++i) {
      /* 遇到 "/" "*" 注释开始*/
      if (i + 1 < length && line[i] == '/' && line[i + 1] == '*') {
        flag = 1;
        cando = 0;
        count++;
        i += 1;

        /* 出现嵌套的"/" "*" */
        if (count > 1) {
          fprintf(stderr, "error at line %d: %s\n", nu, line);
          exit(1);
        }

        /* 遇到 "*" "/" 注释结束 */
      } else if (i + 1 < length && line[i] == '*' && line[i + 1] == '/') {
        flag = 0;
        cando = 1;
        count--;
        i += 2;

        /* 出现嵌套的"/" "*" */
        if (count < 0) {
          fprintf(stderr, "error at line %d: %s\n", nu, line);
          exit(1);
        }

        /* 单行注释 */
      } else if (i + 1 < length && line[i] == '/' && line[i + 1] == '/') {
        goto newline;
      }

      /* 当前字符是否可以输出 */
      if (flag == 0 && cando == 1 && i < length && line[i] != '\n' &&
          line[i] != '\t') {
        fputc(line[i], ofp);
        line_ch++;
      }
    }

    /* 如果当前行没有可输出的字符，则不输出'\n' 避免多余空行 */
    if (line_ch > 0)
      fputc('\n', ofp);
  }

  fclose(ifp);
  fclose(ofp);

  NextState = MainProcess;
  printf("Preprocessed Complete!\n");
}

void MainProcess() {
  if ((TempFileFp = fopen("temp", "r")) ==
      nullptr) { /* 打开预处理产生的文件temp */
    fprintf(stderr, "open temp file error\n");
    exit(1);
  }

  printf("Begin Main Process\n");
  while (GetToken()) {
    if (GlobalToken.type == -1) { /* 未定义的标识符 */
      fprintf(stderr, "undefined identifier: %s", GlobalToken.str.c_str());
      exit(1);
    }

    Words.push_back(GlobalToken);
  }

#ifdef RELEASE
  FILE *TargetFileFp;
  if ((TargetFileFp = fopen(TargetFile, "w")) == nullptr) {
    fprintf(stderr, "open target file error\n");
    exit(1);
  }
  std::vector<Token>::iterator it;
  for (it = Words.begin(); it != Words.end(); it++) {
    fprintf("<  %-8s,%2d  >\n", GlobalToken.str.c_str(), GlobalToken.type);
  }
  fclose(TargetFileFp);
#else
  std::vector<Token>::iterator it;
  for (it = Words.begin(); it != Words.end(); it++) {
    printf("<  %-8s,%2d  >\n", it->str.c_str(), it->type);
  }
#endif
  fclose(TempFileFp);
  NextState = nullptr;
  printf("Main Process Complete!\n");
}

bool GetToken() {
  char *p = CstrToken;

retry:
  /* 跳过空格，回车，换行，制表符 */
  while (((*p = fgetc(TempFileFp)) == ' ' || *p == '\n' || *p == '\t' ||
          *p == '\r') &&
         (*p != -1))
    ;

  if (*p == -1)
    return false;    /* 文件已经读完 */
  if (isalnum(*p)) { /* 读到的字符是字母或数字 */

    while (isalnum((*(++p) = fgetc(TempFileFp))) && *p != -1)
      ; /* 期望读取后面的字母或数字 */
    if (*p == -1)
      return false;

    ungetc(*p, TempFileFp);
    *p = '\0';

  } else if (*p == '"') { /* 可能出现了字符串常量 */

    while ((*(++p) = fgetc(TempFileFp)) != '"' && *p != -1)
      ;
    if (*p == -1)
      return false;
    if (*p == '\n')
      *p = '\0';
    else
      *(++p) = '\0';

  } else if (*p == '\'') { /* 可能出现了字符常量 */

    while ((*(++p) = fgetc(TempFileFp)) != '\'' && *p != -1)
      ;
    if (*p == -1)
      return false;
    if (*p == '\n')
      *p = '\0';
    else
      *(++p) = '\0';

  } else if (MaybeDoublyCharacterOperator(*p)) {

    while (MaybeDoublyCharacterOperator(*(++p) = fgetc(TempFileFp))) {
      *(++p) = '\0';
    }
    if (Table.count(CstrToken) == 0) {
      ungetc(*p, TempFileFp);
      *p = '\0';
    }

  } else { /* 可能读到了运算符，界符或者换行符 */
    if (*p == '\n') /* 如果是换行符，则跳跃到函数开头重新读取下一个 */
      goto retry;
    *++p = '\0';
  }

  if (*p == -1)
    return false;

  GlobalToken.str = CstrToken;
  GlobalToken.type = Classify(); /* 对读到的标识符进行分类 */
  return true;
}

int Classify() {
  if (Table.count(GlobalToken.str) != 0) /* 可以在Table中找到 */
    return Table[GlobalToken.str];

  std::string::iterator it = GlobalToken.str.begin();

  if (*it == '"' || *it == '\'') /* 字符串常量，字符常量 */
    return Table["stringConstant"];

  else if (IsNumber(GlobalToken.str)) /* 数字常量 */
    return Table["numberConstant"];

  else if (!(*it >= '0' && *it <= '9')) /* 不以数字开头则可能是标识符 */
    return Table["identifier"];

  return -1; /* 未定义 */
}

bool IsNumber(std::string &text) { return std::regex_match(text, Pattern); }

bool MaybeDoublyCharacterOperator(char c) {
  switch (c) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
  case '&':
  case '|':
  case '<':
  case '>':
  case '!':
  case '=':
    return true;
  default:
    break;
  }
  return false;
}

/*
    keywords table

auto                1
break               2
case                3
char                4
const               5
continue            6
default             7
do                  8
double              9
else                10
enum                11
extern              12
float               13
for                 14
goto                15
if                  16
int                 17
long                18
register            19
return              20
short               21
signed              22
sizeof              23
static              24
struct              25
switch              26
typedef             27
union               28
unsigned            29
void                30
volatile            31
while               32
-                   33
--                  34
-=                  35
->                  36
!                   37
!=                  38
%                   39
%=                  40
&                   41
&&                  42
&=                  43
(                   44
)                   45
*                   46
*=                  47
,                   48
.                   49
/                   50
/=                  51
:                   52
;                   53
?                   54
[                   55
]                   56
^                   57
^=                  58
{                   59
|                   60
||                  61
|=                  62
}                   63
~                   64
+                   65
++                  66
+=                  67
<                   68
<<                  69
<<=                 70
<=                  71
=                   72
==                  73
>                   74
>=                  75
>>                  76
>>=                 77
"                   78
stringConstant      80
numberConstant      81
identifier          82
*/