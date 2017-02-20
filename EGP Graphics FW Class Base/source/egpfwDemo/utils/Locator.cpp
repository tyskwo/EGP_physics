#include "Locator.h"

#include "SaveManager.h"

SaveManager *Locator::m_service = nullptr;

SaveManager *Locator::getSaveManager()
{
	return m_service;
}

void Locator::provide(SaveManager *service)
{
	m_service = service;
}

void Locator::cleanup()
{
	if (m_service != nullptr)
	{
		delete m_service;
		m_service = nullptr;
	}
}