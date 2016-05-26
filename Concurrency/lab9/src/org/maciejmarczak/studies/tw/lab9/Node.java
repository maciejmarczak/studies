package org.maciejmarczak.studies.tw.lab9;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class Node {
	final Lock lock = new ReentrantLock();
	Node next;
	Object o;
}
