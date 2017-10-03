# Parser

CSE340 Summer 2016 Project

Parse with scoping 

Grammar:
  program
  global_vars
  global_vars
  var_list
  var_list
  scope
  public_vars
  public_vars
  private_vars  -> ε
  private_vars  -> PRIVATE COLON var_list SEMICOLON
  stmt_list     -> stmt
  stmt_list     -> stmt stmt_list
  stmt          -> ID EQUA
