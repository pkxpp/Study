/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "Common/Common.h"
#include "display/DisplayCommon.h"
#include "display/DisplayHierarchy.h"
#include "display/DisplayAnimation.h"
#include "display/DisplayMarker.h"
#include "display/DisplaySkeleton.h"
#include "display/DisplayMesh.h"
#include "display/DisplayNurb.h"
#include "display/DisplayPatch.h"
#include "display/DisplayLodGroup.h"
#include "display/DisplayCamera.h"
#include "display/DisplayLight.h"
#include "display/DisplayGlobalSettings.h"
#include "display/DisplayPose.h"
#include "display/DisplayPivotsAndLimits.h"
#include "display/DisplayUserProperties.h"
#include "display/DisplayGenericInfo.h"
#include "KParser.h"
#include <Windows.h>

//#include <max.h>
//#include <decomp.h>
// Local function prototypes.
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayMetaData(FbxScene* pScene);

static bool gVerbose = true;


KParser g_KParser;


int main(int argc, char** argv)
{
	//Quat q;
	//Matrix3 mat;
	//AffineParts ap;
	//decomp_affine(mat, &ap);
	
	
    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;
    bool lResult;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);



	
	if(argc < 4)
	{
		("\n\nUsage: export <FBX file name>   <directory>\n\n");
		return 0;
	}

	const char *szFilePath = argv[1];
	const char *szFileOutDir = argv[2];
	const char *szTgaOutDir = argv[3];

	
	lResult = LoadScene(lSdkManager, lScene, szFilePath);	
    if(lResult == false)
    {
        _FBXSDK_printf("\n\nAn error occurred while loading the scene...");
    }
    else 
    {
		char szName[MAX_PATH];
		GetNameFromPath(szFilePath, szName, MAX_PATH);

		char szParentPathName[MAX_PATH];
		if(!GetParentPathName(szFilePath, szParentPathName, MAX_PATH))
		{
			printf("error: the input fbxfile has no parent path");
			return 0;
		}
		///////////////////////////////////////////////
		g_KParser.Parse(lScene, szName, szParentPathName, lSdkManager, TRUE);
		//g_KParser.Save(szFileOutDir, szTgaOutDir, 1, 33, FALSE, 0, g_KParser.m_nAnimationFrames, FALSE, 2.54f, TRUE);

		///////////////////////////////////////////
		
        // Display the scene.
		
  //      DisplayMetaData(lScene);

  //      _FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

  //      if( gVerbose ) DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

  //      _FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

  //      if( gVerbose ) DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

  //      _FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

  //      if( gVerbose ) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

  //      _FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

  //      if( gVerbose ) DisplayHierarchy(lScene);

  //      _FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

  //      if( gVerbose ) DisplayContent(lScene);

  //      _FBXSDK_printf("\n\n----\nPose\n----\n\n");

  //      if( gVerbose ) DisplayPose(lScene);

  //      _FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");
		//
  //      if( gVerbose ) DisplayAnimation(lScene);

  //      //now display generic information

  //      _FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
  //      if( gVerbose ) DisplayGenericInfo(lScene);
		
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);

    return 0;
}

/*
void DisplayContent(FbxScene* pScene)
{
    int i;
    FbxNode* lNode = pScene->GetRootNode();

    if(lNode)
    {
        for(i = 0; i < lNode->GetChildCount(); i++)
        {
            DisplayContent(lNode->GetChild(i));
        }
    }
}

void DisplayContent(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    int i;

    if(pNode->GetNodeAttribute() == NULL)
    {
        _FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
	    default:
	        break;
        case FbxNodeAttribute::eMarker:  
            DisplayMarker(pNode);
            break;

        case FbxNodeAttribute::eSkeleton:  
            DisplaySkeleton(pNode);
            break;

        case FbxNodeAttribute::eMesh:      
            DisplayMesh(pNode);
            break;

        case FbxNodeAttribute::eNurbs:      
            DisplayNurb(pNode);
            break;

        case FbxNodeAttribute::ePatch:     
            DisplayPatch(pNode);
            break;

        case FbxNodeAttribute::eCamera:    
            DisplayCamera(pNode);
            break;

        case FbxNodeAttribute::eLight:     
            DisplayLight(pNode);
            break;

        case FbxNodeAttribute::eLODGroup:
            DisplayLodGroup(pNode);
            break;
        }   
    }

    DisplayUserProperties(pNode);
    DisplayTarget(pNode);
    DisplayPivotsAndLimits(pNode);
    DisplayTransformPropagation(pNode);
    DisplayGeometricTransform(pNode);

    for(i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }
}


void DisplayTarget(FbxNode* pNode)
{
    if(pNode->GetTarget() != NULL)
    {
        DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
    }
}

void DisplayTransformPropagation(FbxNode* pNode)
{
    _FBXSDK_printf("    Transformation Propagation\n");

    // 
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

    _FBXSDK_printf("        Rotation Space: ");

    switch (lRotationOrder)
    {
    case eEulerXYZ: 
        _FBXSDK_printf("Euler XYZ\n");
        break;
    case eEulerXZY:
        _FBXSDK_printf("Euler XZY\n");
        break;
    case eEulerYZX:
        _FBXSDK_printf("Euler YZX\n");
        break;
    case eEulerYXZ:
        _FBXSDK_printf("Euler YXZ\n");
        break;
    case eEulerZXY:
        _FBXSDK_printf("Euler ZXY\n");
        break;
    case eEulerZYX:
        _FBXSDK_printf("Euler ZYX\n");
        break;
    case eSphericXYZ:
        _FBXSDK_printf("Spheric XYZ\n");
        break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    _FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
        pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    _FBXSDK_printf("        Transformation Inheritance: ");

    switch (lInheritType)
    {
    case FbxTransform::eInheritRrSs:
        _FBXSDK_printf("RrSs\n");
        break;
    case FbxTransform::eInheritRSrs:
        _FBXSDK_printf("RSrs\n");
        break;
    case FbxTransform::eInheritRrs:
        _FBXSDK_printf("Rrs\n");
        break;
    }
}

void DisplayGeometricTransform(FbxNode* pNode)
{
    FbxVector4 lTmpVector;

    _FBXSDK_printf("    Geometric Transformations\n");

    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    _FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    _FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    _FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}


void DisplayMetaData(FbxScene* pScene)
{
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        _FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        _FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        _FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        _FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        _FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        _FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        _FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            _FBXSDK_printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat())
            {
            case FbxThumbnail::eRGB_24:
                _FBXSDK_printf("        Format: RGB\n");
                break;
            case FbxThumbnail::eRGBA_32:
                _FBXSDK_printf("        Format: RGBA\n");
                break;
            }

            switch (thumbnail->GetSize())
            {
	        default:
	            break;
            case FbxThumbnail::eNotSet:
                _FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e64x64:
                _FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e128x128:
                _FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
            }
        }
    }
}

*/