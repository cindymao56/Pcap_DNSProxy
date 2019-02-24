﻿// This code is part of Pcap_DNSProxy
// Pcap_DNSProxy, a local DNS server based on packet capturing
// Copyright (C) 2012-2019 Chengr28
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include "Constructor.h"

//GlobalStatus class constructor
GlobalStatus::GlobalStatus(
	void)
{
//Class constructor
	memset(this, 0, sizeof(GLOBAL_STATUS));
	try {
	//Running status
		IsNeedExit = new std::atomic<bool>();
		RandomEngine = new std::default_random_engine();
/* Temporary Disabled
		FileList_Config = new std::vector<FILE_DATA>();
		FileList_IPFilter = new std::vector<FILE_DATA>();
		FileList_Hosts = new std::vector<FILE_DATA>();
		DNSCacheList = new std::list<DNS_CACHE_DATA>();
		DNSCacheIndexMap = new std::unordered_multimap<std::string, std::list<DNS_CACHE_DATA>::iterator>();
*/

	//Mutex status
		ScreenLock = new std::mutex();
/*
		DNSCacheLock = new std::mutex();
*/

	//Path list and file list status
		Path_Global_WCS = new std::vector<std::wstring>();
		Path_ErrorLog_WCS = new std::wstring();
		FileList_Hosts_WCS = new std::vector<std::wstring>();
		FileList_IPFilter_WCS = new std::vector<std::wstring>();
	#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		Path_Global_MBS = new std::vector<std::string>();
		Path_ErrorLog_MBS = new std::string();
		FileList_Hosts_MBS = new std::vector<std::string>();
		FileList_IPFilter_MBS = new std::vector<std::string>();
	#endif
	}
	catch (std::bad_alloc &)
	{
		delete IsNeedExit;
		delete RandomEngine;
/* Temporary Disabled
		delete FileList_Config;
		delete FileList_IPFilter;
		delete FileList_Hosts;
		delete DNSCacheList;
		delete DNSCacheIndexMap;
*/
		delete ScreenLock;
/* Temporary Disabled
		delete DNSCacheLock;
*/
		delete Path_Global_WCS;
		delete Path_ErrorLog_WCS;
		delete FileList_Hosts_WCS;
		delete FileList_IPFilter_WCS;
	#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		delete Path_Global_MBS;
		delete Path_ErrorLog_MBS;
		delete FileList_Hosts_MBS;
		delete FileList_IPFilter_MBS;
	#endif
		IsNeedExit = nullptr;
		RandomEngine = nullptr;
/* Temporary Disabled
		FileList_Config = nullptr;
		FileList_IPFilter = nullptr;
		FileList_Hosts = nullptr;
		DNSCacheList = nullptr;
		DNSCacheIndexMap = nullptr;
*/
		ScreenLock = nullptr;
/* Temporary Disabled
		DNSCacheLock = nullptr;
*/
		Path_Global_WCS = nullptr;
		Path_ErrorLog_WCS = nullptr;
		FileList_Hosts_WCS = nullptr;
		FileList_IPFilter_WCS = nullptr;
	#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		Path_Global_MBS = nullptr;
		Path_ErrorLog_MBS = nullptr;
		FileList_Hosts_MBS = nullptr;
		FileList_IPFilter_MBS = nullptr;
	#endif

	//Exit process.
		exit(EXIT_FAILURE);
//		return;
	}

//Set GlobalStatus class to default.
	SetDefault_GlobalStatus(this);

//Library initialization
//Temporary Disabled


	return;
}

//Set GlobalStatus class to default
void SetDefault_GlobalStatus(
	GLOBAL_STATUS * const GlobalRunningStatusParameter)
{
#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX))
	GlobalRunningStatusParameter->IsDaemon = true;
#endif
	GlobalRunningStatusParameter->IsNeedExit->store(false);

//Temporary Disabled


	return;
}

//ConfigurationTable class constructor
ConfigurationTable::ConfigurationTable(
	void)
{
//Library initialization
#if defined(ENABLE_LIBSODIUM)
//Libsodium
	if (sodium_init() == LIBSODIUM_ERROR)
	{
		exit(EXIT_FAILURE);
//		return;
	}
#endif

//Class constructor
	memset(this, 0, sizeof(CONFIGURATION_TABLE));
	try {
	//Temporary Disabled

	}
	catch (std::bad_alloc &)
	{
	//Temporary Disabled


	//Exit process.
		exit(EXIT_FAILURE);
//		return;
	}

//Set ConfigurationTable class to default.
	SetDefault_ConfigurationTable(this);

	return;
}

//ConfigurationTable class SetToAlternate function
void ConfigurationTable::SetToAlternate(
	void)
{


	return;
}

//Set ConfigurationTable class to default
void SetDefault_ConfigurationTable(
	CONFIGURATION_TABLE * const ConfigurationParameter)
{
//[Base] block
	ConfigurationParameter->FileRefreshTime = DEFAULT_FILE_REFRESH_TIME;

//[Log] block
	ConfigurationParameter->PrintLogLevel = DEFAULT_LOG_LEVEL;
	ConfigurationParameter->LogMaxSize = DEFAULT_LOG_READING_MAXSIZE;

//Temporary Disabled


	return;
}

//SocketValueTable class SocketValueInit function
bool SocketValueTable::SocketValueInit(
	const uint16_t SocketNetwork, 
	const uint16_t SocketType, 
	const uint16_t SocketTransport, 
	const uint16_t SocketPort, 
	const void * const SocketAddress, 
	ssize_t * const ErrorCode)
{
//Initialization
	SOCKET_DATA ValueItem;
	memset(&ValueItem, 0, sizeof(ValueItem));

//IPv6
	if (SocketNetwork == AF_INET6)
	{
		ValueItem.Socket = socket(AF_INET6, SocketType, SocketTransport);
		if (!SetSocketAttribute(ValueItem.Socket, SOCKET_SETTING_TYPE::INVALID_CHECK, true, nullptr))
		{
			if (ErrorCode != nullptr)
				*ErrorCode = WSAGetLastError();
		}
		else {
			ValueItem.AddrLen = sizeof(sockaddr_in6);
			ValueItem.SockAddr.ss_family = AF_INET6;
			reinterpret_cast<sockaddr_in6 *>(&ValueItem.SockAddr)->sin6_port = hton16(SocketPort);
			if (SocketAddress != nullptr)
				memcpy_s(&reinterpret_cast<sockaddr_in6 *>(&ValueItem.SockAddr)->sin6_addr, sizeof(reinterpret_cast<const sockaddr_in6 *>(&ValueItem.SockAddr)->sin6_addr), SocketAddress, sizeof(in6_addr));

		//Add item to list.
			ValueSet.push_back(ValueItem);
			return true;
		}
	}
//IPv4
	else if (SocketNetwork == AF_INET)
	{
		ValueItem.Socket = socket(AF_INET, SocketType, SocketTransport);
		if (!SetSocketAttribute(ValueItem.Socket, SOCKET_SETTING_TYPE::INVALID_CHECK, true, nullptr))
		{
			if (ErrorCode != nullptr)
				*ErrorCode = WSAGetLastError();
		}
		else {
			ValueItem.AddrLen = sizeof(sockaddr_in);
			ValueItem.SockAddr.ss_family = AF_INET;
			reinterpret_cast<sockaddr_in *>(&ValueItem.SockAddr)->sin_port = hton16(SocketPort);
			if (SocketAddress != nullptr)
				memcpy_s(&reinterpret_cast<sockaddr_in *>(&ValueItem.SockAddr)->sin_addr, sizeof(reinterpret_cast<const sockaddr_in *>(&ValueItem.SockAddr)->sin_addr), SocketAddress, sizeof(in_addr));

		//Add item to list.
			ValueSet.push_back(ValueItem);
			return true;
		}
	}

	return false;
}

//SocketValueTable class ClearAllSocket function
void SocketValueTable::ClearAllSocket(
	const bool IsPrintError)
{
//Close all sockets and clear list.
	if (!ValueSet.empty())
	{
		for (auto &SocketItem:ValueSet)
			SetSocketAttribute(SocketItem.Socket, SOCKET_SETTING_TYPE::CLOSE, IsPrintError, nullptr);

		ValueSet.clear();
		ValueSet.shrink_to_fit();
	}

	return;
}

//DNS_PacketTable class constructor
DNS_PacketTable::DNS_PacketTable(
	void)
{
	IsNeedWriteEDNS = false;
	PacketID = 0;
	PacketFlags = 0;
	StorageSize = 0;
	StorageLength = 0;

	return;
}

//DNS_PacketTable class IsEmpty function
bool DNS_PacketTable::IsEmpty(
	void)
{
//Query
	if (!PacketQuestion.empty())
		return false;

//Response
	if (!PacketAnswer.empty() || !PacketAuthority.empty() || !PacketAdditional.empty())
		return false;

//DNS Cookies request
	if (PacketQuestion.empty() && !PacketExtension.empty())
		return false;

	return true;
}

//DNS_PacketTable class IsVaild function
bool DNS_PacketTable::IsVaild(
	const bool IsResponse)
{
//Empty check
	if (IsEmpty())
		return false;

//Flag check
	if ((IsResponse && (PacketFlags & DNS_FLAG_GET_BIT_RESPONSE) == 0) || 
		(PacketFlags & DNS_FLAG_GET_BIT_Z) != 0)
			return false;

//Question check
	if (PacketQuestion.size() > UINT16_NUM_ONE)
		return false;

//Store full RCode.
	uint16_t WholeRCode = 0;
	if (!PacketExtension.empty() && PacketExtension.front().RecordExtendRCode != 0)
	{
		WholeRCode = PacketExtension.front().RecordExtendRCode;
		WholeRCode <<= DNS_RCODE_SIZE;
		WholeRCode &= PacketFlags & DNS_FLAG_GET_BIT_RCODE;
	}
	else {
		WholeRCode = PacketFlags & DNS_FLAG_GET_BIT_RCODE;
	}

//Resource record check
	if (IsResponse && (
	//Must not any non-Question resource records when RCode is No Error and not Truncated.
		((PacketFlags & DNS_FLAG_GET_BIT_TC) == 0 && WholeRCode == DNS_RCODE_NOERROR && PacketAnswer.empty() && PacketAuthority.empty() && PacketAdditional.empty()) || 
	//Response are not authoritative when there are no Authoritative Nameservers Records and Additional resource records.
//		((PacketFlags & DNS_FLAG_GET_BIT_AA) != 0 && PacketAuthority.empty() && PacketAdditional.empty()) || 
	//Do query recursively bit must be set when RCode is No Error and there are Answers resource records.
		((PacketFlags & DNS_FLAG_GET_BIT_RD) == 0 && WholeRCode == DNS_RCODE_NOERROR && PacketAnswer.empty())))
			return false;

//Strict resource record TTL check when enforce strict RFC 2181(https://tools.ietf.org/html/rfc2181) compliance
	std::vector<const DNS_PACKET_RECORD *> ComplianceRecordList;
	for (const auto &RecordItem:PacketAnswer)
		ComplianceRecordList.push_back(&RecordItem);
	for (const auto &RecordItem:PacketAuthority)
		ComplianceRecordList.push_back(&RecordItem);
	for (const auto &RecordItem:PacketAdditional)
		ComplianceRecordList.push_back(&RecordItem);
	for (size_t Index = 0;Index < ComplianceRecordList.size();++Index)
	{
	//Empty pointer.
		if (ComplianceRecordList.at(Index) == nullptr)
			continue;

	//TTL in resource records must less than 2 ^ 31(2147483647).
		if ((ComplianceRecordList.at(Index)->RecordTTL & DNS_RECORD_TTL_GET_BIT_HIGHEST) != 0)
			return false;

	//Multiple RRs of the same type and for the same domain with different TTLs.
	//This will cause filter to reject DNS answers with incorrect timestamp settings.
		if (!ComplianceRecordList.at(Index)->RecordName.empty())
		{
			for (size_t InnerIndex = Index + 1U;InnerIndex < ComplianceRecordList.size();++InnerIndex)
			{
			//Empty pointer or record name.
				if (ComplianceRecordList.at(InnerIndex) == nullptr)
					continue;
				else if (ComplianceRecordList.at(InnerIndex)->RecordName.empty())
					continue;
			
			//It is however possible for most record types to exist with the same label, class and type, but with different data.
				if (ComplianceRecordList.at(Index)->RecordName == ComplianceRecordList.at(InnerIndex)->RecordName && 
					ComplianceRecordList.at(Index)->RecordType == ComplianceRecordList.at(InnerIndex)->RecordType && 
					ComplianceRecordList.at(Index)->RecordClass == ComplianceRecordList.at(InnerIndex)->RecordClass)
				{
					if (ComplianceRecordList.at(Index)->RecordTTL != ComplianceRecordList.at(InnerIndex)->RecordTTL)
						return false;
					else 
						ComplianceRecordList.at(InnerIndex) = nullptr;
				}
			}
		}
	}

	return true;
}

//DNS_PacketTable class WritePacketToTable function
bool DNS_PacketTable::WritePacketToTable(
	const uint8_t * const PacketBuffer, 
	const size_t PacketLength)
{
	size_t BufferOffset = 0;

//Header
	if (BufferOffset + sizeof(dns_hdr) >= PacketLength)
	{
		return false;
	}
	else {
		PacketID = ntoh16(reinterpret_cast<const dns_hdr *>(PacketBuffer + BufferOffset)->ID);
		PacketFlags = ntoh16(reinterpret_cast<const dns_hdr *>(PacketBuffer + BufferOffset)->Flags);
		BufferOffset += sizeof(dns_hdr);
	}
	const auto HeaderPointer = reinterpret_cast<const dns_hdr *>(PacketBuffer);

//Question
	if (ntoh16(HeaderPointer->Question) > UINT16_NUM_ONE)
	{
		return false;
	}
	else if (ntoh16(HeaderPointer->Question) == UINT16_NUM_ONE)
	{
		DNS_PACKET_QUESTION WritePacketQuestion;
		if (GetDomainNameLength(PacketBuffer + BufferOffset, PacketLength - BufferOffset) == 0 || 
			BufferOffset + GetDomainNameLength(PacketBuffer + BufferOffset, PacketLength - BufferOffset) + sizeof(dns_qry) > PacketLength)
		{
			return false;
		}
		else if (!PacketQueryToString(PacketBuffer, PacketLength, PacketBuffer + BufferOffset, GetDomainNameLength(PacketBuffer + BufferOffset, PacketLength - BufferOffset), nullptr, WritePacketQuestion.QuestionName))
		{
			return false;
		}
		else if (!WritePacketQuestion.QuestionName.empty() && !CheckDomainAcceptable(WritePacketQuestion.QuestionName))
		{
			return false;
		}
		else {
			BufferOffset += GetDomainNameLength(PacketBuffer + BufferOffset, PacketLength - BufferOffset);
			WritePacketQuestion.QuestionType = ntoh16(reinterpret_cast<const dns_qry *>(PacketBuffer + BufferOffset)->Type);
			WritePacketQuestion.QuestionClass = ntoh16(reinterpret_cast<const dns_qry *>(PacketBuffer + BufferOffset)->Classes);
			BufferOffset += sizeof(dns_qry);
		}

	//Add Question to table.
		PacketQuestion.push_back(std::move(WritePacketQuestion));
	}

//Resource record(Answer, Authority, Additional, Extension)
	auto IsAfterExtension = false;
	for (size_t Index = 0;Index < static_cast<const size_t>(ntoh16(HeaderPointer->Answer)) + static_cast<const size_t>(ntoh16(HeaderPointer->Authority)) + static_cast<const size_t>(ntoh16(HeaderPointer->Additional));++Index)
	{
	//Record length check
		if (GetDomainNameLength(PacketBuffer + BufferOffset, PacketLength - BufferOffset) == 0 || 
			BufferOffset + GetDomainNameLength(PacketBuffer + BufferOffset, PacketLength - BufferOffset) + sizeof(dns_record_standard) > PacketLength)
				return false;

	//Name
		DNS_PACKET_RECORD WritePacketRecord;
		if (!PacketQueryToString(PacketBuffer, PacketLength, PacketBuffer + BufferOffset, GetDomainNameLength(PacketBuffer + BufferOffset, PacketLength - BufferOffset), nullptr, WritePacketRecord.RecordName))
			return false;
		else if (!WritePacketRecord.RecordName.empty() && !CheckDomainAcceptable(WritePacketRecord.RecordName))
			return false;
		else 
			BufferOffset += GetDomainNameLength(PacketBuffer + BufferOffset, PacketLength - BufferOffset);

	//Record data length check
		if (BufferOffset + sizeof(dns_record_standard) + ntoh16(reinterpret_cast<const dns_record_standard *>(PacketBuffer + BufferOffset)->Length) > PacketLength)
			return false;

	//Extension record
		if (ntoh16(reinterpret_cast<const dns_record_standard *>(PacketBuffer + BufferOffset)->Type) == DNS_TYPE_EDNS)
		{
		//A message can only include one Extension record, and Name in Extension record is ROOT.
			if (IsAfterExtension || !WritePacketRecord.RecordName.empty())
				return false;
			else 
				IsAfterExtension = true;

		//Record selection
			DNS_PACKET_EXTENSION_RECORD WritePacketExtension;
			if (Index < static_cast<const size_t>(ntoh16(HeaderPointer->Answer)))
				WritePacketExtension.RecordSelection = DNS_RECORD_SECTION::ANSWER;
			else if (Index < static_cast<const size_t>(ntoh16(HeaderPointer->Answer)) + static_cast<const size_t>(ntoh16(HeaderPointer->Authority)))
				WritePacketExtension.RecordSelection = DNS_RECORD_SECTION::AUTHORITY;
			else 
				WritePacketExtension.RecordSelection = DNS_RECORD_SECTION::ADDITIONAL;

		//Resource in record
			WritePacketExtension.RecordType = ntoh16(reinterpret_cast<const dns_record_edns *>(PacketBuffer + BufferOffset)->Type);
			WritePacketExtension.RecordPayloadSize = ntoh16(reinterpret_cast<const dns_record_edns *>(PacketBuffer + BufferOffset)->UDP_PayloadSize);
			WritePacketExtension.RecordExtendRCode = reinterpret_cast<const dns_record_edns *>(PacketBuffer + BufferOffset)->Extended_RCode;
			WritePacketExtension.RecordVersion = reinterpret_cast<const dns_record_edns *>(PacketBuffer + BufferOffset)->Version;
			WritePacketExtension.RecordFlags = ntoh16(reinterpret_cast<const dns_record_edns *>(PacketBuffer + BufferOffset)->Flags);
			WritePacketExtension.RecordLength = ntoh16(reinterpret_cast<const dns_record_edns *>(PacketBuffer + BufferOffset)->DataLength);
			BufferOffset += sizeof(dns_record_edns);

		//Prepare storage.
			if (WritePacketExtension.RecordLength > 0)
				ExpandStorage(PacketLength);

		//Option in record
			DNS_PACKET_EXTENSION_OPTION WritePacketExtensionOption;
			for (size_t OptionOffset = BufferOffset;OptionOffset < BufferOffset + WritePacketExtension.RecordLength;)
			{
			//Option length check
				if (OptionOffset + sizeof(edns_data_option) > BufferOffset + WritePacketExtension.RecordLength)
					return false;

			//Option in record
				WritePacketExtensionOption.OptionCode = ntoh16(reinterpret_cast<const edns_data_option *>(BufferOffset + OptionOffset)->Code);
				WritePacketExtensionOption.OptionLength = ntoh16(reinterpret_cast<const edns_data_option *>(BufferOffset + OptionOffset)->Length);
				OptionOffset += sizeof(edns_data_option);

			//Option data length check
				if (OptionOffset + WritePacketExtensionOption.OptionLength > BufferOffset + WritePacketExtension.RecordLength)
					return false;

			//Copy option data to storage.
				memcpy_s(StorageBuffer.get() + StorageLength, StorageSize - StorageLength, PacketBuffer + OptionOffset, WritePacketExtensionOption.OptionLength);
				WritePacketExtensionOption.OptionDataOffset = StorageLength;
				StorageLength += WritePacketExtensionOption.OptionLength;
				OptionOffset += WritePacketExtensionOption.OptionLength;

			//Add option to table.
				WritePacketExtension.RecordData.push_back(WritePacketExtensionOption);
			}
			BufferOffset += WritePacketExtension.RecordLength;

		//Add Question to table.
			PacketExtension.push_back(std::move(WritePacketExtension));
		}
	//Other record
		else {
		//Resource in record
			WritePacketRecord.IsAfterExtension = IsAfterExtension;
			WritePacketRecord.RecordType = ntoh16(reinterpret_cast<const dns_record_standard *>(PacketBuffer + BufferOffset)->Type);
			WritePacketRecord.RecordClass = ntoh16(reinterpret_cast<const dns_record_standard *>(PacketBuffer + BufferOffset)->Classes);
			WritePacketRecord.RecordTTL = ntoh32(reinterpret_cast<const dns_record_standard *>(PacketBuffer + BufferOffset)->TTL);
			WritePacketRecord.RecordLength = ntoh16(reinterpret_cast<const dns_record_standard *>(PacketBuffer + BufferOffset)->Length);
			BufferOffset += sizeof(dns_record_standard);

		//Copy record data to storage.
			if (WritePacketRecord.RecordLength > 0)
			{
			//Prepare storage.
				ExpandStorage(PacketLength);

			//Copy record data.
				memcpy_s(StorageBuffer.get() + StorageLength, StorageSize - StorageLength, PacketBuffer + BufferOffset, WritePacketRecord.RecordLength);
				WritePacketRecord.RecordDataOffset = StorageLength;
				StorageLength += WritePacketRecord.RecordLength;
				BufferOffset += WritePacketRecord.RecordLength;
			}

		//Add Question to table.
			if (Index < static_cast<const size_t>(ntoh16(HeaderPointer->Answer)))
				PacketAnswer.push_back(std::move(WritePacketRecord));
			else if (Index < static_cast<const size_t>(ntoh16(HeaderPointer->Answer)) + static_cast<const size_t>(ntoh16(HeaderPointer->Authority)))
				PacketAuthority.push_back(std::move(WritePacketRecord));
			else 
				PacketAdditional.push_back(std::move(WritePacketRecord));
		}
	}

	return true;
}

//DNS_PacketTable class GetTableBufferLength function
size_t DNS_PacketTable::GetTableBufferLength(
	bool IsTCP)
{
//Header
	size_t Length = 0;
	if (IsTCP)
		Length = sizeof(uint16_t) + sizeof(dns_hdr);
	else 
		Length = sizeof(dns_hdr);

//Question
	for (const auto &QuestionItem:PacketQuestion)
		Length += GetStringToDomainSize(QuestionItem.QuestionName.length()) + sizeof(dns_qry);

//Resource record(Answer, Authority, Additional)
	for (const auto &RecordItem:PacketAnswer)
		Length += GetStringToDomainSize(RecordItem.RecordName.length()) + sizeof(dns_record_standard) + RecordItem.RecordLength;
	for (const auto &RecordItem:PacketAuthority)
		Length += GetStringToDomainSize(RecordItem.RecordName.length()) + sizeof(dns_record_standard) + RecordItem.RecordLength;
	for (const auto &RecordItem:PacketAdditional)
		Length += GetStringToDomainSize(RecordItem.RecordName.length()) + sizeof(dns_record_standard) + RecordItem.RecordLength;

//Resource record(Extension)
	for (const auto &RecordItem:PacketExtension)
		Length += sizeof(uint8_t) + sizeof(dns_record_edns) + RecordItem.RecordLength;

	return Length;
}

//DNS_PacketTable class GetTableHeader function
void DNS_PacketTable::GetTableHeader(
	uint16_t &GetPacketID, 
	uint16_t &GetPacketFlags)
{
	GetPacketID = PacketID;
	GetPacketFlags = PacketFlags;

	return;
}

//DNS_PacketTable class UpdateTableHeader function
void DNS_PacketTable::UpdateTableHeader(
	const uint16_t UpdatePacketID, 
	const uint16_t UpdatePacketFlags)
{
	PacketID = UpdatePacketID;
	PacketFlags = UpdatePacketFlags;

	return;
}

//DNS_PacketTable class GetTableQuestion function
void DNS_PacketTable::GetTableQuestion(
	std::string &QuestionName, 
	uint16_t &QuestionType, 
	uint16_t &QuestionClass)
{
	QuestionName = PacketQuestion.back().QuestionName;
	QuestionType = PacketQuestion.back().QuestionType;
	QuestionClass = PacketQuestion.back().QuestionClass;

	return;
}

//DNS_PacketTable class UpdateTableQuestion function
bool DNS_PacketTable::UpdateTableQuestion(
	const uint8_t * const QuestionName, 
	const uint16_t QuestionType, 
	const uint16_t QuestionClass)
{
//Question name length check
	if (QuestionName != nullptr && strnlen_s(reinterpret_cast<const char *>(QuestionName), DOMAIN_WHOLE_MAXSIZE) >= DOMAIN_WHOLE_MAXSIZE)
		return false;

//Make packet Question item.
	DNS_PACKET_QUESTION UpdatePacketQuestion;
	if (QuestionName != nullptr)
	{
		UpdatePacketQuestion.QuestionName = reinterpret_cast<const char *>(QuestionName);
		if (!CheckDomainAcceptable(UpdatePacketQuestion.QuestionName))
		{
			UpdatePacketQuestion.QuestionName.clear();
			UpdatePacketQuestion.QuestionName.shrink_to_fit();

			return false;
		}
	}
	else {
		UpdatePacketQuestion.QuestionType = QuestionType;
		UpdatePacketQuestion.QuestionClass = QuestionClass;
	}

//Clear and update Question list.
	PacketQuestion.clear();
	PacketQuestion.shrink_to_fit();
	PacketQuestion.push_back(std::move(UpdatePacketQuestion));

	return true;
}

//DNS_PacketTable class AddRecordToTable function
void DNS_PacketTable::AddRecordToTable(
	const DNS_RECORD_SECTION RecordSection, 
	DNS_PACKET_RECORD RecordHeader, 
	const uint8_t * const RecordData)
{
//Copy record data to storage.
	if (RecordHeader.RecordLength > 0)
	{
	//Prepare storage.
		ExpandStorage(StorageLength + RecordHeader.RecordLength);

	//Copy record data.
		memcpy_s(StorageBuffer.get() + StorageLength, StorageSize - StorageLength, RecordData, RecordHeader.RecordLength);
		RecordHeader.RecordDataOffset = StorageLength;
		StorageLength += RecordHeader.RecordLength;
	}

//Find Extension position.
	if (!PacketExtension.empty())
	{
		if ((RecordSection == DNS_RECORD_SECTION::ADDITIONAL && PacketExtension.back().RecordSelection != DNS_RECORD_SECTION::ADDITIONAL) || 
			(RecordSection == DNS_RECORD_SECTION::AUTHORITY && PacketExtension.back().RecordSelection == DNS_RECORD_SECTION::ANSWER))
				RecordHeader.IsAfterExtension = true;
		else 
			RecordHeader.IsAfterExtension = false;
	}
	else {
		RecordHeader.IsAfterExtension = false;
	}

//Add record to table.
	if (RecordSection == DNS_RECORD_SECTION::ANSWER)
	{
		PacketAnswer.push_back(RecordHeader);
	}
	else if (RecordSection == DNS_RECORD_SECTION::AUTHORITY)
	{
		PacketAuthority.push_back(RecordHeader);
	}
	else {
	//Some cases need for the TSIG or SIG(0) RRs to be the last in the additional section whenever they are present.
		if (!PacketAdditional.empty())
		{
		//Pop all SIG and TSIG records back from Additional.
			std::deque<DNS_PACKET_RECORD> SigRecordList;
			while (!PacketAdditional.empty() && (PacketAdditional.back().RecordType == DNS_TYPE_SIG || PacketAdditional.back().RecordType == DNS_TYPE_TSIG))
			{
				SigRecordList.push_front(PacketAdditional.back());
				PacketAdditional.pop_back();
			}

		//Add record to Additional.
			PacketAdditional.push_back(RecordHeader);

		//Push all SIG and TSIG records back to Additional.
			for (const auto &RecordItem:SigRecordList)
				PacketAdditional.push_back(RecordItem);
		}
	//Add record to Additional.
		else {
			PacketAdditional.push_back(RecordHeader);
		}
	}

	return;
}

//DNS_PacketTable class RemoveRecordFromTable function
bool DNS_PacketTable::RemoveRecordFromTable(
	const DNS_RECORD_SECTION RecordSection, 
	const size_t RecordIndex)
{
//Remove from list.
	switch (RecordSection)
	{
		case DNS_RECORD_SECTION::ANSWER:
		{
			if (PacketAnswer.empty())
				break;
			if (RecordIndex >= PacketAnswer.size())
				return false;
			else 
				PacketAnswer.erase(PacketAnswer.begin() + RecordIndex);
		}break;
		case DNS_RECORD_SECTION::AUTHORITY:
		{
			if (PacketAuthority.empty())
				break;
			if (RecordIndex >= PacketAuthority.size())
				return false;
			else 
				PacketAuthority.erase(PacketAuthority.begin() + RecordIndex);
		}break;
		case DNS_RECORD_SECTION::ADDITIONAL:
		{
			if (PacketAdditional.empty())
				break;
			if (RecordIndex >= PacketAdditional.size())
				return false;
			else 
				PacketAdditional.erase(PacketAdditional.begin() + RecordIndex);
		}break;
		default:
		{
			return false;
		}
	}

//Rebuild storage.
	return RebuildStorage();
}

//DNS_PacketTable class GetTableExtension function
bool DNS_PacketTable::GetTableExtension(
	DNS_PACKET_EXTENSION_RECORD &ExtensionHeader, 
	uint8_t * const OptionData, 
	const size_t OptionSize)
{
//Empty Extension list
	if (PacketExtension.empty())
		return true;

//Copy option data to table.
	if (PacketExtension.back().RecordLength > 0)
	{
	//Option buffer check
		if (OptionData == nullptr || OptionSize < PacketExtension.back().RecordLength)
		{
			ExtensionHeader.RecordLength = PacketExtension.back().RecordLength;
			return false;
		}

	//Option in record
		DNS_PACKET_EXTENSION_OPTION GetTableExtensionOption;
		size_t OptionBufferLength = 0;
		for (const auto &OptionItem:PacketExtension.back().RecordData)
		{
		//Option buffer length check
			if (OptionBufferLength + OptionItem.OptionLength > OptionSize)
			{
				memset(OptionData, 0, OptionSize);
				return false;
			}

		//Copy option data to buffer.
			memcpy_s(OptionData + OptionBufferLength, OptionSize - OptionBufferLength, StorageBuffer.get() + OptionItem.OptionDataOffset, OptionItem.OptionLength);
			GetTableExtensionOption.OptionDataOffset = OptionBufferLength;
			OptionBufferLength += OptionItem.OptionLength;
				
		//Option in record
			GetTableExtensionOption.OptionCode = OptionItem.OptionCode;
			GetTableExtensionOption.OptionLength = OptionItem.OptionLength;

		//Add option to Extension.
			ExtensionHeader.RecordData.push_back(GetTableExtensionOption);

		}

	//Resource in record
		ExtensionHeader.RecordLength = PacketExtension.back().RecordLength;
	}
//Empty option list.
	else {
		ExtensionHeader.RecordLength = 0;
		ExtensionHeader.RecordData.clear();
		ExtensionHeader.RecordData.shrink_to_fit();
	}

//Resource in record
	ExtensionHeader.RecordSelection = PacketExtension.back().RecordSelection;
	ExtensionHeader.RecordType = PacketExtension.back().RecordType;
	ExtensionHeader.RecordPayloadSize = PacketExtension.back().RecordPayloadSize;
	ExtensionHeader.RecordExtendRCode = PacketExtension.back().RecordExtendRCode;
	ExtensionHeader.RecordVersion = PacketExtension.back().RecordVersion;
	ExtensionHeader.RecordFlags = PacketExtension.back().RecordFlags;

	return true;
}

//DNS_PacketTable class UpdateTableExtension function
void DNS_PacketTable::UpdateTableExtension(
	DNS_PACKET_EXTENSION_RECORD ExtensionHeader, 
	const uint8_t * const OptionData)
{
//Add a new Extension to table.
	if (PacketExtension.empty())
	{
	//Option in record
		if (ExtensionHeader.RecordLength > 0)
		{
		//Prepare storage.
			ExpandStorage(StorageLength + ExtensionHeader.RecordLength);

		//Copy record data to storage.
			for (auto &OptionItem:ExtensionHeader.RecordData)
			{
				memcpy_s(StorageBuffer.get() + StorageLength, StorageSize - StorageLength, OptionData + OptionItem.OptionDataOffset, OptionItem.OptionLength);
				OptionItem.OptionDataOffset = StorageLength;
				StorageLength += OptionItem.OptionLength;
			}
		}

	//Mark Extension position.
		if (ExtensionHeader.RecordSelection == DNS_RECORD_SECTION::ANSWER)
		{
			for (auto &RecordItem:PacketAuthority)
				RecordItem.IsAfterExtension = true;
			for (auto &RecordItem:PacketAdditional)
				RecordItem.IsAfterExtension = true;
		}
		else if (ExtensionHeader.RecordSelection == DNS_RECORD_SECTION::AUTHORITY)
		{
			for (auto &RecordItem:PacketAdditional)
				RecordItem.IsAfterExtension = true;
		}
		else {
		//Some cases need for the TSIG or SIG(0) RRs to be the last in the additional section whenever they are present.
			for (auto &RecordItem:PacketAdditional)
			{
				if (RecordItem.RecordType == DNS_TYPE_SIG || RecordItem.RecordType == DNS_TYPE_TSIG)
					RecordItem.IsAfterExtension = true;
			}
		}

	//Add Extension to table.
		PacketExtension.push_back(ExtensionHeader);
	}
//Modify exist Extension.
	else {
	//Resource in record
//		PacketExtension.back().RecordSelection = ExtensionHeader.RecordSelection;
//		PacketExtension.back().RecordType = ExtensionHeader.RecordType;
		PacketExtension.back().RecordPayloadSize = ExtensionHeader.RecordPayloadSize;
		PacketExtension.back().RecordExtendRCode = ExtensionHeader.RecordExtendRCode;
		PacketExtension.back().RecordVersion = ExtensionHeader.RecordVersion;
		PacketExtension.back().RecordFlags = ExtensionHeader.RecordFlags;

	//Option in record
		if (ExtensionHeader.RecordLength > 0)
		{
		//Prepare storage.
			ExpandStorage(StorageLength + ExtensionHeader.RecordLength);

		//Copy record data to storage.
			for (auto &OptionItem:ExtensionHeader.RecordData)
			{
				memcpy_s(StorageBuffer.get() + StorageLength, StorageSize - StorageLength, OptionData + OptionItem.OptionDataOffset, OptionItem.OptionLength);
				OptionItem.OptionDataOffset = StorageLength;
				StorageLength += OptionItem.OptionLength;
			}
			PacketExtension.back().RecordData = ExtensionHeader.RecordData;
			PacketExtension.back().RecordData.shrink_to_fit();
			PacketExtension.back().RecordLength = ExtensionHeader.RecordLength;
		}
	//Clear and update Option list.
		else {
			PacketExtension.back().RecordData.clear();
			PacketExtension.back().RecordData.shrink_to_fit();
			PacketExtension.back().RecordLength = 0;
		}

	//Rebuild storage.
		RebuildStorage();
	}

	return;
}

//DNS_PacketTable class WriteTableToPacket function
size_t DNS_PacketTable::WriteTableToPacket(
	uint8_t * const TargetBuffer, 
	const size_t TargetSize, 
	const bool IsTCP)
{
//Target buffer size check
	if (GetTableBufferLength(IsTCP) > TargetSize)
		return 0;

//Header length value
	uint16_t *HeaderLengthValue = nullptr;
	size_t TargetLength = 0;
	if (IsTCP)
	{
		HeaderLengthValue = reinterpret_cast<uint16_t *>(TargetBuffer + TargetLength);
		TargetLength += sizeof(uint16_t);
	}

//Header
	reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->ID = hton16(PacketID);
	reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Flags = hton16(PacketFlags);
	reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Question = hton16(static_cast<uint16_t>(PacketQuestion.size()));
	reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Answer = hton16(static_cast<uint16_t>(PacketAnswer.size()));
	reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Authority = hton16(static_cast<uint16_t>(PacketAuthority.size()));
	reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Additional = hton16(static_cast<uint16_t>(PacketAdditional.size()));
	if (!PacketExtension.empty())
	{
		if (PacketExtension.back().RecordSelection == DNS_RECORD_SECTION::ANSWER)
			reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Answer = hton16(ntoh16(reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Answer) + 1U);
		else if (PacketExtension.back().RecordSelection == DNS_RECORD_SECTION::AUTHORITY)
			reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Authority = hton16(ntoh16(reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Authority) + 1U);
		else 
			reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Additional = hton16(ntoh16(reinterpret_cast<dns_hdr *>(TargetBuffer + TargetLength)->Additional) + 1U);
	}
	TargetLength += sizeof(dns_hdr);

//Question


	return 0;
}

//DNS_PacketTable class ClearTable function
void DNS_PacketTable::ClearTable(
	void)
{
	IsNeedWriteEDNS = false;
	PacketID = 0;
	PacketFlags = 0;
	PacketQuestion.clear();
	PacketQuestion.shrink_to_fit();
	PacketAnswer.clear();
	PacketAnswer.shrink_to_fit();
	PacketAuthority.clear();
	PacketAuthority.shrink_to_fit();
	PacketAdditional.clear();
	PacketAdditional.shrink_to_fit();
	PacketExtension.clear();
	PacketExtension.shrink_to_fit();
	StorageBuffer.reset();
	StorageSize = 0;
	StorageLength = 0;

	return;
}

//DNS_PacketTable class ExpandStorage function
void DNS_PacketTable::ExpandStorage(
	const size_t TargetSize)
{
//Caculate storage size.
	size_t ExpandSize = 0;
	if (TargetSize == 0)
	{
		if (StorageSize > 0)
			return;
		else if (GlobalConfiguration.LargeBufferSize > 0)
			ExpandSize = GlobalConfiguration.LargeBufferSize;
		else 
			ExpandSize = DEFAULT_LARGE_BUFFER_SIZE;
	}
	else if (TargetSize <= StorageSize)
	{
		return;
	}
	else {
		ExpandSize = TargetSize;
	}

//Expand storage size.
	auto ExpandBuffer = std::make_unique<uint8_t[]>(ExpandSize + MEMORY_RESERVED_BYTES);
	memset(ExpandBuffer.get(), 0, ExpandSize + MEMORY_RESERVED_BYTES);
	if (StorageBuffer && StorageSize > 0 && StorageLength > 0)
		memcpy_s(ExpandBuffer.get(), ExpandSize, StorageBuffer.get(), StorageLength);
	std::swap(ExpandBuffer, StorageBuffer);
	StorageSize = ExpandSize;

	return;
}

//DNS_PacketTable class RebuildStorage function
bool DNS_PacketTable::RebuildStorage(
	void)
{
//Prepare storage.
	auto RebuildBuffer = std::make_unique<uint8_t[]>(StorageSize + MEMORY_RESERVED_BYTES);
	memset(RebuildBuffer.get(), 0, StorageSize + MEMORY_RESERVED_BYTES);
	size_t RebuildLength = 0, RebuildSize = StorageSize;

//Copy data from the old one.
	std::deque<size_t> DataOffsetList;
	for (const auto &RecordItem:PacketAnswer)
	{
		if (RecordItem.RecordLength > 0)
		{
			if (RebuildLength + RecordItem.RecordLength > RebuildSize)
			{
				return false;
			}
			else {
				memcpy_s(RebuildBuffer.get() + RebuildLength, RebuildSize - RebuildLength, StorageBuffer.get() + RecordItem.RecordDataOffset, RecordItem.RecordLength);
				DataOffsetList.push_back(RebuildLength);
				RebuildLength += RecordItem.RecordLength;
			}
		}
	}
	for (const auto &RecordItem:PacketAuthority)
	{
		if (RecordItem.RecordLength > 0)
		{
			if (RebuildLength + RecordItem.RecordLength > RebuildSize)
			{
				return false;
			}
			else {
				memcpy_s(RebuildBuffer.get() + RebuildLength, RebuildSize - RebuildLength, StorageBuffer.get() + RecordItem.RecordDataOffset, RecordItem.RecordLength);
				DataOffsetList.push_back(RebuildLength);
				RebuildLength += RecordItem.RecordLength;
			}
		}
	}
	for (const auto &RecordItem:PacketAdditional)
	{
		if (RecordItem.RecordLength > 0)
		{
			if (RebuildLength + RecordItem.RecordLength > RebuildSize)
			{
				return false;
			}
			else {
				memcpy_s(RebuildBuffer.get() + RebuildLength, RebuildSize - RebuildLength, StorageBuffer.get() + RecordItem.RecordDataOffset, RecordItem.RecordLength);
				DataOffsetList.push_back(RebuildLength);
				RebuildLength += RecordItem.RecordLength;
			}
		}
	}
	for (const auto &RecordItem:PacketExtension)
	{
		for (const auto &OptionItem:RecordItem.RecordData)
		{
			if (OptionItem.OptionLength > 0)
			{
				if (RebuildLength + OptionItem.OptionLength > RebuildSize)
				{
					return false;
				}
				else {
					memcpy_s(RebuildBuffer.get() + RebuildLength, RebuildSize - RebuildLength, StorageBuffer.get() + OptionItem.OptionDataOffset, OptionItem.OptionLength);
					DataOffsetList.push_back(RebuildLength);
					RebuildLength += OptionItem.OptionLength;
				}
			}
		}
	}

//Comfirm to swap storage, locate data offset.
	for (auto &RecordItem:PacketAnswer)
	{
		if (DataOffsetList.empty())
		{
			return false;
		}
		else {
			RecordItem.RecordDataOffset = DataOffsetList.front();
			DataOffsetList.pop_front();
		}
	}
	for (auto &RecordItem:PacketAuthority)
	{
		if (DataOffsetList.empty())
		{
			return false;
		}
		else {
			RecordItem.RecordDataOffset = DataOffsetList.front();
			DataOffsetList.pop_front();
		}
	}
	for (auto &RecordItem:PacketAdditional)
	{
		if (DataOffsetList.empty())
		{
			return false;
		}
		else {
			RecordItem.RecordDataOffset = DataOffsetList.front();
			DataOffsetList.pop_front();
		}
	}
	for (auto &RecordItem:PacketExtension)
	{
		for (auto &OptionItem:RecordItem.RecordData)
		{
			if (DataOffsetList.empty())
			{
				return false;
			}
			else {
				OptionItem.OptionDataOffset = DataOffsetList.front();
				DataOffsetList.pop_front();
			}
		}
	}

//Swap storage.
	std::swap(RebuildBuffer, StorageBuffer);
	StorageSize = RebuildSize;
	StorageLength = RebuildLength;

	return true;
}
