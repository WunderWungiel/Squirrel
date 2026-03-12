#ifndef SQUIRRELSDLGS_h
#define SQUIRRELDLGS_h


//  INCLUDES
#include <aknform.h>

class CContactDatabase;
class CGeneratorSettings;

class CGeneratorSettingsDlg: public CAknForm
{
public:
    static CGeneratorSettingsDlg* NewL(CGeneratorSettings *aSettings);
    virtual ~CGeneratorSettingsDlg();
    //void HandleControlStateChangeL(TInt aControlId);

private: // Constructors
    CGeneratorSettingsDlg();
    void ConstructL(CGeneratorSettings *aSettings);

    void SetIntValue(TInt aId, TInt aValue);
    TInt GetIntValue(TInt aId);
    void GetColorValue(TInt aId, TUint32 &aValue);
    void SetColorValue(TInt aId, TUint32 &aValue); 
    void UpdateSettingsL();

protected:  // From CEikDialog
    TBool OkToExitL(TInt aButtonId);    
    void PreLayoutDynInitL();
    
public:  // New methods
    TBool ShowLD();
private:
    CGeneratorSettings *iSettings;
};


class CContactSelectionDlg: public CBase
{
public:
    static CContactSelectionDlg* NewL();
    virtual ~CContactSelectionDlg();

private:
    CContactSelectionDlg();
    void OpenContactDatabaseL();
    //void ReadContactGroupsL();
    void ReadContactsL();

public:
    TPtr8 GetVCardL(const TInt aIndex);
    TInt ShowL();

private:
    CContactDatabase *iContactDb;
    CDesCArrayFlat *iContactArray;
    CBufFlat *iVCardsBuf;
};


#endif
// End of File

