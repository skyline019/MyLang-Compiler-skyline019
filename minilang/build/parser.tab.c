/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "../src/parser.y"

#include "../src/ast.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern FILE *yyin;
extern char* yytext;

void yyerror(const char *s);
ASTNode *program_root;


#line 86 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER = 3,                    /* INTEGER  */
  YYSYMBOL_FLOAT = 4,                      /* FLOAT  */
  YYSYMBOL_IDENTIFIER = 5,                 /* IDENTIFIER  */
  YYSYMBOL_STRING = 6,                     /* STRING  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_INT = 11,                       /* INT  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_PRINT = 13,                     /* PRINT  */
  YYSYMBOL_FUNCTION = 14,                  /* FUNCTION  */
  YYSYMBOL_INT_ARRAY = 15,                 /* INT_ARRAY  */
  YYSYMBOL_FLOAT_ARRAY = 16,               /* FLOAT_ARRAY  */
  YYSYMBOL_STRING_ARRAY = 17,              /* STRING_ARRAY  */
  YYSYMBOL_EQ = 18,                        /* EQ  */
  YYSYMBOL_NE = 19,                        /* NE  */
  YYSYMBOL_LE = 20,                        /* LE  */
  YYSYMBOL_GE = 21,                        /* GE  */
  YYSYMBOL_AND = 22,                       /* AND  */
  YYSYMBOL_OR = 23,                        /* OR  */
  YYSYMBOL_IFX = 24,                       /* IFX  */
  YYSYMBOL_25_ = 25,                       /* '['  */
  YYSYMBOL_26_ = 26,                       /* ']'  */
  YYSYMBOL_27_ = 27,                       /* ';'  */
  YYSYMBOL_28_ = 28,                       /* '('  */
  YYSYMBOL_29_ = 29,                       /* ')'  */
  YYSYMBOL_30_ = 30,                       /* ','  */
  YYSYMBOL_31_ = 31,                       /* ':'  */
  YYSYMBOL_32_ = 32,                       /* '='  */
  YYSYMBOL_33_ = 33,                       /* '{'  */
  YYSYMBOL_34_ = 34,                       /* '}'  */
  YYSYMBOL_35_ = 35,                       /* '<'  */
  YYSYMBOL_36_ = 36,                       /* '>'  */
  YYSYMBOL_37_ = 37,                       /* '+'  */
  YYSYMBOL_38_ = 38,                       /* '-'  */
  YYSYMBOL_39_ = 39,                       /* '*'  */
  YYSYMBOL_40_ = 40,                       /* '/'  */
  YYSYMBOL_41_ = 41,                       /* '%'  */
  YYSYMBOL_YYACCEPT = 42,                  /* $accept  */
  YYSYMBOL_program = 43,                   /* program  */
  YYSYMBOL_stmt_list = 44,                 /* stmt_list  */
  YYSYMBOL_stmt = 45,                      /* stmt  */
  YYSYMBOL_function_def = 46,              /* function_def  */
  YYSYMBOL_param_list = 47,                /* param_list  */
  YYSYMBOL_param = 48,                     /* param  */
  YYSYMBOL_type_specifier = 49,            /* type_specifier  */
  YYSYMBOL_decl = 50,                      /* decl  */
  YYSYMBOL_array_decl = 51,                /* array_decl  */
  YYSYMBOL_block = 52,                     /* block  */
  YYSYMBOL_if_stmt = 53,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 54,                /* while_stmt  */
  YYSYMBOL_for_stmt = 55,                  /* for_stmt  */
  YYSYMBOL_expr = 56,                      /* expr  */
  YYSYMBOL_logical_expr = 57,              /* logical_expr  */
  YYSYMBOL_equality_expr = 58,             /* equality_expr  */
  YYSYMBOL_rel_expr = 59,                  /* rel_expr  */
  YYSYMBOL_add_expr = 60,                  /* add_expr  */
  YYSYMBOL_mul_expr = 61,                  /* mul_expr  */
  YYSYMBOL_primary = 62,                   /* primary  */
  YYSYMBOL_array_access = 63,              /* array_access  */
  YYSYMBOL_expr_list = 64                  /* expr_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  56
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   206

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  161

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   279


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    41,     2,     2,
      28,    29,    39,    37,    30,    38,     2,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    31,    27,
      35,    32,    36,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    25,     2,    26,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    33,     2,    34,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    38,    38,    46,    51,    61,    62,    63,    64,    65,
      66,    67,    68,    92,    99,   145,   146,   147,   150,   155,
     159,   164,   174,   180,   181,   182,   183,   184,   185,   188,
     189,   190,   191,   192,   193,   196,   197,   198,   199,   200,
     201,   204,   205,   211,   214,   219,   224,   229,   232,   235,
     238,   241,   244,   247,   250,   253,   256,   259,   262,   265,
     268,   271,   274,   277,   280,   283,   286,   289,   292,   293,
     294,   295,   296,   297,   298,   302,   334,   339,   348
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER", "FLOAT",
  "IDENTIFIER", "STRING", "IF", "ELSE", "WHILE", "FOR", "INT", "RETURN",
  "PRINT", "FUNCTION", "INT_ARRAY", "FLOAT_ARRAY", "STRING_ARRAY", "EQ",
  "NE", "LE", "GE", "AND", "OR", "IFX", "'['", "']'", "';'", "'('", "')'",
  "','", "':'", "'='", "'{'", "'}'", "'<'", "'>'", "'+'", "'-'", "'*'",
  "'/'", "'%'", "$accept", "program", "stmt_list", "stmt", "function_def",
  "param_list", "param", "type_specifier", "decl", "array_decl", "block",
  "if_stmt", "while_stmt", "for_stmt", "expr", "logical_expr",
  "equality_expr", "rel_expr", "add_expr", "mul_expr", "primary",
  "array_access", "expr_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     115,   -57,    -3,    40,     2,   -10,     4,    13,    45,     0,
      36,    66,    69,    86,    97,   -57,     0,    10,   111,   115,
     -57,   -57,   106,   113,   -57,   -57,   -57,   -57,   117,    82,
      96,    72,    79,    55,   -57,    77,   142,     0,    57,     0,
     143,     0,     0,     0,   144,   -57,   -57,   150,    75,   151,
     153,   155,   156,   154,   -57,    42,   -57,   -57,   -57,   -57,
     -57,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,     0,     0,   158,   -57,   -57,   108,
     -57,     0,   157,   159,   160,     0,   -57,   133,   161,   177,
       0,     0,     0,   -57,   -57,    14,    96,   -57,    96,    72,
      72,    79,    79,    79,    79,    55,    55,   -57,   -57,   -57,
     -57,   -57,   -57,   -57,     0,   -57,   115,   115,     0,   -57,
     162,     0,   164,   163,   135,   -57,   166,   167,   169,   -57,
     188,   -57,   170,   -57,   137,   -57,   130,   168,   177,   -57,
     -57,   -57,   115,     0,   171,   -57,   -57,   -57,   -57,   -57,
     -57,   -57,   130,   -57,   -57,   172,   -57,   152,   115,   -57,
     -57
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    68,    69,    71,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,     0,     0,     0,     2,
       3,    15,     0,     0,    16,     8,     9,    10,     0,    49,
      52,    55,    60,    63,    67,    72,    31,     0,     0,     0,
      33,     0,     0,     0,    29,    69,    70,     0,     0,     0,
      35,    37,    39,     0,    42,     0,     1,     4,     6,     7,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,    77,     0,
      47,     0,     0,     0,     0,     0,    11,    70,     0,    19,
       0,     0,     0,    73,    41,    71,    50,    72,    51,    53,
      54,    58,    59,    56,    57,    61,    62,    64,    65,    66,
      48,    32,    76,    75,     0,    34,     0,     0,     0,    30,
       0,     0,     0,     0,     0,    20,     0,     0,     0,    78,
      43,    45,     0,    13,     0,    12,     0,     0,     0,    36,
      38,    40,     0,     0,     0,    24,    25,    23,    26,    27,
      28,    22,     0,    21,    44,     0,    14,     0,     0,    18,
      46
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -57,   -57,   183,   -19,   -57,   -57,    64,    51,   -57,   -57,
      47,   -57,   -57,   -57,    -8,   -57,   107,   109,   -56,   101,
      28,    88,    85
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    18,    19,    20,    21,   124,   125,   151,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    79
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      57,    47,    36,     1,    45,     3,    46,    40,    53,   101,
     102,   103,   104,     1,     2,     3,     4,     5,    41,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    16,    76,
      78,    80,    42,    82,    83,    84,    57,    15,    16,    37,
      88,    43,    38,    17,    54,     1,     2,     3,     4,     5,
      44,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       1,    45,     3,    46,    48,    37,   110,   111,    38,    15,
      16,    49,    39,   115,    50,    17,    94,   119,     1,    45,
       3,    87,   126,   127,   128,    16,    77,     1,    45,    95,
      46,    51,    65,    66,    71,    72,    73,   130,   131,   107,
     108,   109,    52,    16,    61,    62,   129,    67,    68,    74,
     132,    56,    16,    78,    63,    64,    69,    70,     1,     2,
       3,     4,     5,   154,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    58,   145,   155,   146,   113,   114,   160,
      59,   147,    15,    16,    60,   148,   149,   150,    17,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,   120,   121,   137,   138,   144,   114,    96,    98,
     105,   106,    99,   100,    75,    81,    85,    86,    90,    89,
      91,    92,   123,    93,   112,    17,   116,   118,   117,   133,
     122,   135,   139,   140,   136,   141,   142,   143,   156,   152,
      55,   158,   153,   157,   159,     0,   134
};

static const yytype_int16 yycheck[] =
{
      19,     9,     5,     3,     4,     5,     6,     5,    16,    65,
      66,    67,    68,     3,     4,     5,     6,     7,    28,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    28,    37,
      38,    39,    28,    41,    42,    43,    55,    27,    28,    25,
      48,    28,    28,    33,    34,     3,     4,     5,     6,     7,
       5,     9,    10,    11,    12,    13,    14,    15,    16,    17,
       3,     4,     5,     6,    28,    25,    74,    75,    28,    27,
      28,     5,    32,    81,     5,    33,    34,    85,     3,     4,
       5,     6,    90,    91,    92,    28,    29,     3,     4,     5,
       6,     5,    20,    21,    39,    40,    41,   116,   117,    71,
      72,    73,     5,    28,    22,    23,   114,    35,    36,    32,
     118,     0,    28,   121,    18,    19,    37,    38,     3,     4,
       5,     6,     7,   142,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    27,     4,   143,     6,    29,    30,   158,
      27,    11,    27,    28,    27,    15,    16,    17,    33,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    29,    30,    29,    30,    29,    30,    61,    62,
      69,    70,    63,    64,    32,    32,    32,    27,    25,    28,
      25,    25,     5,    29,    26,    33,    29,    27,    29,    27,
      29,    27,    26,    26,    31,    26,     8,    27,    27,    31,
      17,    29,   138,   152,   157,    -1,   121
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    27,    28,    33,    43,    44,
      45,    46,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,     5,    25,    28,    32,
       5,    28,    28,    28,     5,     4,     6,    56,    28,     5,
       5,     5,     5,    56,    34,    44,     0,    45,    27,    27,
      27,    22,    23,    18,    19,    20,    21,    35,    36,    37,
      38,    39,    40,    41,    32,    32,    56,    29,    56,    64,
      56,    32,    56,    56,    56,    32,    27,     6,    56,    28,
      25,    25,    25,    29,    34,     5,    58,    63,    58,    59,
      59,    60,    60,    60,    60,    61,    61,    62,    62,    62,
      56,    56,    26,    29,    30,    56,    29,    29,    27,    56,
      29,    30,    29,     5,    47,    48,    56,    56,    56,    56,
      45,    45,    56,    27,    64,    27,    31,    29,    30,    26,
      26,    26,     8,    27,    29,     4,     6,    11,    15,    16,
      17,    49,    31,    48,    45,    56,    27,    49,    29,    52,
      45
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    46,    47,
      47,    47,    48,    49,    49,    49,    49,    49,    49,    50,
      50,    50,    50,    50,    50,    51,    51,    51,    51,    51,
      51,    52,    52,    53,    53,    54,    55,    56,    56,    56,
      57,    57,    57,    58,    58,    58,    59,    59,    59,    59,
      59,    60,    60,    60,    61,    61,    61,    61,    62,    62,
      62,    62,    62,    62,    62,    62,    63,    64,    64
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     1,     1,
       1,     3,     5,     5,     7,     1,     1,     1,     8,     0,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     2,
       4,     2,     4,     2,     4,     2,     5,     2,     5,     2,
       5,     3,     2,     5,     7,     5,     9,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     3,     3,     4,     4,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmt_list  */
#line 38 "../src/parser.y"
                   {
    program_root = (yyvsp[0].ast);
    printf("Parse successful! Generated AST:\n");
    ast_print(program_root, 0);
    YYACCEPT;
}
#line 1251 "parser.tab.c"
    break;

  case 3: /* stmt_list: stmt  */
#line 46 "../src/parser.y"
                { 
    ASTNode **stmts = malloc(sizeof(ASTNode *));
    stmts[0] = (yyvsp[0].ast);
    (yyval.ast) = ast_new_block(stmts, 1, yylineno);
}
#line 1261 "parser.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 51 "../src/parser.y"
                 {
    ASTNode **new_stmts = realloc((yyvsp[-1].ast)->block.statements, 
                                 ((yyvsp[-1].ast)->block.count + 1) * sizeof(ASTNode *));
    new_stmts[(yyvsp[-1].ast)->block.count] = (yyvsp[0].ast);
    (yyvsp[-1].ast)->block.statements = new_stmts;
    (yyvsp[-1].ast)->block.count++;
    (yyval.ast) = (yyvsp[-1].ast);
}
#line 1274 "parser.tab.c"
    break;

  case 5: /* stmt: expr ';'  */
#line 61 "../src/parser.y"
               { (yyval.ast) = (yyvsp[-1].ast); }
#line 1280 "parser.tab.c"
    break;

  case 6: /* stmt: decl ';'  */
#line 62 "../src/parser.y"
           { (yyval.ast) = (yyvsp[-1].ast); }
#line 1286 "parser.tab.c"
    break;

  case 7: /* stmt: array_decl ';'  */
#line 63 "../src/parser.y"
                 { (yyval.ast) = (yyvsp[-1].ast); }
#line 1292 "parser.tab.c"
    break;

  case 8: /* stmt: if_stmt  */
#line 64 "../src/parser.y"
          { (yyval.ast) = (yyvsp[0].ast); }
#line 1298 "parser.tab.c"
    break;

  case 9: /* stmt: while_stmt  */
#line 65 "../src/parser.y"
             { (yyval.ast) = (yyvsp[0].ast); }
#line 1304 "parser.tab.c"
    break;

  case 10: /* stmt: for_stmt  */
#line 66 "../src/parser.y"
           { (yyval.ast) = (yyvsp[0].ast); }
#line 1310 "parser.tab.c"
    break;

  case 11: /* stmt: RETURN expr ';'  */
#line 67 "../src/parser.y"
                  { (yyval.ast) = ast_new_return((yyvsp[-1].ast), yylineno); }
#line 1316 "parser.tab.c"
    break;

  case 12: /* stmt: PRINT '(' expr ')' ';'  */
#line 68 "../src/parser.y"
                         { 
    // print(a) -> printf("%d", a) 或 printf("%f", a) 或 printf("%s", a)
    char *format_str;
    if ((yyvsp[-2].ast)->type == AST_INTEGER) {
        format_str = "%d";
    } else if ((yyvsp[-2].ast)->type == AST_FLOAT) {
        format_str = "%f";
    } else if ((yyvsp[-2].ast)->type == AST_STRING) {
        format_str = "%s";
    } else {
        // 对于变量或其他表达式，使用通用格式
        format_str = "%d";
    }
    
    // 创建格式字符串节点
    ASTNode *format_node = ast_new_string(format_str, yylineno);
    
    // 创建参数数组，包含格式字符串和表达式
    ASTNode **args = malloc(2 * sizeof(ASTNode *));
    args[0] = format_node;
    args[1] = (yyvsp[-2].ast);
    
    (yyval.ast) = ast_new_formatted_print(format_str, args, 2, yylineno);
}
#line 1345 "parser.tab.c"
    break;

  case 13: /* stmt: PRINT '(' STRING ')' ';'  */
#line 92 "../src/parser.y"
                           { 
    // print("hello") -> printf("hello")
    ASTNode **args = malloc(sizeof(ASTNode *));
    args[0] = ast_new_string((yyvsp[-2].string), yylineno);
    
    (yyval.ast) = ast_new_formatted_print((yyvsp[-2].string), args, 1, yylineno);
}
#line 1357 "parser.tab.c"
    break;

  case 14: /* stmt: PRINT '(' STRING ',' expr_list ')' ';'  */
#line 99 "../src/parser.y"
                                         { 
    // print("value: %d", a) -> printf("value: %d", a)
    // 检查格式字符串中的格式说明符数量
    char *format = (yyvsp[-4].string);
    int format_count = 0;
    for (int i = 0; format[i]; i++) {
        if (format[i] == '%' && format[i + 1] != '%') {
            format_count++;
        }
    }
    
    // 计算实际参数数量
    int arg_count = 0;
    ASTNode *current = (yyvsp[-2].ast);
    while (current != NULL) {
        arg_count++;
        current = current->binary.right;
    }
    
    if (format_count != arg_count) {
        fprintf(stderr, "Error: Format string contains %d format specifiers but %d arguments provided\n", format_count, arg_count);
        YYERROR;
    }
    
    // 创建参数数组
    ASTNode **args = malloc((arg_count + 1) * sizeof(ASTNode *));
    args[0] = ast_new_string((yyvsp[-4].string), yylineno);
    
    // 将表达式列表转换为参数数组
    current = (yyvsp[-2].ast);
    for (int i = 0; i < arg_count; i++) {
        args[i + 1] = current->binary.left;
        current = current->binary.right;
    }
    
    (yyval.ast) = ast_new_formatted_print((yyvsp[-4].string), args, arg_count + 1, yylineno);
    
    // 释放临时的表达式列表节点
    current = (yyvsp[-2].ast);
    while (current != NULL) {
        ASTNode *next = current->binary.right;
        free(current->binary.op);
        free(current);
        current = next;
    }
}
#line 1408 "parser.tab.c"
    break;

  case 15: /* stmt: function_def  */
#line 145 "../src/parser.y"
               { (yyval.ast) = (yyvsp[0].ast); }
#line 1414 "parser.tab.c"
    break;

  case 16: /* stmt: block  */
#line 146 "../src/parser.y"
        { (yyval.ast) = (yyvsp[0].ast); }
#line 1420 "parser.tab.c"
    break;

  case 17: /* stmt: ';'  */
#line 147 "../src/parser.y"
      { (yyval.ast) = ast_new_empty(yylineno); }
#line 1426 "parser.tab.c"
    break;

  case 18: /* function_def: FUNCTION IDENTIFIER '(' param_list ')' ':' type_specifier block  */
#line 150 "../src/parser.y"
                                                                              {
    (yyval.ast) = ast_new_function_def((yyvsp[-6].string), (yyvsp[-1].string), (yyvsp[-4].ast), (yyvsp[0].ast), yylineno);
}
#line 1434 "parser.tab.c"
    break;

  case 19: /* param_list: %empty  */
#line 155 "../src/parser.y"
                        { 
    ASTNode **params = NULL;
    (yyval.ast) = ast_new_block(params, 0, yylineno);
}
#line 1443 "parser.tab.c"
    break;

  case 20: /* param_list: param  */
#line 159 "../src/parser.y"
        { 
    ASTNode **params = malloc(sizeof(ASTNode *));
    params[0] = (yyvsp[0].ast);
    (yyval.ast) = ast_new_block(params, 1, yylineno);
}
#line 1453 "parser.tab.c"
    break;

  case 21: /* param_list: param_list ',' param  */
#line 164 "../src/parser.y"
                       {
    ASTNode **new_params = realloc((yyvsp[-2].ast)->block.statements, 
                                  ((yyvsp[-2].ast)->block.count + 1) * sizeof(ASTNode *));
    new_params[(yyvsp[-2].ast)->block.count] = (yyvsp[0].ast);
    (yyvsp[-2].ast)->block.statements = new_params;
    (yyvsp[-2].ast)->block.count++;
    (yyval.ast) = (yyvsp[-2].ast);
}
#line 1466 "parser.tab.c"
    break;

  case 22: /* param: IDENTIFIER ':' type_specifier  */
#line 174 "../src/parser.y"
                                     {
    (yyval.ast) = ast_new_param_declaration((yyvsp[-2].string), (yyvsp[0].string), yylineno);
}
#line 1474 "parser.tab.c"
    break;

  case 23: /* type_specifier: INT  */
#line 180 "../src/parser.y"
                    { (yyval.string) = strdup("int"); }
#line 1480 "parser.tab.c"
    break;

  case 24: /* type_specifier: FLOAT  */
#line 181 "../src/parser.y"
        { (yyval.string) = strdup("float"); }
#line 1486 "parser.tab.c"
    break;

  case 25: /* type_specifier: STRING  */
#line 182 "../src/parser.y"
         { (yyval.string) = strdup("string"); }
#line 1492 "parser.tab.c"
    break;

  case 26: /* type_specifier: INT_ARRAY  */
#line 183 "../src/parser.y"
            { (yyval.string) = strdup("int[]"); }
#line 1498 "parser.tab.c"
    break;

  case 27: /* type_specifier: FLOAT_ARRAY  */
#line 184 "../src/parser.y"
              { (yyval.string) = strdup("float[]"); }
#line 1504 "parser.tab.c"
    break;

  case 28: /* type_specifier: STRING_ARRAY  */
#line 185 "../src/parser.y"
               { (yyval.string) = strdup("string[]"); }
#line 1510 "parser.tab.c"
    break;

  case 29: /* decl: INT IDENTIFIER  */
#line 188 "../src/parser.y"
                     { (yyval.ast) = ast_new_declaration((yyvsp[0].string), yylineno); }
#line 1516 "parser.tab.c"
    break;

  case 30: /* decl: INT IDENTIFIER '=' expr  */
#line 189 "../src/parser.y"
                          { (yyval.ast) = ast_new_declaration_init((yyvsp[-2].string), (yyvsp[0].ast), yylineno); }
#line 1522 "parser.tab.c"
    break;

  case 31: /* decl: FLOAT IDENTIFIER  */
#line 190 "../src/parser.y"
                   { (yyval.ast) = ast_new_declaration((yyvsp[0].string), yylineno); }
#line 1528 "parser.tab.c"
    break;

  case 32: /* decl: FLOAT IDENTIFIER '=' expr  */
#line 191 "../src/parser.y"
                            { (yyval.ast) = ast_new_declaration_init((yyvsp[-2].string), (yyvsp[0].ast), yylineno); }
#line 1534 "parser.tab.c"
    break;

  case 33: /* decl: STRING IDENTIFIER  */
#line 192 "../src/parser.y"
                    { (yyval.ast) = ast_new_declaration((yyvsp[0].string), yylineno); }
#line 1540 "parser.tab.c"
    break;

  case 34: /* decl: STRING IDENTIFIER '=' expr  */
#line 193 "../src/parser.y"
                             { (yyval.ast) = ast_new_declaration_init((yyvsp[-2].string), (yyvsp[0].ast), yylineno); }
#line 1546 "parser.tab.c"
    break;

  case 35: /* array_decl: INT_ARRAY IDENTIFIER  */
#line 196 "../src/parser.y"
                                 { (yyval.ast) = ast_new_array_declaration((yyvsp[0].string), NULL, yylineno); }
#line 1552 "parser.tab.c"
    break;

  case 36: /* array_decl: INT_ARRAY IDENTIFIER '[' expr ']'  */
#line 197 "../src/parser.y"
                                    { (yyval.ast) = ast_new_array_declaration((yyvsp[-3].string), (yyvsp[-1].ast), yylineno); }
#line 1558 "parser.tab.c"
    break;

  case 37: /* array_decl: FLOAT_ARRAY IDENTIFIER  */
#line 198 "../src/parser.y"
                         { (yyval.ast) = ast_new_array_declaration((yyvsp[0].string), NULL, yylineno); }
#line 1564 "parser.tab.c"
    break;

  case 38: /* array_decl: FLOAT_ARRAY IDENTIFIER '[' expr ']'  */
#line 199 "../src/parser.y"
                                      { (yyval.ast) = ast_new_array_declaration((yyvsp[-3].string), (yyvsp[-1].ast), yylineno); }
#line 1570 "parser.tab.c"
    break;

  case 39: /* array_decl: STRING_ARRAY IDENTIFIER  */
#line 200 "../src/parser.y"
                          { (yyval.ast) = ast_new_array_declaration((yyvsp[0].string), NULL, yylineno); }
#line 1576 "parser.tab.c"
    break;

  case 40: /* array_decl: STRING_ARRAY IDENTIFIER '[' expr ']'  */
#line 201 "../src/parser.y"
                                       { (yyval.ast) = ast_new_array_declaration((yyvsp[-3].string), (yyvsp[-1].ast), yylineno); }
#line 1582 "parser.tab.c"
    break;

  case 41: /* block: '{' stmt_list '}'  */
#line 204 "../src/parser.y"
                         { (yyval.ast) = (yyvsp[-1].ast); }
#line 1588 "parser.tab.c"
    break;

  case 42: /* block: '{' '}'  */
#line 205 "../src/parser.y"
          { 
    ASTNode **stmts = NULL;
    (yyval.ast) = ast_new_block(stmts, 0, yylineno);
}
#line 1597 "parser.tab.c"
    break;

  case 43: /* if_stmt: IF '(' expr ')' stmt  */
#line 211 "../src/parser.y"
                                        { 
    (yyval.ast) = ast_new_if((yyvsp[-2].ast), (yyvsp[0].ast), NULL, yylineno); 
}
#line 1605 "parser.tab.c"
    break;

  case 44: /* if_stmt: IF '(' expr ')' stmt ELSE stmt  */
#line 214 "../src/parser.y"
                                 { 
    (yyval.ast) = ast_new_if((yyvsp[-4].ast), (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1613 "parser.tab.c"
    break;

  case 45: /* while_stmt: WHILE '(' expr ')' stmt  */
#line 219 "../src/parser.y"
                                    { 
    (yyval.ast) = ast_new_while((yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1621 "parser.tab.c"
    break;

  case 46: /* for_stmt: FOR '(' expr ';' expr ';' expr ')' stmt  */
#line 224 "../src/parser.y"
                                                  { 
    (yyval.ast) = ast_new_for((yyvsp[-6].ast), (yyvsp[-4].ast), (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1629 "parser.tab.c"
    break;

  case 47: /* expr: IDENTIFIER '=' expr  */
#line 229 "../src/parser.y"
                          { 
    (yyval.ast) = ast_new_assignment(ast_new_variable((yyvsp[-2].string), yylineno), (yyvsp[0].ast), yylineno); 
}
#line 1637 "parser.tab.c"
    break;

  case 48: /* expr: array_access '=' expr  */
#line 232 "../src/parser.y"
                        {
    (yyval.ast) = ast_new_array_assignment((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
}
#line 1645 "parser.tab.c"
    break;

  case 49: /* expr: logical_expr  */
#line 235 "../src/parser.y"
               { (yyval.ast) = (yyvsp[0].ast); }
#line 1651 "parser.tab.c"
    break;

  case 50: /* logical_expr: logical_expr AND equality_expr  */
#line 238 "../src/parser.y"
                                             { 
    (yyval.ast) = ast_new_binary_op("&&", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1659 "parser.tab.c"
    break;

  case 51: /* logical_expr: logical_expr OR equality_expr  */
#line 241 "../src/parser.y"
                                { 
    (yyval.ast) = ast_new_binary_op("||", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1667 "parser.tab.c"
    break;

  case 52: /* logical_expr: equality_expr  */
#line 244 "../src/parser.y"
                { (yyval.ast) = (yyvsp[0].ast); }
#line 1673 "parser.tab.c"
    break;

  case 53: /* equality_expr: equality_expr EQ rel_expr  */
#line 247 "../src/parser.y"
                                         { 
    (yyval.ast) = ast_new_binary_op("==", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1681 "parser.tab.c"
    break;

  case 54: /* equality_expr: equality_expr NE rel_expr  */
#line 250 "../src/parser.y"
                            { 
    (yyval.ast) = ast_new_binary_op("!=", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1689 "parser.tab.c"
    break;

  case 55: /* equality_expr: rel_expr  */
#line 253 "../src/parser.y"
           { (yyval.ast) = (yyvsp[0].ast); }
#line 1695 "parser.tab.c"
    break;

  case 56: /* rel_expr: rel_expr '<' add_expr  */
#line 256 "../src/parser.y"
                                { 
    (yyval.ast) = ast_new_binary_op("<", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1703 "parser.tab.c"
    break;

  case 57: /* rel_expr: rel_expr '>' add_expr  */
#line 259 "../src/parser.y"
                        { 
    (yyval.ast) = ast_new_binary_op(">", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1711 "parser.tab.c"
    break;

  case 58: /* rel_expr: rel_expr LE add_expr  */
#line 262 "../src/parser.y"
                       { 
    (yyval.ast) = ast_new_binary_op("<=", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1719 "parser.tab.c"
    break;

  case 59: /* rel_expr: rel_expr GE add_expr  */
#line 265 "../src/parser.y"
                       { 
    (yyval.ast) = ast_new_binary_op(">=", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1727 "parser.tab.c"
    break;

  case 60: /* rel_expr: add_expr  */
#line 268 "../src/parser.y"
           { (yyval.ast) = (yyvsp[0].ast); }
#line 1733 "parser.tab.c"
    break;

  case 61: /* add_expr: add_expr '+' mul_expr  */
#line 271 "../src/parser.y"
                                { 
    (yyval.ast) = ast_new_binary_op("+", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1741 "parser.tab.c"
    break;

  case 62: /* add_expr: add_expr '-' mul_expr  */
#line 274 "../src/parser.y"
                        { 
    (yyval.ast) = ast_new_binary_op("-", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1749 "parser.tab.c"
    break;

  case 63: /* add_expr: mul_expr  */
#line 277 "../src/parser.y"
           { (yyval.ast) = (yyvsp[0].ast); }
#line 1755 "parser.tab.c"
    break;

  case 64: /* mul_expr: mul_expr '*' primary  */
#line 280 "../src/parser.y"
                               { 
    (yyval.ast) = ast_new_binary_op("*", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1763 "parser.tab.c"
    break;

  case 65: /* mul_expr: mul_expr '/' primary  */
#line 283 "../src/parser.y"
                       { 
    (yyval.ast) = ast_new_binary_op("/", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1771 "parser.tab.c"
    break;

  case 66: /* mul_expr: mul_expr '%' primary  */
#line 286 "../src/parser.y"
                       { 
    (yyval.ast) = ast_new_binary_op("%", (yyvsp[-2].ast), (yyvsp[0].ast), yylineno); 
}
#line 1779 "parser.tab.c"
    break;

  case 67: /* mul_expr: primary  */
#line 289 "../src/parser.y"
          { (yyval.ast) = (yyvsp[0].ast); }
#line 1785 "parser.tab.c"
    break;

  case 68: /* primary: INTEGER  */
#line 292 "../src/parser.y"
                 { (yyval.ast) = ast_new_integer((yyvsp[0].int_val), yylineno); }
#line 1791 "parser.tab.c"
    break;

  case 69: /* primary: FLOAT  */
#line 293 "../src/parser.y"
        { (yyval.ast) = ast_new_float((yyvsp[0].float_val), yylineno); }
#line 1797 "parser.tab.c"
    break;

  case 70: /* primary: STRING  */
#line 294 "../src/parser.y"
         { (yyval.ast) = ast_new_string((yyvsp[0].string), yylineno); }
#line 1803 "parser.tab.c"
    break;

  case 71: /* primary: IDENTIFIER  */
#line 295 "../src/parser.y"
             { (yyval.ast) = ast_new_variable((yyvsp[0].string), yylineno); }
#line 1809 "parser.tab.c"
    break;

  case 72: /* primary: array_access  */
#line 296 "../src/parser.y"
               { (yyval.ast) = (yyvsp[0].ast); }
#line 1815 "parser.tab.c"
    break;

  case 73: /* primary: '(' expr ')'  */
#line 297 "../src/parser.y"
               { (yyval.ast) = (yyvsp[-1].ast); }
#line 1821 "parser.tab.c"
    break;

  case 74: /* primary: IDENTIFIER '(' ')'  */
#line 298 "../src/parser.y"
                     {
    ASTNode **args = NULL;
    (yyval.ast) = ast_new_function_call((yyvsp[-2].string), args, 0, yylineno);
}
#line 1830 "parser.tab.c"
    break;

  case 75: /* primary: IDENTIFIER '(' expr_list ')'  */
#line 302 "../src/parser.y"
                               {
    // 正确解析参数列表
    int arg_count = 0;
    ASTNode *current = (yyvsp[-1].ast);
    
    // 计算参数数量
    while (current != NULL) {
        arg_count++;
        current = current->binary.right;
    }
    
    // 创建参数数组
    ASTNode **args = malloc(arg_count * sizeof(ASTNode *));
    current = (yyvsp[-1].ast);
    for (int i = 0; i < arg_count; i++) {
        args[i] = current->binary.left;
        current = current->binary.right;
    }
    
    (yyval.ast) = ast_new_function_call((yyvsp[-3].string), args, arg_count, yylineno);
    
    // 释放临时的表达式列表节点
    current = (yyvsp[-1].ast);
    while (current != NULL) {
        ASTNode *next = current->binary.right;
        free(current->binary.op);
        free(current);
        current = next;
    }
}
#line 1865 "parser.tab.c"
    break;

  case 76: /* array_access: IDENTIFIER '[' expr ']'  */
#line 334 "../src/parser.y"
                                      {
    (yyval.ast) = ast_new_array_access((yyvsp[-3].string), (yyvsp[-1].ast), yylineno);
}
#line 1873 "parser.tab.c"
    break;

  case 77: /* expr_list: expr  */
#line 339 "../src/parser.y"
                {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->line_no = yylineno;
    node->binary.op = strdup(",");
    node->binary.left = (yyvsp[0].ast);
    node->binary.right = NULL;
    (yyval.ast) = node;
}
#line 1887 "parser.tab.c"
    break;

  case 78: /* expr_list: expr_list ',' expr  */
#line 348 "../src/parser.y"
                     {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->line_no = yylineno;
    node->binary.op = strdup(",");
    node->binary.left = (yyvsp[0].ast);
    node->binary.right = NULL;
    
    ASTNode *current = (yyvsp[-2].ast);
    while (current->binary.right != NULL) {
        current = current->binary.right;
    }
    current->binary.right = node;
    
    (yyval.ast) = (yyvsp[-2].ast);
}
#line 1908 "parser.tab.c"
    break;


#line 1912 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 366 "../src/parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d near token: %s\n", yylineno, yytext);
}
