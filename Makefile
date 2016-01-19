MS_PLUGIN_CURL_CORE = microservices/core
msiLoadMetadataFromDataObj	= microservices/msiLoadMetadataFromDataObj
msiLoadMetadataFromXml	= microservices/msiLoadMetadataFromXml

MAKEFLAGS += --no-print-directory

LIBS = $(MS_PLUGIN_CURL_CORE) $(msiLoadMetadataFromDataObj) $(msiLoadMetadataFromXml)

.PHONY: all $(LIBS) clean
all: $(LIBS)

$(LIBS):
	@$(MAKE) -C $@;
	
$(msiLoadMetadataFromDataObj): $(MS_PLUGIN_CURL_CORE)

$(msiLoadMetadataFromXml): $(MS_PLUGIN_CURL_CORE)

clean:
	@$(MAKE) -C $(MS_PLUGIN_CURL_CORE) clean;
	@$(MAKE) -C $(msiLoadMetadataFromDataObj) clean;
	@$(MAKE) -C $(msiLoadMetadataFromXml) clean;
