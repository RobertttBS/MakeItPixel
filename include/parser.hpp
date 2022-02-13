/* A Bison parser, made by GNU Bison 3.8.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_INCLUDE_PARSER_HPP_INCLUDED
# define YY_YY_INCLUDE_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/parser.bison"

  #include <iostream>
  #include <stack>
  #include <stdexcept>
  #include <cstring>
  #include "Value.hpp"
  #include "ProgramState.hpp"
  extern int yylex();
  extern FILE* yyin;
  void yyerror(const char* msg);
  void prompt(bool force=false);
  #include "BuiltInFunctions.hpp"

#line 63 "include/parser.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IN = 258,                      /* IN  */
    SHIFTR = 259,                  /* SHIFTR  */
    SHIFTL = 260,                  /* SHIFTL  */
    LERP = 261,                    /* LERP  */
    SATURATE = 262,                /* SATURATE  */
    DESATURATE = 263,              /* DESATURATE  */
    LIGHTEN = 264,                 /* LIGHTEN  */
    DARKEN = 265,                  /* DARKEN  */
    GROP1 = 266,                   /* GROP1  */
    GROP2 = 267,                   /* GROP2  */
    VALUE = 268,                   /* VALUE  */
    VARIABLE = 269                 /* VARIABLE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "src/parser.bison"

    char string[32];
    mipa::Value *innervalue;
    std::stack<mipa::Value*> *stack;

#line 100 "include/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);

/* "%code provides" blocks.  */
#line 14 "src/parser.bison"

  extern void set_input_string(const char* in);
  extern void end_lexical_scan(void);

#line 120 "include/parser.hpp"

#endif /* !YY_YY_INCLUDE_PARSER_HPP_INCLUDED  */
