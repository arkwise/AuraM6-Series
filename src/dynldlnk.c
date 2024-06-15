////////////////////////////////////////////////////////////////////////////////
//
//	Dynamic Loading System - Linker
//
//	Copyright (c) 2013 Finn Technologies and Chase Finn. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/dynld.h"
#include "../include/zlib.h"

#ifdef DJGPP
	#include<coff.h>
	#include <conio.h>
#else
	#include<linux/elf.h>
	#define RELOC_REL32	20	/* 32-bit PC-relative address */
	#define RELOC_ADDR32	6	/* 32-bit absolute address */
#endif

typedef struct TResEntrie {

	char Name[32];
	unsigned long DataType;
	unsigned long DataEncode;
	unsigned long DataSize;

} TResEntrie;

typedef struct TResHead {

	unsigned long Magic;
	unsigned long FormatVersion;
	unsigned long Entries;

} TResHead;


l_bool WriteString ( FILE *f, l_text s )
{
	l_ushort length = strlen(s);

	fwrite(&length, 1, 2, f);
	fwrite(s, 1, length, f);
	return 1;
}

int main(int args, char **argv)
{
	char Cmd[2048] = { 0, 0, 0 };
	FILE *in,*out, *sc;
	
#ifdef DJGPP
	FILHDR  CoffHead;
	SCNHDR *CoffSections;
	RELOC   CoffReloc;
	RELOC  *CoffRelocs;
	SYMENT *CoffSymbols;
	char   *CoffStrings;
	l_ulong CoffStringsSize;
	unsigned long CoffRelocsSize;
#else
	Elf32_Ehdr 		ElfHeader;
	Elf32_Shdr 		*ElfSecHeader;
	Elf32_Sym  		*ElfSymbols = 0;
	unsigned long    ElfSymbolsNb = 0;
	Elf32_Rel  		*ElfRel = 0;
	unsigned long    ElfRelNb = 0;
	Elf32_Rela  	*ElfRela = 0;
	unsigned long    ElfRelaNb = 0;
	char 			*ElfText = NULL;
	char 			*ElfSymText = NULL;
	fpos_t tmp;
#endif

	TDynLdHeader  DynHead;
	void        *DynData;
	TDynLdReloc   DynReloc;
	char        *DynName = "(unknown)";
	char         Compress = 0;
	unsigned char d = 0;
	char *RessourceFile = 0;
	l_ulong i;
	
	
	if ( args > 1 ) {
		unsigned char i;
		for(i=1; argv[i]; i++) {
			if (!strcmp(argv[i], "-C")) {
				Compress = 1;
				d++;
			} else if (!strncmp(argv[i], "-R",2)) {
				RessourceFile = strdup(argv[i]+2);
				d++;
			}
		}
	}



	memset(&DynHead, 0, sizeof(TDynLdHeader));
	DynHead.Magic = ULONG_ID('D','n','L','d');
	DynHead.FileFormatVersion = DYNLDVERSION;
	DynHead.Importations = 0;
	DynHead.Time = time(0);
	DynHead.MainOffset  = NoneOffset;
	DynHead.CloseOffset = NoneOffset;
	DynHead.LibsOffset = NoneOffset;

	printf("DynLd Linker [%d.%d.%d.%d]",ULID_A(DYNLDVERSION),ULID_B(DYNLDVERSION),ULID_C(DYNLDVERSION),ULID_D(DYNLDVERSION));
#ifdef DJGPP
	printf(" (coff-go32)\n");
#else 
	printf(" (elf32-i386)\n");
#endif
	printf("Copyright (c) Doscore 2007-2023\n");

	
	
	printf("\n");

	if ( args < 3 + d )
	{
		printf("Usage: DYNLDLNK [-C] [-Rfile] <destination> <source> [source2] [source3] [...]\n");
		printf("\t[-C]           Enable Compression (Powered by ZLIB)\n");
		printf("\t[-Rfile]       Include ressource file 'file' (DMS res files only)\n");
		printf("\t<destination>  Executable (.APP) / Library (.DL) to generate\n");
		printf("\t<sources>      COFF file/s\n");

		return 1;
	}

	/**
	*	Print compilation info
	*/
	if ( RessourceFile ) printf("(i) Ressource file: %s\n", RessourceFile);
	
#ifdef DJGPP
	in = fopen(argv[2 + d], "rb");

	if ( !in )
	{
		printf("/!\\ Error: Input file do not exists.\n");

		return 1;
	}


	fread(&CoffHead, 1, FILHSZ, in);

	if ( CoffHead.f_nscns != 1 || args > 3 ) // Multiple sections... Call LD to make a common one
	{
		fclose(in);

		if ( !(sc = fopen("dynld.ld","rt") ) )
		{
			sc = fopen("dynld.ld","wt");

			if ( sc )
			{
				fprintf(sc ,"OUTPUT_FORMAT(\"coff-go32\")\n\nFORCE_COMMON_ALLOCATION\n\nSECTIONS {\n  .text : {\n\t*(.text)\n\t*(.data)\n\t*(.bss)\n\t*(COMMON)\n  }\n}\n");

				fclose(sc);
				printf("(i) Script - dynld.ld - generated.\n");
			}
			else
			{
				printf("/!\\ Error : Unable to generate dynld.ld script\n");

				return 1;
			}
		}
		else fclose(sc);

		strcpy(Cmd,"ld -X -r -o _tmp.o ");

		for(i = 2 + d; argv[i]; i++)
		{
			strcat(Cmd,argv[i]);
			strcat(Cmd," ");
		}

		strcat(Cmd," -T dynld.ld");
		printf("(i) Calling LD (linker)\n");
		// -S skipped as LD crash

		if ( system(Cmd) )
		{
			printf("/!\\ Error : LD have not return correct code...\n");

			return 1;
		}

		in = fopen("_tmp.o","rb");

		if ( !in )
		{
			printf("/!\\ Error: Temporary file do not exists.\n");

			return 1;
		}

		fread(&CoffHead, 1, FILHSZ, in);
	}

	out = fopen(argv[1+d],"wb+");

	if ( !out )
	{
		printf("/!\\ Error: Unable to open output file.\n");

		return 1;
	}
	
	// Load Coff Sections
	fseek(in, CoffHead.f_opthdr, SEEK_CUR);
	CoffSections = (SCNHDR *) malloc(SCNHSZ*CoffHead.f_nscns);
	fread(CoffSections, CoffHead.f_nscns, SCNHSZ, in);

	// Load Coff Data
	DynHead.Size = CoffSections[0].s_paddr+CoffSections[0].s_size;
	DynData = (void*)malloc(DynHead.Size);
	memset(DynData, 0, DynHead.Size);
	fseek(in, CoffSections[0].s_scnptr, SEEK_SET);
	fread(DynData, 1, CoffSections[0].s_size, in);

	// Load Coff Symbols
	fseek(in, CoffHead.f_symptr, SEEK_SET);
	CoffSymbols = (SYMENT *) malloc(SYMESZ*CoffHead.f_nsyms);
	fread(CoffSymbols, CoffHead.f_nsyms, SYMESZ, in);

	// Load Coff Strings
	fread(&CoffStringsSize, 1, 4, in);
	CoffStrings = (char *) malloc(CoffStringsSize-4);
	memset(CoffStrings, 0, 4);
	fread(CoffStrings+4, 1, CoffStringsSize-4, in);
	
	// Load Coff Relocations
    fseek(in, CoffSections[0].s_relptr, SEEK_SET);
    CoffRelocsSize = RELSZ*CoffSections[0].s_nreloc;
    CoffRelocs = malloc(CoffRelocsSize);
    fread(CoffRelocs, 1, CoffRelocsSize, in);    
    
	// Write DynHeader
	fwrite(&DynHead, 1, sizeof(TDynLdHeader), out);

	// Look for Key Data
	for (i=0;i<CoffHead.f_nsyms;i++)
	{
		char *name, nam8[9];

		if (CoffSymbols[i].e.e.e_zeroes)
		{
			memcpy(nam8, CoffSymbols[i].e.e_name, 8);
			nam8[8] = 0;
			name = nam8;
		}
		else name = CoffStrings+CoffSymbols[i].e.e.e_offset;
		//printf("  %d : 0x%06x : %s\n",i,CoffSymbols[i].e_value,name);

		if ( name[0] != '.' /*&& CoffSymbols[i].e_value*/ )
		{
			if ( !strcmp(name+1,"LibMain") ) DynHead.MainOffset = CoffSymbols[i].e_value;
			if ( !strcmp(name+1,"Main") )
			{
				DynHead.Type = DYNLD_TYPEAPP;
				DynHead.MainOffset = CoffSymbols[i].e_value;
				printf("  Threading App\n");
			}
			if ( !strcmp(name+1,"nUID") )  memcpy(&DynHead.UID,DynData+CoffSymbols[i].e_value,12);
			if ( !strcmp(name+1,"AppName") ) DynName = (char*)DynData+CoffSymbols[i].e_value;
			if ( !strcmp(name+1,"Close") )  DynHead.CloseOffset = CoffSymbols[i].e_value;
			if ( !strcmp(name+1,"NeededLibs") )  DynHead.LibsOffset = CoffSymbols[i].e_value;

			if ( !strcmp(name+1,"AppVersion") )  memcpy(&DynHead.FileVersion,DynData+CoffSymbols[i].e_value,4);
			//printf("  %d : 0x%06x : %s\n",i,CoffSymbols[i].e_value,name+1);
		}

    i += CoffSymbols[i].e_numaux;
  }

  if ( DynHead.MainOffset == NoneOffset ) {
	  printf("\nWARNING : No Main() is defined.\n");
	  printf("Example :\n");
    printf("#include\"kernel.h\"\n");
    printf("l_bool Main ( int argc, l_text *argv ) {\n");
    printf("  return true;\n");
    printf("}\n");
  }

	if ( DynHead.CloseOffset == NoneOffset && DynHead.Type != DYNLD_TYPEAPP )
	{
		printf("\nWARNING : No Close() is defined.\n");
		printf("Example :\n");
		printf("#include\"kernel.h\"\n");
		printf("void Close ( void ) {\n");
		printf("}\n");
	}

	//Write Application name to file
  WriteString(out, DynName);

  
  	 // Precalculate relocations
	for (i=0;i<CoffSections[0].s_nreloc;i++) {
		if ( CoffRelocs[i].r_type == RELOC_REL32 && !CoffSymbols[CoffRelocs[i].r_symndx].e_scnum ) {
			*(long*)(((((long)DynData))+CoffRelocs[i].r_vaddr)) += CoffRelocs[i].r_vaddr;
		}
	}
  
  
  //Write Data to File (after haeder and informations text)
  if ( Compress ) {
	  void *Temp;
	  DynHead.Compression = 0x01;
	  DynHead.OriginalSize = DynHead.Size;
	  DynHead.Size = DynHead.OriginalSize+DynHead.OriginalSize/5+12;
	  Temp = (void*)malloc(DynHead.Size);
	  compress(Temp,&DynHead.Size,DynData,DynHead.OriginalSize);
	  fwrite(Temp, 1, DynHead.Size, out);
	  free(Temp);
  } else
    fwrite(DynData, 1, DynHead.Size, out);


  // Write Inportaions
  for (i=0;i<CoffHead.f_nsyms;i++) {
    char *name, nam8[9];
    if (CoffSymbols[i].e.e.e_zeroes) {
      memcpy(nam8, CoffSymbols[i].e.e_name, 8);
      nam8[8] = 0;
      name = nam8;
    } else
      name = CoffStrings+CoffSymbols[i].e.e.e_offset;

    if ( name[0] != '.' && !CoffSymbols[i].e_scnum ) {
      WriteString(out, name+1);
      fwrite(&i, 1, 4, out);
      DynHead.Importations++;
    }
    i += CoffSymbols[i].e_numaux;
  }


	for (i=0;i<CoffSections[0].s_nreloc;i++) {
		if ( CoffRelocs[i].r_type == RELOC_ADDR32 || !CoffSymbols[CoffRelocs[i].r_symndx].e_scnum ) {
			
			if ( CoffRelocs[i].r_type == RELOC_ADDR32 )
				DynReloc.Type = REL32_ABSOLUTE;
			else 
				DynReloc.Type = REL32_RELATIVE;
				
			DynReloc.Address = CoffRelocs[i].r_vaddr;
			DynReloc.Symbol = CoffRelocs[i].r_symndx;
			fwrite(&DynReloc, 1, sizeof(TDynLdReloc), out);
			DynHead.Relocations++;
		}
	}

#else

	if ( !(sc = fopen("dynldlnx.ld","rt") ) ) {
		sc = fopen("dynldlnx.ld","wt");
		if ( sc ) {
			fprintf(sc ,"OUTPUT_FORMAT(\"elf32-i386\")\n\nFORCE_COMMON_ALLOCATION\n\nSECTIONS {\n  .text : {\n	*(.text)\n	*(.data)\n	*(.rodata*)\n	*(.bss)\n	*(COMMON)\n  }\n}\n");
			fclose(sc);
			printf("(i) Script - dynldlnx.ld - generated.\n");
		} else {
			printf("/!\\ Error : Unable to generate dynldlnx.ld script\n");
			return 1;
		}
	}
	else fclose(sc);

	strcpy(Cmd,"ld -S -X -r -o _tmp.o ");

	for(i = 2 + d; argv[i]; i++) {
		strcat(Cmd,argv[i]);
		strcat(Cmd," ");
	}

	strcat(Cmd," -T dynldlnx.ld");
	printf("(i) Calling LD (linker)\n");

	if ( system(Cmd) ) {
		printf("/!\\ Error : LD have not return correct code...\n");
		return 1;
	}

	in = fopen("_tmp.o","rb");

	if ( !in ) {
		printf("/!\\ Error: Temporary file do not exists.\n");
		return 1;
	}
	
	fread(&ElfHeader,1,sizeof(Elf32_Ehdr),in);

	if ( ElfHeader.e_type != 1 ) {
		printf("/!\\ Error: DynLd needs a realocatable file\n");
		fclose(in);
		return 1;
	}
	
	if ( ElfHeader.e_machine != 3 ) {
		printf("/!\\ Error: DynLd support only i386!\n");
		fclose(in);
		return 1;
	}
	
	out = fopen(argv[1+d],"wb+");

	if ( !out ) {
		printf("/!\\ Error: Unable to open output file.\n");
		return 1;
	}

	fseek(in, ElfHeader.e_shoff, SEEK_SET);

	ElfSecHeader = (Elf32_Shdr*)malloc(ElfHeader.e_shnum*sizeof(Elf32_Shdr));
	fread(ElfSecHeader,ElfHeader.e_shnum,sizeof(Elf32_Shdr),in);

	if ( ElfHeader.e_shstrndx ) {
		Elf32_Shdr TxtSec = ElfSecHeader[ElfHeader.e_shstrndx];
		ElfText = (char*)malloc(TxtSec.sh_size);
		fseek(in, TxtSec.sh_offset, SEEK_SET);
		fread(ElfText,1,TxtSec.sh_size,in);
	}

	for(i = 0; i < ElfHeader.e_shnum; i++) {

		if ( (ElfSecHeader[i].sh_type == SHT_SYMTAB) && !strcmp(".symtab",(char*)((long)ElfText)+ElfSecHeader[i].sh_name) ) {
			fseek(in, ElfSecHeader[i].sh_offset, SEEK_SET);
			ElfSymbols = (Elf32_Sym*)malloc(ElfSecHeader[i].sh_size);
			fread(ElfSymbols,1,ElfSecHeader[i].sh_size,in);
			ElfSymbolsNb = ElfSecHeader[i].sh_size/sizeof(Elf32_Sym);
		}
		if ( (ElfSecHeader[i].sh_type == SHT_STRTAB) && !strcmp(".strtab",(char*)((long)ElfText)+ElfSecHeader[i].sh_name)) {

			fseek(in, ElfSecHeader[i].sh_offset, SEEK_SET);
			ElfSymText = (char*)malloc(ElfSecHeader[i].sh_size);
			fread(ElfSymText,1,ElfSecHeader[i].sh_size,in);

		}
		if ( (ElfSecHeader[i].sh_type == SHT_REL) && !strcmp(".rel.text",(char*)((long)ElfText)+ElfSecHeader[i].sh_name)) {
			fseek(in, ElfSecHeader[i].sh_offset, SEEK_SET);
			ElfRel = (Elf32_Rel*)malloc(ElfSecHeader[i].sh_size);
			fread(ElfRel,1,ElfSecHeader[i].sh_size,in);
			ElfRelNb = ElfSecHeader[i].sh_size/sizeof(Elf32_Rel);

		}
		if ( (ElfSecHeader[i].sh_type == SHT_RELA) && !strcmp(".rela.text",(char*)((long)ElfText)+ElfSecHeader[i].sh_name)) {
			fseek(in, ElfSecHeader[i].sh_offset, SEEK_SET);
			ElfRela = (Elf32_Rela*)malloc(ElfSecHeader[i].sh_size);
			fread(ElfRela,1,ElfSecHeader[i].sh_size,in);
			ElfRelaNb = ElfSecHeader[i].sh_size/sizeof(Elf32_Rela);

		}

		if ( /*(ElfSecHeader[i].sh_type == SHT_PROGBITS) && */!strcmp(".text",(char*)((long)ElfText)+ElfSecHeader[i].sh_name)) {
			fseek(in, ElfSecHeader[i].sh_offset, SEEK_SET);
			DynHead.Size = ElfSecHeader[i].sh_size;
			DynData = malloc(DynHead.Size);
			fread(DynData,1,DynHead.Size,in);
		}
	}

	if ( ElfSymbolsNb ) {
		for(i = 0; i < ElfSymbolsNb; i++) {
			if ( ElfSymbols[i].st_name && ElfSymbols[i].st_shndx ) {
				char *Name = (char*)((long)ElfSymText)+ElfSymbols[i].st_name;
				if ( !strcmp(Name,"LibMain") ) DynHead.MainOffset = ElfSymbols[i].st_value;
				if ( !strcmp(Name,"Main") )
				{
					DynHead.Type = DYNLD_TYPEAPP;
					DynHead.MainOffset = ElfSymbols[i].st_value;
				}
				if ( !strcmp(Name,"nUID") )  memcpy(&DynHead.UID,DynData+ElfSymbols[i].st_value,12);
				if ( !strcmp(Name,"AppName") ) DynName = (char*)DynData+ElfSymbols[i].st_value;
				if ( !strcmp(Name,"Close") )  DynHead.CloseOffset = ElfSymbols[i].st_value;
				if ( !strcmp(Name,"NeededLibs") )  DynHead.LibsOffset = ElfSymbols[i].st_value;
				if ( !strcmp(Name,"AppVersion") )  memcpy(&DynHead.FileVersion,DynData+ElfSymbols[i].st_value,4);
			}
		}
	}
	  
	
	if ( DynHead.MainOffset == NoneOffset ) {
		printf("\nWARNING : No Main() is defined.\n");
		printf("Example :\n");
		printf("#include\"kernel.h\"\n");
		printf("l_bool Main ( int argc, l_text *argv ) {\n");
		printf("  return true;\n");
		printf("}\n");
	}

	if ( DynHead.CloseOffset == NoneOffset && DynHead.Type != DYNLD_TYPEAPP )
	{
		printf("\nWARNING : No Close() is defined.\n");
		printf("Example :\n");
		printf("#include\"kernel.h\"\n");
		printf("void Close ( void ) {\n");
		printf("}\n");
	}
	
	if ( ElfRelNb ) { // Precalculate relocations
		int symbol;
		for(i = 0; i < ElfRelNb; i++) {
			symbol = ELF32_R_SYM(ElfRel[i].r_info);;

			*((long*)(DynData+ElfRel[i].r_offset)) += ElfSymbols[symbol].st_value;
		}
	}
	
	if ( ElfRelaNb ) { // Precalculate relocations
		int symbol;
		for(i = 0; i < ElfRelaNb; i++) {
			symbol = ELF32_R_SYM(ElfRela[i].r_info);
			*((long*)(DynData+ElfRela[i].r_offset)) = ElfRela[i].r_addend+ElfSymbols[symbol].st_value;
		}
	}
	
	// Write DynHeader
	fwrite(&DynHead, 1, sizeof(TDynLdHeader), out);

	// Write Application name to file
  	WriteString(out, DynName);
	
  	// Write Data to File (after haeder and informations text)
	if ( Compress ) {
		void *Temp;
		DynHead.Compression = 0x01;
		DynHead.OriginalSize = DynHead.Size;
		DynHead.Size = DynHead.OriginalSize+DynHead.OriginalSize/5+12;
		Temp = (void*)malloc(DynHead.Size);
		compress(Temp,&DynHead.Size,DynData,DynHead.OriginalSize);
		fwrite(Temp, 1, DynHead.Size, out);
		free(Temp);
	} else
		fwrite(DynData, 1, DynHead.Size, out);
	
	if ( ElfSymbolsNb ) {
		for(i = 0; i < ElfSymbolsNb; i++) {
			if ( ElfSymbols[i].st_name && !ElfSymbols[i].st_shndx ) {
				char *Name = (char*)((long)ElfSymText)+ElfSymbols[i].st_name;
				WriteString(out, Name);
				fwrite(&i, 1, 4, out);
				DynHead.Importations++;
			}
		}
	}
	
	if ( ElfRelNb ) {
		for(i = 0; i < ElfRelNb; i++) {
			if ( ELF32_R_TYPE(ElfRel[i].r_info) == 1 )
				DynReloc.Type = REL32_ABSOLUTE;
			else if ( ELF32_R_TYPE(ElfRel[i].r_info) == 2 )
				DynReloc.Type = REL32_RELATIVE;
			else
				DynReloc.Type = 0x80 | (ELF32_R_TYPE(ElfRela[i].r_info));
			DynReloc.Address = ElfRel[i].r_offset;
			DynReloc.Symbol = ELF32_R_SYM(ElfRel[i].r_info);
			fwrite(&DynReloc, 1, sizeof(TDynLdReloc), out);
			DynHead.Relocations++;
		}
	}
	
	if ( ElfRelaNb ) {
		for(i = 0; i < ElfRelaNb; i++) {
			if ( ELF32_R_TYPE(ElfRela[i].r_info) == 1 )
				DynReloc.Type = REL32_ABSOLUTE;
			else if ( ELF32_R_TYPE(ElfRela[i].r_info) == 2 )
				DynReloc.Type = REL32_RELATIVE;
			else
				DynReloc.Type = 0x80 | (ELF32_R_TYPE(ElfRela[i].r_info));
			DynReloc.Address = ElfRela[i].r_offset;
			DynReloc.Symbol = ELF32_R_SYM(ElfRela[i].r_info);
			fwrite(&DynReloc, 1, sizeof(TDynLdReloc), out);
			DynHead.Relocations++;
		}
	}
		
#endif
  
  
#ifdef DJGPP
	fgetpos(out,&(DynHead.RessourceOffset));
#else
	DynHead.RessourceOffset = ftell(out);
#endif
  
  
  if ( RessourceFile ) {
  	FILE *r = fopen(RessourceFile,"rb");
	  if ( r ) {
			TResHead Head;
			unsigned long Entries;
			TResEntrie E;
			void *buffer;
			fread(&Head,sizeof(TResHead),1,r);
			if ( Head.Magic == ULONG_ID('D','M','S','R') ) {
			  if ( Head.FormatVersion == ULONG_ID(0,0,0,1) ) {
				  Entries = Head.Entries;
				  while ( Entries ) {
					  fread(&E,sizeof(TResEntrie),1,r);
					  fwrite(&E,sizeof(TResEntrie),1,out);
					  buffer = malloc(E.DataSize);
					  fread(buffer,E.DataSize,1,r);
					  fwrite(buffer,E.DataSize,1,out);
					 	Entries--;
				  }
				  DynHead.RessourceEntries = Head.Entries;
			  }
		  }
  		fclose(r);
		}
	}

	printf("\nApplication Informations\n");
	printf("	UID : '%s' Name : '%s' Version : %d.%d.%d.%d\n",&DynHead.UID,DynName,ULID_A(DynHead.FileVersion),ULID_B(DynHead.FileVersion),ULID_C(DynHead.FileVersion),ULID_D(DynHead.FileVersion));
	if ( Compress )
	printf("	Compreession : %d %%\n",DynHead.OriginalSize*100/DynHead.Size);
	
	if ( DynHead.RessourceEntries )
	printf("	Ressource   : %d Entrie(s)\n",DynHead.RessourceEntries);
	fseek(out, 0, SEEK_SET);
	fwrite(&DynHead, 1, sizeof(TDynLdHeader), out);

#ifdef DJGPP  
  	free(CoffStrings);
  	free(CoffSymbols);
  	free(CoffSections);
  	free(CoffRelocs);
#else
	if ( ElfSymText ) free(ElfSymText);
	if ( ElfText ) free(ElfText);
	if ( ElfSymbols ) free(ElfSymbols);
	if ( ElfRel ) free(ElfRel);
#endif

	free(DynData);
	fclose(in);
	fclose(out);
	remove("_tmp.o");
	printf("\n");
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
