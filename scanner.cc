/*******************************************************************
  この次の行に，課題作成に参加したメンバのアカウントを記入すること．

*******************************************************************/

#include <string>
#include <stdio.h>
#include <ctype.h>
using namespace std;
#include "token.h"
#include "scanner.h"
#include "error.h"
#include "const.h"


// 定数の定義
#define FOREVER	1

/*	          */
/* 文字判定マクロ */
/*		  */

// 空白文字なら真
#define isWhiteSpace(c)	(c == ' ' || c == '\t' || c == '\n')

// ファイル内部だけで有効な関数のプロトタイプ宣言

static int getIdentifier(int c);
static int getCharacter(void);

// プログラム全体で有効な変数

int lineNo;	// 処理中の行の行番号(scaner.ccとerror.ccで使用)

// ファイル内部だけで有効な変数

static FILE *srcFilePointer;	// 原始プログラムのファイルディスクリプタ
static int currentChar;		// 先読み文字バッファ

#ifdef YYDEBUG
extern int yydebug;
#endif

/* 字句解析系を初期化する                              */
/*   filename: 原始プログラム名                        */
/*   返り値: なし                                      */
/*   副作用:   srcFilePointer にファイルポインタを代入 */
void initializeScanner(char *filename)
{
}


/* 字句読み取りモジュール                                */
/* 返り値: トークンを表す定数                            */
/* 副作用: 共用体 yylval に、必要に応じて属性を代入      */
/* 概要:   原始プログラムから文字をよみ，字句を切り出す．*/
/*         読み込んだ文字に応じてトークンの種類を決定し，*/
/*         トークンを返す．またトークンの属性があれば    */
/*         それをyylvalに格納する．                      */
int yylex()
{
 //START:
  int ch_double=0, nowLine;//現在の行数、"//"の読み飛ばしに利用
  char first_c;static unsigned char skipped;//skipprd 一文字余分に読んだときのために代入
  string *id;// 識別子にいれるため

  if(skipped < 128 && skipped >= ' ' )//余分に読み込んでいる
    {
      currentChar = skipped;
      skipped = 150;//次回のif をはじくため
    }
  while (isWhiteSpace(currentChar))  {  // 空白の読み飛ばし
    currentChar = getCharacter();
    nowLine = lineNo;
  }
  // この時点では currentChar には空白以外の文字が入っている
  yylval.num = 0;
  // 識別子または予約語の取得
  if (isalpha(currentChar))  {//英字
    id += currentChar;first_c = currentChar;//最初の一文字を代入
    while(isalnum(currentChar)){currentChar = getCharacter(); id += currentChar;}
    //英数字読込、文字列に連結
    yylval.symbol = id;
    skipped = currentChar; //一文字余分に読んでる
    return getIdentifier(first_c);//最初の英字を使う
  }
  else if(currentChar== Cadd){ yylval.op = Cadd; return ADDOP;}
  else if( currentChar == Csubtract){ //加減算
    //加算と減算で別々の処理をするのは仕様にかいてあったため(ADDOPの表) 
      yylval.op = Csubtract;
      return ADDOP;
    }
  else if(currentChar == Cmultiply){//剰余算、のかけ算を判定
      // else if(currentChar == MULT || currentChar == MOD ){//自分で定義を使用
      yylval.op = Cmultiply;return MULOP;
  }else if(currentChar == Cmodulo){
    yylval.op = Cmodulo;
    return MULOP;
  }else if(currentChar == Cdivide){// "/"を読んだ後
    currentChar = getCharacter();
    if(currentChar == Cdivide){// "//" を読み、コメント文と判明
      //改行かEOFまで読み進め
      while( getCharacter() != EOF || lineNo == nowLine );
    }else{// "/" 以外の文字を読んだ場合
      yylval.op = Cdivide;//return MULOP;
      skipped = currentChar;//一文字余分に読んでる
          return MULOP;
    }
  }
  else if(currentChar == Cand){// "&"を読んだ後
    currentChar = getCharacter();
    if(currentChar == Cand){// "&&" を読み、論理積と判明
      yylval.op = Cand;//
      return LOGOP;
    }else{// "&" 以外の文字を読んだ場合、不正なもじのためエラー
      compileError(EIllegalChar,currentChar,currentChar);
      //    return getIdentifier(currentChar);
    }
  }
  else if(currentChar == Cor){// "|" を読んだあと
    currentChar = getCharacter();
    if(currentChar == Cor){// "||" を読み、論理積と判明
      yylval.op = Cor;//
      return LOGOP;
      }else{// "|" 以外の文字を読んだ場合、不正なもじのためエラー
    compileError(EIllegalChar,currentChar,currentChar);
    //    return getIdentifier(currentChar);
      }
    // ファイルの終わりを表すEOFを読んだとき
  }else if (currentChar == EOF)  {
    return EOF;
  }
  // その他の文字は不正な文字なのでエラー
  else  {
    compileError(EIllegalChar,currentChar,currentChar);
  }

  return 0;
}

/* 識別子を取得する                                 */
/*   c: 識別子の１文字目（英字）                    */
/*   返り値: トークン ID                            */
/*   副作用: yylval.symbol に字句へのポインタを代入 */
static int getIdentifier(int c)
{
  /*
    ここで，識別子の字句を読み取る有限オートマトンをシミュレートする．
    字句を保存するための局所変数を用意すること．
    字句へのポインタを yylval.symbol に代入し，識別子のトークンを返す．
  */
  /*
  yylval.symbol = グローバル変数 yylval に字句を保存
                  yylval.symbol の型は y.tab.h を参照のこと．
  */
  return 0;
}

/* 文字読み取りモジュール                       */
/*  返り値: ファイルから読んだ文字              */
/*  副作用: 改行文字を読んだとき lineNo が1増加 */
static int getCharacter()    
{
  //retにファイルから読み込んだ一文字を代入
  char ret;
  ret = fgetc(srcFilePointer);
  //改行文字を読み込んだ場合の処理
  if(ret == '\n'){ 
    lineNo += 1;
  }
  return ret;
}
