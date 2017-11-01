irods_microservice_plugins_loadmeta
=======================

Port of metadata related microservices from community iRODS

Originally pulled from DICE-UNC repository.

Adapted by Paul Frederiks for Utrecht University:

- Added msiXsltApply port
- Added modified msiLoadMetadataFromXML which loads XML directly from a input buffer, msiLoadMetadataFromXMLBuf
- Detects CentOS 7 and adds this to package name
- New dependency: libxslt-devel(rpm) or libxslt1-dev(deb) for msiXsltApply
- Fixed bugs in msiXsltApplu
