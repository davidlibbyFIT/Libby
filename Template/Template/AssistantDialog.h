#pragma once

#include <atlbase.h>
#include <atlwin.h>




class AssistantDialog : public CDialogImpl<AssistantDialog>
{
public:
	AssistantDialog(stAFAssistData &inDataStructure);
	~AssistantDialog(void);
	LRESULT AssistantDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(AssistantDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

private:
	stAFAssistData m_inDataStructure;
	//CFont *m_pFont;
	//CFont m_BoldFont;



};

