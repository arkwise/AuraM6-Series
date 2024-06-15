/**
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			registry.c
*
*	PROJECT:		Phoenix engine - Core
*
*	DESCRIPTION:	Used for reading and writing into the system registry.
*
*	CONTRIBUTORS:
*					Lukas Lipka
*
*	TODO:			@ Add writing support
*
*	BUG:			@
*
*	MISTAKE:		@
*
*	IDEA:			@
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"
#include "ini.h"

#define REGISTRY_FILE "./SYSTEM/REGS/xsystem.reg"

#define RGB(r,g,b)  makecol(r, g, b)

l_char   cBuffer[65536];
l_char   cBuff2[1024];

void RemoveSpaces(l_text szString)
{
	while (szString[0] == ' ')
		memmove(szString, szString+1, strlen(szString));
	while (szString[strlen(szString)-1] == ' ')
		szString[strlen(szString)-1] = 0;
}

l_bool INISectionExists (l_text szFileName, l_text szSection)
{
	FILE  *f;

	if (!szFileName || !szSection)
		return false;

	f = fopen(szFileName, "r");

	if (!f)
		return false;

	while (!feof(f))
	{
		fgets(cBuffer, 65536, f);

		if (cBuffer[0] == ';') break;

		if (cBuffer[0] == '[' && cBuffer[strlen(cBuffer)-2] == ']') // note \r\n
		{
			cBuffer[strlen(cBuffer)-2] = 0;

			if (!stricmp(cBuffer+1, szSection))
				return true;
		}
	}

	fclose(f);

	return false;
}

void INIGetText(l_text szFileName, l_text szSection, l_text szItem, l_text szValue, l_text szDefault)
{
	FILE  *f;

	if ( !szFileName || !szSection || !szItem || !szValue )
		return;

//DebugMessage("INIGetText: Open %s...", szFileName);
	f = fopen(szFileName, "r");

	if ( !f )
	{
		if ( szDefault )
			strcpy(szValue, szDefault);
		else
			strcpy(szValue, "");

		return;
	}

	while (!feof(f))
	{
		fgets(cBuffer, 65536, f);

		if (cBuffer[0] == ';') break;

		if (cBuffer[0] == '[' && cBuffer[strlen(cBuffer)-2] == ']') // note \r\n
		{
			cBuffer[strlen(cBuffer)-2] = 0;

			if (!stricmp(cBuffer+1, szSection))
			{
				while (!feof(f))
				{
					l_text equalpos;
					fgets(cBuffer, 65536, f);

					if (cBuffer[0] == '[' && cBuffer[strlen(cBuffer)-2] == ']') break;

					equalpos = strchr(cBuffer, '=');

					if (equalpos)
					{
						equalpos[0] = 0;
						RemoveSpaces(cBuffer);

						if (!stricmp(cBuffer, szItem))
						{
							l_int eqplen;
							fclose(f);
							equalpos++;
							equalpos[strlen(equalpos)-1] = 0;
							RemoveSpaces(equalpos);
							eqplen = strlen(equalpos);

							if (eqplen > 1 && equalpos[0] == '"' && equalpos[eqplen-1] == '"')
							{
								memmove(equalpos, equalpos+1, eqplen);
								eqplen -= 2;
								equalpos[eqplen] = 0;
							}

							strcpy(szValue, equalpos);

							return;
						}
					}
				}
			}
		}
	}

	if (szDefault)
		strcpy(szValue, szDefault);
	else
		strcpy(szValue, "");

	fclose(f);
}

l_text INIGetNew(l_text szFileName, l_text szSection, l_text szItem, l_text szDefault)
{
	char  value[65536];

	INIGetText(szFileName, szSection, szItem, value, szDefault);

	return strdup(value);
}

l_int INIGetInteger(l_text szFileName, l_text szSection, l_text szItem, l_int iDefault)
{
	if ( !szFileName || !szSection || !szItem )
		return iDefault;

	INIGetText(szFileName, szSection, szItem, cBuff2, "\001\002\003\004");

	if (!strcmp(cBuff2, "\001\002\003\004"))
		return iDefault;
	else
		return atoi(cBuff2);
}

l_color INIGetColor(l_text szFileName, l_text szSection, l_text szItem, l_color colDefault)
{
	if (!szFileName || !szSection || !szItem)
		return RGB(0, 0, 0);

	INIGetText(szFileName, szSection, szItem, cBuff2, "\001\002\003\004");

	if (!strcmp(cBuff2, "\001\002\003\004"))
		return colDefault;
	else
	{
		l_char       rgb[3][10];
		l_int        i, j = 0;
		memset(rgb, 0, sizeof(rgb));

		for (i=0;cBuff2[i];i++)
			if (cBuff2[i] == ',')
			{
				if (j == 2)
					break;
				else
					j++;
			}
			else if (cBuff2[i] != ' ')
				rgb[j][strlen(rgb[j])] = cBuff2[i];

		return RGB(atoi(rgb[0]), atoi(rgb[1]), atoi(rgb[2]));
	}
}

l_char *INIGet ( l_text file, l_text path, l_text key )
{
	return INIGetNew(file, path, key, "");
}

l_char *RegistryGetKey ( l_text path, l_text key )
{
	return INIGetNew(REGISTRY_FILE, path, key, "");
}

int RegistryGetInteger ( l_text path, l_text key )
{
	return INIGetInteger(REGISTRY_FILE, path, key, 0);
}
