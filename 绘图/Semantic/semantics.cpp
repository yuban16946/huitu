// ----------------------------- semantics.cpp ---------------------------------
// ������ͼ��Ķ���

#include "semantics.h"

// ------------------------ ������
void semantics_class::Errmsg (char *string) { exit (1); }
void semantics_class::error_msg(int line, char * descrip, char * string)
{
	const int buf_len=256;
	char msg[buf_len];

	memset(msg, 0, buf_len);
	sprintf(msg, "Line No %3d:  %s %s !", lexical.line_no, descrip, string);
	MessageBox(NULL, msg, "error!", MB_OK);
	lexical.close_lexical();
	exit(1);
}

// ------------------------ ���㱻���Ƶ������
void semantics_class::cal_coord(tree_node_ptr 	x_ptr,
						tree_node_ptr y_ptr,
						double &x_ret,
						double &y_ret)
{	
	double x_val, x_temp, y_val;

	// ������ʽ��ֵ���õ����ԭʼ����
	x_val = get_tree_value(x_ptr);
	y_val = get_tree_value(y_ptr);

	// �����任
	x_val *= Scale_x ;
	y_val *= Scale_y ;

	// ��ת�任
	x_temp = x_val*cos(rot_angle) + y_val*sin(rot_angle);
	y_val  = y_val*cos(rot_angle) - x_val*sin(rot_angle);
	x_val  = x_temp;
	
	// ƽ�Ʊ任
	x_val += Origin_x;
	y_val += Origin_y;

	// ���ر任��������
	x_ret = x_val;
	y_ret = y_val;
}

// ------------------------ ѭ�����Ƶ�����
void semantics_class::draw_loop
			(	double 		start_val,
				double 		end_val,
				double 		step_val,
				tree_node_ptr 	x_ptr,
				tree_node_ptr 	y_ptr)
{
	double x_val, y_val;
	for(parameter = start_val; parameter <= end_val; parameter += step_val)
	{	cal_coord(x_ptr, y_ptr, x_val, y_val);
//		y_val = 460-y_val;			// �޸���ʾ����ֱ������ϵ
		draw_pixel((unsigned long)x_val, (unsigned long)y_val);
	}
}

// ------------------------ ������ʽ��ֵ
double semantics_class::get_tree_value(tree_node_ptr root)
{	
	if (root == NULL) return 0.0;
	switch (root -> op_code)
	{	case PLUS  :
			return get_tree_value(root->content.tag_op.left ) + get_tree_value(root->content.tag_op.right) ;
		case MINUS :
			return get_tree_value(root->content.tag_op.left ) - get_tree_value(root->content.tag_op.right) ;
		case MUL   :
			return get_tree_value(root->content.tag_op.left ) * get_tree_value(root->content.tag_op.right) ;
		case DIV   :
			return get_tree_value(root->content.tag_op.left ) / get_tree_value(root->content.tag_op.right) ;
		case POWER :
			return pow(get_tree_value(root->content.tag_op.left ), get_tree_value(root->content.tag_op.right) );
		case FUNC  :
			return (* root->content.tag_func.math_func_ptr)	(get_tree_value(root->content.tag_func.child) );
		case CONST_ID :
			return root->content.tag_const ;
		case T  :
			return *(root->content.tag_parameter);
		default :
			return 0.0 ;
	}
}

// ------------------------ ɾ��һ���﷨��
void semantics_class::delete_tree(tree_node_ptr root)
{	
	if (root == NULL) return;
	switch (root -> op_code)
	{   case PLUS  :			// �������ӵ��ڲ��ڵ�
	    case MINUS :
	    case MUL   :
	    case DIV   :
	    case POWER :
				delete_tree (root->content.tag_op.left ) ;
				delete_tree (root->content.tag_op.right) ;
				break ;
	    case FUNC  : 			// һ�����ӵ��ڲ��ڵ�
				delete_tree (root->content.tag_func.child) ;
				break ;
	    default    : 			// Ҷ�ӽڵ�
				break ;
	}
	delete(root) ;				// ɾ���ڵ�
}

// ------------------------ ����һ����
void semantics_class::draw_pixel(unsigned long x, unsigned long y)
{
	if (line_color==red_color) {
		SetPixel(hDC, x, y, red());
		SetPixel(hDC, x+1, y, red());
		SetPixel(hDC, x, y+1, red());
		SetPixel(hDC, x+1, y+1, red());
	}
	else {
		SetPixel(hDC, x, y, black());
		SetPixel(hDC, x+1, y, black());
		SetPixel(hDC, x, y+1, black());
		SetPixel(hDC, x+1, y+1, black());
	}
}

// ------------------------ origin_statement�ĵݹ��ӳ���
void semantics_class::origin_statement()
{
	grammer_class::origin_statement();		// �﷨������ȡ�﷨������grammer�е����Ա��棩

	Origin_x = get_tree_value(x_ptr);		// �����﷨������������ƽ�ƾ���
	delete_tree(x_ptr);

	Origin_y = get_tree_value(y_ptr);		// �����﷨�������������ƽ�ƾ���
	delete_tree(y_ptr);
}

// ------------------------ scale_statement�ĵݹ��ӳ���
void semantics_class::scale_statement () 
{
	grammer_class::scale_statement();		// �﷨������ȡ�﷨������grammer�е����Ա��棩

	Scale_x = get_tree_value(x_ptr);		// �����﷨�����������ı�������
	delete_tree(x_ptr);

	Scale_y = get_tree_value(y_ptr);		// �����﷨������������ı�������
	delete_tree(y_ptr);
}

// ------------------------ rot_statement�ĵݹ��ӳ���
void semantics_class::rot_statement () 
{	
	grammer_class::rot_statement();			// �﷨������ȡ�﷨������grammer�е����Ա��棩
	rot_angle = get_tree_value(angle_ptr);		// �����﷨��������ת�Ƕ�
	delete_tree(angle_ptr);
}

// ------------------------ for_statement �ĵݹ��ӳ���
void semantics_class::for_statement () 
{	
	double start_val, end_val, step_val;		// ��ͼ��㡢�յ㡢����

	grammer_class::for_statement();			// �﷨������ȡ�����ʽ���﷨������grammer�е����Ա��棩
	
	start_val = get_tree_value(start_ptr);		// ���������ʽ��ֵ
	end_val = get_tree_value(end_ptr);		// �����յ���ʽ��ֵ
	step_val = get_tree_value(step_ptr);		// ���㲽�����ʽ��ֵ

	draw_loop (start_val, end_val, step_val, x_ptr, y_ptr); // ��ͼ

	delete_tree(start_ptr);					// �ͷſռ�
	delete_tree(end_ptr);
	delete_tree(step_ptr);
	delete_tree(x_ptr);	
	delete_tree(y_ptr);	
}

// ---------------------- �����﷨�������еĸ��ٵ���, �˴�����������
void semantics_class::enter() {}
void semantics_class::back() {}
void semantics_class::call_match() {} 
void semantics_class::tree_trace() {}

