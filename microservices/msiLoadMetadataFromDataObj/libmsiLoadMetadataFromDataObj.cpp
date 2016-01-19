/*
 * libmsiLoadMetadataFromDataObj.cpp
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
 * \fn msiLoadMetadataFromDataObj(msParam_t *inpParam, msParam_t *outParam, ruleExecInfo_t *rei)
 *
 * \brief Parses an iRODS object for new metadata AVUs
 *
 * \module ERA
 *
 * \since pre-2.1
 *
 * \author  Antoine de Torcy
 * \date    2007-09-26
 *
 * \note The metadata file is read as a text file and must contain one AVU per line.
 *
 * \code
 *    See example file snippet below:
 *
 *    C-/tempZone/home/antoine/testColl |CollAttr000 |CollVal00 |CollUnits00
 *    /tempZone/home/antoine/testColl/file00.txt |myAttr000 |myVal001
 *    /tempZone/home/antoine/testColl/file00.txt |myAttr001 |myVal001 |myUnits001
 *    # Some comments
 *    /tempZone/home/antoine/testColl/file01.txt |myAttr011 |myVal011
 *    /tempZone/home/antoine/testColl/file01.txt |myAttr010 |myVal011
 *
 *    Collection paths must start with C-.
 *
 *    Units in AVUs are optional.
 *
 *    Lines starting with # won't be parsed.
 *  \endcode
 *
 * \usage See clients/icommands/test/rules3.0/
 *
 * \param[in] inpParam - A DataObjInp_MS_T or a STR_MS_T with the irods path of the metadata file.
 * \param[out] outParam - An INT_MS_T containing the status.
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
msiLoadMetadataFromDataObj(msParam_t *inpParam, msParam_t *outParam, ruleExecInfo_t *rei)
{
	dataObjInp_t dataObjInp, *myDataObjInp;
	rsComm_t *rsComm;

	
	/* For testing mode when used with irule --test */
	RE_TEST_MACRO ("    Calling msiLoadMetadataFromDataObj")


	/* Sanity checks */
	if (rei == NULL || rei->rsComm == NULL) {
		rodsLog (LOG_ERROR, "msiLoadMetadataFromDataObj: input rei or rsComm is NULL.");
		return (SYS_INTERNAL_NULL_INPUT_ERR);
	}

	rsComm = rei->rsComm;	
	
	/* parse inpParam1 */
	rei->status = parseMspForDataObjInp (inpParam, &dataObjInp, &myDataObjInp, 0);
	if (rei->status < 0) {
		rodsLogAndErrorMsg (LOG_ERROR, &rsComm->rError, rei->status,
			"msiLoadMetadataFromDataObj: input inpParam1 error. status = %d", rei->status);
		return (rei->status);
	}

	
	/* call loadMetadataFromDataObj() */
	rei->status = loadMetadataFromDataObj(myDataObjInp, rsComm);
	
	/* send status out to outParam */
	fillIntInMsParam(outParam, rei->status);

	return (rei->status);
}
irods::ms_table_entry * plugin_factory() {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
    msvc->add_operation("msiLoadMetadataFromDataObj","msiLoadMetadataFromDataObj");
    return msvc;
}
}