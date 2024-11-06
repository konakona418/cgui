//
// Created on: 2024/11/6, 15:26.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_ERROR_MSG_H
#define CGUI_ERROR_MSG_H

#define CGUI_ERROR_ILLEGAL_NULL_PTR_MESSAGE "Used null pointer where a non-null pointer was expected."
#define CGUI_ERROR_ILLEGAL_NULL_PTR_CODE 0x00000001

#define CGUI_ERROR_NOT_IMPLEMENTED_MESSAGE "Not implemented."
#define CGUI_ERROR_NOT_IMPLEMENTED_CODE 0x00000002

#define CGUI_ERROR_INVALID_ARGUMENT_MESSAGE "Invalid argument."
#define CGUI_ERROR_INVALID_ARGUMENT_CODE 0x00000003

#define CGUI_WINDOW_CLASS_REGISTRATION_FAILED_MESSAGE "Window class registration failed."
#define CGUI_WINDOW_CLASS_REGISTRATION_FAILED_CODE 0x00000011

#define CGUI_WINDOW_CREATE_FAILED_MESSAGE "Window creation failed."
#define CGUI_WINDOW_CREATE_FAILED_CODE 0x00000012

#endif //CGUI_ERROR_MSG_H
