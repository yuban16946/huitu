// ----------------------------- lexical.cpp ---------------------------------
// �ʷ���������Ķ���

#include "lexical.h"

// ---------------��ʼ���ʷ�������
int lexical_class::init_lexical(const char *file_name)
{
	in_flie = fopen(file_name, "r");
	if (in_flie != NULL)  return 1;
	else                  return 0;
}

// ---------------�رմʷ�������
void lexical_class::close_lexical(void)
{
	if (in_flie != NULL) fclose (in_flie) ;
}

// ---------------������Դ�����ж���һ���ַ�
char lexical_class::get_char(void)
{
	int next_char = getc(in_flie);   return toupper(next_char);
}

// ---------------��Ԥ�����ַ��˻ص�����Դ������
void lexical_class::back_char(char next_char)
{
	if (next_char != EOF)   ungetc(next_char, in_flie);
}

// ---------------�����ַ����ǺŻ�����
void lexical_class::add_in_token_str(char next_char)
{
	int token_len = strlen (token_buf);
	if (token_len + 1 >= sizeof (token_buf)) return;
	token_buf[token_len]   = next_char;
	token_buf[token_len+1] = str_end;
}

// ----------------- ��ռǺŻ�����
void lexical_class::empty_token_str ()
{	
	memset(token_buf, 0, token_len);
} 

// ----------------- �ж��������ַ����Ƿ��ڷ��ű���
token_rec lexical_class::check_token(const char * c_str)
{	
	int count;
	token_rec err_token;
	
	for (count=0; count<sizeof(token_table)/sizeof(token_table[0]); count++)
	{ if (strcmp(token_table[count].lexeme, c_str)==0) return token_table[count];
	}
	memset(&err_token, 0, sizeof(token_rec));
	err_token.type = ERRTOKEN;
	return err_token;
}

// ---------------��ȡһ���Ǻ�
token_rec lexical_class::get_token(void)
{	
	token_rec token;
	int next_char ;
	
	memset(&token, 0, sizeof(token_rec));
	empty_token_str();
	token.lexeme = token_buf;
	for (;;)	// ����Դ�����еĿո�TAB���س��ȣ������ļ����������ؿռǺ�
	{	next_char = get_char() ;
		if (next_char == EOF)
		{	token.type = NONTOKEN;
			return token;
		}
		if (next_char == '\n')	line_no ++ ;
		if (!isspace(next_char))	break ;
	} // end of for 
	add_in_token_str (next_char);	// �����ǿո�TAB���س����ļ��������ȣ�
					// ���ȼ��뵽�Ǻŵ��ַ���������
	if(isalpha(next_char))		// ��char��A-Za-z������һ���Ǻ������ؼ��֡�PI��E��
	{	for (;;)
		{	next_char = get_char () ;
			if ( isalnum(next_char) )	add_in_token_str (next_char) ;
			else				break ;
		}
		back_char (next_char) ;
		token = check_token (token_buf);
		token.lexeme = token_buf;
		return token;
	}	
	else if(isdigit(next_char))		// ����һ�����֣���һ���ǳ���
	{	for (;;)
		{	next_char = get_char () ;
			if (isdigit(next_char))	add_in_token_str (next_char) ;
			else			break ;
		}
		if (next_char == '.')
		{	add_in_token_str (next_char) ;
			for (;;)
			{	next_char = get_char() ;
				if (isdigit(next_char))	add_in_token_str (next_char) ;
				else			break ;
			}
			
		} // end of if (next_char == '.')
		back_char (next_char) ;
		token.type = CONST_ID;
		token.value = atof (token_buf);
		return token;
	}
	else	// ������ĸ�����֣���һ���Ƿ���
	{	switch (next_char)
		{	case ';' : token.type = SEMICO    ; break;
			case '(' : token.type = L_BRACKET ; break;
			case ')' : token.type = R_BRACKET ; break;
			case ',' : token.type = COMMA     ; break;
			case '+' : token.type =	PLUS      ; break;
			case '-' :
				next_char = get_char();
				if (next_char =='-')
				{	while (next_char != '\n' && next_char != EOF) next_char = get_char();
					back_char(next_char);
					return get_token();
				}
				else 
				{	back_char(next_char);
					token.type = MINUS;
					break;
				}
			case '/' :
				next_char = get_char();
				if (next_char =='/') 
				{	while (next_char != '\n' && next_char != EOF) next_char = get_char();
					back_char(next_char);
					return get_token();
				}
				else
				{	back_char(next_char);
					token.type = DIV;
					break;
				}
			case '*' :
				next_char = get_char() ;
				if (next_char == '*') 
				{   token.type = POWER ;
				    add_in_token_str (next_char);
				}
				else 
				{	back_char (next_char);	
					token.type = MUL; 
				}
				break;
			default: 
				token.type = ERRTOKEN; 
		} // end of switch
	} // end of else(������ĸ�����֣���һ���Ƿ���)
	return token;
} // end of get_token
