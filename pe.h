#ifndef PE_H
#define PE_H

#include <windows.h>
typedef IMAGE_RESOURCE_DIRECTORY        pe_ResDir;
typedef IMAGE_RESOURCE_DIRECTORY_ENTRY  pe_ResDirEntry;
typedef IMAGE_RESOURCE_DATA_ENTRY       pe_ResDataEntry;
typedef IMAGE_DOS_HEADER                pe_DosHeader;
typedef IMAGE_NT_HEADERS                pe_NtHeader;
typedef IMAGE_FILE_HEADER               pe_FileHeader;
typedef IMAGE_OPTIONAL_HEADER           pe_OptionHeader;
typedef IMAGE_DATA_DIRECTORY            pe_DataDirectroy;
typedef IMAGE_SECTION_HEADER            pe_SectionHeader;


class PE
{
    void*   m_pData;
    bool    m_bIsFileAlign;
public:
    PE(void* pFileData,bool bIsFileAlign=true);

    pe_DosHeader*     dosHeader();
    pe_NtHeader*      ntHeader();
    pe_FileHeader*    fileHeader();
    pe_OptionHeader*  optionHeader();
    pe_DataDirectroy* dataDirectory();
    pe_SectionHeader* sectionHeader();

    DWORD RVAToOfs( DWORD dwRVA);
};

#endif // PE_H
