#include "StdAfx.h"
#include "AssistantDialog.h"


AssistantDialog::AssistantDialog(UINT nID,stAFAssistData &inDataStructure):
	m_DialogId (nID),
	m_inDataStructure (inDataStructure) 

{
	//m_pFont = new CFont();
	//m_pFont->CreatePointFont(165, _T("Arial"));
	
}


AssistantDialog::~AssistantDialog(void)
{

	/*
	if(m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
	*/

}


LRESULT AssistantDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
 // Do some initialization code
 return 1;
};
