//Siddharth Shah 2009A7PS115P
//Valleri Agarwal 2009A7PS027P
//Group 3

#include "lexer.h"
#include "parser.h"
#include "dataDefinition.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include"symboltable.h"
#include"ASTops.h"
#include"assembler.h"

int main(int argc, char * argv[])
{
	hashTable h = initializeHashTable(keycount);
	FILE *fp;//*ferror;
	//ferror=fopen("error.txt","w");
	//fclose(ferror);
	fp = fopen("keywords","r");
	keyword k; int c;	
	while(1)
	{
		fscanf(fp,"%d %s",&c,k);
		if (feof(fp))
			break;		
		addKeyword(h, c, k);
	}
	fclose(fp);

	printf("\n\nPress option for the defined task:\n");

	printf("1: For printing the token list generated by the lexer.\n");

        printf("2: For producing clean code by removal of comments.\n");

        printf("3: For parsing to verify the syntactic correctness of the input source code and to produce parse tree.\n");
	
	printf("4: For printing the Symbol Table.\n");

	printf("5: For printing the Abstract Syntax Tree in depth first order in appropriate format.\n");

	printf("6: For verifying the syntactic and semantic correctness of the input source code and to produce errors as per the ERROR list.\n");

	printf("7: For producing assembly code.\n");

	printf("8: To exit.\n");
	
	int option;

	scanf("%d",&option);

	
	
while(option!=8)
{

	//ferror=fopen("error.txt","w");
	//fclose(ferror);

	if(option == 1)
	{
		printf("\n\n\n------------------------------------------------------------------------------------------------\n");
		populateSymbolTable(argv[1], h,0);//removeComments not used to show commentmark tokens
		printf("\ntokenstream.txt created. Create cleanfile first and then use lexer to remove inconsistencies caused by invalid commentmark usage. Use option 6  to see errors.\n"); 
		printf("\n\n------------------------------------------------------------------------------------------------\n\n");		
	}
	else if(option==2)
	{
		printf("\n\n\n------------------------------------------------------------------------------------------------\n");
		removeComments(argv[1],"cleancode.txt");printf("cleancode.txt created\n");
		printf("\n\n------------------------------------------------------------------------------------------------\n\n");
	}
	else if(option==3)
	{
		Grammar G1=(Grammar)malloc(GRAMCONST*sizeof(gHead));
		removeComments(argv[1], "clean.txt");
		G1=createGrammar();
 		createFirstSets();
		createFollowSets();
 		createParseTable(G1);
 		parseTree Tree;
		printf("\n\n\n------------------------------------------------------------------------------------------------\n"); 		
		Tree=parseInputSourceCode("clean.txt", h, G1,0);
		printf("parsetree.txt created");
 		printParseTree("parsetree.txt",Tree);
		printf("\n\n------------------------------------------------------------------------------------------------\n\n");
 	}
	else if(option==4)
	{
		Grammar G1=(Grammar)malloc(GRAMCONST*sizeof(gHead));
		removeComments(argv[1], "clean.txt");
		G1=createGrammar();
 		createFirstSets();
		createFollowSets();
 		createParseTable(G1);
 		parseTree Tree;
		parse_status=1;
		printf("\n\n\n------------------------------------------------------------------------------------------------\n"); 		
		Tree=parseInputSourceCode("clean.txt", h, G1,0);
 		//printParseTree("outfile.txt",Tree);
		
		//populateSymbolTable(argv[1], h,0);
		if(parse_status!=-1)
		{tokenInfo t = getFirstToken();
		hashTable2 h_ID = initializeHashTable2(13);
		hashTable2 h_fn = initializeHashTable2(13);
		Total_Scope_List scope_tot = (Total_Scope_List) malloc (20* sizeof(struct total_scope_list));
		int j;
		for(j=0;j<20;j++)
		{
			scope_tot[j].scope_start=0;
			scope_tot[j].scope_end=0;
		}
		createIDTable(t,h_fn,h_ID,scope_tot,0);
		t=getFirstToken();
		second_run(t,h_fn,h_ID,0);
		//fp=fopen("error.txt","w");
		//fclose(fp);
		printf("\nSymbol Table generated in symboltable.txt\n\n\n");
		printf("\n\n------------------------------------------------------------------------------------------------\n\n");
		fp=fopen("symboltable.txt","w");
		//fprintf(fp,"\nPrinting Symbol Table:\n");
		fprintf(fp,"\nVariables:\n\n");
		fprintf(fp,"Name\t\t Scope Start\t Scope End\t Type\t\t Width\n\n");
		print_ST(fp,h_ID,scope_tot,0);
		fprintf(fp,"\nModules:\n\n");
		fprintf(fp,"Name\t\t Scope Start\t Scope End\n\n");
		print_ST(fp,h_fn,scope_tot,1);
		fclose(fp);
		}
		else
		{
			//printf("\n\n\n------------------------------------------------------------------------------------------------\n"); 		
			printf("\nAs parse is not successful, no symbol table generated. However, you can check some possible further errors by choosing option 6\n\n\n");
			printf("\n\n------------------------------------------------------------------------------------------------\n\n");	
		}
	}	
	else if(option==5)
	{
	  	Grammar G1=(Grammar)malloc(GRAMCONST*sizeof(gHead));
		removeComments(argv[1], "cleancode.txt");
		G1=createGrammar();
 		createFirstSets();
		createFollowSets();
 		createParseTable(G1);
		parseTree Tree;
		parse_status=1;
		Tree=parseInputSourceCode("cleancode.txt", h, G1,0);
                //printParseTree("outfile.txt",Tree);
		//populateSymbolTable(argv[1], h,0);
		if(parse_status==1)
		{
		tokenInfo t = getFirstToken();
		hashTable2 h_ID = initializeHashTable2(13);
		hashTable2 h_fn = initializeHashTable2(13);
		Total_Scope_List scope_tot = (Total_Scope_List) malloc (20* sizeof(struct total_scope_list));
		int j;
		for(j=0;j<20;j++)
		{
			scope_tot[j].scope_start=0;
			scope_tot[j].scope_end=0;
		}
		createIDTable(t,h_fn,h_ID,scope_tot,0);
		ASTree astTree;
                astTree=buildASTree(Tree,h_ID,h_fn,"ast.txt");
             	printf("\n\n\n------------------------------------------------------------------------------------------------\n"); 		
		printf("AST Generated and displayed in ast.txt\n The format is Token   :  Value if num/rnum:    Parent: ParentList:\nWhere Parent List is the list of linearly collapsed non terminals");
		printf("\n\n------------------------------------------------------------------------------------------------\n\n");
	        }
		else
		{
			printf("\n\n\n------------------------------------------------------------------------------------------------\n"); 		
			printf("AST not generated as parse is unsuccessful");
			printf("\n\n------------------------------------------------------------------------------------------------\n\n");   
		}
    	}
	else if(option==6)
	{	
		Grammar G1=(Grammar)malloc(GRAMCONST*sizeof(gHead));
		removeComments(argv[1], "cleancode.txt");
		G1=createGrammar();
 		createFirstSets();
		createFollowSets();
 		createParseTable(G1);
		parseTree Tree;
		semantic_status=1;
		parse_status=1;
		printf("\n\n\n------------------------------------------------------------------------------------------------\n");
                Tree=parseInputSourceCode("cleancode.txt", h, G1,1);
		
		//populateSymbolTable(argv[1], h,1);
		tokenInfo t = getFirstToken();
		hashTable2 h_ID = initializeHashTable2(13);
		hashTable2 h_fn = initializeHashTable2(13);
		Total_Scope_List scope_tot = (Total_Scope_List) malloc (20* sizeof(struct total_scope_list));
		int j;
		for(j=0;j<20;j++)
		{
			scope_tot[j].scope_start=0;
			scope_tot[j].scope_end=0;
		}
		createIDTable(t,h_fn,h_ID,scope_tot,1);
		t=getFirstToken();
		second_run(t,h_fn,h_ID,1);
		ASTree astTree;
                if (parse_status!=-1)
			{astTree=buildASTree(Tree,h_ID,h_fn,"ast.txt");
			int type_error=ASTQueueTypeChecker(astTree.root,1);
			if(semantic_status!=-1&&parse_status!=-1&&type_error!=1)
				{
				 printf("Code compiles successfully. No Semantic Errors were found.");
				}
			}
		else
			printf("\nAST not generated as parse is unsuccessful");
		printf("\n\n------------------------------------------------------------------------------------------------\n\n");
	}
	else if(option == 7)
	{
		Grammar G1=(Grammar)malloc(GRAMCONST*sizeof(gHead));
		removeComments(argv[1], "cleancode.txt");
		G1=createGrammar();
 		createFirstSets();
		createFollowSets();
 		createParseTable(G1);
		parseTree Tree;
		semantic_status=1;
		parse_status=1;
                Tree=parseInputSourceCode("cleancode.txt", h, G1,0);
                //printParseTree("outfile.txt",Tree);
		//populateSymbolTable(argv[1], h,0);
		tokenInfo t = getFirstToken();
		hashTable2 h_ID = initializeHashTable2(13);
		hashTable2 h_fn = initializeHashTable2(13);
		Total_Scope_List scope_tot = (Total_Scope_List) malloc (20* sizeof(struct total_scope_list));
		int j;
		for(j=0;j<20;j++)
		{
			scope_tot[j].scope_start=0;
			scope_tot[j].scope_end=0;
		}
		createIDTable(t,h_fn,h_ID,scope_tot,0);


                ASTree astTree;
                astTree=buildASTree(Tree,h_ID,h_fn,"ast.txt");
		int type_error=ASTQueueTypeChecker(astTree.root,0);
		if(semantic_status!=-1&&parse_status!=-1&&type_error!=1)
		{
		//FILE * f1;
		printf("\n\n\n------------------------------------------------------------------------------------------------\n"); 		
		printf("Code generated in code.asm");
		printf("\n\n------------------------------------------------------------------------------------------------\n\n");
		if (argc==2)                
		fp = fopen(argv[2],"w");
		else fp = fopen("code.asm","w");
		fprintf(fp,".model tiny\n");
		fprintf(fp,".data\n");
		fprintf(fp,"LCHILD DW 8 DUP (?)\nFOR_CTRL DW 8 DUP (?)\nOUT DW 10 DUP (?)\nTemp DW ?\nRCHILD DW ?\n");
		generate_data(h_ID,fp);
		fprintf(fp,".code\n.startup\n");
		evaluateQueue(astTree.root,fp); 
		fprintf(fp,".exit\nend");
		fclose(fp);
		}
		else
		{
			printf("\n\n\n------------------------------------------------------------------------------------------------\n"); 		
			printf("Code not generated as errors are present. Use option 6 for further details.");
			printf("\n\n------------------------------------------------------------------------------------------------\n\n");
		}
		     
	}
	else
	{
		printf("\n\nInvalid Option\n\n");
	}

			printf("\n\nPress option for the defined task:\n");

			printf("1: For printing the token list generated by the lexer.\n");

       			printf("2: For producing clean code by removal of comments.\n");
	
       			printf("3: For parsing to verify the syntactic correctness of the input source code and to produce parse tree.\n");
	
			printf("4: For printing the Symbol Table.\n");

			printf("5: For printing the Abstract Syntax Tree in depth first order in appropriate format.\n");

			printf("6: For verifying the syntactic and semantic correctness of the input source code and to produce errors as per the ERROR list.\n");

			printf("7: For producing assembly code.\n");

			printf("8: To exit.\n");
	
			scanf("%d",&option);
		
}


return(0);
	
}
