/*******************************************************************
  この次の行に，課題作成に参加したメンバのアカウントを記入すること．
s1190220 s1190153 s1190186 s1190197
*******************************************************************/

#include <string>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
using namespace std;
#include "token.h"
#include "scanner.h"
#include "error.h"
#include "const.h"

// 定数の定義
#define FOREVER	1

//文字判定マクロ
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
  srcFilePointer = fopen(filename, "r");
  // 原始プログラムのファイルが見つからない場合
  if(srcFilePointer == NULL){
    //エラー処理
    errorExit(EFileNotFound);
  }
  //lineNoの初期化
  lineNo = 1;
  //文字の先読み
  currentChar = getCharacter();
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
 START:
  while (isWhiteSpace(currentChar)){ // 空白の読み飛ばし
    currentChar = getCharacter();
  }
  // この時点では currentChar には空白以外の文字が入っている
  // 識別子または予約語の取得
  if (isalpha(currentChar)){
    return getIdentifier(currentChar);
  }
  //ADD演算子を読み込んでいた場合の処理
  if(currentChar == Cadd){
    yylval.op = Cadd;
    currentChar = getCharacter();
    return ADDOP;
  }
  //SUB演算子を読み込んでいた場合の処理
  else if(currentChar == Csubtract){ //加減算
    //加算と減算で別々の処理をするのは仕様にかいてあったため(ADDOPの表) 
    yylval.op = Csubtract;
    currentChar = getCharacter();
    return ADDOP;
  }
  //MUL演算子を読み込んでいた場合の処理
  else if(currentChar == Cmultiply){ //かけ算を判定
    yylval.op = Cmultiply;
    currentChar = getCharacter();
    return MULOP;
  }
  //DIV演算子もしくはコメント文を読み込んでいた場合の処理
  else if(currentChar == Cdivide){ // "/"を読んだ後
    currentChar = getCharacter();
    if(currentChar == Cdivide){ // "//" を読み、コメント文と判明
      //改行かEOFまで読み進める
      while(currentChar != '\n' && currentChar != EOF){
        currentChar = getCharacter();
      }
      //コメント文を最後まで読み進めた為、次の処理へ移る
      goto START;
    }
    else{ // "/" 以外の文字を読んだ場合
      yylval.op = Cdivide;
      return MULOP;
    }
  }
  //MOD演算子を読み込んでいた場合の処理
  else if(currentChar == Cmodulo){
    yylval.op = Cmodulo;
    currentChar = getCharacter();
    return MULOP;
  }
  //AND演算子を読み込んでいた場合の処理
  else if(currentChar == Cand){ // "&"を読んだ後
    currentChar = getCharacter();
    if(currentChar == Cand){ // "&&" を読み、論理積と判明
      yylval.op = Cand;
      currentChar = getCharacter();
      return LOGOP;
    }
    else{ // "&" 以外の文字を読んだ場合、不正な文字のためエラー
      compileError(EIllegalChar,currentChar,currentChar);
    }
  }
  //COR演算子を読み込んでいた場合の処理
  else if(currentChar == Cor){ // "|" を読んだあと
    currentChar = getCharacter();
    if(currentChar == Cor){ // "||" を読み、論理積と判明
      yylval.op = Cor;
      currentChar = getCharacter();
      return LOGOP;
    }
    else{// "|" 以外の文字を読んだ場合、不正な文字のためエラー
      compileError(EIllegalChar,currentChar,currentChar);
    }    
  }
  //EOFを読んだ場合の処理
  else if (currentChar == EOF){ 
    return EOF;
  }
  // その他の文字は不正な文字のためエラー
  else{
    compileError(EIllegalChar,currentChar,currentChar);
  }
}

/* 識別子を取得する                                 */
/*   c: 識別子の１文字目（英字）                    */
/*   返り値: トークン ID                            */
/*   副作用: yylval.symbol に字句へのポインタを代入 */
static int getIdentifier(int c)
{
  //引数のcを使っていないが、問題ないと判断した
  //字句を保存する為の局所変数
  string tmp = "";
  //英字,数値以外の文字を受け取るまで読み進める
  while(isalpha(currentChar) || isdigit(currentChar)){
    tmp += currentChar;
    currentChar = getCharacter();
  }
  //yylval.symbol = グローバル変数 yylval に字句を保存
  yylval.symbol = new string(tmp); 
  return ID;
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
