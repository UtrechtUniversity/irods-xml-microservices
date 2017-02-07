/**
 * @file	msiLoadMetadataFromXmlBuf.cpp
 *
 */ 

/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemas.h>
#include <libxml/uri.h>
#include "apiHeaderAll.hpp"
#include "msParam.hpp"
#include "reGlobalsExtern.hpp"
#include "irods_ms_plugin.hpp"
#include "irods_server_api_call.hpp"
#include "microservice.hpp"
#include "objMetaOpr.hpp"
#include "miscUtil.h"






/* get the first child node with a given name */
static xmlNodePtr
getChildNodeByName(xmlNodePtr cur, char *name)
{
        if (cur == NULL || name == NULL)
	{
		return NULL;
	}

        for(cur=cur->children; cur != NULL; cur=cur->next)
	{
                if(cur->name && (strcmp((char*)cur->name, name) == 0))
		{
                        return cur;
                }
        }

	return NULL;
}



/* custom handler to catch XML validation errors and print them to a buffer */
static int 
myErrorCallback(bytesBuf_t *errBuf, const char* errMsg, ...)
{
	va_list ap;
	char tmpStr[MAX_NAME_LEN];
	int written;

	va_start(ap, errMsg);
	written = vsnprintf(tmpStr, MAX_NAME_LEN, errMsg, ap);
	va_end(ap);

	appendToByteBuf(errBuf, tmpStr);
	return (written);
}


/**
 * \fn msiLoadMetadataFromXmlBuf (msParam_t *targetObj, msParam_t *xmlObj, ruleExecInfo_t *rei)
 *
 * \brief   This microservice parses an XML iRODS file to extract metadata tags.
 *
 * \module XML
 *
 * \since pre-2.1
 *
 * \author  Antoine de Torcy
 * \date    2008/05/29
 *
 * \usage See clients/icommands/test/rules3.0/
 *  
 * \param[in] targetObj - Optional - a msParam of type DataObjInp_MS_T or STR_MS_T
 * \param[in] xmlObj - a msParam of type DataObjInp_MS_T or STR_MS_T
 * \param[in,out] rei - The RuleExecInfo structure that is automatically
 *    handled by the rule engine. The user does not include rei as a
 *    parameter in the rule invocation.
 *
 * \DolVarDependence None
 * \DolVarModified None
 * \iCatAttrDependence None
 * \iCatAttrModified None
 * \sideeffect None
 *
 * \return integer
 * \retval 0 on success
 * \pre None
 * \post None
 * \sa None
**/

extern "C" {
#if defined(LIBXML_XPATH_ENABLED) && \
    defined(LIBXML_SAX1_ENABLED) && \
    defined(LIBXML_OUTPUT_ENABLED)

int 
msiLoadMetadataFromXmlBuf(msParam_t *targetObj, msParam_t *xmlParam, ruleExecInfo_t *rei) 
{
	/* for parsing msParams and to open iRODS objects */
//	dataObjInp_t xmlDataObjInp, *myXmlDataObjInp;
	dataObjInp_t targetObjInp, *myTargetObjInp;
//	 int xmlObjID;


	/* for getting size of objects to read from */
	rodsObjStat_t *rodsObjStatOut = NULL;

	/* for reading from iRODS objects */
	openedDataObjInp_t openedDataObjInp;
	bytesBuf_t *xmlBuf;

	/* misc. to avoid repeating rei->rsComm */
	rsComm_t *rsComm;

	/* for xml parsing */
	xmlDocPtr doc;

	/* for XPath evaluation */
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;
	xmlChar xpathExpr[] = "//AVU";
	xmlNodeSetPtr nodes;
	int avuNbr, i;
	
	/* for new AVU creation */
	modAVUMetadataInp_t modAVUMetadataInp;
	int max_attr_len = 2700;
	char attrStr[max_attr_len];
	rodsLong_t coll_id;



	/*********************************  USUAL INIT PROCEDURE **********************************/
	
	/* For testing mode when used with irule --test */
	RE_TEST_MACRO ("    Calling msiLoadMetadataFromXmlBuf")


	/* Sanity checks */
	if (rei == NULL || rei->rsComm == NULL)
	{
		rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: input rei or rsComm is NULL.");
		return (SYS_INTERNAL_NULL_INPUT_ERR);
	}

	rsComm = rei->rsComm;


	
	/********************************** RETRIEVE INPUT PARAMS **************************************/

	/* Get path of target object */
	rei->status = parseMspForDataObjInp (targetObj, &targetObjInp, &myTargetObjInp, 0);
	if (rei->status < 0)
	{
		rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: input targetObj error. status = %d", rei->status);
		return (rei->status);
	}


	/* Get path of XML document */
	/*
	rei->status = parseMspForDataObjInp (xmlObj, &xmlDataObjInp, &myXmlDataObjInp, 0);
	if (rei->status < 0)
	{
		rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: input xmlObj error. status = %d", rei->status);
		return (rei->status);
	}

	*/

	if ( strcmp( xmlParam->type, BUF_LEN_MS_T ) != 0 ||
	     xmlParam->inpOutBuf == NULL ) {
	     return USER_PARAM_TYPE_ERR;
	}


	xmlBuf = ( bytesBuf_t * ) xmlParam->inpOutBuf;

	/******************************** OPEN AND READ FROM XML OBJECT ********************************/

	/* Open XML file */
	/*
	if ((xmlObjID = rsDataObjOpen(rsComm, &xmlDataObjInp)) < 0) 
	{
		rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: Cannot open XML data object. status = %d", xmlObjID);
		return (xmlObjID);
	}

	*/
	/* Get size of XML file */
	/*
	rei->status = rsObjStat (rsComm, &xmlDataObjInp, &rodsObjStatOut);
	if (rei->status < 0 || !rodsObjStatOut)
	{
		rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: Cannot stat XML data object. status = %d", rei->status);
		return (rei->status);
	}
	*/

	/* xmlBuf init */
	/* memory for xmlBuf->buf is allocated in rsFileRead() */
	/*
	xmlBuf = (bytesBuf_t *) malloc (sizeof (bytesBuf_t));
	memset (xmlBuf, 0, sizeof (bytesBuf_t));
	*/

	/* Read XML file */
	/*
	memset (&openedDataObjInp, 0, sizeof (openedDataObjInp_t));
	openedDataObjInp.l1descInx = xmlObjID;
	openedDataObjInp.len = (int)rodsObjStatOut->objSize;

	rei->status = rsDataObjRead (rsComm, &openedDataObjInp, xmlBuf);
	*/	
	/* Make sure that the result is null terminated */
	/*
	if (strlen((char*)xmlBuf->buf) > (size_t)openedDataObjInp.len)
	{
		((char*)xmlBuf->buf)[openedDataObjInp.len-1]='\0';
	}
	*/

	/* Close XML file */
	/*
	rei->status = rsDataObjClose (rsComm, &openedDataObjInp);
	*/
	/* cleanup */
	/*
	freeRodsObjStat (rodsObjStatOut);
	*/


	/******************************** PARSE XML DOCUMENT ********************************/

	xmlSubstituteEntitiesDefault(1);
	xmlLoadExtDtdDefaultValue = 1;


	/* Parse xmlBuf.buf into an xmlDocPtr */
	doc = xmlParseDoc((xmlChar*)xmlBuf->buf);


	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if(xpathCtx == NULL) 
	{
		rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: Unable to create new XPath context.");
		xmlFreeDoc(doc); 
		return(-1);
	}


	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
	if(xpathObj == NULL) 
	{
		rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: Unable to evaluate XPath expression \"%s\".", xpathExpr);
		xmlXPathFreeContext(xpathCtx); 
		xmlFreeDoc(doc); 
		return(-1);
	}

	/* How many AVU nodes did we get? */
	if ((nodes = xpathObj->nodesetval) != NULL)
	{
		avuNbr = nodes->nodeNr;
	}
	else 
	{
		avuNbr = 0;
	}



	/******************************** CREATE AVU TRIPLETS ********************************/

	/* Add a new AVU for each node. It's ok to process the nodes in forward order since we're not modifying them */
	for(i = 0; i < avuNbr; i++) 
	{
		if (nodes->nodeTab[i])
		{
//			/* temporary: Add index number to avoid duplicating attribute names */
//			memset(attrStr, '\0', MAX_NAME_LEN);
//			snprintf(attrStr, MAX_NAME_LEN - 1, "%04d: %s", i+1, (char*)xmlNodeGetContent(getChildNodeByName(nodes->nodeTab[i], "Attribute")) );

			/* Truncate if needed. No prefix. */
			memset(attrStr, '\0', max_attr_len);
			snprintf(attrStr, max_attr_len - 1, "%s", (char*)xmlNodeGetContent(getChildNodeByName(nodes->nodeTab[i], "Attribute")) );

			/* init modAVU input */
			memset (&modAVUMetadataInp, 0, sizeof(modAVUMetadataInp_t));
			modAVUMetadataInp.arg0 = "add";

			/* Use target object if one was provided, otherwise look for it in the XML doc */
			if (myTargetObjInp->objPath != NULL && strlen(myTargetObjInp->objPath) > 0)
			{
				modAVUMetadataInp.arg2 = myTargetObjInp->objPath;
			}
			else
			{
				modAVUMetadataInp.arg2 = xmlURIUnescapeString((char*)xmlNodeGetContent(getChildNodeByName(nodes->nodeTab[i], "Target")),
						MAX_NAME_LEN, NULL);
			}

			/* check if data or collection */
			if (isColl(rsComm, modAVUMetadataInp.arg2, &coll_id) < 0) {
				modAVUMetadataInp.arg1 = "-d";
			} else {
				modAVUMetadataInp.arg1 = "-C";
			}

			modAVUMetadataInp.arg3 = attrStr;
			modAVUMetadataInp.arg4 = (char*)xmlNodeGetContent(getChildNodeByName(nodes->nodeTab[i], "Value"));
			modAVUMetadataInp.arg5 = (char*)xmlNodeGetContent(getChildNodeByName(nodes->nodeTab[i], "Unit"));
			
			/* invoke rsModAVUMetadata() */
			rei->status = rsModAVUMetadata (rsComm, &modAVUMetadataInp);
			if(rei->status < 0)
			{
				rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: rsModAVUMetadata error for %s, status=%d",
						modAVUMetadataInp.arg2, rei->status);
			}
		}
	}



	/************************************** WE'RE DONE **************************************/

	/* cleanup of all xml parsing stuff */
	xmlFreeDoc(doc);
        xmlCleanupParser();

	return (rei->status);
}


/* Default stub if no XPath support */
#else
/**
 * \fn msiLoadMetadataFromXmlBuf (msParam_t *targetObj, msParam_t *xmlObj, ruleExecInfo_t *rei)
**/
int 
msiLoadMetadataFromXmlBuf(msParam_t *targetObj, msParam_t *xmlObj, ruleExecInfo_t *rei) 
{
	rodsLog (LOG_ERROR, "msiLoadMetadataFromXmlBuf: XPath support not compiled in.");
	return (SYS_NOT_SUPPORTED);
}
#endif

irods::ms_table_entry * plugin_factory() {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
    msvc->add_operation("msiLoadMetadataFromXmlBuf","msiLoadMetadataFromXmlBuf");
    return msvc;
}

}
