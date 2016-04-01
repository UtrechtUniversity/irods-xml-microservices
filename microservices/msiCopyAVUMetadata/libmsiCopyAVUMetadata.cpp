/*
 * libmsiCopyAVUMetadata.cpp
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
 * \fn msiCopyAVUMetadata(msParam_t *inpParam1, msParam_t *inpParam2, msParam_t *outParam, ruleExecInfo_t *rei)
 *
 * \brief Copies metadata triplets from one iRODS object to another
 *
 * \author  Antoine de Torcy
 * \date    2007-09-26
 *
 * \note This microservice copies metadata triplets from one iRODS object to another.
 *    The source and destination can be a data object or a collection, independently.
 *
 * \usage See clients/icommands/test/rules3.0/
 *
 * \param[in] inpParam1 - A STR_MS_T with the irods path of the source object.
 * \param[in] inpParam2 - A STR_MS_T with the irods path of the destination object.
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
msiCopyAVUMetadata(msParam_t *inpParam1, msParam_t *inpParam2, msParam_t *outParam, ruleExecInfo_t *rei)
{
	rsComm_t *rsComm;

	/* For testing mode when used with irule --test */
	RE_TEST_MACRO ("    Calling msiCopyAVUMetadata")


	/* Sanity check */
	if (rei == NULL || rei->rsComm == NULL) {
		rodsLog (LOG_ERROR, "msiCopyAVUMetadata: input rei or rsComm is NULL");
		return (SYS_INTERNAL_NULL_INPUT_ERR);
	}
	
	rsComm = rei->rsComm;


	/* Check for proper input */
	if ((parseMspForStr(inpParam1) == NULL)) {
		rodsLog (LOG_ERROR, "msiCopyAVUMetadata: input inpParam1 is NULL");
		return (USER__NULL_INPUT_ERR);
	}

	if ((parseMspForStr (inpParam2) == NULL)) {
		rodsLog (LOG_ERROR, "msiCopyAVUMetadata: input inpParam2 is NULL");
		return (USER__NULL_INPUT_ERR);
	}


	/* Call copyAVUMetadata() */
	rei->status = copyAVUMetadata((char*)inpParam2->inOutStruct, (char*)inpParam1->inOutStruct, rsComm);

	/* Return operation status */
	fillIntInMsParam (outParam, rei->status);
	return (rei->status);
}
irods::ms_table_entry * plugin_factory() {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
    msvc->add_operation("msiCopyAVUMetadata","msiCopyAVUMetadata");
    return msvc;
}
}
