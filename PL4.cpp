
// StyleCheckType vector
// StyleCheckType string
// StyleCheckType stringstream

# include <iostream>
# include <math.h>
# include <stdio.h>
# include <sstream>
# include <stdlib.h>
# include <string>
# include <vector>


using namespace std ;

enum Type {
  VOID, BOOL, INT, FLOAT, STRING, CHAR
} ;

struct Values {
  int type ;
  string value ;
  bool signB ;
  bool signN ;
} ;

struct Tokens {
  string str ;
  bool hasjump ;
  int jumpAddr ;
  int jumpEnd ;
} ;

struct Idents {
  
  bool initial ; 
  string name ;
  int type ;
  
  string value ;
  
  int size ; // array size
  vector <string> values ; // array num
  
} ;

struct Functions {
  string name ;
  int type ;
  vector<string> token ;
} ;

typedef Values Value ;
typedef Tokens Token ;
typedef Idents * Ident ;
typedef Functions Function ;

// ========全域變數========== 

bool gTest = false ;

static int uTestNum = 0 ;

string gRead ;
string gTemp ;
bool gHasTemp = false ;

vector <Token> gToken ;

vector <Ident> gIdent ;
vector <Ident> gIdent_Function ;

vector <Function> gFunction ;

bool gTime ; // @false : SyntaxCheck  @true : Evaluate 
bool gDone = false ; // Done() ;
bool gAtFunction = false ;
string gAtFunctionName = "" ;

int gI = 0 ;
int gLine = 0 ;
int gLine_add = 0 ;
int gLevel ;

// ========全域變數==========

void ShowVector( vector <string> temp ) {
  cout << "\n[Vector element]\n" ;
  for ( int i = 0 ; i != temp.size() ; i++ )
    cout << "[" << temp[i] << "] " ;
  cout << "\n\n" ; 
} // ShowVector()

bool BeingIdent( string str ) {
  if ( gAtFunction ) {
    if ( str == gAtFunctionName ) return true ;
    for ( int i = 0 ; i < gIdent_Function.size() ; i++ )
      if ( str == gIdent_Function[i] -> name ) return true ;
  } // if
  else {
    for ( int i = 0 ; i < gIdent.size() ; i++ )
      if ( str == gIdent[i] -> name ) return true ;
  } // else
  
  for ( int i = 0 ; i < gFunction.size() ; i++ )
    if ( str == gFunction[i].name ) return true ;
  
  return false ;
  
} // BeingIdent()

bool Isletter( char ch ) {
  if ( islower( ch ) || isupper( ch ) ) return true ;
  else return false ;
} // Isletter()

bool Isident( string str ) {
  if ( Isletter( str[0] ) || str[0] == '_' ) { 
    int i = 1 ;
    while ( i < str.size() ) {
      if ( !isdigit( str[i] ) && !Isletter( str[i] ) && str[i] != '_'  ) return false ;
      i++ ;
    } // while
    
    return true ;
  } // if
  
  else return false ;
} // Isident()

bool IsNum( string str ) {
  if ( isdigit( str[0] ) ) { 
    int i = 1 ;
    while ( i < str.size() ) {
      if ( !isdigit( str[i] ) && str[i] != '.'  ) return false ;
      i++ ;
    } // while
        
    return true ;
  } // if
  
  else return false ;
} // IsNum()

bool Bool_const( string str ) {
  if ( str == "true" ) return true ;
  if ( str == "false" ) return true ;
  return false ;
} // Bool_const()

bool Constant( string str, Value &value ) {
  if ( IsNum( str ) ) {
    if ( gTime ) value.value = str ;
    value.type = INT ;
    return true ;
  } // if
  
  if ( Bool_const( str ) ) {
    if ( gTime ) value.value = str ;
    value.type = BOOL ;
    return true ;
  } // if
  
  if ( str[0] == '\'' ) {
    if ( gTime ) value.value = str ;
    value.type = CHAR ;
    return true ;
  } // if
  
  if ( str[0] == '\"' ) {
    if ( gTime ) value.value = str ;
    value.type = STRING ;
    return true ;
  } // if
  
  return false ;
} // Constant()

bool IsSign( string str ) {
  if ( str == "+" ) return true ;
  if ( str == "-" ) return true ;
  if ( str == "!" ) return true ;
  return false ;
} // IsSign()

bool IsPPMM( string str ) {
  if ( str == "++" ) return true ;
  else if ( str == "--" ) return true ;
  else return false ;
} // IsPPMM()

bool AS_op( string str ) {
  if ( str == "+" ) return true ;
  if ( str == "-" ) return true ;
  return false ;
} // AS_op()

bool MS_op( string str ) {
  if ( str == "*" ) return true ;
  else if ( str == "/" ) return true ;
  else if ( str == "%" ) return true ;
  // else if ( str == "^" ) return true ;
  else return false ;
} // MS_op()

bool Calculate_op( string str ) {
  if ( str == "+" ) return true ;
  else if ( str == "-" ) return true ;
  else if ( str == "*" ) return true ;
  else if ( str == "/" ) return true ;
  else if ( str == "%" ) return true ;
  // else if ( str == "^" ) return true ;
  else return false ;
} // Calculate_op()

bool Relate_op( string str ) {
  if ( str == ">" ) return true ;
  if ( str == "<" ) return true ;
  if ( str == ">=" ) return true ;
  if ( str == "<=" ) return true ;
  if ( str == "==" ) return true ;
  if ( str == "!=" ) return true ;
  return false ;
} // Relate_op()

bool Assign_op( string str ) {
  if ( str == "=" ) return true ;
  if ( str == "+=" ) return true ;
  if ( str == "-=" ) return true ;
  if ( str == "*=" ) return true ;
  if ( str == "/=" ) return true ;
  if ( str == "%=" ) return true ;
  return false ;
} // Assign_op()

bool Whitespace( char ch ) {
  if ( ch == ' ' || ch == '\t' || ch == '\0' || ch == '\n' ) return true ;
  else return false ;
} // Whitespace()

bool Delimiter( char ch ) {
  
  if ( ch == ';' ) return true ;
  else if ( ch == '(' ) return true ;
  else if ( ch == ')' ) return true ;
  else if ( ch == '{' ) return true ;
  else if ( ch == '}' ) return true ;
  else if ( ch == '[' ) return true ;
  else if ( ch == ']' ) return true ;
  else if ( ch == '+' ) return true ;
  else if ( ch == '-' ) return true ;
  else if ( ch == '*' ) return true ;
  else if ( ch == '/' ) return true ;
  else if ( ch == '%' ) return true ;
  else if ( ch == '>' ) return true ;
  else if ( ch == '<' ) return true ;
  else if ( ch == '&' ) return true ;
  else if ( ch == '^' ) return true ;
  else if ( ch == '|' ) return true ;
  else if ( ch == '=' ) return true ;
  else if ( ch == ',' ) return true ;
  else if ( ch == '?' ) return true ;
  else if ( ch == '!' ) return true ;
  else if ( ch == '"' ) return true ;
  else if ( ch == '\'' ) return true ;

  return false ;

} // Delimiter()

void SkipLine() {
  char ch = '\0' ;
  while ( cin.peek() != '\n' ) cin.get( ch ) ;
} // SkipLine()

void Skipwhitespace() {
  char ch = '\0' ;
  while ( Whitespace( cin.peek() ) ) {
    cin.get( ch ) ;
    if ( ch == '\n' ) gLine++ ;
  } // while
} // Skipwhitespace()

string Getstring() {
  string str = "" ;
  char ch = '\0' ;

  while ( cin.peek() != '\"' ) {
    cin.get( ch ) ;
    str += ch ;
  } // while
  
  return str ; 
} // Getstring()

void TakeNch( string &str ) {
  char ch ;
  cin.get( ch ) ;
  str += ch ;
} // TakeNch()

string GetokenBase() {
  
  string str = "" ; // 要回傳的字串 
  char ch = '\0' ;
  
  Skipwhitespace() ;
  cin.get( ch ) ;

  
  bool enter = true ;
  
  if ( Delimiter( ch ) ) {
    
    enter = false ;
    str += ch ;
    
    char pch = cin.peek() ;

    if ( ch == '/' && pch == '/' ) {
      SkipLine() ;
      return GetokenBase() ;
    } // if 遇到雙斜線

    else if ( ch == '<' && pch == '<' ) {
      TakeNch( str ) ; // <<
    } // if
    else if ( ch == '<' && pch == '=' ) TakeNch( str ) ; // <=
    else if ( ch == '>' && pch == '>' ) TakeNch( str ) ; // >>
    else if ( ch == '>' && pch == '=' ) TakeNch( str ) ; // >=
    else if ( ch == '=' && pch == '=' ) TakeNch( str ) ; // ==
    else if ( ch == '!' && pch == '=' ) TakeNch( str ) ; // !=
    else if ( ch == '|' && pch == '|' ) TakeNch( str ) ; // ||
    else if ( ch == '&' && pch == '&' ) TakeNch( str ) ; // &&
    else if ( ch == '+' && pch == '+' ) TakeNch( str ) ; // ++
    else if ( ch == '+' && pch == '=' ) TakeNch( str ) ; // +=
    else if ( ch == '-' && pch == '-' ) TakeNch( str ) ; // --
    else if ( ch == '-' && pch == '=' ) TakeNch( str ) ; // -=
    else if ( ch == '*' && pch == '=' ) TakeNch( str ) ; // *=
    else if ( ch == '/' && pch == '=' ) TakeNch( str ) ; // /=
    else if ( ch == '%' && pch == '=' ) TakeNch( str ) ; // %=
    else if ( ch == '\"' ) {
      cin.get( ch ) ;
      while ( ch != '\"' ) {
        str += ch ;
        cin.get( ch ) ;
      } // while
      
      str += ch ;
    } // if
    else if ( ch == '\'' ) {
      TakeNch( str ) ;
      TakeNch( str ) ;
    } // if
   
  } // if
  
  while ( enter ) {
    str += ch ;
    if ( Delimiter( cin.peek() ) || Whitespace( cin.peek() ) ) enter = false ;
    else cin.get( ch ) ;
  } // while
  
  return str ;
  
} // GetokenBase()

void BackToken() {
  if ( gTime == false ) {
    gHasTemp = true ;
    gTemp = gRead ;
    gToken.pop_back() ;
  } // if
  else {
    gI--;
  } // else
  
  gRead = "" ;
} // BackToken()

string GetokenCheck() { // Getoken & Check whether has temptoken or not
  if ( gHasTemp ) {
    gRead = gTemp ;
    gHasTemp = false ;
  } // if
  else 
    gRead = GetokenBase() ;
  return gRead ;
} // GetokenCheck()

string Getoken() { // Getoken & PushBackIt
  if ( gTime == false ) {
    GetokenCheck() ;
    Token temp ;
    temp.str = gRead ;
    temp.hasjump = false ;
    gToken.push_back( temp ) ;
  } // if
  else { // gTime == true
    gRead = gToken[gI].str ;
    gI++ ;
  } // else
  
  return gRead ;
} // Getoken()

bool ReturnFalse( string str ) {
  cout << "line " << gLine << " : syntax error when token is \'" << str << "\'\n" ;
  return false ;
} // ReturnFalse()

bool ReturnFalseC( char ch ) {
  cout << "line " << gLine << " : syntax error when input char is \'" << ch << "\'\n" ;
  return false ;
} // ReturnFalseC()

bool CheckFPC() {
  
  Skipwhitespace() ; // 下一個取得的 char 將不會是 WhiteSpace 
  
  char ch = cin.peek() ; 
  
  if ( ch == '~' ) return ReturnFalseC( ch ) ;
  else if ( ch == '`' ) return ReturnFalseC( ch ) ;
  else if ( ch == '@' ) return ReturnFalseC( ch ) ;
  else if ( ch == '$' ) return ReturnFalseC( ch ) ;
  else if ( ch == '\\' ) return ReturnFalseC( ch ) ;

  
  else return true ; 

 
} // CheckFPC()

void Coutstring( string str ) {
  
  for ( int i = 0 ; i < str.size() ; i++ ) {
    if ( str[i] == '\\' ) {
      i++ ;
      if ( str[i] == '0' ) ;
      if ( str[i] == 'n' ) cout << "\n" ;
      if ( str[i] == 't' ) cout << "\n" ;
      if ( str[i] == 'r' ) cout << "\r" ;
      if ( str[i] == '\"' ) cout << "\"" ;
      if ( str[i] == '\'' ) cout << "\'" ;
      if ( str[i] == '\\' ) cout << "\n" ;
    } // if
    else cout << str[i] ;
  } // for
  
} // Coutstring()

// ========================New Fuction==========================================
bool Actual_parameter_list( string id ) ;
bool Exp( Value &value ) ;
bool Basic_exp( Value &value ) ;
bool Statement() ;
bool Compound_statement() ;
void Assign_id( string id, Value value, int size ) ;
bool GetValue( string id, Value &value ) ;
bool GetValueA( string id, Value &value, int size ) ;

string Int2str( int i ) {
  string s ;
  stringstream ss( s ) ;
  ss << i ;
  return ss.str() ;
} // Int2str()

bool Str2bool( string str ) {
  if ( str == "true" ) return true ;
  if ( str == "false" ) return false ;
} // Str2bool()

int Str2int( string str ) {
  return atoi( str.c_str() ) ;
} // Str2int()

int Ch2int( string str ) {
  int num = str[1] ;
  return num ;
} // Ch2int()

bool CanbeNum( Value value ) {
  if ( value.type == INT ) return true ;
  if ( value.type == CHAR ) return true ;
  return false ;
} // CanbeNum()

bool CanbeStr( Value value ) {
  if ( value.type == STRING ) return true ;
  if ( value.type == CHAR ) return true ;
  return false ;
} // CanbeStr()

string StrAdd( Value v1, Value v2 ) {
  string s1 = v1.value.substr( 1, v1.value.size()-2 ) ;
  string s2 = v2.value.substr( 1, v2.value.size()-2 ) ;
  return '"' + s1 + s2 + '"' ;
} // StrAdd()

Type GetType( string str ) {
  if ( str == "void" ) return VOID ;
  if ( str == "int" ) return INT ;
  if ( str == "char" ) return CHAR ;
  if ( str == "string" ) return STRING ;
  if ( str == "float" ) return INT ;
  if ( str == "bool" ) return BOOL ;
} // GetType()

string GetTypeStr( int type ) {
  if ( type == VOID ) return "void" ;
  if ( type == INT ) return "int"  ;
  if ( type == CHAR ) return "char" ;
  if ( type == STRING ) return "string" ;
  if ( type == FLOAT ) return "float" ;
  if ( type == BOOL ) return "bool" ;
} // GetTypeStr()

bool Type_Specifier( string str ) {
  if ( str == "int" ) return true ;
  if ( str == "char" ) return true ;
  if ( str == "float" ) return true ;
  if ( str == "string" ) return true ;
  if ( str == "bool" ) return true ;
  return false ;
} // Type_Specifier()

bool PPMM() {
  Getoken() ;
  
  if ( IsPPMM( gRead ) ) {   
    return true ;
  } // if
  else {
    BackToken() ;
    return false ;
  } // else
} // PPMM()

bool PPMMB( string id, int size ) {
  Getoken() ;
  
  if ( IsPPMM( gRead ) ) {
    if ( gTime ) {
      Value value ;
      if ( size == 0 ) GetValue( id, value ) ;
      else GetValueA( id, value, size ) ;
      int n = Str2int( value.value ) ;
      if ( gRead == "++" ) value.value = Int2str( n+1 ) ;
      if ( gRead == "--" ) value.value = Int2str( n-1 ) ;
      Assign_id( id, value, size ) ;
    } // if
    
    return true ;
  } // if
  else {
    BackToken() ;
    return false ;
  } // else
} // PPMMB()

bool Sign( Value &value ) {
  Getoken() ;
  
  if ( IsSign( gRead ) ) {
    if ( gRead == "-" ) value.signN = !value.signN ;
    if ( gRead == "!" ) value.signB = !value.signB ;
    return true ;
  } // if
  else {
    BackToken() ;
    return false ;
  } // else
} // Sign()


bool SignCalculate( Value &value ) {
  if ( !gTime ) return false ;
  if ( value.signB && value.type == BOOL ) {
    if ( value.value == "true" ) value.value = "false" ;
    else if ( value.value == "false" ) value.value = "true" ;
    value.signB = false ;
    return true ;
  } // if
  else if ( value.signN && value.type == INT ) {
    value.value = Int2str( atoi( value.value.c_str() ) * -1 ) ;
    value.signN = false ;
    return true ;
  } // if
  else {
    if ( value.signN || value.signB ) { // value type error
      cout << GetTypeStr( value.type ) << "_" << value.signB << "_" << value.signN << endl ;
      cout << "line " << gLine << " : Sign type error!\n" ;
    } // if
    else // No sign operator
      return true ;
  } // else
} // SignCalculate()

bool AddIdent( int type, string name, int size ) {
  Ident id = new Idents;
  id -> initial = false ;
  id -> type = type ;
  id -> name = name ;
  id -> size = size ;
  id -> value = "" ;
  if ( gTime ) id -> values.resize( size ) ;
  
  if ( gAtFunction ) {
    gIdent_Function.push_back( id ) ;
  } // if
  else {
    int l = 0 ;
    bool repeat = false ;
    while ( !repeat && l < gIdent.size() && name >= gIdent[l] -> name ) {
      if ( gIdent[l] -> name == name ) repeat = true ;
      else l++ ;
    } // while
    
    if ( repeat ) {
      gIdent[l] = id ;
      return false ;
    } // if
    else {
      gIdent.insert( gIdent.begin()+l, id ) ;
      return true ;
    } // else
  } // else
} // AddIdent()

void Assign_id( string name, Value value, int size ) {
  int l = 0 ;
  while ( name != gIdent[l] -> name ) l++ ;
  
  gIdent[l] -> initial = true ;
  if ( gIdent[l] -> size == 0 )  gIdent[l] -> value = value.value ;
  else gIdent[l] -> values[size] = value.value ;
  
} // Assign_id()

bool GetValue( string id, Value &value ) {
  int l = 0 ;
  if ( gAtFunction ) {
    while ( id != gIdent_Function[l] -> name ) l++ ;
    value.type = gIdent_Function[l] -> type ;
    if ( gTime ) value.value = gIdent_Function[l] -> value ;
  } // if
  else {
    while ( id != gIdent[l] -> name ) l++ ;
    if ( gIdent[l] -> size == 0 ) {
      value.type = gIdent[l] -> type ;
      if ( gTime ) value.value = gIdent[l] -> value ;
    } // if
    else {
      if ( gIdent[l] -> type == CHAR ) value.type = STRING ;
      else {
        value.type = INT ;
        return true ;
      } // else
      
      if ( gTime ) {
        for ( int i = 0 ; i < gIdent[l] -> values.size() ; i++ )
          value.value = "CHARARRAY" ;
      } // if
    } // else
  } // else
} // GetValue()


bool GetValueA( string id, Value &value, int loc ) {
  int l = 0 ;
  if ( gAtFunction ) {
    while ( id != gIdent_Function[l] -> name ) l++ ;
    value.type = gIdent_Function[l] -> type ;
    if ( gTime ) value.value = gIdent_Function[l] -> value ;
  } // if
  else {
    while ( id != gIdent[l] -> name ) l++ ;
    value.type = gIdent[l] -> type ;
    if ( gTime ) value.value = gIdent[l] -> values[loc] ;
  } // else
  
} // GetValueA()

bool GetValueF( string id, Value &value ) {
  int l = 0 ;
  while ( id != gFunction[l].name ) l++ ;
  
  value.type = gFunction[l].type ;
  if ( gTime ) {
    if ( value.type == INT ) value.value = "1" ;
    if ( value.type == CHAR ) value.value = '1' ;
    if ( value.type == STRING ) value.value = "1" ;
    if ( value.type == BOOL ) value.value = "true" ;
    else value.value = "1" ;
  } // if
} // GetValueF()

bool AddFuction( string name, int type ) {
  Function f ;
  f.type = type ;
  f.name = name ;
  for ( int i = 0 ; i < gToken.size() ; i++ )
    f.token.push_back( gToken[i].str ) ;
    
  int l = 0 ;
  bool repeat = false ;
  while ( !repeat && l < gFunction.size() && name >= gFunction[l].name ) {
    if ( gFunction[l].name == name ) repeat = true ;
    else l++ ;
  } // while
  
  if ( repeat ) {
    gFunction[l] = f ;
    return false ;
  } // if
  else {
    gFunction.insert( gFunction.begin()+l, f ) ;
    return true ;
  } // else
  
} // AddFuction()

bool Rest_of_Identifier_started_unary_exp( string id, Value &value ) {
  Getoken() ;
  if ( gRead == "(" ) {
    if ( !Actual_parameter_list( id ) ) return false ;
    Getoken() ;
    if ( gRead != ")" ) return ReturnFalse( gRead ) ;
    GetValueF( id, value ) ;
    return true ;
  } // if
  else {
    int size = 0 ;
    if ( gRead == "[" ) {
      Value valueA = {0} ;
      if ( !Exp( valueA ) ) return false ;
      Getoken() ;
      if ( gRead != "]" ) return ReturnFalse( gRead ) ;
      size = Str2int( valueA.value ) ;
      GetValueA( id, value, size ) ;
    } // if
    else { // Just Ident : Not A Function or An Array
      GetValue( id, value ) ;
      BackToken() ;
    } // else
    
    PPMMB( id, size ) ; 
    return true ;
  } // else

} // Rest_of_Identifier_started_unary_exp()

bool Signed_unary_exp( Value &value ) {
  Getoken() ;
  if ( gRead == "(" ) {
    bool signN = value.signN ;
    bool signB = value.signB ;
    value.signB = false ;
    value.signN = false ;
    if ( !Exp( value ) ) return false ;
    Getoken() ;
    if ( gRead == ")" ) {
      value.signB = signB ;
      value.signN = signN ;
      SignCalculate( value ) ;
      return true ;
    } // if
    else return ReturnFalse( gRead ) ;
  } // if
  else if ( Constant( gRead, value ) ) {
    SignCalculate( value ) ;
    return true ;
  } // if
  else {
    if ( BeingIdent( gRead ) ) {
      if ( !Rest_of_Identifier_started_unary_exp( gRead, value ) ) return false ;
      SignCalculate( value ) ;
      return true ;
    } // if
    else {
      return ReturnFalse( gRead ) ;
    } // else
  } // else
} // Signed_unary_exp()

bool Unary_exp( Value &value ) {
  if ( PPMM() ) {
    while ( PPMM() ) ;
    Getoken() ;
    if ( BeingIdent( gRead ) ) {
      string id = gRead ;
      Getoken() ;
      
      int size = 0 ;
      if ( gRead == "[" ) {
        Value valueA = {0} ;
        if ( !Exp( valueA ) ) return false ;
        Getoken() ;
        if ( gRead != "]" ) return ReturnFalse( gRead ) ;
        if ( gTime ) size = Str2int( valueA.value ) ;
        else size = 0 ;
        GetValueA( id, value, size ) ;
      } // if
      else {
        GetValue( id, value ) ;
        BackToken() ;
        return true ;
      } // else
      
    } // if
    else 
      return ReturnFalse( gRead ) ;
  } // if
  else {
    while ( Sign( value ) ) ;
    return Signed_unary_exp( value ) ;
  } // else
} // Unary_exp()

bool Rest_of_maybe_mult_exp( Value &value ) {
  
  Getoken() ;
  
  while ( true ) {
    if ( gRead == "*" || gRead == "/" || gRead == "%" ) {
      string op = gRead ;
      Value valueR = {0} ;
      if ( !Unary_exp( valueR ) ) return false ;
        
      if ( !gTime ) {
        if ( CanbeNum( value ) && CanbeNum( valueR ) ) ;
        else {
          cout << "line " << gLine << " : type error!\n" ;
          return false ; 
        } // else
      } // if
      else if ( gTime ) {
        int a, b ;
        
        
        if ( value.type == INT ) a = Str2int( value.value ) ;
        else if ( value.type == CHAR ) a = Ch2int( value.value ) ;
        
        if ( valueR.type == INT ) b = Str2int( valueR.value ) ;
        else if ( valueR.type == CHAR ) b = Ch2int( valueR.value ) ;
        
        if ( op == "*" ) value.value = Int2str( a*b ) ;
        if ( op == "/" ) value.value = Int2str( a/b ) ;
        if ( op == "%" ) value.value = Int2str( a%b ) ;
      } // if
      
      value.type = INT ;
      Getoken() ;
    } // if
    else {
      BackToken() ;
      return true ;
    } // else
  } // while

} // Rest_of_maybe_mult_exp()

bool Maybe_mult_exp( Value &value ) {
  if ( !Unary_exp( value ) ) return false ;
  if ( !Rest_of_maybe_mult_exp( value ) ) return false ;
  return true ;
} // Maybe_mult_exp() 

bool Rest_of_maybe_additive_exp( Value &value ) {
  
  if ( Rest_of_maybe_mult_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "+" || gRead == "-" ) {
        string op = gRead ;
        Value valueR = {0} ;
        if ( !Maybe_mult_exp( valueR ) ) return false ;
        
        if ( !gTime ) {
          if ( CanbeNum( value ) && CanbeNum( valueR ) ) ;
          else if ( CanbeStr( value ) && CanbeStr( valueR ) ) ;
          else {
            cout << "line " << gLine << " : type error!\n" ;
            return false ; 
          } // else
        } // if
        else if ( gTime ) {
          int a, b ;

          if ( value.type == INT ) a = Str2int( value.value ) ;
          else if ( value.type == CHAR ) a = Ch2int( value.value ) ;

          if ( valueR.type == INT ) b = Str2int( valueR.value ) ;
          else if ( valueR.type == CHAR ) b = Ch2int( valueR.value ) ;

          if ( op == "+" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              value.value = Int2str( a+b ) ;
              value.type = INT ;
            } // if
            
            if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              value.value = StrAdd( value, valueR ) ;
              value.type = STRING ;
            } // if
          } // if
          else if ( op == "-" ) {
            value.value = Int2str( a-b ) ;
            value.type = INT ;
          } // if
        } // if
        
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Rest_of_maybe_additive_exp()

bool Maybe_additive_exp( Value &value ) {
  
  if ( Maybe_mult_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "+" || gRead == "-" ) {
        string op = gRead ;
        Value valueR = {0} ;
        if ( !Maybe_mult_exp( valueR ) ) return false ;
        
        if ( !gTime ) {
          if ( CanbeNum( value ) && CanbeNum( valueR ) ) ;
          else if ( CanbeStr( value ) && CanbeStr( valueR ) ) ;
          else {
            cout << "line " << gLine << " : type error!\n" ;
            return false ; 
          } // else
        } // if
        else if ( gTime ) {
          int a, b ;

          if ( value.type == INT ) a = Str2int( value.value ) ;
          else if ( value.type == CHAR ) a = Ch2int( value.value ) ;

          if ( valueR.type == INT ) b = Str2int( valueR.value ) ;
          else if ( valueR.type == CHAR ) b = Ch2int( valueR.value ) ;

          if ( op == "+" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              value.value = Int2str( a+b ) ;
              value.type = INT ;
            } // if
            
            if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              value.value = StrAdd( value, valueR ) ;
              value.type = STRING ;
            } // if
          } // if
          else if ( op == "-" ) {
            value.value = Int2str( a-b ) ;
            value.type = INT ;
          } // if
        } // if
        
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Maybe_additive_exp() 

bool Rest_of_maybe_shift_exp( Value &value ) {
  
  if ( Rest_of_maybe_additive_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( false && ( gRead == "<<" || gRead == ">>" ) ) {
        Value valueR = {0} ;
        if ( !Maybe_additive_exp( valueR ) ) return false ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Rest_of_maybe_shift_exp()

bool Maybe_shift_exp( Value &value ) {
  
  if ( Maybe_additive_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( false && ( gRead == "<<" || gRead == ">>" ) ) {
        Value valueR = {0} ;
        if ( !Maybe_additive_exp( valueR ) ) return false ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Maybe_shift_exp()

bool Rest_of_maybe_relational_exp( Value &value ) {
  
  if ( Rest_of_maybe_shift_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "<" || gRead == ">" || gRead == "<=" || gRead == ">=" ) {
        string op = gRead ;
        Value valueR = {0} ;
        if ( !Maybe_shift_exp( valueR ) ) return false ;
        
        if ( !gTime ) {
          if ( CanbeNum( value ) && CanbeNum( valueR ) ) ;
          else if ( CanbeStr( value ) && CanbeStr( valueR ) ) ;
          else {
            cout << "line " << gLine << " : type error!\n" ;
            return false ; 
          } // else
        } // if
        else if ( gTime ) {
          
          int a, b ;
          string s1, s2 ;
          
          if ( value.type == INT ) a = Str2int( value.value ) ;
          else if ( value.type == CHAR ) a = Ch2int( value.value ) ;
          else if ( value.type == STRING ) s1 = value.value ;

          if ( valueR.type == INT ) b = Str2int( valueR.value ) ;
          else if ( valueR.type == CHAR ) b = Ch2int( valueR.value ) ;
          else if ( valueR.type == STRING ) s2 = valueR.value ;
          
          if ( op == ">" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a > b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 > s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          else if ( op == "<" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a < b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 < s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          else if ( op == ">=" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a > b || a == b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 > s2 || s1 == s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          else if ( op == "<=" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a < b || a == b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 < s2 || a == b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          
        } // if
        
        value.type = BOOL ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Rest_of_maybe_relational_exp()

bool Maybe_relational_exp( Value &value ) {
  
  if ( Maybe_shift_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "<" || gRead == ">" || gRead == "<=" || gRead == ">=" ) {
        string op = gRead ;
        Value valueR = {0} ;
        if ( !Maybe_shift_exp( valueR ) ) return false ;
        
        if ( !gTime ) {
          if ( CanbeNum( value ) && CanbeNum( valueR ) ) ;
          else if ( CanbeStr( value ) && CanbeStr( value ) ) ;
          else {
            cout << "line " << gLine << " : type error!\n" ;
            return false ; 
          } // else
        } // if
        else if ( gTime ) {
          
          int a, b ;
          string s1, s2 ;
          
          if ( value.type == INT ) a = Str2int( value.value ) ;
          else if ( value.type == CHAR ) a = Ch2int( value.value ) ;
          else if ( value.type == STRING ) s1 = value.value ;

          if ( valueR.type == INT ) b = Str2int( valueR.value ) ;
          else if ( valueR.type == CHAR ) b = Ch2int( valueR.value ) ;
          else if ( valueR.type == STRING ) s2 = valueR.value ;
          
          if ( op == ">" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a > b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 > s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          else if ( op == "<" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a < b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 < s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          else if ( op == ">=" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a > b || a == b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 > s2 || s1 == s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          else if ( op == "<=" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a < b || a == b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 < s2 || a == b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          
        } // if
        
        value.type = BOOL ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Maybe_relational_exp()

bool Rest_of_maybe_equality_exp( Value &value ) {
  
  if ( Rest_of_maybe_relational_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "==" || gRead == "!=" ) {
        string op = gRead ;
        Value valueR = {0} ;
        if ( !Maybe_relational_exp( valueR ) ) return false ;
        if ( !gTime ) {
          if ( CanbeNum( value ) && CanbeNum( valueR ) ) ;
          else if ( CanbeStr( value ) && CanbeStr( value ) ) ;
          else {
            cout << "line " << gLine << " : type error!\n" ;
            return false ; 
          } // else
        } // if
        else if ( gTime ) {
          
          int a, b ;
          string s1, s2 ;
          
          if ( value.type == INT ) a = Str2int( value.value ) ;
          else if ( value.type == CHAR ) a = Ch2int( value.value ) ;
          else if ( value.type == STRING ) s1 = value.value ;

          if ( valueR.type == INT ) b = Str2int( valueR.value ) ;
          else if ( valueR.type == CHAR ) b = Ch2int( valueR.value ) ;
          else if ( valueR.type == STRING ) s2 = valueR.value ;
          
          if ( op == "==" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a == b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 == s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          else if ( op == "!=" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a != b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 != s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          
        } // if
        
        value.type = BOOL ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Rest_of_maybe_equality_exp()

bool Maybe_equality_exp( Value &value ) {
  
  if ( Maybe_relational_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "==" || gRead == "!=" ) {
        string op = gRead ;
        Value valueR = {0} ;
        if ( !Maybe_relational_exp( valueR ) ) return false ;
        if ( !gTime ) {
          if ( CanbeNum( value ) && CanbeNum( valueR ) ) ;
          else if ( CanbeStr( value ) && CanbeStr( value ) ) ;
          else {
            cout << "line " << gLine << " : type error!\n" ;
            return false ; 
          } // else
        } // if
        else if ( gTime ) {
          
          int a, b ;
          string s1, s2 ;
          
          if ( value.type == INT ) a = Str2int( value.value ) ;
          else if ( value.type == CHAR ) a = Ch2int( value.value ) ;
          else if ( value.type == STRING ) s1 = value.value ;

          if ( valueR.type == INT ) b = Str2int( valueR.value ) ;
          else if ( valueR.type == CHAR ) b = Ch2int( valueR.value ) ;
          else if ( valueR.type == STRING ) s2 = valueR.value ;
          
          if ( op == "==" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a == b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 == s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          else if ( op == "!=" ) {
            if ( CanbeNum( value ) && CanbeNum( valueR ) ) {
              if ( a != b ) value.value = "true" ;
              else value.value = "false" ;
            } // if
            else if ( CanbeStr( value ) && CanbeStr( valueR ) ) {
              if ( s1 != s2 ) value.value = "true" ;
              else value.value = "false" ;
            } // if
          } // if
          
        } // if
        
        value.type = BOOL ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Maybe_equality_exp()

bool Rest_of_maybe_bit_AND_exp( Value &value ) {
  
  if ( Rest_of_maybe_equality_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "&" ) {
        Value valueR = {0} ;
        if ( !Maybe_equality_exp( valueR ) ) return false ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Rest_of_maybe_bit_AND_exp()

bool Maybe_bit_AND_exp( Value &value ) {
  
  if ( Maybe_equality_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "&" ) {
        Value valueR = {0} ;
        if ( !Maybe_equality_exp( valueR ) ) return false ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Maybe_bit_AND_exp()

bool Rest_of_maybe_bit_EXOR_exp( Value &value ) {
  
  if ( Rest_of_maybe_bit_AND_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "^" ) {
        Value valueR = {0} ;
        if ( !Maybe_bit_AND_exp( valueR ) ) return false ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Rest_of_maybe_bit_EXOR_exp()

bool Maybe_bit_EXOR_exp( Value &value ) {
  
  if ( Maybe_bit_AND_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "^" ) {
        Value valueR = {0} ;
        if ( !Maybe_bit_AND_exp( valueR ) ) return false ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Maybe_bit_EXOR_exp()

bool Rest_of_maybe_bit_OR_exp( Value &value ) {
  
  if ( Rest_of_maybe_bit_EXOR_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "|" ) {
        Value valueR = {0} ;
        if ( !Maybe_bit_EXOR_exp( valueR ) ) return false ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Rest_of_maybe_bit_OR_exp()

bool Maybe_bit_OR_exp( Value &value ) {
  
  if ( Maybe_bit_EXOR_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "|" ) {
        Value valueR = {0} ;
        if ( !Maybe_bit_EXOR_exp( valueR ) ) return false ;
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Maybe_bit_OR_exp()

bool Rest_of_maybe_logical_AND_exp( Value &value ) {
  
  if ( Rest_of_maybe_bit_OR_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "&&" ) {
        Value valueR = {0} ;
        if ( !Maybe_bit_OR_exp( valueR ) ) return false ;
        
        if ( gTime ) {
          bool a = Str2bool( value.value ) ;
          bool b = Str2bool( valueR.value ) ;
          bool result = a && b ;
          if ( result ) value.value = "true" ;
          else value.value = "false" ;
        } // if
        
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Rest_of_maybe_logical_AND_exp()

bool Maybe_logical_AND_exp( Value &value ) {
  
  if ( Maybe_bit_OR_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "&&" ) {
        Value valueR = {0} ;
        if ( !Maybe_bit_OR_exp( valueR ) ) return false ;
        
        if ( gTime ) {
          bool a = Str2bool( value.value ) ;
          bool b = Str2bool( valueR.value ) ;
          bool result = a && b ;
          if ( result ) value.value = "true" ;
          else value.value = "false" ;
        } // if
        
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
} // Maybe_logical_AND_exp() 

bool Rest_of_maybe_logical_OR_exp( Value &value ) {
  
  if ( Rest_of_maybe_logical_AND_exp( value ) ) {
    Getoken() ;
    while ( true ) {
      if ( gRead == "||" ) {
        Value valueR = {0} ;
        if ( !Maybe_logical_AND_exp( valueR ) ) return false ;
        
        if ( gTime ) {
          bool a = Str2bool( value.value ) ;
          bool b = Str2bool( valueR.value ) ;
          bool result = a || b ;
          if ( result ) value.value = "true" ;
          else value.value = "false" ;
        } // if
        
        Getoken() ;
      } // if
      else {
        BackToken() ;
        return true ;
      } // else
    } // while
  } // if
  else
    return false ;
  
} // Rest_of_maybe_logical_OR_exp()

bool Rest_of_non_comma_exp( Value &value ) {
  Getoken() ;
  if ( gRead == "?" ) {
    if ( !Exp( value ) ) return false ;
    Getoken() ;
    if ( gRead == ":" ) {
      if ( !Basic_exp( value ) ) return false ;
      if ( !Rest_of_non_comma_exp( value ) ) return false ;
      return true ;
    } // if
    else ReturnFalse( gRead ) ;
  } // if
  else {
    BackToken() ;
    return true ;
  } // else
} // Rest_of_non_comma_exp()

bool Non_comma_exp( Value &value ) {
  if ( !Basic_exp( value ) ) return false ;
  if ( !Rest_of_non_comma_exp( value ) ) return false ;
  return true ;
} // Non_comma_exp()

bool Actual_parameter_list( string id ) {
  Getoken() ;
  
  if ( gRead == ")" ) {
    BackToken() ;
    return true ;
  } // if
  
  BackToken() ;
  Value value = {0} ;
  if ( !Non_comma_exp( value ) ) return false ;
  Getoken() ;
  
  while ( gRead == "," ) {
    Value value = {0} ;
    if ( !Non_comma_exp( value ) ) return false ;
    Getoken() ;
  } // while
  
  BackToken() ;
  return true ;
  
} // Actual_parameter_list()


bool Rest_id_stated_sign_basic_exp( string id, Value &value ) {
  
  Getoken() ;
  
  if ( gRead == "(" ) {
    if ( !Actual_parameter_list( id ) ) return false ;
    Getoken() ;
    if ( gRead != ")" ) return ReturnFalse( gRead ) ;
    GetValueF( id, value ) ;
    // 未處理: SIGN
    // SignCalculate( value ) ;
    return Rest_of_maybe_logical_OR_exp( value ) ;
  } // if
  else {
    int size = 0 ;
    if ( gRead == "[" ) {
      Value valueA = {0} ;
      if ( !Exp( valueA ) ) return false ;
      Getoken() ;
      if ( gRead != "]" ) return ReturnFalse( gRead ) ;
      if ( gTime ) size = Str2int( valueA.value ) ;
      else size = 0 ;
      GetValueA( id, value, size ) ;
      SignCalculate( value ) ;
    } // if
    else {
      GetValue( id, value ) ;
      SignCalculate( value ) ;
      BackToken() ;
    } // else
      
    PPMMB( id, size ) ;
    
    return Rest_of_maybe_logical_OR_exp( value ) ;
    
  } // else
  
} // Rest_id_stated_sign_basic_exp()

bool Sign_basic_exp( Value &value ) {
    
  Getoken() ;
  
  if ( BeingIdent( gRead ) ) {
    return Rest_id_stated_sign_basic_exp( gRead, value ) ;
  } // if
  
  else {
    if ( Constant( gRead, value ) ) {
      SignCalculate( value ) ;
    } // if
    else {
      if ( gRead == "(" ) {
        bool signN = value.signN ;
        bool signB = value.signB ;
        value.signB = false ;
        value.signN = false ;
        if ( !Exp( value ) ) return false ;
        Getoken() ;
        if ( gRead == ")" ) {
          value.signB = signB ;
          value.signN = signN ;
          SignCalculate( value ) ;
        } // if
        else return ReturnFalse( gRead ) ;
      } // if
      else return ReturnFalse( gRead ) ;
    } // else
    
    return Rest_of_maybe_logical_OR_exp( value ) ;
  } // else

} // Sign_basic_exp()

bool Rest_PPMM_id_stated_basic_exp( Value value ) {
  
} // Rest_PPMM_id_stated_basic_exp()

bool Rest_id_stated_basic_exp( string id, Value &value ) {
  
  Getoken() ;
  
  if ( gRead == "(" ) {
    if ( !Actual_parameter_list( id ) ) return false ;
    Getoken() ;
    if ( gRead != ")" ) return ReturnFalse( gRead ) ;
    GetValueF( id, value ) ;
    return Rest_of_maybe_logical_OR_exp( value ) ;
  } // if
  else {
    int size = 0 ;
    if ( gRead == "[" ) {
      Value valueA = {0} ;
      if ( !Exp( valueA ) ) return false ;
      Getoken() ;
      if ( gRead != "]" ) return ReturnFalse( gRead ) ;
      if ( gTime ) size = Str2int( valueA.value ) ;
      else size = 0 ;
      GetValueA( id, value, size ) ;
    } // if
    else {
      GetValue( id, value ) ;
      BackToken() ;
    } // else
      
    PPMMB( id, size ) ;
    
    Getoken() ;
    if ( Assign_op( gRead ) ) {
      if ( Basic_exp( value ) ) {
        if ( gTime ) Assign_id( id, value, size ) ;
        return true ;
      } // if
      else return false ;
    } // if
    else {
      BackToken() ;
      return Rest_of_maybe_logical_OR_exp( value ) ;
    } // else
    
  } // else
  
} // Rest_id_stated_basic_exp()

bool Basic_exp( Value &value ) {
  
  Getoken() ;
   
  // for ( int i = 0  ; i < gIdent_Function.size() ; i++ ) cout << gIdent_Function[i].name << "][" ;
  // cout << endl ;
  
  if ( BeingIdent( gRead ) ) {
    return Rest_id_stated_basic_exp( gRead, value ) ;
  } // if
  else if ( IsPPMM( gRead ) ) {
    
  } // if
  else if ( IsSign( gRead ) ) {
    BackToken() ;
    while ( Sign( value ) ) ;
    return Sign_basic_exp( value ) ;
  } // if
  else {
    if ( Constant( gRead, value ) ) ;
    else {
      
      if ( gRead == "(" ) {
        if ( !Exp( value ) ) return false ;
        Getoken() ;
        if ( gRead == ")" ) ;
        else return ReturnFalse( gRead ) ;
      } // if
      else return ReturnFalse( gRead ) ;
    } // else
    
    return Rest_of_maybe_logical_OR_exp( value ) ;
    
  } // else

} // Basic_exp()

bool Rest_exp( Value &value ) {
  Getoken() ;
  if ( gRead == "," ) {
    return Exp( value ) ;
  } // if
  else if ( gRead == "?" ) {
    if ( !Exp( value ) ) return false ;
    Getoken() ;
    if ( gRead == ":" ) {
      if ( !Exp( value ) ) return false ;
      return true ;
    } // if
    else ReturnFalse( gRead ) ;
  } // if
  else {
    BackToken() ;
    return true ;
  } // else
} // Rest_exp()

bool Exp( Value &value ) {
  if ( !Basic_exp( value ) ) return false ;
  if ( !Rest_exp( value ) ) return false ;
  return true ;
} // Exp()

bool IO_Input() {
  Getoken() ;
  if ( gRead == ">>" ) {
    Getoken() ;
    if ( !BeingIdent( gRead ) ) return ReturnFalse( gRead ) ;
    Getoken() ;
    if ( gRead == "[" ) {
      Value value ;
      if ( !Exp( value ) ) return false ;
      Getoken() ;
      if ( gRead != "]" ) return ReturnFalse( gRead ) ;
    } // if
    else
      BackToken() ;
    
    Getoken() ;
    if ( gRead == ";" ) return true ;
    else {
      BackToken() ;
      return IO_Input() ;
    } // else
  } // if
  else
    return ReturnFalse( gRead ) ;
} // IO_Input()


bool IO_Output() {
  Getoken() ;
  if ( gRead == "<<" ) {
    Value value = {0} ;
    if ( !Exp( value ) ) return false ;
    if ( gTime ) {
      if ( value.type == STRING ) Coutstring( value.value.substr( 1, value.value.size()-2 ) ) ;
      else cout << value.value ;
    } // if
    
    Getoken() ;
    if ( gRead == ";" ) return true ;
    else {
      BackToken() ;
      return IO_Output() ;
    } // else
  } // if
  else
    return ReturnFalse( gRead ) ;
} // IO_Output()

bool ListAllVariables() {
  Getoken() ; // '('
  
  for ( int i = 0 ; i < gIdent.size() ; i++ ) 
    cout << gIdent[i] -> name << endl ;
  
  Getoken() ; // ')'
  Getoken() ; // ';'
  return true ;
} // ListAllVariables()

bool ListAllFunctions() {
  Getoken() ; // '('
  
  for ( int i = 0 ; i < gFunction.size() ; i++ ) 
    cout << gFunction[i].name << "()\n" ;
  
  Getoken() ; // ')'
  Getoken() ; // ';'
  return true ;
} // ListAllFunctions()

bool ListVariable() {
  Getoken() ; // '('
  Getoken() ; // '"'+name+'"'
  string name = gRead.substr( 1, gRead.size()-2 ) ;
  
  if ( !BeingIdent( name ) ) {
    return false ;
  } // if
  
  int l = 0 ;
  while ( l < gIdent.size() && gIdent[l] -> name != name ) l++ ;
  
  if ( gIdent[l] -> size == 0 )
    cout << GetTypeStr( gIdent[l] -> type ) << " " << gIdent[l] -> name << " ;\n" ;
  else
    cout << GetTypeStr( gIdent[l] -> type ) << " " 
         << gIdent[l] -> name << "[ " << gIdent[l] -> size << " ] ;\n" ;
  
  Getoken() ; // ')'
  Getoken() ; // ';'
  return true ;
} // ListVariable()

bool ListFunction() {
  Getoken() ; // '('
  Getoken() ; // '"'+name+'"'
  string name = gRead.substr( 1, gRead.size()-2 ) ;
  
  if ( !BeingIdent( name ) ) {
    return false ;
  } // if
  
  int l = 0 ;
  while ( l < gFunction.size() && gFunction[l].name != name ) l++ ;
  
  cout << gFunction[l].token[0] ;
  for ( int i = 1, c = 0 ; i < gFunction[l].token.size() ; i++ ) {
    
    if ( gFunction[l].token[i] == "(" && Isident( gFunction[l].token[i-1] ) ) {
      if ( gFunction[l].token[i-1] == "while" || gFunction[l].token[i-1] == "if" )
        cout << " " << gFunction[l].token[i] ;
      else cout << gFunction[l].token[i] ;
    } // if
    else if ( gFunction[l].token[i] == ";" ) {
      cout << " " << gFunction[l].token[i] ;
      cout << endl ;
      cout.width( c ) ;
    } // if
    else if ( gFunction[l].token[i] == "[" || IsPPMM( gFunction[l].token[i] ) ) {
      cout << gFunction[l].token[i] ;
    } // if
    else if ( gFunction[l].token[i] == "," ) {
      cout << gFunction[l].token[i] ;
    } // if
    else if ( gFunction[l].token[i] == "{" ) {
      cout << " " << gFunction[l].token[i] ;
      c += 2 ;
      cout << endl ;
      cout.width( c ) ;
    } // if
    else if ( gFunction[l].token[i] == "}" ) {
      cout.width( c-1 ) ;
      cout << gFunction[l].token[i] ;
      c -= 2 ;
      cout << endl ;
      cout.width( c ) ;
    } // if
    
    else
      cout << " " << gFunction[l].token[i] ;
    
  } // for
  
  Getoken() ; // ')'
  Getoken() ; // ';'
  return true ;
} // ListFunction()

bool If() {
  
  int jump ;
  if ( !gTime ) jump = gToken.size()-1 ; // Location of token "if"
  else jump = gI-1 ;
  
  vector<Ident> temp = gIdent ;
    
  Getoken() ;
  if ( gRead != "(" ) return ReturnFalse( gRead ) ;
  
  Value value = {0} ;
  if ( !Exp( value ) ) return false ;
  
  Getoken() ;
  if ( gRead != ")" ) return ReturnFalse( gRead ) ;
  
  if ( !gTime ) {
    if ( value.type != BOOL ) {
      cout << "line " << gLine << " : type error!\n" ;
      return false ;
    } // if
    
    Getoken() ;
    if ( !Statement() ) {
      gIdent = temp ;
      return false ;
    } // if
    
  } // if
  else { // gTime
    if ( value.value == "true" ) {
      Getoken() ;
      if ( !Statement() ) {
        gIdent = temp ;
        return false ;
      } // if
      
      gI = gToken[jump].jumpEnd ; // Skip else
      gIdent = temp ;
      return true ; // Does not need to excute else
    } // if
    else { // value = false
      if ( gToken[jump].hasjump ) {
        gI = gToken[jump].jumpAddr ;
      } // if
      else {
        gI = gToken[jump].jumpEnd ;
        gIdent = temp ;
        return true ;
      } // else
    } // else
  } // else
  
  gIdent = temp ;
  
  int nl ;
  if ( !gTime ) nl = gLine ;
  Getoken() ;
  if ( !gTime ) nl = gLine - nl ;
  if ( gRead == "else" ) {
    if ( !gTime ) {
      gToken[jump].jumpAddr = gToken.size()-1 ; // Assign location of token "else"
      gToken[jump].hasjump = true ;
    } // if
    
    if ( !gTime ) gLine_add = 0 ;
    Getoken() ;
    if ( !Statement() ) {
      gIdent = temp ;
      return false ;
    } // if
    
    if ( !gTime ) {
      gToken[jump].jumpEnd = gToken.size() ; // Assign location of token "else"
    } // if
    
    gIdent = temp ;
    return true ;
  } // if
  else { // Not Get Token "else" plus gLine_add on next sta's gLine
    if ( !gTime ) {
      gToken[jump].jumpEnd = gToken.size()-1 ; // Assign Last location
      gLine_add = nl ;
    } // if
    
    BackToken() ;
    return true ;
  } // else
  
} // If()

bool While() {
  
  int jump, jumpEXP ;
  
  if ( !gTime ) jump = gToken.size()-1 ; // Location of token "if"
  else jump = gI-1 ;
  
  vector<Ident> temp = gIdent ;
  
  Getoken() ;
  if ( gRead != "(" ) return ReturnFalse( gRead ) ;
  
  if ( gTime ) jumpEXP = gI ;
  Value value = {0} ;
  if ( !Exp( value ) ) return false ;
  
  Getoken() ;
  if ( gRead != ")" ) return ReturnFalse( gRead ) ;
  
  if ( !gTime ) {
    if ( value.type != BOOL ) {
      cout << "line " << gLine << " : type error!\n" ;
      return false ;
    } // if
    
    Getoken() ;
    if ( !Statement() ) {
      gIdent = temp ;
      return false ;
    } // if
    
    gToken[jump].jumpEnd = gToken.size() ;
    
  } // if
  else { // gTime
    while ( value.value == "true" ) {
      
      Getoken() ;
      if ( !Statement() ) {
        gIdent = temp ;
        return false ;
      } // if
      
      gI = jumpEXP ;
      value.value = "" ;
      if ( !Exp( value ) ) return false ;
      gI++ ; // skip ')'
      
    } // while
    
    gI = gToken[jump].jumpEnd ;
  } // else
    
  gIdent = temp ;
  return true ;
} // While()

bool Statement() {
  
  Value value = {0} ;
  
  if ( gRead == ";" ) {
    return true ;
  } // if
  else if ( gRead == "cout" ) {
    return IO_Output() ;
  } // if
  else if ( gRead == "cin" ) {
    return IO_Input() ;
  } // if
  else if ( gRead == "ListAllVariables" ) {
    if ( gTime ) return true ;
    return ListAllVariables() ;
  } // if
  else if ( gRead == "ListVariable" ) {
    if ( gTime ) return true ;
    return ListVariable() ;
  } // if
  else if ( gRead == "ListAllFunctions" ) {
    if ( gTime ) return true ;
    return ListAllFunctions() ;
  } // if
  else if ( gRead == "ListFunction" ) {
    if ( gTime ) return true ;
    return ListFunction() ;
  } // if
  else if ( gRead == "{" ) {
    return Compound_statement() ;
  } // if
  else if ( gRead == "if" ) {
    return If() ;
  } // if
  else if ( gRead == "while" ) {
    return While() ;
  } // if
  else if ( gRead == "do" ) {
    return true ;
  } // if
  else if ( gRead == "return" ) {
    Getoken() ;
    if ( gRead == ";" ) return true ;
    else {
      BackToken() ;
      if ( !Exp( value ) ) return false ;
      Getoken() ;
      if ( gRead != ";" ) return ReturnFalse( gRead ) ;
      return true ;
    } // else
  } // if
  else {
    BackToken() ;
    if ( !Exp( value ) ) return false ;
    Getoken() ;
    if ( gRead == ";" ) return true ;
    else return ReturnFalse( gRead ) ;
  } // else
} // Statement()

bool ArrayConstant( int &size ) {
  if ( gRead == "[" ) {
    if ( IsNum( Getoken() ) ) {
      size = atoi( gRead.c_str() ) ;
      if ( Getoken() == "]" ) {
        return true ;
      } // if
      else return false ;
    } // if
    else return false ;
  } // if
  // else 沒有接ArrayConstant
  else {
    BackToken() ;
    size = 0 ;
    return true ;
  } // else
  
} // ArrayConstant()

bool Declaration( string name, int type ) {
  
  if ( !Isident( name ) ) return ReturnFalse( name ) ;
  
  Getoken() ;
  
  int size ;

  if ( !ArrayConstant( size ) ) return false ;

  Getoken() ;

  while ( gRead == "," ) {
    AddIdent( type, name, size ) ;
    
    Getoken() ;
    if ( Isident( gRead ) ) {
      name = gRead ;
      Getoken() ;
      if ( !ArrayConstant( size ) ) return false ;
      Getoken() ; // ',' || ';' 逗號繼續迴圈 分號OR其他出迴圈
    } // if
    else
      return ReturnFalse( gRead ) ;          
  } // while

  if ( gRead == ";" ) {
    AddIdent( type, name, size ) ;
    return true ;
  } // if

  else 
    return ReturnFalse( gRead ) ;
  
} // Declaration()

bool Compound_statement() {
  
  for ( bool b = false ; !b ; ) {
    Getoken() ;
    if ( Type_Specifier( gRead ) ) {
      if ( !Declaration( Getoken(), GetType( gRead ) ) ) return false ;
    } // if
    else {
      BackToken() ;
      b = true ;
    } // else
  } // for
  
  while ( true ) {
    Getoken() ;
    
    if ( gRead == "}" ) return true ;
    else if ( !Statement() ) return false ;
  } // while

} // Compound_statement()

bool Formal_parameter_list() {
  int type ;
  string name ;
  int size ;
  if ( Type_Specifier( gRead ) ) {
    type = GetType( gRead ) ;
    Getoken() ;
    if ( gRead == "&" ) {
      Getoken() ;
    } // if
    
    if ( !Isident( gRead ) ) return ReturnFalse( gRead ) ;
    name = gRead ;
    
    Getoken() ;
    
    if ( !ArrayConstant( size ) )
      return false ;
    
    Getoken() ;
    while ( gRead == "," ) {
      AddIdent( type, name, size ) ;
      Getoken() ;
      if ( !Type_Specifier( gRead ) ) return ReturnFalse( gRead ) ;
      Getoken() ;
      if ( gRead == "&" ) {
        Getoken() ;
      } // if
      
      if ( !Isident( gRead ) ) return ReturnFalse( gRead ) ;
      name = gRead ;
      
      Getoken() ;
      if ( gRead == "[" ) {
        Getoken() ;
        Value value = {0} ;
        if ( !Constant( gRead, value ) ) return ReturnFalse( gRead ) ;
        Getoken() ;
        if ( gRead != "]" ) return ReturnFalse( gRead ) ;
      } // if
      else {
        size = 0 ;
        BackToken() ;
      } // else
      
      Getoken() ;
    } // while
    
    AddIdent( type, name, size ) ;
    
    BackToken() ;
    return true ;
    
  } // if
  else {
    BackToken() ;
    return true ;
  } // else 
} // Formal_parameter_list()

// (' [ VOID | formal_parameter_list ] ')' compound_statement
bool Function_Definition_Without_ID( string name, int type ) {
  gAtFunction = true ;
  gAtFunctionName = name ;
  Getoken() ;
  if ( gRead != "(" ) return ReturnFalse( gRead ) ;
  Getoken() ;
  if ( gRead == "void" ) ;
  else if ( !Formal_parameter_list() ) return false ;
  
  Getoken() ;
  if ( gRead != ")" ) return ReturnFalse( gRead ) ;
  
  Getoken() ;
  if ( gRead != "{" ) return ReturnFalse( gRead ) ; 
  else {
    if ( !Compound_statement() ) return false ;
    if ( !gTime ) {
      if ( AddFuction( name, type ) )
        cout << "Definition of " << name << "() entered ...\n" ;
      else
        cout << "New definition of " << name << "() entered ...\n" ;
    } // if
    
    return true ;
  } // else
  
} // Function_Definition_Without_ID()

bool Function_Definition_or_Declarators( string name, int type ) {
  
  if ( !Isident( name ) ) return ReturnFalse( name ) ;
  
  Getoken() ;
  
  int size ;
  
  if ( gRead == "(" ) {
    BackToken() ;
    if ( gTime ) return true ;
    else return Function_Definition_Without_ID( name, type ) ;
  } // if
  else {
    
    if ( !ArrayConstant( size ) )
      return false ;
    
    Getoken() ;
        
    while ( gRead == "," ) {
      if ( gTime ) {
        if ( AddIdent( type, name, size ) )
          cout << "Definition of " << name << " entered ...\n" ;
        else
          cout << "New definition of " << name << " entered ...\n" ;
      } // if
      
      Getoken() ;
      if ( Isident( gRead ) ) {
        name = gRead ;
        Getoken() ;
        if ( !ArrayConstant( size ) ) return false ;
        Getoken() ; // ',' || ';' 逗號繼續迴圈 分號OR其他出迴圈
      } // if
      else
        return ReturnFalse( gRead ) ;          
    } // while

    if ( gRead == ";" ) {
      if ( gTime ) {
        if ( AddIdent( type, name, size ) )
          cout << "Definition of " << name << " entered ...\n" ;
        else
          cout << "New definition of " << name << " entered ...\n" ;
      } // if
      
      return true ;
    } // if
    
    else 
      return ReturnFalse( gRead ) ;
    
  } // else
  
} // Function_Definition_or_Declarators()


bool Definition() {
  
  if ( gRead == "void" ) {
    if ( gTime ) return true ;
    else return Function_Definition_Without_ID( Getoken(), VOID ) ;
  } // if
  // else : str is Type_Specifier
  else {
    return Function_Definition_or_Declarators( Getoken(), GetType( gRead ) ) ;
  } // else
  
} // Definition()
// =============================================================================

bool User_input() {
  
  Getoken() ;
    
  if ( gRead == "Done" ) {
    gDone = true ;
    return true ;
  } // if
  
  else if ( Type_Specifier( gRead ) || gRead == "void" ) {
    return Definition() ;
  } // if
  else {
    if ( !Statement() ) return false ;
    else {
      if ( gTime ) cout << "Statement executed ...\n" ;
      return true ;
    } // else
  } // else
  
} // User_input()

bool SyntaxCheck() {
  gTime = false ;
  return User_input() ;
} // SyntaxCheck()

bool Evaluate() {
  gTime = true ;
  gI = 0 ;
  return User_input() ;
} // Evaluate()


int main() { 
    
  printf( "Our-C running ...\n" ) ;
  
  if ( !gTest ) cin >> uTestNum ;
  while ( !gDone ) {
        
    gLine = 0 + gLine_add ;
    gLine_add = 0 ;
    gLevel = 1 ;
    gAtFunction = false ;
    gToken.clear() ;
    
    cout << "> " ;
    
    if ( SyntaxCheck() ) {
      
      if ( !gDone )
        if ( Evaluate() ) {
          // cout << "Successful Evaluate!\n" ;
        } // if
      
    } // if
    else {
      gHasTemp = false ;
      gTemp = "" ;
      SkipLine() ;
    } // else
    
  } // while
  
  
  printf( "Our-C exited ..." ) ;
  system("pause") ;
  return EXIT_SUCCESS;
   
  
} // main()
