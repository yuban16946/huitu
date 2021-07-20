// ----------------------------- lexicalmain.cpp �ʷ����������� ---------------------------------
// �ʷ�����������������

#include "lexical.h"



char *TOKEN_TYPE[] =  {	"RESERVED_WORD", "RESERVED_WORD",	"RESERVED_WORD", "RESERVED_WORD", "RESERVED_WORD" ,			// ������
	"RESERVED_WORD" , "RESERVED_WORD" , "RESERVED_WORD" ,	"RESERVED_WORD" ,			                    // ������
	"RESERVED_WORD" , "RESERVED_WORD",	"RESERVED_WORD",				                        // ������
	"PARAMETER",    // ����
	"SEMICO", "L_BRACKET", "R_BRACKET", "COMMA",		// �ָ�����
	"PLUS", "MINUS", "MUL", "DIV", "POWER",			    // �����
	"FUNC",						// ����
	"CONST_ID",					// ����
	"NONTOKEN",					// �ռǺ�
	"ERRTOKEN"
};



int main(int argc, char *argv[])
{
	lexical_class  lexical;
	token_rec token;
	if (argc < 2)
	{	
		printf("please input Source File !\n" );	
		return 0;
	}
	if(lexical.init_lexical(argv[1])!=1)            //��ʼ���ʷ�������
	{	
		printf("Open Source File Error ! \n");		
		return 0;
	}
	printf("****************�ʷ����������*******************\n");
	printf("   �Ǻ����       �ַ���        ����ֵ	      ָ��\n");
    printf("________________________________________________________\n");
	while(1)
	{	
		token = lexical.get_token();			//��üǺ�
		if(token.type != NONTOKEN)			//��ӡ�Ǻŵ�����
		{
			printf("|%13s |%10s |%12f |%12x |\n",TOKEN_TYPE[token.type], token.lexeme, token.value, token.func_ptr); 
			//printf("|___________|___________|_____________|_____________|\n");
		}
			
		else	
			break;
	};
		printf("________________________________________________________\n");
	lexical.close_lexical();
	return 0;
}
