#ifndef __LOCATOR_H
#define __LOCATOR_H

class SaveManager;

class Locator
{
private:
	static SaveManager *m_service;
public:
	static SaveManager *getSaveManager();

	static void provide(SaveManager *service);

	static void cleanup();
};

#endif // __LOCATOR_H