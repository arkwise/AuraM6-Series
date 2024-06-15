/**
*	User Functions for registry
*/
l_bool INISectionExists (l_text szFileName, l_text szSection);
l_char *RegistryGetKey ( l_text path, l_text key );
l_int RegistryGetInteger ( l_text path, l_text key );
l_color  RegistryGetColor(l_char *name);
l_char *INIGet ( l_text file, l_text path, l_text key );
l_int INIGetInteger(l_text szFileName, l_text szSection, l_text szItem, l_int iDefault);
void INIGetText(l_text szFileName, l_text szSection, l_text szItem, l_text szValue, l_text szDefault);
l_text INIGetNew(l_text szFileName, l_text szSection, l_text szItem, l_text szDefault);
l_color INIGetColor(l_text szFileName, l_text szSection, l_text szItem, l_color colDefault);
