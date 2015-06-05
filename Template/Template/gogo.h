#ifndef GOGO_H
#define GOGO_H

#include <string>

INT_PTR CALLBACK HSFCControlProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

std::wstring strToStrWide(std::string &NewString);

void OnInitDialog(HWND hDlg);
void OnPaint(HWND hDlg);
void OnVScroll(WPARAM wParam, LPARAM lParam);
void OnHScroll(WPARAM wParam, LPARAM lParam);


INT_PTR CALLBACK WmCommands(WPARAM wParam);
INT_PTR CALLBACK OnCancel();
INT_PTR CALLBACK OnOk();
INT_PTR CALLBACK KillFocus(WPARAM wParam, HWND hCommandWindowHandel);
INT_PTR CALLBACK OnCheckCh1(WPARAM wParam);
INT_PTR CALLBACK OnCheckCh2(WPARAM wParam);
INT_PTR CALLBACK OnCheckSmallPart(WPARAM wParam);
INT_PTR CALLBACK OnButtonClickAppily(WPARAM wParam);
INT_PTR CALLBACK OnButtonClickRestore(WPARAM wParam);


void initVar();

void saveToContext();
void readFromContext();

void RedrawControls(bool refreshEdit=true);
void SetEditWindowValue(HWND &CurWindow,double NewValue);
void setGainSliderPos(int newGain);
void SetThresholdSliderPos(HWND &CurWindow,double NewPos);
void setSmallParticle(bool newValue);

std::string HSFCDoubleToString(double Value,int decPlaces=2);
double RetriveDoubleFromWinHandel(HWND &TempWindow);
std::string RetriveStdStringFromWinHandel(HWND &TempWindow);



void DrawVerticalDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int yLineEnd,int DecadeCount);
void DrawDecadeText(HDC &hdc,RECT DrawArea,int size,std::string TextString);
void DrawHoroztallDecadeLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd,int DecadeCount);
void DrawHoroztallLogScale(HDC &hdc, int xLineStart, int yLineStart,int xLineEnd);
void DrawVerticalLogScale(HDC &hdc, int yLineStart,int yLineEnd, int xLineStart);


bool setCh1Min			(double newMin,bool refreshEdit=true);
bool setCh1Max			(double newMax,bool refreshEdit=true);
bool setCh2Min			(double newMin,bool refreshEdit=true);
bool setCh2Max			(double newMax,bool refreshEdit=true);
void setCh1Enable		(bool newValue);
void setCh2Enable		(bool newValue);
void setGain			(int NewGain);
void setSmallParticle	(bool newValue);

int getSliderPos(HWND &CurWindow);
int getGainSliderPos();



BOOL IsVisible();



#endif //GOGO_H



