//
// Created on: 2024/11/18, 16:16.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_TRAIT_H
#define CGUI_TRAIT_H

typedef unsigned int CGUI_Trait;

#define CGUI_Trait_UIComponent      0x00000001
#define CGUI_Trait_UIDisposable     0x00000002
#define CGUI_Trait_UIState          0x00000004
#define CGUI_Trait_UIDrawable       0x00000008
#define CGUI_Trait_UILayout         0x00000010
#define CGUI_Trait_UIStyle          0x00000020

#define CGUI_Trait_UIWin32            0x00000100

/**
 * @note This macro can be applied to evaluating whether the given component implements all the required traits.
 * @param _implFlag The implementation flag ( @p implFlag ) of a certain component instance.
 * @param _Traits The required traits, connected by bitwise OR.
 * @return Whether the component implements all the required traits.
 */
#define impl(_implFlag, _Traits) (((_implFlag) & (_Traits)) == (_Traits))

#endif //CGUI_TRAIT_H
