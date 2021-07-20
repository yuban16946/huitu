// ----------------------------- lexical.h ---------------------------------
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
using namespace std;

//--------------------------------- �ʷ����������е����Ͷ����볣������
enum token_type						// �Ǻ�����
{	ORIGIN, SCALE,	ROT, IS,  TO,			// ������
	STEP,   DRAW,	FOR,	FROM,			// ������
	COLOR,	RED,	BLACK,				// ������
	T,						// ����
	SEMICO,L_BRACKET, R_BRACKET, COMMA,		// �ָ�����
	PLUS, MINUS, MUL, DIV, POWER,			// �����
	FUNC,						// ����
	CONST_ID,					// ����
	NONTOKEN,					// �ռǺ�
	ERRTOKEN					// ����Ǻ�
};
enum color_type { red_color, black_color };		// ������ɫ
struct token_rec					// �Ǻ�����ű�ṹ
{	token_type  	type;				// �Ǻŵ����
	char		* lexeme;			// ���ɼǺŵ��ַ���
	double		value;				// ��Ϊ���������ǳ�����ֵ
	double		(* func_ptr)(double);		// ��Ϊ���������Ǻ�����ָ��
};
static token_rec token_table[] =			// ���ű�����
{	{CONST_ID,	"PI",		3.1415926,	NULL},
	{CONST_ID,	"E",		2.71828,	NULL},
	{T,		"T",		0.0,		NULL},
	{FUNC,		"SIN",		0.0,		sin},
	{FUNC,		"COS",		0.0,		cos},
	{FUNC,		"TAN",		0.0,		tan},
	{FUNC,		"LN",		0.0,		log},
	{FUNC,		"EXP",		0.0,		exp},
	{FUNC,		"SQRT",		0.0,		sqrt},
	{ORIGIN,	"ORIGIN",	0.0,		NULL},
	{SCALE,		"SCALE",	0.0,		NULL},
	{COLOR,		"COLOR",	0.0,		NULL},
	{RED,		"RED",		0.0,		NULL},
	{BLACK,		"BLACK",	0.0,		NULL},
	{ROT,		"ROT",		0.0,		NULL},
	{IS,		"IS",		0.0,		NULL},
	{FOR,		"FOR",		0.0,		NULL},
	{FROM,		"FROM",		0.0,		NULL},
	{TO,		"TO",		0.0,		NULL},
	{STEP,		"STEP",		0.0,		NULL},
	{DRAW,		"DRAW",		0.0,		NULL}
};
const int  token_len = 100;		// �Ǻ���󳤶�
const char str_end   = '\0';		// �ַ���������־

//--------------------------------- �ʷ��������������
class lexical_class {							
protected:
	FILE *in_flie;					// �����ļ���
	char token_buf[token_len];			// �Ǻ��ַ�����

public:
	unsigned int line_no;				// ���ټǺ�����Դ�ļ��к�
	lexical_class()	{ line_no = 1; }
	~lexical_class() {}
	int init_lexical(const char *FileName);		// ��ʼ���ʷ�������
	void close_lexical(void);			// �رմʷ�������
	token_rec get_token(void);			// ��ȡ�Ǻź���

private:
	char get_char(void);				// ������Դ�����ж���һ���ַ�
	void back_char(char next_char);			// ��Ԥ�����ַ��˻ص�����Դ������
	void add_in_token_str(char next_char);		// �����ַ����ǺŻ�����
	void empty_token_str();				// ��ռǺŻ�����
	token_rec check_token(const char *c_str);	// �ж��������ַ����Ƿ��ڷ��ű���
};
