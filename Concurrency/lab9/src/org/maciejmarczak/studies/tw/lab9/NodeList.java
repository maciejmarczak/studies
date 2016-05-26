package org.maciejmarczak.studies.tw.lab9;

abstract class NodeList {

	/*
	 * ar - active readers
	 * aw - active writers
	 * wr - waiting readers
	 * ww - waiting writers
	 */
	int ar = 0, aw = 0, wr = 0, ww = 0;
	Node head = null;

	abstract boolean contains(Object o);

	abstract boolean remove(Object o);

	abstract boolean add(Object o);
}
