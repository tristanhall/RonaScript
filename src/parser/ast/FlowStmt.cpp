/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "FlowStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
FlowStmt::FlowStmt()
{
}

/*****************************************************************************/
FlowStmt::~FlowStmt()
{

}

/*****************************************************************************/
std::string FlowStmt::ToString(bool nl)
{
	return AstNode::ToString(nl);
}
