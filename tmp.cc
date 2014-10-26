// 空白文字なら真（ファイルにもとからあったものを書き足し） 
#define isWhiteSpace(c)    (c == ' ' || c == '\t' || c == '\n')

//kitayama
void initializeScanner(char *filename){}

//onoda
int yylex(void){
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
  yyval.num = 0;
  // 識別子または予約語の取得
  if (isalpha(currentChar))  {//英字
    id += currentChar;first_c = currentChar;//最初の一文字を代入
    while(isalnum(currentChar)){currentChar = getCharacter(); id += currentChar}
    //英数字読込、文字列に連結
    yyval.symbol = id;
    skipped = currentChar; //一文字余分に読んでる
    return getIdentifier(first_c);//最初の英字を使う
  }
  else if(currentChar== Cadd){ yyval.op = Cadd; return ADDOP;}
  else if( currentChar == Csubtract){ //加減算
    //加算と減算で別々の処理をするのは仕様にかいてあったため(ADDOPの表) 
      yyval.op = Csubtract;
      return ADDOP;
    }
  else if(currentChar == Cmultiply){//剰余算、のかけ算を判定
      // else if(currentChar == MULT || currentChar == MOD ){//自分で定義を使用
      yyval.op = Cmultiply;return MULOP;
  }else if(currentChar == Cmodulo){
    yyval.op = Cmodulo;
    return MULOP;
  }else if(currentChar == Cdivide){// "/"を読んだ後
    currentChar = getCharacter();
    if(currentChar == Cdivide){// "//" を読み、コメント文と判明
      //改行かEOFまで読み進め
      while( getCharacter() != EOF || lineNo == nowLine );
    }else{// "/" 以外の文字を読んだ場合
      yyval.op = Cdivide;//return MULOP;
      skipped = currentChar;//一文字余分に読んでる
          return MULOP;
    }
  }
  else if(currentChar == Cand){// "&"を読んだ後
    currentChar = getCharacter();
    if(currentChar == Cand){// "&&" を読み、論理積と判明
      yyval.op = Cand;//
      return LOGOP;
    }else{// "&" 以外の文字を読んだ場合、不正なもじのためエラー
      compileError(EIllegalChar,currentChar,currentChar);
      //    return getIdentifier(currentChar);
    }
  }
  else if(currentChar == Cor){// "|" を読んだあと
    currentChar = getCharacter();
    if(currentChar == Cor){// "||" を読み、論理積と判明
      yyval.op = Cor;//
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
}


//tanaka
static int getIdentifier(int c){}


//krihara
static int getCharacter(void){}


