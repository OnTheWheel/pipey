#include "BaseTarget.h"

using namespace pipey::io;

IBaseTarget::IBaseTarget()
	: m_descriptor(NULL), m_pParent(NULL)
{
}

IBaseTarget::IBaseTarget(IBaseTarget *pParent, descriptor_t descriptor)
	: m_descriptor(descriptor), m_pParent(pParent)
{
}

bool IBaseTarget::IsValid() const
{
	return m_descriptor != NULL;
}

IBaseTarget::operator bool() const
{
	return IsValid();
}

IIOTarget *IBaseTarget::GetParent() const
{
	return m_pParent;
}

descriptor_t IBaseTarget::GetDescriptor() const
{
	return m_descriptor;
}

void IBaseTarget::NullifyParent()
{
	m_pParent = NULL;
}

descriptor_t IBaseTarget::SetDescriptor(descriptor_t descriptor)
{
	descriptor_t prev = descriptor;
	m_descriptor = descriptor;
	return prev;
}