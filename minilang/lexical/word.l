%{
	int count=0;
%}

%%

int|if|then|else|while {
	count = count + 1;
	printf("word_%d keyword: %s\n", count, yytext);
}

[A-Za-z]([A-Za-z]|[0-9])* {
	count = count + 1;
	printf("word_%d identifier: %s\n", count, yytext);
}

[0-9]* {
	count = count + 1;
	printf("word_%d constant: %s\n", count, yytext);
}

"+"|"-"|"*"|"/"|"="|"=="|"!="|"<"|"<="|">"|">=" {
	count = count + 1;
	printf("word_%d operator: %s\n", count, yytext);
}

","|";"|"{"|"}"|"("|")" {
	count = count + 1;
	printf("word_%d delimiter: %s\n", count, yytext);
}

"\x20"|"\x09"|"\x0d"|"\x0a"

. { printf("error: %c\n", *yytext); }

%%

int yywrap() {
	return 1;
}

int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("usage: %s filename\n", argv[0]);
		return 0;
	}

	yyin = fopen(argv[1], "r");
	
	if(yyin == NULL) {
		printf("error: open %s failed\n", argv[1]);
		return 0;
	}

	yylex();

	fclose(yyin);
	return 0;
}
