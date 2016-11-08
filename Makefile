MS_PLUGIN_CORE = microservices/core
msiLoadMetadataFromDataObj	= microservices/msiLoadMetadataFromDataObj
msiLoadMetadataFromXml	= microservices/msiLoadMetadataFromXml
msiXMLDocSchemaValidate	= microservices/msiXMLDocSchemaValidate
msiSetDataType = microservices/msiSetDataType
msiCopyAVUMetadata = microservices/msiCopyAVUMetadata
msiStripAVUs = microservices/msiStripAVUs
msiGetDataObjAIP = microservices/msiGetDataObjAIP

MAKEFLAGS += --no-print-directory

LIBS = $(MS_PLUGIN_CORE) $(msiLoadMetadataFromDataObj) $(msiLoadMetadataFromXml) $(msiXMLDocSchemaValidate) $(msiSetDataType) $(msiCopyAVUMetadata) $(msiStripAVUs) $(msiGetDataObjAIP)

.PHONY: all $(LIBS) clean
all: $(LIBS)

$(LIBS):
	@$(MAKE) -C $@;
	
$(msiLoadMetadataFromDataObj): $(MS_PLUGIN_CORE)

$(msiLoadMetadataFromXml): $(MS_PLUGIN_CORE)

$(msiXMLDocSchemaValidate): $(MS_PLUGIN_CORE)

$(msiSetDataType): $(MS_PLUGIN_CORE)

$(msiCopyAVUMetadata): $(MS_PLUGIN_CORE)

$(msiStripAVUs): $(MS_PLUGIN_CORE)

$(msiGetDataObjAIP): $(MS_PLUGIN_CORE)

clean:
	@$(MAKE) -C $(MS_PLUGIN_CORE) clean;
	@$(MAKE) -C $(msiLoadMetadataFromDataObj) clean;
	@$(MAKE) -C $(msiLoadMetadataFromXml) clean;
	@$(MAKE) -C $(msiXMLDocSchemaValidate) clean;
	@$(MAKE) -C $(msiSetDataType) clean;
	@$(MAKE) -C $(msiCopyAVUMetadata) clean;
	@$(MAKE) -C $(msiStripAVUs) clean;
	@$(MAKE) -C $(msiGetDataObjAIP) clean;

