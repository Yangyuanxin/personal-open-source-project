#include "menu.h"

/*菜单操作表定义*/
static OP_MENU_PAGE g_opStruct[] = 
{
	{MAIN_PAGE      	, 	main_page_process},	
	{LOG_PAGE					, 	log_page_process},
	{CONF_PAGE      	, 	conf_page_process},
	{PARA_PAGE				,   para_setting_page_process},
	{VERSION_PAGE   	, 	version_info_page_process},
	{PASSWD_PAGE			,   password_page_process},
	{PASSWD_INPUT_PAGE,		password_input_page_process},
	{DATETIME_PAGE		,		datetime_page_process},
	{RECOVERY_PAGE		,   recovery_page_process},
};





/*跳转到表所对应的页面*/
static int JUMP_Table(int8_t op , uint8_t KeyValue)
{
	assert(op >= sizeof(g_opStruct)/sizeof(g_opStruct[0]));
	assert(op < 0);
	g_opStruct[op].opfun(KeyValue);
	return 0 ;
}

/*菜单选择项*/
void Menu_Select_Item(uint8_t current_index, uint8_t KeyValue)
{
	JUMP_Table(current_index , KeyValue);
}
/****************************************************************************************/





