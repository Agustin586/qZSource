/**********************************************************************
* File: Lab_10.cmd -- File for Lab 10
* Devices: TMS320F28335
* Author: Technical Training Organization (TTO), Texas Instruments
* History:
*   07/15/08 - original
**********************************************************************/

#ifndef LINKER_SYMBOLS_H_
#define LINKER_SYMBOLS_H_

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
                        EXTERNAL VARIABLES
**********************************************************************/
extern Uint16 secureRamFuncs_loadstart;
extern Uint16 secureRamFuncs_loadsize;
extern Uint16 secureRamFuncs_runstart;
extern Uint16 hwi_vec_loadstart;
extern Uint16 hwi_vec_loadsize;
extern Uint16 hwi_vec_runstart;
extern Uint16 trcdata_loadstart;
extern Uint16 trcdata_loadsize;
extern Uint16 trcdata_runstart;

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
}
#endif

#endif /* LINKER_SYMBOLS_H_ */
