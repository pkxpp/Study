/********************************************************************
	created:	2015/09/07   11:31
	filename: 	KMaterail.h		
	author:		LiKan
	
	purpose:	
*********************************************************************/
#ifndef KMATERAIL_H
#define KMATERAIL_H

class KMesh;
void ParseSubsetMapping(KMesh *pMesh);
void ParseMaterails(KMesh *pMesh);
void ParseTextureMapping(KMesh *pMesh);


class KParser;
void SaveMaterailFile(KMesh *pMesh, const char *szOutDir, const char* pszTgaOutPath, KParser *pParser);

#endif