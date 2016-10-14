#include "pe.h"



/** 求一个值经过一定粒度对齐后的值 */
DWORD ToAligentSize(DWORD nSize , DWORD nAligent)
{
    // 超出了多少倍的内存对齐,超出多少倍,就有多少倍内存对齐单位 ;
    // 零头是否超出内存对齐,超出则是一个内存对齐单位
    if(nSize%nAligent != 0)
        return (nSize / nAligent + 1)*nAligent;
    return nSize;
}




PE::PE(void *pFileData, bool bIsFileAlign)
    :m_pData(pFileData)
    ,m_bIsFileAlign(bIsFileAlign)
{
}

pe_DosHeader *PE::dosHeader()
{
    return (pe_DosHeader*)m_pData;
}

pe_NtHeader *PE::ntHeader()
{
    if(m_pData==nullptr)
        return nullptr;
	return  (pe_NtHeader*)(dosHeader()->e_lfanew + (int)m_pData);
}

pe_FileHeader *PE::fileHeader()
{
    if(m_pData==nullptr)
        return nullptr;
    return &ntHeader()->FileHeader;
}

pe_OptionHeader *PE::optionHeader()
{
    if(m_pData==nullptr)
        return nullptr;

    return &ntHeader()->OptionalHeader;
}

pe_DataDirectroy *PE::dataDirectory()
{
    if(m_pData == nullptr  )
        return nullptr;
    return optionHeader()->DataDirectory;
}

DWORD PE::RVAToOfs(DWORD dwRVA)
{
    if(m_pData==nullptr)
        return 0;
    //开始遍历区段查找包含RVA地址的区段
    //获取标准头指针,以获取区段数目
    //获取区段数目
    DWORD	dwSecTotal = fileHeader()->NumberOfSections;

    //获取第一个区段
    PIMAGE_SECTION_HEADER	pScn = sectionHeader();

    //遍历区段
    for(DWORD i = 0; i < dwSecTotal; i++)
    {
        if(dwRVA >= pScn->VirtualAddress
           && dwRVA < pScn->VirtualAddress + pScn->Misc.VirtualSize)
        {
            // rva 转 文件偏移公式:
            // rva - 区段所在rva + 区段所在文件偏移
            return dwRVA - pScn->VirtualAddress + pScn->PointerToRawData;
        }
        ++pScn;
    }
    return 0;
}

pe_SectionHeader* PE::sectionHeader()
{
	if(m_pData == nullptr)
		return nullptr;
	return IMAGE_FIRST_SECTION(ntHeader());
}
