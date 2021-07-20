// ----------------------------- grammer.h�﷨���� ---------------------------------

#include "lexical.h"

//--------------------------------- �﷨���������е����Ͷ���
typedef double (* func_ptr)(double);
typedef struct tree_node				// �﷨���ڵ�����
{	enum token_type op_code;			// PLUS, FUNC, CONST_ID, ...
	union
	{	struct { tree_node *left, *right; } tag_op;
		struct { tree_node *child;
				 func_ptr math_func_ptr; } tag_func;
		double tag_const;
		double * tag_parameter;
	} content;
} * tree_node_ptr;

//--------------------------------- �﷨������������
class grammer_class {

protected:
	double    	parameter;	// ����T�Ĵ洢�ռ�
	int       	line_color;	// ������ɫ
	token_rec 	token;		// �Ǻ�
	tree_node_ptr 	start_ptr,	// ��ͼ�����ʽ���﷨��
					end_ptr, 	// ��ͼ�յ���ʽ���﷨��
					step_ptr,	// �������ʽ���﷨��
					x_ptr,		// ��ĺ�������ʽ���﷨��
					y_ptr,		// ��ĺ�������ʽ���﷨��
					angle_ptr;	// ��ת�Ƕȱ��ʽ���﷨��
	lexical_class 	lexical;	// �ʷ�����������

	// --------------- ��������
	void fetch_token ();						// ��ȡ�Ǻ�
	void match_token (enum token_type the_token);			// ƥ��Ǻ�
	void syntax_error (int case_of);				// ָ���﷨����(����error_msg)
	void print_syntax_tree(tree_node *root, int indent);		// ��ӡ�﷨��
	tree_node_ptr make_tree_node(enum token_type opcode,...);	// �����﷨��
	virtual void error_msg(int line, char *descrip, char *string);	// ��Semantics������Ϊ���ڴ�ӡ��ʽ


	// ---------------���ս���ĵݹ��ӳ���
	void program();
	void statement();
	void color_statement();
	void colors();
	// �����ĸ�������Semantics_class�������ã���ʵ���﷨�Ƶ�����
	virtual void for_statement();	
	virtual void origin_statement();
	virtual void rot_statement();
	virtual void scale_statement();
	tree_node_ptr expression();
	tree_node_ptr term();
	tree_node_ptr factor();
	tree_node_ptr component();
	tree_node_ptr atom();

public:
	grammer_class() 				// ��������ʱ���ó�ֵ
	{
		parameter = 0;
		line_color= red_color;
		start_ptr = NULL;
		end_ptr   = NULL;
		step_ptr  = NULL;
		x_ptr	  = NULL;
		y_ptr	  = NULL;
	}
	~grammer_class() {}		//ȡ��
	void grammer(char * file_name);		// �﷨�������ӿ�

private:
	// �������������﷨�������ĸ��ٵ���, ��Semantics_class��������Ϊ��������
	virtual void enter(char * x);
    	virtual void back(char * x);
    	virtual void call_match(char * x); 
    	virtual void tree_trace(tree_node_ptr x);
};
