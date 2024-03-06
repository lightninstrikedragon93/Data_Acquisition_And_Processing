/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_PANEL                       1       /* callback function: OnFreq */
#define  FREQ_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  FREQ_PANEL_RAW_SIGNAL_GRAPH_3    3       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_RAW_SIGNAL_GRAPH_2    4       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_RAW_SIGNAL_GRAPH      5       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_RIPPLE                6       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_ATTEND                7       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_ORDER                 8       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_POINTS                9       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_FNEXT                 10      /* control type: command, callback function: FreqNext */
#define  FREQ_PANEL_FPREV                 11      /* control type: command, callback function: FreqPrevious */
#define  FREQ_PANEL_GRAPH_SELECTION_FQ    12      /* control type: ring, callback function: GraphSelectionFq */
#define  FREQ_PANEL_WINDOW_TYPE           13      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_FSTOP                 14      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_FSTART                15      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_TEXTMSG               16      /* control type: textMsg, callback function: (none) */

#define  MAIN_PANEL                       2       /* callback function: OnWave */
#define  MAIN_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  MAIN_PANEL_FILTER_DATA           3       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_RAW_DATA              4       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_LOAD_WAVE             5       /* control type: command, callback function: LoadWave */
#define  MAIN_PANEL_ENVELOPE              6       /* control type: command, callback function: ENVELOPE */
#define  MAIN_PANEL_APLICA                7       /* control type: command, callback function: AplicaButton */
#define  MAIN_PANEL_NEXT                  8       /* control type: command, callback function: Next */
#define  MAIN_PANEL_PREV                  9       /* control type: command, callback function: Previous */
#define  MAIN_PANEL_ZERO                  10      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEAN                  11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIAN                12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DISPERSION            13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_INDEX_MINIM           14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_INDEX_MAXIM           15      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MAXIM                 16      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_KURTOSIS              17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_SKEWNESS              18      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MINIM                 19      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_STOP                  20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_START                 21      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_TIP                   22      /* control type: ring, callback function: (none) */
#define  MAIN_PANEL_FILTRU                23      /* control type: ring, callback function: (none) */
#define  MAIN_PANEL_HISTOGRAM             24      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_TEXTMSG               25      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_ALPHA                 26      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_SAVE                  27      /* control type: command, callback function: Save */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AplicaButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ENVELOPE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FreqNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FreqPrevious(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GraphSelectionFq(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadWave(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Next(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreq(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnWave(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Previous(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif