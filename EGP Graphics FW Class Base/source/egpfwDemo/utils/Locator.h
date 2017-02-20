//
//  Locator.h
//  egpfwDemo
//
//  Created by Wednesday-David Hartman on 2/20/17.
//  Service Locator, based on the one described 
//	in the book "Game Programming Patterns" by Bob Nystrom.
//	http://gameprogrammingpatterns.com/service-locator.html
//

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