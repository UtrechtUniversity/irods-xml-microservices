/*
 * libmsiStripAVUs.cpp
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
 * \fn msiStripAVUs(msParam_t *target, msParam_t *options, msParam_t *status, ruleExecInfo_t *rei)
 *
 * \brief Strips an object of its metadata
 *
 * \module ERA
 *
 * \since 2.4.x
 *
 * \author  Antoine de Torcy
 * \date    2010-11-17
 *
 * \note  Only data objects are supported for now.
 *
 * \usage See clients/icommands/test/rules3.0/
 *
 * \param[in] target - A STR_MS_T with a data/collection path or user/resource name
 * \param[in] options - Optional - a STR_MS_T that contains one of more options in
 *      the format keyWd1=value1++++keyWd2=value2++++keyWd3=value3...
 *      The type of target will be specified here (i.e data, collection, user, resource, etc...).
 *      Valid keywords will be added as more types are supported.
 * \param[out] status - An INT_MS_T containing the operation status.
 * \param[in,out] rei - The RuleExecInfo structure that is automatically
 *    handled by the rule engine. The user does not include rei as a
 *    parameter in the rule invocation.
 *
 * \DolVarDependence None
 * \DolVarModified None
 * \iCatAttrDependence None
 * \iCatAttrModified Some
 * \sideeffect None
 *
 * \return integer
 * \retval 0 on success
 * \pre None
 * \post None
 * \sa None
**/
int
msiStripAVUs(msParam_t *target, msParam_t *options, msParam_t *status, ruleExecInfo_t *rei)
{
	/* for parsing target object */
	dataObjInp_t dataObjInpCache, *dataObjInp;

	/* for AVU deletion */
	modAVUMetadataInp_t modAVUMetadataInp;

	/* For testing mode when used with irule --test */
	RE_TEST_MACRO ("    Calling msiStripAVUs")


	/* init modAVU input */
	memset (&modAVUMetadataInp, 0, sizeof(modAVUMetadataInp_t));
	modAVUMetadataInp.arg0 = "rmw";		// remove with wildcards
	modAVUMetadataInp.arg3 = "%";   	// any attribute
	modAVUMetadataInp.arg4 = "%";		// any value
	modAVUMetadataInp.arg5 = "%";		// any unit

	/* Check for type in options and parse target accordingly */
	rei->status = parseMspForDataObjInp (target, &dataObjInpCache, &dataObjInp, 0);
	if (rei->status < 0)
	{
		rodsLog (LOG_ERROR, "msiStripAVUs: input target error. status = %d", rei->status);
		return (rei->status);
	}

	/* Set object type to data and get its path */
	modAVUMetadataInp.arg1 = "-d";
	modAVUMetadataInp.arg2 = dataObjInp->objPath;

	/* Call API */
	rei->status = rsModAVUMetadata (rei->rsComm, &modAVUMetadataInp);

	/* Avoid throwing an error if object did not have any metadata */
	if (rei->status == CAT_SUCCESS_BUT_WITH_NO_INFO)
	{
		rei->status = 0;
	}

	/* Return operation status */
	fillIntInMsParam (status, rei->status);
	return (rei->status);
}
irods::ms_table_entry * plugin_factory() {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(3);
    msvc->add_operation("msiStripAVUs","msiStripAVUs");
    return msvc;
}
}
