// ----------------------------- grammer.cpp ---------------------------------
// �﷨��������Ķ���

#include "grammer.h"

// ------------------------ ͨ���ʷ��������ӿ�get_token��ȡһ���Ǻ�-------
void grammer_class::fetch_token()
{	
	token = lexical.get_token () ;
	if (token.type == ERRTOKEN)	syntax_error(1);
}

// ------------------------ ƥ��Ǻ�---------------------------
void grammer_class::match_token (enum token_type the_token) 
{	
	if (token.type != the_token) syntax_error(2);
	fetch_token();
}

// ------------------------ �﷨������--------------------------
void grammer_class::syntax_error (int case_of) 
{
	switch(case_of)
	{	case 1:	error_msg (lexical.line_no," �Ƿ��Ǻ� ", token.lexeme) ;
				break;
		case 2: error_msg (lexical.line_no, token.lexeme, " ����Ԥ�ڼǺ�") ;
				break;
	}
}

// ------------------------ ��ӡ������Ϣ---------------
void grammer_class::error_msg(int line, char *descrip, char *string)
{	
	cout << "Line No " << line << ": " << descrip << string << endl;
	lexical.close_lexical();
	exit(1);
}

// ------------------------ �����������ӡ���ʽ���﷨��----------
void grammer_class::print_syntax_tree(tree_node_ptr root, int indent)	
{	
	int	temp;

	for (temp=1; temp<=indent; temp++) 
		cout << "	";	// ����
	switch(root->op_code)					// ��ӡ���ڵ�
	{	case PLUS:      cout << "+" << endl;	break;
		case MINUS:     cout << "-" << endl;	break;
		case MUL:       cout << "*" << endl;	break;
		case DIV:       cout << "/" << endl;	break;
		case POWER:     cout << "**" << endl;	break;
		case FUNC:      cout << root->content.tag_func.math_func_ptr << endl;	break;
		case CONST_ID:  cout <<root->content.tag_const << endl;			break;
		case T:         cout << "T" << endl;					break;
		default:        cout << "�Ƿ������ڵ�!" << endl;			exit(0);
	}
	if(root->op_code == CONST_ID || root->op_code == T) 
		return;		// Ҷ�ӽڵ㷵��
	if(root->op_code == FUNC)						// �ݹ��ӡһ�����ӵĽڵ�
		print_syntax_tree(root->content.tag_func.child,indent+1);
	else									// �ݹ��ӡ�������ӵĽڵ�
	{	
		print_syntax_tree(root->content.tag_op.left,  indent+1);
		print_syntax_tree(root->content.tag_op.right, indent+1);
	}
}

// ------------------------ ��ͼ���Խ�������ڣ�����������ⲿ�ӿڣ�-----------
void grammer_class::grammer(char * file_name) 
{	
	enter("grammer");
	if(!(lexical.init_lexical(file_name)))		// ��ʼ���ʷ�������
	{	
		cout << "���ļ����� !" << endl;	
		return;	
	}
	fetch_token();					// ��ȡ��һ���Ǻ�
	program();					// �ݹ��½�����
	lexical.close_lexical();			// �رմʷ�������
	back("grammer");
	return;
}

// ------------------------ program �ĵݹ��ӳ���---------------------
void grammer_class::program ()
{	
	enter("program");
	while (token.type != NONTOKEN)
	{	statement();	
		match_token(SEMICO) ;
	}
	back("program");
}

// ------------------------ statement �ĵݹ��ӳ���---------------------
void grammer_class::statement()
{
	enter("statement");
	switch (token.type)
	{	case ORIGIN :	origin_statement();	break;
		case SCALE  :	scale_statement();	break;
		case ROT    :	rot_statement();	break;
		case FOR    :	for_statement();	break;
		case COLOR  :	color_statement();  	break;
		default     :	syntax_error(2);		
	}
	back("statement");
}

// ------------------------ origin_statement �ĵݹ��ӳ���--------------
void grammer_class::origin_statement()
{
	enter("origin_statement");
	call_match("ORIGIN");		match_token (ORIGIN);
	call_match("IS");		match_token (IS);
	call_match("(");		match_token (L_BRACKET);
					x_ptr = expression();
	call_match(",");		match_token (COMMA);
					y_ptr = expression();
	call_match(")");		match_token (R_BRACKET);
	back("origin_statement");
}

// ------------------------ scale_statement �ĵݹ��ӳ���---------------
void grammer_class::scale_statement () 
{
	enter("scale_statement");
	call_match("SCALE");		match_token (SCALE);
	call_match("IS");		match_token (IS);
	call_match("(");		match_token (L_BRACKET);
					x_ptr = expression();
	call_match(",");		match_token (COMMA);
					y_ptr = expression();
	call_match(")");		match_token (R_BRACKET);
	back("scale_statement");
}

// ------------------------ rot_statement �ĵݹ��ӳ���-----------------------
void grammer_class::rot_statement () 
{	
	enter("rot_statement");
	call_match("ROT");		match_token (ROT);
	call_match("IS");		match_token (IS);
					angle_ptr = expression();
	back("rot_statement");
}

// ------------------------ color_statement �ĵݹ��ӳ���
void grammer_class::color_statement () 
{	
	enter("color_statement");
	match_token (COLOR);
	match_token (IS);
    	colors();
	back("color_statement");
}

// ------------------------ color_statement �ĵݹ��ӳ���
void grammer_class::colors() 
{	
	enter("colors");
//	printf("red=%d, black=%d, token_type=%d\n", RED, BLACK, token.type);
	if (token.type==RED) line_color = red_color; 
	else if (token.type==BLACK)  line_color = black_color;
	 	 else syntax_error(2);
//	printf("black_color=%d, red_color=%d, line_color=%d\n", black_color, red_color, line_color);
	match_token (token.type);
	back("colors");
}

// ------------------------ for_statement �ĵݹ��ӳ���
void grammer_class::for_statement () 
{	
	enter("for_statement");
	call_match("FOR");			match_token (FOR);
    	call_match("T");			match_token(T);
	call_match("FROM");			match_token (FROM);
						start_ptr = expression();	// ���������ʼ���ʽ�﷨��
    	call_match("TO");			match_token (TO);
						end_ptr = expression();		// ��������ս���ʽ�﷨��
    	call_match("STEP");			match_token (STEP);
						step_ptr = expression();	// ��������������ʽ�﷨��
	call_match("DRAW");			match_token (DRAW);
	call_match("(");			match_token (L_BRACKET);
						x_ptr = expression();		// �����������ʽ�﷨��
	call_match(",");			match_token (COMMA);
						y_ptr = expression(); 		// ������������ʽ�﷨��
	call_match(")");			match_token (R_BRACKET);
	back("for_statement");
}

// ------------------------ expression �ĵݹ��ӳ���
tree_node_ptr grammer_class::expression()
{	
	tree_node_ptr left, right;			// ���������ڵ��ָ��
	token_type token_tmp;				// ��ǰ�Ǻ�

    enter("expression");
	left = term();					// ������������ҵõ����﷨��
	while (token.type==PLUS || token.type==MINUS)
	{	token_tmp = token.type;
		match_token(token_tmp);
		right = term();				// �����Ҳ������ҵõ����﷨��
		left = make_tree_node(token_tmp, left, right);
							// ����������﷨�������Ϊ������
	}
	tree_trace(left);				// ��ӡ���ʽ���﷨��
    back("expression");
	return left;					// �������ձ��ʽ���﷨��
}

// ------------------------ term �ĵݹ��ӳ���
tree_node_ptr  grammer_class::term()
{	
	tree_node_ptr left, right;
	token_type token_tmp;

    left = factor();
	while (token.type==MUL || token.type==DIV)
	{	token_tmp = token.type;
		match_token(token_tmp);
		right = factor();
		left = make_tree_node(token_tmp, left, right);
	}
	return left;
}

// ------------------------ factor �ĵݹ��ӳ���
tree_node_ptr grammer_class::factor ()
{	
	tree_node_ptr left, right;

	if(token.type == PLUS) 			// ƥ��һԪ������
	{	match_token(PLUS);
		right = factor();		//  ���ʽ�˻�Ϊ�����Ҳ������ı��ʽ
	}
	else if(token.type == MINUS)		// ƥ��һԪ������
	{   match_token(MINUS); 		// ���ʽת��Ϊ��Ԫ������ı��ʽ
		right = factor();
		left = new tree_node;
		left->op_code = CONST_ID;
		left->content.tag_const = 0.0;
		right = make_tree_node(MINUS, left, right);
	}
	else right = component();		// ƥ����ս��component
	return right;
}

// ------------------------ component �ĵݹ��ӳ���
tree_node_ptr grammer_class::component()
{	
	tree_node_ptr left, right;

	left = atom();
	if(token.type == POWER)
	{	match_token(POWER);
		right = component();	// �ݹ����component��ʵ��POWER���ҽ������
		left  = make_tree_node(POWER, left, right);
	}
	return left;
}

// ------------------------ atom �ĵݹ��ӳ���
tree_node_ptr grammer_class::atom()	
{   
	struct token_rec t=token;
	tree_node_ptr t_ptr, tmp;

	switch (token.type)
	{	case CONST_ID :
			match_token (CONST_ID) ;
			t_ptr = make_tree_node(CONST_ID,t.value);
			break;
		case T:
			match_token(T);
			t_ptr = make_tree_node(T);
			break;
		case FUNC :
			match_token (FUNC);
			match_token (L_BRACKET);
			tmp = expression ();
			t_ptr = make_tree_node(FUNC,t.func_ptr,tmp);
			match_token (R_BRACKET);
			break ;
		case L_BRACKET :
			match_token (L_BRACKET);
			t_ptr = expression ();
			match_token (R_BRACKET);
			break ;
		default :
			syntax_error (2);
	}
	return t_ptr;
}

// ----------------- �����﷨����һ���ڵ�
tree_node_ptr grammer_class::make_tree_node(enum token_type opcode, ...)
{	
	tree_node_ptr t_ptr = new (struct tree_node);		// ����ڵ�洢�ռ�
	t_ptr->op_code = opcode;				// ���ռǺŵ����
	va_list arg_ptr ;
	va_start (arg_ptr, opcode);
	switch(opcode)						// ���ݼǺŵ�����첻ͬ�Ľڵ�
	{	case CONST_ID:					// �����ڵ�
			t_ptr->content.tag_const = (double)va_arg(arg_ptr, double);
			break;
		case T:						// �����ڵ�
			t_ptr->content.tag_parameter = &parameter;
			break;
		case FUNC:					// �������ýڵ�
			t_ptr->content.tag_func.math_func_ptr = (func_ptr)va_arg(arg_ptr, func_ptr);
			t_ptr->content.tag_func.child 
					= tree_node_ptr va_arg (arg_ptr, tree_node_ptr);
			break;
		default:					// ��Ԫ����ڵ�
			t_ptr->content.tag_op.left 
					= tree_node_ptr va_arg (arg_ptr, tree_node_ptr);
			t_ptr->content.tag_op.right
					= tree_node_ptr va_arg (arg_ptr, tree_node_ptr);
			break;
	}
	va_end(arg_ptr);
	return t_ptr;
}

// ---------------------- �����﷨�������еĸ��ٵ���
void grammer_class::enter(char * x)		
{
	cout << "enter in  "    << x << endl; 
}
void grammer_class::back(char * x)		
{ 
	cout << "exit from  "   << x << endl; 
}
void grammer_class::call_match(char * x)		
{ 
	cout << "matchtoken	" << x << endl; 
} 
void grammer_class::tree_trace(tree_node_ptr x)
{
	print_syntax_tree(x, 1); 
}
