/*
 *  Author: bwilliams
 *  Created: April 6, 2012
 *
 *  Copyright (C) 2012-2016 VMware, Inc.  All rights reserved. -- VMware Confidential
 *
 *  This code was generated by the script "build/dev/codeGen/genCppXml". Please
 *  speak to Brian W. before modifying it by hand.
 *
 */

#include "stdafx.h"

#include "Doc/DocXml/SchemaTypesXml/CmdlMetadataXml.h"
#include "Doc/DocXml/SchemaTypesXml/CmdlUnionXml.h"
#include "Doc/DocXml/SchemaTypesXml/DataClassSubInstanceXml.h"

#include "Doc/SchemaTypesDoc/CCmdlMetadataDoc.h"
#include "Doc/SchemaTypesDoc/CCmdlUnionDoc.h"
#include "Doc/SchemaTypesDoc/CDataClassInstanceDoc.h"
#include "Doc/SchemaTypesDoc/CDataClassPropertyDoc.h"
#include "Doc/SchemaTypesDoc/CDataClassSubInstanceDoc.h"
#include "Xml/XmlUtils/CXmlElement.h"
#include "Doc/DocXml/SchemaTypesXml/DataClassInstanceXml.h"
#include "Doc/DocXml/SchemaTypesXml/DataClassPropertyXml.h"

using namespace Caf;

void DataClassInstanceXml::add(
	const SmartPtrCDataClassInstanceDoc dataClassInstanceDoc,
	const SmartPtrCXmlElement thisXml) {
	CAF_CM_STATIC_FUNC_VALIDATE("DataClassInstanceXml", "add");

	CAF_CM_ENTER {
		CAF_CM_VALIDATE_SMARTPTR(dataClassInstanceDoc);
		CAF_CM_VALIDATE_SMARTPTR(thisXml);

		const std::string namespaceValVal = dataClassInstanceDoc->getNamespaceVal();
		CAF_CM_VALIDATE_STRING(namespaceValVal);
		thisXml->addAttribute("namespace", namespaceValVal);

		const std::string nameVal = dataClassInstanceDoc->getName();
		CAF_CM_VALIDATE_STRING(nameVal);
		thisXml->addAttribute("name", nameVal);

		const std::string versionVal = dataClassInstanceDoc->getVersion();
		CAF_CM_VALIDATE_STRING(versionVal);
		thisXml->addAttribute("version", versionVal);

		const std::deque<SmartPtrCCmdlMetadataDoc> cmdlMetadataVal =
			dataClassInstanceDoc->getCmdlMetadataCollection();
		if (! cmdlMetadataVal.empty()) {
			for (TConstIterator<std::deque<SmartPtrCCmdlMetadataDoc> > cmdlMetadataIter(cmdlMetadataVal);
				cmdlMetadataIter; cmdlMetadataIter++) {
				const SmartPtrCXmlElement cmdlMetadataXml =
					thisXml->createAndAddElement("cmdlMetadata");
				CmdlMetadataXml::add(*cmdlMetadataIter, cmdlMetadataXml);
			}
		}

		const std::deque<SmartPtrCDataClassPropertyDoc> propertyVal =
			dataClassInstanceDoc->getPropertyCollection();
		if (! propertyVal.empty()) {
			for (TConstIterator<std::deque<SmartPtrCDataClassPropertyDoc> > propertyIter(propertyVal);
				propertyIter; propertyIter++) {
				const SmartPtrCXmlElement propertyXml =
					thisXml->createAndAddElement("property");
				DataClassPropertyXml::add(*propertyIter, propertyXml);
			}
		}

		const std::deque<SmartPtrCDataClassSubInstanceDoc> instancePropertyVal =
			dataClassInstanceDoc->getInstancePropertyCollection();
		if (! instancePropertyVal.empty()) {
			for (TConstIterator<std::deque<SmartPtrCDataClassSubInstanceDoc> > instancePropertyIter(instancePropertyVal);
				instancePropertyIter; instancePropertyIter++) {
				const SmartPtrCXmlElement instancePropertyXml =
					thisXml->createAndAddElement("instanceProperty");
				DataClassSubInstanceXml::add(*instancePropertyIter, instancePropertyXml);
			}
		}

		const SmartPtrCCmdlUnionDoc cmdlUnionVal =
			dataClassInstanceDoc->getCmdlUnion();
		if (! cmdlUnionVal.IsNull()) {
			const SmartPtrCXmlElement cmdlUnionXml =
				thisXml->createAndAddElement("cmdlUnion");
			CmdlUnionXml::add(cmdlUnionVal, cmdlUnionXml);
		}
	}
	CAF_CM_EXIT;
}

SmartPtrCDataClassInstanceDoc DataClassInstanceXml::parse(
	const SmartPtrCXmlElement thisXml) {
	CAF_CM_STATIC_FUNC_VALIDATE("DataClassInstanceXml", "parse");

	SmartPtrCDataClassInstanceDoc dataClassInstanceDoc;

	CAF_CM_ENTER {
		CAF_CM_VALIDATE_SMARTPTR(thisXml);

		const std::string namespaceValStrVal =
			thisXml->findRequiredAttribute("namespace");
		const std::string namespaceValVal = namespaceValStrVal;

		const std::string nameStrVal =
			thisXml->findRequiredAttribute("name");
		const std::string nameVal = nameStrVal;

		const std::string versionStrVal =
			thisXml->findRequiredAttribute("version");
		const std::string versionVal = versionStrVal;

		const CXmlElement::SmartPtrCElementCollection cmdlMetadataChildrenXml =
			thisXml->findOptionalChildren("cmdlMetadata");

		std::deque<SmartPtrCCmdlMetadataDoc> cmdlMetadataVal;
		if (! cmdlMetadataChildrenXml.IsNull() && ! cmdlMetadataChildrenXml->empty()) {
			for (TConstIterator<CXmlElement::CElementCollection> cmdlMetadataXmlIter(*cmdlMetadataChildrenXml);
				cmdlMetadataXmlIter; cmdlMetadataXmlIter++) {
				const SmartPtrCXmlElement cmdlMetadataXml = cmdlMetadataXmlIter->second;
				const SmartPtrCCmdlMetadataDoc cmdlMetadataDoc =
					CmdlMetadataXml::parse(cmdlMetadataXml);
				cmdlMetadataVal.push_back(cmdlMetadataDoc);
			}
		}

		const CXmlElement::SmartPtrCElementCollection propertyChildrenXml =
			thisXml->findOptionalChildren("property");

		std::deque<SmartPtrCDataClassPropertyDoc> propertyVal;
		if (! propertyChildrenXml.IsNull() && ! propertyChildrenXml->empty()) {
			for (TConstIterator<CXmlElement::CElementCollection> propertyXmlIter(*propertyChildrenXml);
				propertyXmlIter; propertyXmlIter++) {
				const SmartPtrCXmlElement propertyXml = propertyXmlIter->second;
				const SmartPtrCDataClassPropertyDoc propertyDoc =
					DataClassPropertyXml::parse(propertyXml);
				propertyVal.push_back(propertyDoc);
			}
		}

		const CXmlElement::SmartPtrCElementCollection instancePropertyChildrenXml =
			thisXml->findOptionalChildren("instanceProperty");

		std::deque<SmartPtrCDataClassSubInstanceDoc> instancePropertyVal;
		if (! instancePropertyChildrenXml.IsNull() && ! instancePropertyChildrenXml->empty()) {
			for (TConstIterator<CXmlElement::CElementCollection> instancePropertyXmlIter(*instancePropertyChildrenXml);
				instancePropertyXmlIter; instancePropertyXmlIter++) {
				const SmartPtrCXmlElement instancePropertyXml = instancePropertyXmlIter->second;
				const SmartPtrCDataClassSubInstanceDoc instancePropertyDoc =
					DataClassSubInstanceXml::parse(instancePropertyXml);
				instancePropertyVal.push_back(instancePropertyDoc);
			}
		}

		const SmartPtrCXmlElement cmdlUnionXml =
			thisXml->findOptionalChild("cmdlUnion");

		SmartPtrCCmdlUnionDoc cmdlUnionVal;
		if (! cmdlUnionXml.IsNull()) {
			cmdlUnionVal = CmdlUnionXml::parse(cmdlUnionXml);
		}

		dataClassInstanceDoc.CreateInstance();
		dataClassInstanceDoc->initialize(
			namespaceValVal,
			nameVal,
			versionVal,
			cmdlMetadataVal,
			propertyVal,
			instancePropertyVal,
			cmdlUnionVal);
	}
	CAF_CM_EXIT;

	return dataClassInstanceDoc;
}
