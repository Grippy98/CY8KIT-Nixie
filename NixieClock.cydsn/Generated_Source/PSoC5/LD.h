/*******************************************************************************
* File Name: LD.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_LD_H) /* Pins LD_H */
#define CY_PINS_LD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LD_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LD__PORT == 15 && ((LD__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    LD_Write(uint8 value);
void    LD_SetDriveMode(uint8 mode);
uint8   LD_ReadDataReg(void);
uint8   LD_Read(void);
void    LD_SetInterruptMode(uint16 position, uint16 mode);
uint8   LD_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the LD_SetDriveMode() function.
     *  @{
     */
        #define LD_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define LD_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define LD_DM_RES_UP          PIN_DM_RES_UP
        #define LD_DM_RES_DWN         PIN_DM_RES_DWN
        #define LD_DM_OD_LO           PIN_DM_OD_LO
        #define LD_DM_OD_HI           PIN_DM_OD_HI
        #define LD_DM_STRONG          PIN_DM_STRONG
        #define LD_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define LD_MASK               LD__MASK
#define LD_SHIFT              LD__SHIFT
#define LD_WIDTH              1u

/* Interrupt constants */
#if defined(LD__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LD_SetInterruptMode() function.
     *  @{
     */
        #define LD_INTR_NONE      (uint16)(0x0000u)
        #define LD_INTR_RISING    (uint16)(0x0001u)
        #define LD_INTR_FALLING   (uint16)(0x0002u)
        #define LD_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define LD_INTR_MASK      (0x01u) 
#endif /* (LD__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LD_PS                     (* (reg8 *) LD__PS)
/* Data Register */
#define LD_DR                     (* (reg8 *) LD__DR)
/* Port Number */
#define LD_PRT_NUM                (* (reg8 *) LD__PRT) 
/* Connect to Analog Globals */                                                  
#define LD_AG                     (* (reg8 *) LD__AG)                       
/* Analog MUX bux enable */
#define LD_AMUX                   (* (reg8 *) LD__AMUX) 
/* Bidirectional Enable */                                                        
#define LD_BIE                    (* (reg8 *) LD__BIE)
/* Bit-mask for Aliased Register Access */
#define LD_BIT_MASK               (* (reg8 *) LD__BIT_MASK)
/* Bypass Enable */
#define LD_BYP                    (* (reg8 *) LD__BYP)
/* Port wide control signals */                                                   
#define LD_CTL                    (* (reg8 *) LD__CTL)
/* Drive Modes */
#define LD_DM0                    (* (reg8 *) LD__DM0) 
#define LD_DM1                    (* (reg8 *) LD__DM1)
#define LD_DM2                    (* (reg8 *) LD__DM2) 
/* Input Buffer Disable Override */
#define LD_INP_DIS                (* (reg8 *) LD__INP_DIS)
/* LCD Common or Segment Drive */
#define LD_LCD_COM_SEG            (* (reg8 *) LD__LCD_COM_SEG)
/* Enable Segment LCD */
#define LD_LCD_EN                 (* (reg8 *) LD__LCD_EN)
/* Slew Rate Control */
#define LD_SLW                    (* (reg8 *) LD__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LD_PRTDSI__CAPS_SEL       (* (reg8 *) LD__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define LD_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LD__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define LD_PRTDSI__OE_SEL0        (* (reg8 *) LD__PRTDSI__OE_SEL0) 
#define LD_PRTDSI__OE_SEL1        (* (reg8 *) LD__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define LD_PRTDSI__OUT_SEL0       (* (reg8 *) LD__PRTDSI__OUT_SEL0) 
#define LD_PRTDSI__OUT_SEL1       (* (reg8 *) LD__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define LD_PRTDSI__SYNC_OUT       (* (reg8 *) LD__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(LD__SIO_CFG)
    #define LD_SIO_HYST_EN        (* (reg8 *) LD__SIO_HYST_EN)
    #define LD_SIO_REG_HIFREQ     (* (reg8 *) LD__SIO_REG_HIFREQ)
    #define LD_SIO_CFG            (* (reg8 *) LD__SIO_CFG)
    #define LD_SIO_DIFF           (* (reg8 *) LD__SIO_DIFF)
#endif /* (LD__SIO_CFG) */

/* Interrupt Registers */
#if defined(LD__INTSTAT)
    #define LD_INTSTAT            (* (reg8 *) LD__INTSTAT)
    #define LD_SNAP               (* (reg8 *) LD__SNAP)
    
	#define LD_0_INTTYPE_REG 		(* (reg8 *) LD__0__INTTYPE)
#endif /* (LD__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LD_H */


/* [] END OF FILE */
