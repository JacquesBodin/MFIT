//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("PestMDMiAutoChnls_Dialog.cpp", PestADEiAutoChnls);
USEFORM("PestMDMiUserChnls_Dialog.cpp", PestADEiUserChnls);
USEFORM("PestSFDMautoChnls_Dialog.cpp", PestSFDMautoChnls);
USEFORM("PestMDMedUserChnls_Dialog.cpp", PestADEniUserChnls);
USEFORM("PestcheckOutput_MessageBox.cpp", PestcheckOutputMessage);
USEFORM("PestGeneralForm.cpp", PestGeneral);
USEFORM("PestMDMedAutoChnls_Dialog.cpp", PestADEniAutoChnls);
USEFORM("UserObsFilter_Dialog.cpp", UserObsFilter_Form);
USEFORM("WrkDirError_MessageBox.cpp", WDerrorMessage);
USEFORM("WrkDir_Dialog.cpp", WrkDir_Form);
USEFORM("TCobs_Dialog.cpp", TCobs_Form);
USEFORM("PestSFDMuserChnls_Dialog.cpp", PestSFDMuserChnls);
USEFORM("CalUncertHist.cpp", PostCalHist_Form);
USEFORM("MainForm.cpp", Main_Form);
USEFORM("MDMed_Dialog.cpp", MDMed);
USEFORM("MDMi_Dialog.cpp", MDMi);
USEFORM("FormatPlot_Dialog.cpp", FormatPlot_Form);
USEFORM("EndInvUserChnls_Dialog.cpp", EndInversionUserChnls_Form);
USEFORM("Error_MessageBox.cpp", ErrorMessage);
USEFORM("FilterResult_Dialog.cpp", FilterResult_Form);
USEFORM("MultiFlow_Generic_Dialog.cpp", MultiFlow_Generic);
USEFORM("Pest2RNEautoChnls_Dialog.cpp", Pest2RNEautoChnls);
USEFORM("Pest2RNEuserChnls_Dialog.cpp", Pest2RNEuserChnls);
USEFORM("MDP_2RNE_Dialog.cpp", MDP_2RNE);
USEFORM("MDP_SFDM_Dialog.cpp", MDP_SFDM);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMain_Form), &Main_Form);
		Application->CreateForm(__classid(TWrkDir_Form), &WrkDir_Form);
		Application->CreateForm(__classid(TPestADEiUserChnls), &PestADEiUserChnls);
		Application->CreateForm(__classid(TPestGeneral), &PestGeneral);
		Application->CreateForm(__classid(TTCobs_Form), &TCobs_Form);
		Application->CreateForm(__classid(TFormatPlot_Form), &FormatPlot_Form);
		Application->CreateForm(__classid(TUserObsFilter_Form), &UserObsFilter_Form);
		Application->CreateForm(__classid(TFilterResult_Form), &FilterResult_Form);
		Application->CreateForm(__classid(TMultiFlow_Generic), &MultiFlow_Generic);
		Application->CreateForm(__classid(TPestcheckOutputMessage), &PestcheckOutputMessage);
		Application->CreateForm(__classid(TWDerrorMessage), &WDerrorMessage);
		Application->CreateForm(__classid(TPestADEiAutoChnls), &PestADEiAutoChnls);
		Application->CreateForm(__classid(TEndInversionUserChnls_Form), &EndInversionUserChnls_Form);
		Application->CreateForm(__classid(TErrorMessage), &ErrorMessage);
		Application->CreateForm(__classid(TMDMi), &MDMi);
		Application->CreateForm(__classid(TMDMed), &MDMed);
		Application->CreateForm(__classid(TPestADEniUserChnls), &PestADEniUserChnls);
		Application->CreateForm(__classid(TPestADEniAutoChnls), &PestADEniAutoChnls);
		Application->CreateForm(__classid(TMDP_SFDM), &MDP_SFDM);
		Application->CreateForm(__classid(TPestSFDMuserChnls), &PestSFDMuserChnls);
		Application->CreateForm(__classid(TPestSFDMautoChnls), &PestSFDMautoChnls);
		Application->CreateForm(__classid(TMDP_2RNE), &MDP_2RNE);
		Application->CreateForm(__classid(TPest2RNEuserChnls), &Pest2RNEuserChnls);
		Application->CreateForm(__classid(TPest2RNEautoChnls), &Pest2RNEautoChnls);
		Application->CreateForm(__classid(TPostCalHist_Form), &PostCalHist_Form);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
