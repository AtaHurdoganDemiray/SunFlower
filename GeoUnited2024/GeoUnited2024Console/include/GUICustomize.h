// GUICustomize.h

#ifndef __GUICustomize_h__
#define __GUICustomize_h__

class GUICustomize
{
public:
	GUICustomize();

	void ChangeApplicationName(const VStr &newName);
	void ChangeApplicationIcon(int iconResourceId);
	void ChangeSplashWindowBmpId(int splashWindowBmpId);
	VStr GetApplicationName()const;
	int GetApplicationIconId()const;
	int GetSplashWindowBmpId()const;
	void Read();
	void Write();
	void LoadDefaults();
	int GetShowNetPane()const;
	int GetShowTreePane()const;
	int GetShowSpinFormPane()const;
	int GetTreeHolderPaneWidth()const;
	int GetTreeHolderPaneHeight()const;
	int GetNetHolderPaneWidth()const;
	int GetNetHolderPaneHeight()const;
	int GetSpinFormPaneWidth()const;
	int GetSpinFormPaneHeight()const;
	VStr GetMenuLangSettings()const;
	VStr GetApplicationIconStr()const;
	int GetDocumentIconId()const;
	VStr GetSettingsBaseDir();
	void SetSettingsBaseDir(const VStr &baseDirStr);
	int GetMacroTablesDialogPaneHeight()const;
	int GetMacroTablesDialogPaneWidth()const;
	int GetShowMacroTablesDialogPane()const;

private:
	std::map<VStr, VStr> m_keyValMap;

};
#endif