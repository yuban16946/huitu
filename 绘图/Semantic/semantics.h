// ----------------------------- semantics.h ---------------------------------

//#include <windows.h>
#include <windows.h>
#include <wingdi.h>
#include "grammer.h"

extern HDC hDC;

//---------------- ������ͼ�������, ����grammer_class��������
class semantics_class : public grammer_class {	
protected:
	double 	Origin_x, Origin_y,		// �ᡢ��ƽ�ƾ���
		Scale_x,  Scale_y, 		// �ᡢ�ݱ�������
		rot_angle;			// ��ת�Ƕ�
public:
	semantics_class()			// ��������ʱ��������ֵ
	{	Origin_x  = 0;	Origin_y = 0;	// ԭ����(0, 0)
		Scale_x   = 1;	Scale_y  = 1;	// �ᡢ�����������1:1
		rot_angle = 0;			// ��ת�Ƕ���0
	};
	~semantics_class() {};

private:
	// ����ɫ���趨
	const red()   { return RGB(30,144,255);};		// ������ɫ��������
	const black() { return RGB(0, 0, 0);};			// ��ɫ

	// ���û���grammer_class�еķ����������﷨�����Ļ�����ʵ���﷨�Ƶ�����
	void Errmsg (char *string);				// ������
	void error_msg (int line, char *descrip, char *string);	// ������
	void for_statement ();
	void origin_statement();
	void rot_statement();
	void scale_statement();
	void colors();
	// ����������semantics_class�б�����Ϊ�������ã��������﷨����ʱ�ĸ��ٲ���
	void enter();
   	void back();
    	void call_match(); 
    	void tree_trace();

	// semantics_class�еķ������������﷨�Ƶ���������������庯��
    	double get_tree_value(tree_node_ptr root);			// ��ñ��ʽ��ֵ
	void cal_coord(	tree_node_ptr hor_ptr,				// ����������
					tree_node_ptr ver_ptr,
					double &hor_val,
					double &ver_val);
	void draw_pixel(unsigned long x, unsigned long y);		// ����һ����
	void draw_loop(	double start_val,				// ͼ�λ���
					double end_val,
					double step_val,
					tree_node_ptr x_ptr,
					tree_node_ptr y_ptr);
	void delete_tree(tree_node_ptr root);				// ɾ��һ����
};
