/*
 * libmsiSetDataType.cpp
 */



// =-=-=-=-=-=-=-
#include "eraUtil.h"
#include "msParam.hpp"
#include "reGlobalsExtern.hpp"
#include "irods_ms_plugin.hpp"
#include "irods_server_api_call.hpp"
#include "microservice.hpp"


extern "C"{
/**
 * \fn msiSetDataType(msParam_t *inpParam1, msParam_t *inpParam2, msParam_t *inpParam3, msParam_t *outParam, ruleExecInfo_t *rei)
 *
 * \brief Sets the data_type_name attribute of a data object.
 *
 * \module ERA
 *
 * \since pre-2.1
 *
 * \author  Antoine de Torcy
 * \date    2007
 *
 * \usage See clients/icommands/test/rules3.0/
 *
 * \param[in] inpParam1 - A STR_MS_T containing the object ID
 * \param[in] inpParam2 - A STR_MS_T containing the object's iRODS path
 * \param[in] inpParam3 - A STR_MS_T containing the data type to be set
 * \param[out] outParam - an INT_MS_T containing the status
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
int
msiSetDataType(msParam_t *inpParam1, msParam_t *inpParam2, msParam_t *inpParam3, msParam_t *outParam, ruleExecInfo_t *rei)
{
	char *dataTypeStr, *dataIdStr;		/* for parsing of input params */
	dataObjInp_t dataObjInp, *myDataObjInp;
	
	modDataObjMeta_t modDataObjMetaInp;	/* for rsModDataObjMeta() */
	dataObjInfo_t dataObjInfo;		/* for rsModDataObjMeta() */
	keyValPair_t regParam;			/* for rsModDataObjMeta() */
	
	
	RE_TEST_MACRO ("    Calling msiSetDataType")
	
	if (rei == NULL || rei->rsComm == NULL) {
		rodsLog (LOG_ERROR, "msiSetDataType: input rei or rsComm is NULL.");
		return (SYS_INTERNAL_NULL_INPUT_ERR);
	}


	/* zero out input for rsModDataObjMeta */
	memset (&dataObjInfo, 0, sizeof (dataObjInfo_t));
	memset (&regParam, 0, sizeof (keyValPair_t));


	/* parse inpParam1: data object ID */
	if ((dataIdStr = parseMspForStr(inpParam1)) != NULL)  {
		dataObjInfo.dataId = (rodsLong_t) atoll(dataIdStr);
	}


	/* parse inpParam2: data object path */
	if ( parseMspForDataObjInp (inpParam2, &dataObjInp, &myDataObjInp, 0) >= 0) {
		strncpy(dataObjInfo.objPath, dataObjInp.objPath, MAX_NAME_LEN);
	}


	/* make sure to have at least data ID or path */
	if (!(dataIdStr || dataObjInfo.objPath)) {
		rodsLog (LOG_ERROR, "msiSetDataType: No data object ID or path provided.");
		return (USER__NULL_INPUT_ERR);
	}


	/* parse inpParam3: data type string */
	if ((dataTypeStr = parseMspForStr (inpParam3)) == NULL) {
		rodsLog (LOG_ERROR, "msiSetDataType: parseMspForStr error for param 3.");
		return (USER__NULL_INPUT_ERR);
	}
	addKeyVal (&regParam, DATA_TYPE_KW, dataTypeStr);

	
	/* call rsModDataObjMeta() */
	modDataObjMetaInp.dataObjInfo = &dataObjInfo;
	modDataObjMetaInp.regParam = &regParam;
	rei->status = rsModDataObjMeta (rei->rsComm, &modDataObjMetaInp);


	/* failure? */
	if (rei->status < 0) {
		if (dataObjInfo.objPath) {
			rodsLog (LOG_ERROR, "msiSetDataType: rsModDataObjMeta failed for object %s, status = %d", dataObjInfo.objPath, rei->status);
		}
		else {
			rodsLog (LOG_ERROR, "msiSetDataType: rsModDataObjMeta failed for object ID %d, status = %d", dataObjInfo.dataId, rei->status);
		}
	}


	/* return operation status through outParam */
	fillIntInMsParam (outParam, rei->status);
	
	return (rei->status);

}
irods::ms_table_entry * plugin_factory() {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
    msvc->add_operation("msiSetDataType","msiSetDataType");
    return msvc;
}
}
