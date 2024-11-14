//
// Created on: 2024/11/6, 15:11.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_ERROR_H
#define CGUI_ERROR_H

#include "common.h"
#include "error_msg.h"

#define CGUI_Error_Prototype(_errCode, _errMsg) createError(_errCode, _errMsg)

#define CGUI_Error_IllegalNullPtr() (CGUI_Error_Prototype(CGUI_ERROR_ILLEGAL_NULL_PTR_CODE, CGUI_ERROR_ILLEGAL_NULL_PTR_MESSAGE))

#define CGUI_Error_NotImplemented() (CGUI_Error_Prototype(CGUI_ERROR_NOT_IMPLEMENTED_CODE, CGUI_ERROR_NOT_IMPLEMENTED_MESSAGE))

#define CGUI_Error_InvalidArgument() (CGUI_Error_Prototype(CGUI_ERROR_INVALID_ARGUMENT_CODE, CGUI_ERROR_INVALID_ARGUMENT_MESSAGE))

#define CGUI_Error_MemAllocFailed() (CGUI_Error_Prototype(CGUI_ERROR_MEMORY_ALLOCATION_FAILED_CODE, CGUI_ERROR_MEMORY_ALLOCATION_FAILED_MESSAGE))

#define CGUI_Error_WindowClassRegistrationFailed() (CGUI_Error_Prototype(CGUI_WINDOW_CLASS_REGISTRATION_FAILED_CODE, CGUI_WINDOW_CLASS_REGISTRATION_FAILED_MESSAGE))

#define CGUI_Error_WindowCreateFailed() (CGUI_Error_Prototype(CGUI_WINDOW_CREATE_FAILED_CODE, CGUI_WINDOW_CREATE_FAILED_MESSAGE))

#define CGUI_Error_WindowClassAlreadyExist() (CGUI_Error_Prototype(CGUI_WINDOW_CLASS_ALREADY_EXISTS_CODE, CGUI_WINDOW_CLASS_ALREADY_EXISTS_MESSAGE))

#define CGUI_Error_WindowClassNotFound() (CGUI_Error_Prototype(CGUI_WINDOW_CLASS_NOT_FOUND_CODE, CGUI_WINDOW_CLASS_NOT_FOUND_MESSAGE))

#define CGUI_Error_ComponentIdAlreadyExists() (CGUI_Error_Prototype(CGUI_COMPONENT_ID_ALREADY_EXISTS_CODE, CGUI_COMPONENT_ID_ALREADY_EXISTS_MESSAGE))

#define CGUI_Error_ComponentNotFound() (CGUI_Error_Prototype(CGUI_COMPONENT_NOT_FOUND_CODE, CGUI_COMPONENT_NOT_FOUND_MESSAGE))

#endif //CGUI_ERROR_H
